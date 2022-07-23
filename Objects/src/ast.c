//
// Created by Kevin on 05/07/2022.
//

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "ast.h"

#include <stdio.h>

#include "lib.h"

#include "exec_error.h"
#include "unbounded_int.h"
#include "token.h"
#include "hashmap.h"

/**
 * Apply the arithmetic expression stored in the AST.
 * @param storage The storage tree.
 * @param asn The root of the sub AST.
 */
static UnboundedInt ASN_apply(HashMap *storage, ASN *asn, int *err, HashMap *map);

/**
 * Allocate a new memory block for an ASN.
 * @param t The storage tree.
 * @param s The node's data
 * @return The node's pointer
 */
static ASN *ASN_new(UnboundedInt value, Token token);

/**
 * Add a new Node in the AST according to the string given in argument. Manage the operator priority.
 * @param asn The root of the sub AST.
 * @param storage. The storage tree.
 * @param s The node value.
 */
static int ASN_add(UnboundedInt value, ASN **asn, Token token);

/**
 * Free the ASN.
 * @param asn The node which will be free.
 * @return NULL.
 */
static ASN *ASN_free(ASN *asn);


/**
 * Realize the operation represented by the node.
 *
 * @param asn The current node.
 * @param storage The storage tree.
 * @param left The left AST result.
 * @param right The right AST result
 * @return The value of the operation. \n
 *         The DEFAULT_OP(empty node) is considered as an addition.
 *         if the node is not an operator, return the result of the node.
 */
static UnboundedInt op(ASN *asn, HashMap *storage, UnboundedInt left, UnboundedInt right);


static ASN *ASN_new(UnboundedInt value, Token token) {
	ASN *node = malloc(sizeof(ASN));
	if (node == NULL) return NULL;
	switch (token.mType) {
		case OPERATOR:
			node->result = UnboundedInt_newll(0);
			node->token = token;
			break;
		case NUMBER:
			node->token = token;
			node->result = UnboundedInt_newString(token.mData);
			break;
		case VAR:
			node->result = value;
			node->token = token;
			break;
		case FUNCTION:
			node->result = UnboundedInt_newll(0);
			break;

		default:
			node->result = UnboundedInt_newll(0);
			node->token = token;
			break;
	}

	node->right = NULL;
	node->left = NULL;
	return node;
}

static int ASN_add(UnboundedInt value, ASN **asn, Token token) {
	if (*asn == NULL) {
		*asn = ASN_new(value, Token_new("", 0, OPERATOR));
		(*asn)->left = ASN_new(value, token);
	}
	else if ((*asn)->token.mData[0] == DEFAULT_OP) {
		if ((*asn)->token.mType != token.mType) {
			perror_file(INVALID_SYNTAX);
			return 0;
		}
		strncpy((*asn)->token.mData, token.mData, strlen(token.mData));
	}
	else if ((*asn)->right == NULL || token.mType == NUMBER || token.mType == FUNCTION || token.mType == VAR ||
		!isHigher((*asn)->token.mData[0], token.mData[0])) {
		return ASN_add(value, &(*asn)->right, token);
	}
	else {
		ASN *tmp = *asn;
		*asn = ASN_new(value, token);
		(*asn)->left = tmp;
	}
	return 1;
}

static ASN *ASN_free(ASN *n) {
	if (n != NULL) {
		ASN_free(n->right);
		ASN_free(n->left);
		//unboundedInt_free(n->result);
		free(n);
	}

	return n = NULL;
}

static UnboundedInt op(ASN *asn, HashMap *storage, UnboundedInt left, UnboundedInt right) {
	switch (asn->token.mData[0]) {
		case '=':
			EQUALS(storage, asn->left->token.mData, &right);
			return right;
		case '*':
			return MULTIPLICATION(right, left);
		case '-':
			return SUBSTRACTION(left, right);
		case DEFAULT_OP:
		case '+':
			return ADDITION(left, right);
		case '/':
			return DIVISION(left, right);
		case '%':
			return MODULO(left, right);
		default:
			return asn->result;
	}
}


static UnboundedInt ASN_apply(HashMap *storage, ASN *asn, int *err, HashMap *map) {
	if (asn == NULL) return UnboundedInt_newll(0);
	if (asn->token.mType == NUMBER || asn->token.mType == VAR) {
		return asn->result;
	}
	UnboundedInt left = ASN_apply(storage, asn->left, err, map);
	UnboundedInt right = ASN_apply(storage, asn->right, err, map);
	if (((asn->token.mData[0] != DEFAULT_OP) && asn->token.mType == OPERATOR) && // missing binary operator argument
		((asn->left == NULL) || (asn->right == NULL))) {
		if (*err) perror_file(INVALID_SYNTAX);
		*err = 0;
		return UnboundedInt_newll(0);
	}
	if (asn->token.mData[0] == '=' && asn->left->token.mType == NUMBER) { //Invalid assignation
		if (*err) perror_file(INVALID_SYNTAX);
		*err = 0;
		return UnboundedInt_newll(0);
	}
	if (asn->token.mType == FUNCTION) {
		//*err = function_apply(map, asn->token.mData, asn);
		printf("CALL to %s", asn->token.mData);
		return asn->result;
	}
	return asn->result = op(asn, storage, left, right);
}

AST *AST_new() {
	AST *tree = malloc(sizeof(AST));
	if (tree == NULL) return NULL;
	tree->root = NULL;
	return tree;
}

int AST_add(AST *ast, UnboundedInt value, Token token) {
	if (ast == NULL || UnboundedInt_isError(value) || isspace(token.mData[0]) || token.mType == VOID) {
		perror_file(INTERNAL);
		return 0;
	}

	return ASN_add(value, &ast->root, token);
}

int AST_apply(HashMap *storage, AST *ast, HashMap *map) {
	if (storage == NULL || ast == NULL) {
		errno = 22; //Invalid arguments
		perror_src("Internal Error");
		return 0;
	}
	if (ast->root == NULL) {
		perror_file(INVALID_SYNTAX);
		return 0;
	}
	if (ast->root->token.mData[0] == DEFAULT_OP && ast->root->left == NULL) {
		perror_file(INVALID_OPERATOR);
		return 0;
	}
	int err = 1;
	ASN_apply(storage, ast->root, &err, map);
	return err;
}

AST *AST_clear(AST *ast) {
	if (ast->root != NULL) {
		ASN_free(ast->root);
	}
	ast->root = NULL;
	return ast;
}

AST *AST_free(AST *ast) {
	ASN_free(ast->root);
	free(ast);
	return NULL;
}

int AST_hasFunction(AST *ast) {
	if (ast == NULL || ast->root == NULL) return 0;
	return ASN_hasFunction(ast->root);
}

int ASN_hasFunction(ASN *asn) {
	if (asn->token.mType == FUNCTION) return 1;
	int left = (asn->left == NULL) ? 0 : ASN_hasFunction(asn->left);
	int right = (asn->right == NULL) ? 0 : ASN_hasFunction(asn->right);
	return left || right;
}
