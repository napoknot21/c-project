#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
//#include "unbounded_int.h"

#define DEFAULT_OP '\0'
#define MULTIPLICATION(a, b) ((a)*(b))
#define ADDITION(a, b) ((a)+(b))
#define SUBSTRACTION(a, b) ((a)-(b))
#define DIVISION(a, b) ((a)/(b))
#define EQUALS(t, a, b) (tree_add((t),(a),(b)))
#define MODULO(a, b) ((a)%(b))
static int MALLOC_COUNTER = 0;

/**
 * Token types.
 */
enum tokenType {
    NUMBER, VAR, OPERATOR
};

/**
 * Variables storage tree.
 */
typedef struct Tree Tree;

/**
 * Create the storage tree
 * @return an empty tree.
 */
static Tree *tree_new();

/**
 * Free the storage tree.
 *
 * @param t the storage tree.
 */
static void tree_free(Tree *t);

/**
 * Add a variable in the tree, if the variable is already present, its data will be updated.
 *
 * @param t the storage tree.
 * @param n the data value.
 * @param string the variable's name.
 * @return   0 if the variable was normally added. <br>
 *          -1 if the name is empty. <br>
 *          -2 if the tree is NULL.
 */
static int tree_add(Tree *t, int n, char *string);

/**
 * Gets the value of the given variable.
 *
 * @param t The storage tree.
 * @param string The variable's name
 * @return the data value if the value is found. <br>
 *          -1 if the value wasn't found. <br>
 *          -2 if the tree's integrity is compromised.
 */
static int tree_getValue(Tree *t, char *string);

/**
 * Calculates the size of the tree.
 *
 * @param t The storage tree.
 * @return the size of the tree.
 */
static int tree_size(Tree *t);


/**
 * Variables storage node.
 */
typedef struct Node Node;

/**
 * Create a node in a tree.
 *
 * @param id the node's id
 * @param data the node's data.
 * @return the pointer of the created node.
 */
static Node *node_new(char id, int data);

/**
 * free the sub-tree.
 *
 * @param n the sub-tree's root.
 */
static void node_free(Node *n);

/**
 * Add a variable in the tree, if the variable is already present, its data will be updated.
 *
 * @param pNode the current node.
 * @param n the data value.
 * @param string the variable's name.
 * @return  0 if the variable was normally added. <br>
 *          -1 if the variable's name is empty
 */
static int node_add(Node **pNode, int n, const char *string);

/**
 * Gets the value of the given variable.
 *
 * @param node The current node.
 * @param string The variable's name.
 * @return the data value if the value is found. <br>
 *          -1 if the value wasn't found. <br>
 *          -2 if the tree's integrity is compromised.
 */
static int node_getValue(Node **node, char *string);

/**
 * Calculate the size of the sub-tree.
 *
 * @param pNode the root of the sub-tree.
 * @return the size of the sub-tree.
 */
static int node_size(Node *node);

/**
 * Abstract syntax tree used for arithmetic evaluations.
 */
typedef struct AST AST;

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
static void AST_add(AST *ast, Tree *storage, char *s);

/**
 * Apply the arithmetic expression stored in the ast.
 * @param storage The storage tree.
 * @param ast The AST.
 */
static void AST_apply(Tree *storage, AST *ast);

/**
 * Abstract syntax node used for arithmetic evaluations.
 */
typedef struct ASN ASN;

/**
 * Allocate a new memory block for an ASN.
 * @param t The storage tree.
 * @param s The node's data
 * @return The node's pointer
 */
static ASN *ASN_new(Tree *t, char *s);

/**
 * Add a new Node in the AST according to the string given in argument. Manage the operator priority.
 * @param asn The root of the sub AST.
 * @param storage. The storage tree.
 * @param s The node value.
 */
static void ASN_add(Tree *storage, ASN **asn, char *s);

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
static int ASN_apply(Tree *storage, ASN *asn);

/**
 * Lexers Tokens.
 */
typedef struct Token Token;

/**
 * Free a token.
 * @param t The token which will be free
 */
static void token_free(Token t);

/**
 * Create a new token according to the string given in argument and the type.
 * @param s The token' data.
 * @param type The token's type.
 * @return The created Token.
 */
static Token token_new(char *s, enum tokenType type);

/**
 * Realize the operation represented by the node.
 * @param asn The current node.
 * @param storage The storage tree.
 * @param left The left AST's result.
 * @param right The right ATS's result
 * @return The value of the operation. <br>
 *         The DEFAULT_OP(empty node) is considered as an addition.<br>
 *         if the node is not an operator, return the result of the node.
 */
static int op(ASN *asn, Tree *storage, int left, int right);

/**
 * Indicate if the char given in argument is an operator
 * @param s The char which will be tested.
 * @return return true if the char is an operator, false otherwise.
 */
static int isAnOperator(char s);

/**
 * Indicate if the char a has a higher precedence than b.
 * @param a The first operator.
 * @param b The second operator.
 * @return true if a has a higher precedence than b, false otherwise.
 */
static int isHigher(char a, char b);

/**
 * Associate a priority with an operator.
 * @param a The operator.
 * @return The operator's priority
 */
static unsigned short priority(char a);

/**
 * Trim the string with the given regex. <br>
 * For example, all the space before the other character is ignored, and all the space after are deleted.
 * @param s The string which will be treated.
 * @return return the trimmed string
 */
static char *trim(const char *s);

/**
 * Indicate if the char is a number.
 * @param a The char which will be tested.
 * @return true if the char is a number, false otherwise.
 */

/**
 * Indicate if the string is a number.
 * @param string The string which will be tested.
 * @return true if the string is a number, false otherwise.
 */
static int isNum(char *string);

/**
 * Some tests
 */

//C (sur CLion) n'admet pas plusieurs mains, dc pour tester ce fichier, décommenter ce main et commenter le main de calc_unbounded_int.c
int main() {
    //printf("Hello, World!\n");
    Tree *t = tree_new();
    tree_add(t, 10243, "abc");
    printf("t(abc) = %d \n", tree_getValue(t, "abc"));
    tree_add(t, 1552, "abc");
    printf("t(abc) = %d \n", tree_getValue(t, "abc"));
    tree_add(t, 25320, "abcd");
    printf("t(abcd) = %d \n", tree_getValue(t, "abcd"));
    tree_add(t, 15420, "abcgzeqg");
    printf("t(abcgzeqg) = %d \n", tree_getValue(t, "abcgzeqg"));
    tree_add(t, 15425, "abcqggqg");
    printf("t(abc) = %d \n", tree_getValue(t, "abcqggqg"));
    tree_add(t, 254240, "abcdrqgr");
    printf("t(abcdrqgr) = %d \n", tree_getValue(t, "abcdrqgr"));
    tree_add(t, 14520, "abcqrg");
    printf("t(abcqrg) = %d \n", tree_getValue(t, "abcqrg"));
    tree_add(t, 15425, "qrgrabc");
    printf("t(qrgrabc) = %d \n", tree_getValue(t, "qrgrabc"));
    tree_add(t, 24520, "abgqrcd");
    printf("t(abgqrcd) = %d \n", tree_getValue(t, "abgqrcd"));
    tree_add(t, 24, "cddz");
    printf("t(cddz) = %d \n", tree_getValue(t, "cddz"));
    tree_add(t, 24, "cddz");
    printf("size : %d \n", tree_size(t));
    //FILE * in = stdin;
    //char * buf = malloc(20 * sizeof(char));
    //char *s = fgets(buf,20,in);

    //printf(s);
    //char *d = trim(s,' ');
    AST *op = AST_new();
    AST_add(op, t, "a");
    AST_add(op, t, "=");
    AST_add(op, t, "101");
    AST_add(op, t, "%");
    AST_add(op, t, "100");
    AST_apply(t, op);
    printf(" a = 5 + 2 * 7 * 7 + 100 * 5 = %d \n", tree_getValue(t, "a"));
    AST_free(op);
    tree_free(t);
    printf("MALLOC_COUNTER = %d \n", MALLOC_COUNTER);
    return 0;
}


struct Node {
    char id;
    int data;
    Node *left;
    Node *middle;
    Node *right;
};


struct Tree {
    Node *root;
};

struct Token {
    char *data;
    enum tokenType type;
};

struct ASN {
    Token token;
    int result;
    ASN *left;
    ASN *right;
};

struct AST {
    ASN *root;
};

static Token token_new(char *s, enum tokenType type) {
    Token token = {.data = trim(s), .type = type};
    return token;
}

static void token_free(Token t) {
    free(t.data);
    MALLOC_COUNTER--;
}

static int isNum(char *string) {
    size_t len = strlen(string);
    for (int i = 0; i < len; i++) {
        if (!isdigit(string[i])) {
            return 0;
        }
    }
    return 1;
}

static AST *AST_new() {
    AST *tree = malloc(sizeof(AST));
    MALLOC_COUNTER++;
    if (tree == NULL) return NULL;
    tree->root = NULL;
    return tree;
}

static ASN *ASN_new(Tree *t, char *s) {
    ASN *node = malloc(sizeof(ASN));
    MALLOC_COUNTER++;
    if (node == NULL) return NULL;
    if (s[0] == DEFAULT_OP || isAnOperator(s[0])) {
        node->result = 0;
        node->token = token_new(s, OPERATOR);
    } else if (isNum(s)) {
        node->token = token_new(s, NUMBER);
        node->result = atoi(s);
    } else {
        node->result = tree_getValue(t, s);
        node->token = token_new(s, VAR);
    }
    node->right = NULL;
    node->left = NULL;
    return node;
}

static int isAnOperator(const char s) {
    return s == '*' || s == '/' || s == '+' || s == '-' || s == '=' || s == '%';
}


static void ASN_add(Tree *storage, ASN **asn, char *s) {
    if (*asn == NULL) {
        *asn = ASN_new(storage, "");
        (*asn)->left = ASN_new(storage, s);
    } else if ((*asn)->token.data[0] == DEFAULT_OP) {
        strncpy((*asn)->token.data, s, strlen(s));
    } else if ((*asn)->right == NULL || isNum(s) || !isHigher((*asn)->token.data[0], s[0])) {
        ASN_add(storage, &(*asn)->right, s);
    } else {
        ASN *tmp = *asn;
        *asn = ASN_new(storage, s);
        (*asn)->left = tmp;
    }
}

static void AST_add(AST *ast, Tree *storage, char *s) {
    if (ast == NULL || storage == NULL || isspace(s[0])) {
        return;
    }
    ASN_add(storage, &ast->root, s);
}

static ASN *ASN_free(ASN *n) {
    if (n != NULL) {
        ASN_free(n->right);
        ASN_free(n->left);
        token_free(n->token);
        free(n);
        MALLOC_COUNTER--;
    }
    return n = NULL;
}

static int op(ASN *asn, Tree *storage, int left, int right) {
    switch (asn->token.data[0]) {
        case '=':
            EQUALS(storage, right, asn->left->token.data);
            return right;
        case '*':
            return MULTIPLICATION(right, left);
        case '-' :
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

static int ASN_apply(Tree *storage, ASN *asn) {
    if (asn == NULL) return 0;
    int left = ASN_apply(storage, asn->left);
    int right = ASN_apply(storage, asn->right);
    return asn->result = op(asn, storage, left, right);
}

static void AST_apply(Tree *storage, AST *ast) {
    if (storage == NULL || ast == NULL) return;
    ASN_apply(storage, ast->root);
}

static Tree *tree_new() {
    Tree *t = malloc(sizeof(Tree));
    MALLOC_COUNTER++;
    if (t == NULL) return NULL;
    t->root = NULL;
    return t;
}


static Node *node_new(char id, int data) {
    Node *n = malloc(sizeof(Node));
    MALLOC_COUNTER++;
    if (n == NULL) {
        return NULL;
    }
    n->id = id;
    n->data = data;
    n->left = n->middle = n->right = NULL;
    return n;
}


static int tree_getValue(Tree *t, char *string) {
    int len = (int) strlen(string);
    if (len < 1) return -1;
    return node_getValue(&t->root, string);
}


static int node_getValue(Node **node, char *string) {
    int len = (int) strlen(string);
    if (len < 1) return -1;
    if (node == NULL || *node == NULL) return -2;
    if (string[0] < (*node)->id) {
        return node_getValue(&(*node)->left, string);
    }
    if (string[0] > (*node)->id) {
        return node_getValue(&(*node)->right, string);
    }
    if (len == 1) {
        return (*node)->data;
    }
    return node_getValue(&(*node)->middle, string + 1);
}


static int tree_add(Tree *t, int n, char *string) {
    if (t == NULL) return -2;
    int len = (int) strlen(string);
    if (len < 1) return -1;
    node_add(&t->root, n, string);
    return 0;
}


static int node_add(Node **pNode, const int n, const char *string) {
    int len = (int) strlen(string);
    if (len < 1) return -1;
    if (*pNode == NULL) {
        *pNode = node_new(string[0], 0);
    }
    if (string[0] < (*pNode)->id) {
        return node_add(&(*pNode)->left, n, string);
    }
    if (string[0] > (*pNode)->id) {
        return node_add(&(*pNode)->right, n, string);
    }
    if (len > 1) {
        return node_add(&(*pNode)->middle, n, string + 1);
    }
    (*pNode)->data = n;
    return 0;
}


static int tree_size(Tree *t) {
    if (t->root == NULL) return 0;
    return node_size(t->root);
}


static int node_size(Node *node) {
    if (node == NULL) return 0;
    int size = 0;
    size += node_size(node->left);
    size += node_size(node->middle);
    size += node_size(node->right);
    return ++size;
}


static void node_free(Node *n) {
    if (n == NULL) return;
    node_free(n->left);
    node_free(n->middle);
    node_free(n->right);
    free(n);
    MALLOC_COUNTER--;
}


static void tree_free(Tree *t) {
    node_free(t->root);
    free(t);
    MALLOC_COUNTER--;
}

static int isHigher(const char a, const char b) {
    if (!isAnOperator(a)) return 0;
    if (!isAnOperator(b)) return 1;
    return priority(a) > priority(b);
}


static unsigned short priority(const char a) {
    switch (a) {
        case '=' :
            return 1;
        case '+' :
        case '-' :
            return 2;
        case '/':
        case '*':
        case '%':
            return 3;
        default:
            return 0;
    }
}

static char *trim(const char *s) {
    int size = 0;
    int slength = (int) strlen(s);
    int i = 0;
    for (; i < slength; i++) {
        if (!isspace(s[i])) break;
    }
    for (; i < slength; i++) {
        if (isspace(s[i])) break;
        size++;
    }
    char *ret = malloc(size * sizeof(char) + 1);
    MALLOC_COUNTER++;
    if (ret == NULL) return NULL;
    char *tmp = memmove(ret, s, sizeof(char) * size);
    if (tmp == NULL) return NULL;
    ret = tmp;
    ret[size] = '\0';
    return ret;
}

static AST *AST_free(AST *ast) {
    if (ast != NULL) {
        ASN_free(ast->root);
        free(ast);
        MALLOC_COUNTER--;
    }
    return NULL;
}
