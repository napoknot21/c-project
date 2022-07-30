//
// Created by Kevin on 05/07/2022.
//

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "ast.h"

#include <stdio.h>

#include "app_informations.h"
#include "lib.h"

#include "exec_error.h"
#include "function.h"
#include "variable.h"
#include "token.h"
#include "hashmap.h"

/**
 * Apply the arithmetic expression stored in the AST.
 * @param storage The storage tree.
 * @param asn The root of the sub AST.
 */
static Variable ASN_apply(HashMap *storage, ASN *asn, int *err, HashMap *map);

/**
 * Allocate a new memory block for an ASN.
 * @param t The storage tree.
 * @param s The node's data
 * @return The node's pointer
 */
static ASN *ASN_new(Variable value, Token token);

/**
 * Add a new Node in the AST according to the string given in argument. Manage the operator priority.
 * @param asn The root of the sub AST.
 * @param storage. The storage tree.
 * @param s The node value.
 */
static int ASN_add(Variable value, ASN **asn, Token token);

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
static Variable op(ASN *asn, HashMap *storage, Variable left, Variable right);


static ASN *ASN_new(Variable value, Token token) {
	ASN *node = malloc(sizeof(ASN));
	if (node == NULL) return NULL;
	switch (token.mType) {
		case OPERATOR:
			node->result = VAR_NULL;
			node->token = token;
			break;
		case NUMBER:
			node->token = token;
			UnboundedInt tmp = UnboundedInt_newString(token.mData);
			node->result = Variable_new("", &tmp, VARTYPE_INT);
			break;
		case VAR:
			node->result = Variable_cpy(value);
			node->token = token;
			break;
		default:
			node->result = VAR_NULL;
			node->token = token;
			break;
	}

	node->right = NULL;
	node->left = NULL;
	return node;
}

static int ASN_add(Variable value, ASN **asn, Token token) {
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
		n->right = ASN_free(n->right);
		n->left = ASN_free(n->left);
		Variable_free(n->result);
		n->result = VAR_NULL;
		free(n);
	}

	return n = NULL;
}

static Variable equals(HashMap *storage, char *name, Variable value) {
	value.mName = name;
	Variable cpy = Variable_cpy(value);
	Variable *old = malloc(sizeof(Variable));
	if (old == NULL) {
		perror_src("");
		return Variable_cpy(value);
	}
	HashMap_put(storage, name, &cpy, sizeof(Variable));
	return Variable_cpy(value);
}

static Variable multipliply(Variable left, Variable right) { //TODO: finish the other cases
	switch (left.mType) {
		case VARTYPE_INT:
			if (right.mType == VARTYPE_INT) {
				UnboundedInt tmp = UnboundedInt_multiply(right.mValue.ui, left.mValue.ui);
				return Variable_new("", &tmp, VARTYPE_INT);
			}
			break;
	}
	return VAR_NULL;

}

static Variable subtract(Variable left, Variable right) {
	switch (left.mType) {
		case VARTYPE_INT:
			if (right.mType == VARTYPE_INT) {
				UnboundedInt tmp = UnboundedInt_subtract(right.mValue.ui, left.mValue.ui);
				return Variable_new("", &tmp, VARTYPE_INT);
			}
			break;
	}
	return VAR_NULL;
}

static Variable add(Variable left, Variable right) {
	switch (left.mType) {
		case VARTYPE_INT:
			if (right.mType == VARTYPE_INT) {
				UnboundedInt tmp = UnboundedInt_add(right.mValue.ui, left.mValue.ui);
				return Variable_new("", &tmp, VARTYPE_INT);
			}
			break;
	}
	return VAR_NULL;
}

static Variable divide(Variable left, Variable right) {
	switch (left.mType) {
		case VARTYPE_INT:
			if (right.mType == VARTYPE_INT) {
				UnboundedInt tmp = UnboundedInt_divide(right.mValue.ui, left.mValue.ui);
				return Variable_new("", &tmp, VARTYPE_INT);
			}
			break;
	}
	return VAR_NULL;
}

static Variable modulo(Variable left, Variable right) {
	switch (left.mType) {
		case VARTYPE_INT:
			if (right.mType == VARTYPE_INT) {
				UnboundedInt tmp = UnboundedInt_modulo(right.mValue.ui, left.mValue.ui);
				return Variable_new("", &tmp, VARTYPE_INT);
			}
			break;
	}
	return VAR_NULL;
}

static Variable defaultOP(ASN *asn, Variable left, Variable right) {
	return left.mType == VARTYPE_NULL ? Variable_cpy(right) : Variable_cpy(left);
	/*if (left.mType == VARTYPE_NULL) {
		asn->left = ASN_free(asn->left);
		free(asn->right);
		asn->right = NULL;
		return right;
	} 
	asn->right = ASN_free(asn->right);
	free(asn->left);
	asn->left = NULL;
	return left;*/
}


static Variable op(ASN *asn, HashMap *storage, Variable left, Variable right) {
	switch (asn->token.mData[0]) {
		case '=':
			return equals(storage, asn->left->token.mData, right);
		case '*':
			return multipliply(left, right);
		case '-':
			return subtract(left, right);
		case DEFAULT_OP:
			return defaultOP(asn, left, right);
		case '+':
			return add(left, right);
		case '/':
			return divide(left, right);
		case '%':
			return modulo(left, right);
		default:
			return asn->result;
	}
}

/* #####################################################################################################################
 * Function's functions
 */
static int function_apply(HashMap *map, char *name, ASN *node) {
	Function *f = malloc(sizeof(Function));
	if (f == NULL) {
		perror_src("");
		return 0;
	}
	if (!HashMap_get(map, name, f)) {
		free(f);
		return 0;
	}
	if (f->mRequested > f->mArgc) {
		if (!EXIT_REQUEST) {
			perror_file(MISSING_ARGUMENTS);
		}
		free(f);
		EXIT_REQUEST = -1;
		return 0;
	}
	if (f->mRequested < f->mArgc) {
		if (!EXIT_REQUEST) {
			perror_file(TOO_MANY_ARGUMENTS);
		}
		free(f);
		EXIT_REQUEST = -1;
		return 0;
	}
	f->mFunc(f->mArgc, f->mArgv, f->mArgn);
	if (node != NULL && f->mRetType != RETURN_VOID) {
		node->result = f->mArgv[f->mArgc];
	}
	HashMap_remove(map, f->mName);
	free(f);
	return 1;
}


static Variable ASN_apply(HashMap *storage, ASN *asn, int *err, HashMap *map) {
	if (asn == NULL) return VAR_NULL;
	if (asn->token.mType == NUMBER || asn->token.mType == VAR) {
		return asn->result;
	}
	Variable left = ASN_apply(storage, asn->left, err, map);
	Variable right = ASN_apply(storage, asn->right, err, map);
	if (((asn->token.mData[0] != DEFAULT_OP) && asn->token.mType == OPERATOR) && // missing binary operator argument
		((asn->left == NULL) || (asn->right == NULL))) {
		if (*err) perror_file(INVALID_SYNTAX);
		*err = 0;
		return VAR_NULL;
	}
	if (asn->token.mData[0] == '=' && asn->left->token.mType == NUMBER) { //Invalid assignation
		if (*err) perror_file(INVALID_SYNTAX);
		*err = 0;
		return VAR_NULL;
	}
	if (asn->token.mType == FUNCTION) {
		*err = function_apply(map, asn->token.mData, asn);
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

int AST_add(AST *ast, Variable value, Token token) {
	if (ast == NULL || isspace(token.mData[0]) || token.mType == VOID) {
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
