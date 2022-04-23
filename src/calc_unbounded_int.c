#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
//#include "unbounded_int.h"

#define printErr(c) (fprintf(stderr,"%s in file %s in line %d\n %s\n", strerror(errno) ,__FILE__,__LINE__, (c)))
#define DEFAULT_OP '\0'
#define MULTIPLICATION(a, b) ((a)*(b))
#define ADDITION(a, b) ((a)+(b))
#define SUBSTRACTION(a, b) ((a)-(b))
#define DIVISION(a, b) ((a)/(b))
#define EQUALS(t, a, b) (tree_add((t),(a),(b)))
#define MODULO(a, b) ((a)%(b))

#define BUFFER_SIZE 256

static char *FILE_NAME = "stdin";
static int FILE_LINE = 1;
static int MALLOC_COUNTER = 0;

/**
 * Token types.
 */
typedef enum TokenType {
    NUMBER, VAR, OPERATOR, VOID
} TokenType;
/**
 * Lexers Tokens.
 */
typedef struct Token {
    char *data;
    enum TokenType type;
} Token;

typedef struct Stack {
    char *buffer;
    int capacity;
    int length;
} Stack;

Stack *stack_new() {
    Stack *stack = malloc(sizeof(Stack));
    MALLOC_COUNTER++;
    if (stack == NULL) {
        printErr("");
        MALLOC_COUNTER--;
        return NULL;
    }
    stack->buffer = calloc(BUFFER_SIZE, sizeof(char));
    MALLOC_COUNTER++;
    if (stack->buffer == NULL) {
        printErr("");
        free(stack);
        MALLOC_COUNTER -= 2;
        return NULL;
    }
    stack->capacity = BUFFER_SIZE;
    stack->length = 0;
    return stack;
}

Stack *stack_clear(Stack *stack) {
    free(stack->buffer);
    MALLOC_COUNTER--;
    stack->buffer = calloc(BUFFER_SIZE, sizeof(char));
    MALLOC_COUNTER++;
    if (stack->buffer == NULL) {
        printErr("");
        free(stack);
        MALLOC_COUNTER -= 2;
        return NULL;
    }
    stack->capacity = BUFFER_SIZE;
    stack->length = 0;
    return stack;
}

Stack *stack_free(Stack *stack) {
    free(stack->buffer);
    free(stack);
    MALLOC_COUNTER -= 2;
    return NULL;
}

int stack_add(Stack *stack, const char e) {
    if (stack == NULL) {
        printErr("");
        return 0;
    }
    if (stack->length + 1 >= stack->capacity) {
        char *tmp = realloc(stack->buffer, 2 * stack->capacity);
        if (tmp == NULL) {
            printErr("");
            return 0;
        }
        stack->capacity *= 2;
        stack->buffer = tmp;
    }
    stack->buffer[stack->length++] = e;
    return 1;
}

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
 *          0 otherwise
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
 *          0 otherwise.
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
static void AST_add(AST *ast, Tree *storage, Token token);

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
static ASN *ASN_new(Tree *t, Token token);

/**
 * Add a new Node in the AST according to the string given in argument. Manage the operator priority.
 * @param asn The root of the sub AST.
 * @param storage. The storage tree.
 * @param s The node value.
 */
static void ASN_add(Tree *storage, ASN **asn, Token token);

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
static Token token_new(char *s, size_t len, enum TokenType type);

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
static char *trim(const char *s, size_t len);

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

static void connect(FILE **in, FILE **out, int argc, char **argv);

static void disconnect(FILE **in, FILE **out);

static int str_equals(char *s1, char *s2);

int run(FILE *in, FILE *out, AST *ast, Tree *storage);

AST *AST_clear(AST *ast);

int treatment(Stack *stack, AST *ast, Tree *storage, TokenType type);

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

struct ASN {
    Token token;
    int result;
    ASN *left;
    ASN *right;
};

struct AST {
    ASN *root;
};

//C (sur CLion) n'admet pas plusieurs mains, dc pour tester ce fichier, décommenter ce main et commenter le main de calc_unbounded_int.c
int main(int argc, char **argv) {
    FILE *in = NULL, *out = NULL;
    connect(&in, &out, argc, argv);
    if (in == NULL) {
        if (out != NULL) fclose(out);
        printErr("Can't open the source file");
        exit(EXIT_FAILURE);
    }

    if (out == NULL) {
        printErr("Can't open the destination file");
        disconnect(&in, &out);
        exit(EXIT_FAILURE);
    }
    AST *ast = AST_new();
    if (ast == NULL) {
        printErr("");
        disconnect(&in, &out);
        exit(EXIT_FAILURE);
    }
    Tree *storage = tree_new();
    if (storage == NULL) {
        printErr("");
        AST_free(ast);
        disconnect(&in, &out);
        exit(EXIT_FAILURE);
    }
    int err = run(in, out, ast, storage);
    AST_free(ast);
    tree_free(storage);
    disconnect(&in, &out);
    printf("MALLOC_COUNTER = %d", MALLOC_COUNTER);
    exit((err) ? EXIT_SUCCESS : EXIT_FAILURE);
}

int run(FILE *in, FILE *out, AST *ast, Tree *storage) {
    TokenType last, current;
    last = current = VOID;
    Stack *stack = stack_new();
    if (stack == NULL) {
        printErr("");
        return 0;
    }
    int c;
    while ((c = fgetc(in)) != EOF) {
        if ((char) c == '\n') {
            treatment(stack, ast, storage, current);
            stack = stack_clear(stack);
            AST_apply(storage, ast);
            ast = AST_clear(ast);
            FILE_LINE += 1;
            if (stack == NULL) {
                return 0;
            }
            last = current = VOID;
        } else if (isspace(c)) {
            current = VOID;
        } else if (isalpha(c)) {
            current = VAR;
        } else if (isAnOperator((char) c)) {
            current = OPERATOR;
        } else if (isdigit(c) && (current == VOID || current == OPERATOR)) {
            current = NUMBER;
        }
        if (current != last && last != VOID) {
            if (!treatment(stack, ast, storage, last)) {
                stack_free(stack);
                return 0;
            }
            stack = stack_clear(stack);
            if (stack == NULL) {
                return 0;
            }
        }
        if (current != VOID) {
            if (!stack_add(stack, (char) c)) {
                stack_free(stack);
                return 0;
            }
        }
        last = current;
    }
    /*Lecture -> detection de token et sauvegarde de la position, si token change, sauvegarde de la longueur parcourue
    relecture du token avec puis trim et direction l'AST
     Fin de ligne ? AST_apply(), AST_clear()
     Boucle
     If EOF, retour a la derniere position connue, mis dans l'AST apply(), puis sortie fonction.
     if print apply puis fprintf(out, a = getValue())
     1er temps, if print, recherche du nom de la variable ->fprintf(out, a = getValue())
     Lecture par block ?
     if (error), print return false
     */
    return 1;
}

//Todo: gestion des erreurs du parser
//todo: gestion du print (type = VAR avec str_equals)
int treatment(Stack *stack, AST *ast, Tree *storage, TokenType type) {
    if (type == VOID) {
        return 1;
    }
    Token token = token_new(stack->buffer, stack->length, type);
    AST_add(ast, storage, token);
    return 1;
}

AST *AST_clear(AST *ast) {
    if (ast->root != NULL) {
        ASN_free(ast->root);
    }
    ast->root = NULL;
    return ast;
}

static void connect(FILE **in, FILE **out, int argc, char **argv) {
    if (argc < 2) {
        *in = stdin;
        *out = stdout;
        return;
    }
    for (int i = 1; i < argc; i++) {
        if ((i + 1 < argc)) {
            if ((str_equals(argv[i], "-i")) && !(str_equals(argv[i + 1], "-o"))) {
                if (in != NULL) fclose(*in);
                *in = fopen(argv[++i], "r");
                if (*in == NULL) {
                    printErr("");
                }
                FILE_NAME = argv[i];
            } else if ((str_equals(argv[i], "-o")) && !(str_equals(argv[i + 1], "-i"))) {
                if (*out != NULL) fclose(*out);
                *out = fopen(argv[++i], "w+");
            }
        }
    }
    if (*in == NULL) {
        *in = stdin;
    }
    if (*out == NULL) {
        *out = stdout;
    }
}

static void disconnect(FILE **in, FILE **out) {
    if (in != NULL) {
        fclose(*in);
        *in = NULL;
    }
    if (out != NULL) {
        fclose(*out);
        *out = NULL;
    }
}

static int str_equals(char *s1, char *s2) {
    int l1 = (int) strlen(s1);
    int l2 = (int) strlen(s2);
    if (l1 != l2) return 0;
    for (int i = 0; i < l1; i++) {
        if (s1[i] != s2[i]) {
            return 0;
        }
    }
    return 1;
}

static Token token_new(char *s, size_t len, enum TokenType type) {
    Token token = {.data = trim(s, len), .type = type};
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

static ASN *ASN_new(Tree *t, Token token) {
    ASN *node = malloc(sizeof(ASN));
    MALLOC_COUNTER++;
    if (node == NULL) return NULL;
    switch (token.type) {
        case OPERATOR:
            node->result = 0;
            node->token = token;
            break;
        case NUMBER:
            node->token = token;
            node->result = atoi(token.data);
            break;
        case VAR:
            node->result = tree_getValue(t, token.data);
            node->token = token;
            break;
        default:
            node->result = 0;
            node->token = token;
            break;
    }

    node->right = NULL;
    node->left = NULL;
    return node;
}

static int isAnOperator(const char s) {
    return s == '*' || s == '/' || s == '+' || s == '-' || s == '=' || s == '%';
}


static void ASN_add(Tree *storage, ASN **asn, Token token) {
    if (*asn == NULL) {
        *asn = ASN_new(storage, token_new("", 0, OPERATOR));
        (*asn)->left = ASN_new(storage, token);
    } else if ((*asn)->token.data[0] == DEFAULT_OP) {
        strncpy((*asn)->token.data, token.data, strlen(token.data));
    } else if ((*asn)->right == NULL || token.type == NUMBER || !isHigher((*asn)->token.data[0], token.data[0])) {
        ASN_add(storage, &(*asn)->right, token);
    } else {
        ASN *tmp = *asn;
        *asn = ASN_new(storage, token);
        (*asn)->left = tmp;
    }
}

static void AST_add(AST *ast, Tree *storage, Token token) {
    if (ast == NULL || storage == NULL || isspace(token.data[0]) || token.type == VOID) {
        return;
    }
    ASN_add(storage, &ast->root, token);
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
    if (len < 1) return 0;
    return node_getValue(&t->root, string);
}


static int node_getValue(Node **node, char *string) {
    int len = (int) strlen(string);
    if (len < 1) return -1;
    if (node == NULL || *node == NULL) return 0;
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

static char *trim(const char *s, size_t len) {
    int size = 0;
    int i = 0;
    for (; i < len; i++) {
        if (!isspace(s[i])) break;
    }
    for (; i < len; i++) {
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
