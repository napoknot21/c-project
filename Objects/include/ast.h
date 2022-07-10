//
// Created by Kevin on 05/07/2022.
//
#ifndef C_PROJECT_AST_H
#define C_PROJECT_AST_H


#include "token.h"
#include "unbounded_int.h"
#include "hashmap.h"

#define DEFAULT_OP '\0'
#define MULTIPLICATION(a, b)(unboundedInt_multiply(a,b))
#define ADDITION(a, b)(unboundedInt_add(a,b))
#define SUBSTRACTION(a, b)(unboundedInt_subtract(a,b))
//#define DIVISION(a, b)(unboundedInt_divide(a, b))

#define EQUALS(t, a, b)(hashMap_put((t), (a), (b)))
//#define MODULO(a, b)((a) % (b))


/**
* Abstract syntax node used for arithmetic evaluations.
*/
typedef struct ASN {
    Token token;
    UnboundedInt result;
    struct ASN *left;
    struct ASN *right;
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
