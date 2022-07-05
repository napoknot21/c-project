//
// Created by Kevin on 05/07/2022.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "ast.h"
#include "execerror.h"
#include "unbounded_int.h"
#include "token.h"
#include "function.h"
#include "hashmap.h"


static AST *AST_new() {
    AST *tree = malloc(sizeof(AST));
    if (tree == NULL) return NULL;
    tree->root = NULL;
    return tree;
}

static ASN *ASN_new(unbounded_int value, Token token) {
    ASN *node = malloc(sizeof(ASN));
    if (node == NULL) return NULL;
    switch (token.type) {
        case OPERATOR:
            node->result = ll2unbounded_int(0);
            node->token = token;
            break;
        case NUMBER:
            node->token = token;
            node->result = string2unbounded_int(token.data);
            break;
        case VAR:
            node->result = value;
            node->token = token;
            break;
        case FUNCTION:
            node->result = ll2unbounded_int(0);

        default:
            node->result = ll2unbounded_int(0);
            node->token = token;
            break;
    }

    node->right = NULL;
    node->left = NULL;
    return node;
}

static int AST_add(AST *ast, unbounded_int value, Token token) {
    if (ast == NULL || storage == NULL || isspace((unsigned) token.data[0]) || token.type == VOID) {
        perror_file(INTERNAL);
        return 0;
    }
    return ASN_add(value, &ast->root, token);
}

static int ASN_add(Tree *storage, ASN **asn, Token token) {
    if (*asn == NULL) {
        *asn = ASN_new(storage, token_new("", 0, OPERATOR));
        (*asn)->left = ASN_new(storage, token);
    } else if ((*asn)->token.data[0] == DEFAULT_OP) {
        if ((*asn)->token.type != token.type) {
            pERROR(INVALID_SYNTAX);
            return 0;
        }
        strncpy((*asn)->token.data, token.data, strlen(token.data));
    } else if ((*asn)->right == NULL || token.type == NUMBER || token.type == FUNCTION || token.type == VAR ||
               !isHigher((*asn)->token.data[0], token.data[0])) {
        return ASN_add(storage, &(*asn)->right, token);
    } else {
        ASN *tmp = *asn;
        *asn = ASN_new(storage, token);
        (*asn)->left = tmp;
    }
    return 1;
}

static AST *AST_clear(AST *ast) {
    if (ast->root != NULL) {
        ASN_free(ast->root);
    }
    ast->root = NULL;
    return ast;
}

static AST *AST_free(AST *ast) {
    ASN_free(ast->root);
    free(ast);
    return NULL;
}

static ASN *ASN_free(ASN *n) {
    if (n != NULL) {
        ASN_free(n->right);
        ASN_free(n->left);
        //unbounded_int_free(n->result);
        free(n);
    }
    return n = NULL;
}

static unbounded_int op(ASN *asn, Tree *storage, unbounded_int left, unbounded_int right) {
    switch (asn->token.data[0]) {
        case '=':
            EQUALS(storage, right, asn->left->token.data);
            return right;
        case '*':
            return MULTIPLICATION(right, left);
        case '-':
            return SUBSTRACTION(left, right);
        case DEFAULT_OP:
        case '+':
            return ADDITION(left, right);
            /*case '/':
                return DIVISION(left, right);
                case '%':
                    return MODULO(left, right);*/
        default:
            return asn->result;
    }
}

static int AST_apply(Tree *storage, AST *ast, HashMap *map) {
    if (storage == NULL || ast == NULL) {
        errno = 22; //Invalid arguments
        printErr("Internal Error");
        return 0;
    }
    if (ast->root == NULL) {
        pERROR(INVALID_SYNTAX);
        return 0;
    }
    if (ast->root->token.data[0] == DEFAULT_OP && ast->root->left == NULL) {
        pERROR(INVALID_OPERATOR);
        return 0;
    }
    int err = 1;
    ASN_apply(storage, ast->root, &err, map);
    return err;
}

static unbounded_int ASN_apply(Tree *storage, ASN *asn, int *err, HashMap *map) {
    if (asn == NULL) return ll2unbounded_int(0);
    if (asn->token.type == NUMBER || asn->token.type == VAR) {
        return asn->result;
    }
    unbounded_int left = ASN_apply(storage, asn->left, err, map);
    unbounded_int right = ASN_apply(storage, asn->right, err, map);
    if (((asn->token.data[0] != DEFAULT_OP) && asn->token.type == OPERATOR) && // missing binary operator argument
        ((asn->left == NULL) || (asn->right == NULL))) {
        if (*err) pERROR(INVALID_SYNTAX);
        *err = 0;
        return ll2unbounded_int(0);
    }
    if (asn->token.data[0] == '=' && asn->left->token.type == NUMBER) { //Invalid assignation
        if (*err) pERROR(INVALID_SYNTAX);
        *err = 0;
        return ll2unbounded_int(0);
    }
    if (asn->token.type == FUNCTION) {
        *err = function_apply(map, asn->token.data, asn);
        return asn->result;
    }
    return asn->result = op(asn, storage, left, right);
}

static int AST_hasFunction(AST *ast) {
    if (ast == NULL || ast->root == NULL) return 0;
    return ASN_hasFunction(ast->root);
}

static int ASN_hasFunction(ASN *asn) {
    if (asn->token.type == FUNCTION) return 1;
    int left = (asn->left == NULL) ? 0 : ASN_hasFunction(asn->left);
    int right = (asn->right == NULL) ? 0 : ASN_hasFunction(asn->right);
    return left || right;
}


