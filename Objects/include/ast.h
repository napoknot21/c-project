//
// Created by Kevin on 05/07/2022.
//
#ifndef C_PROJECT_AST_H
#define C_PROJECT_AST_H

#include "token.h"
#include "unbounded_int.h"
#define DEFAULT_OP '\0'
#define MULTIPLICATION(a, b)(UnboundedInt_produit(a,b))
#define ADDITION(a, b)(UnboundedInt_somme(a,b))
#define SUBSTRACTION(a, b)(UnboundedInt_difference(a,b))
//#define DIVISION(a, b)(UnboundedInt_division(a, b))
#define EQUALS(t, a, b)(tree_add((t), (a), (b)))
//#define MODULO(a, b)((a) % (b))


/**
* Abstract syntax node used for arithmetic evaluations.
*/
typedef struct ASN {
    Token token;
    UnboundedInt result;
    ASN *left;
    ASN *right;
} ASN;

typedef struct AST {
    ASN *root;
} AST;

/**
 * Free the AST.
 * @param ast The AST pointer.
 * @return NULL.
 */
AST *AST_free(AST *ast);

/**
 * Allocate a new memory block for the AST. its root is NULL.
 * @return The pointer of the AST.
 */
AST *AST_new();

/**
 * Add a new Node in the AST according to the string given in argument. Manage the operator priority.
 * @param ast The AST.
 * @param value The UnboundedInt that will be added to the AST.
 * @param s The node value.
 */
int AST_add(AST *ast, UnboundedInt value, Token token);

/**
 * Apply the arithmetic expression stored in the ast.
 * @param storage The storage map.
 * @param ast The AST.
 */
int AST_apply(HashMap *storage, AST *ast, HashMap *map);

/**
 *
 * @param ast
 * @return
 */
AST *AST_clear(AST *ast);

int AST_hasFunction(AST *ast);
int ASN_hasFunction(ASN *asn);
#endif //C_PROJECT_AST_H
