//
// Created by Kevin on 05/07/2022.
//
#ifndef C_PROJECT_AST_H
#define C_PROJECT_AST_H

#define DEFAULT_OP '\0'
#define MULTIPLICATION(a, b)(unbounded_int_produit(a,b))
#define ADDITION(a, b)(unbounded_int_somme(a,b))
#define SUBSTRACTION(a, b)(unbounded_int_difference(a,b))
//#define DIVISION(a, b)(unbounded_int_division(a, b))
#define EQUALS(t, a, b)(tree_add((t), (a), (b)))
//#define MODULO(a, b)((a) % (b))

/**
 * Abstract syntax tree used for arithmetic evaluations.
 */
/**
* Abstract syntax node used for arithmetic evaluations.
*/
typedef struct ASN ASN;
struct ASN {
    Token token;
    unbounded_int result;
    ASN *left;
    ASN *right;
};

typedef struct AST AST;
struct AST {
    ASN *root;
};

/**
 * Free the AST.
 * @param ast The AST pointer.
 * @return NULL.
 */
static AST *AST_free(AST *ast);

/**
 * Allocate a new memory block for the AST. its root is NULL.
 * @return The pointer of the AST.
 */
static AST *AST_new();

/**
 * Add a new Node in the AST according to the string given in argument. Manage the operator priority.
 * @param ast The AST.
 * @param storage. The storage ast.
 * @param s The node value.
 */
static int AST_add(AST *ast, Tree *storage, Token token);

/**
 * Apply the arithmetic expression stored in the ast.
 * @param storage The storage tree.
 * @param ast The AST.
 */
static int AST_apply(Tree *storage, AST *ast, HashMap *map);

/**
 *
 * @param ast
 * @return
 */
static AST *AST_clear(AST *ast);


/**
 * Allocate a new memory block for an ASN.
 * @param t The storage tree.
 * @param s The node's data
 * @return The node's pointer
 */
static ASN *ASN_new(Tree *t, Token token);

/**
 * Add a new Node in the AST according to the string given in argument. Manage the operator priority.
 * @param asn The root of the sub AST.
 * @param storage. The storage tree.
 * @param s The node value.
 */
static int ASN_add(Tree *storage, ASN **asn, Token token);

/**
 * Free the ASN.
 * @param asn The node which will be free.
 * @return NULL.
 */
static ASN *ASN_free(ASN *asn);

/**
 * Apply the arithmetic expression stored in the AST.
 * @param storage The storage tree.
 * @param asn The root of the sub AST.
 */
static unbounded_int ASN_apply(Tree *storage, ASN *asn, int *err, HashMap *map);

/**
 * Realize the operation represented by the node.
 *
 * @param asn The current node.
 * @param storage The storage tree.
 * @param left The left AST result.
 * @param right The right AST result
 * @return The value of the operation. <br>
 *         The DEFAULT_OP(empty node) is considered as an addition.<br>
 *         if the node is not an operator, return the result of the node.
 */
static unbounded_int op(ASN *asn, Tree *storage, unbounded_int left, unbounded_int right);

int AST_hasFunction(AST *ast);
int ASN_hasFunction(ASN *asn);
#endif //C_PROJECT_AST_H
