#include <stdio.h>

#include <malloc.h>

#include <string.h>

#include <stdlib.h>

#include <ctype.h>

#include <math.h>
#include <limits.h>
#include <errno.h>

#include "unbounded_int.h"
#include "lib.h"
#include "hashmap.h"
#include "execerror.h"
#include "function.h"
#include "token.h"
#include "ast.h"
#include "buffer.h"
#include "appinformations.h"

/**
 * Name of the open file.
 */

/**
 * Line position of the reader in the open file.
 */

static FILE *OUT;

static int CALL_ID = 0;

#define BUFFER_SIZE 16
#define INCR_CALL (CALL_ID = CALL_ID + 19 % INT_MAX)

/* ####################################################################################################################
 * STD functions
 */

static int std_print(int argc, unbounded_int *argv, char **argn);

static int std_pow(int argc, unbounded_int *argv, char **argn);

static int std_exit(int argc, unbounded_int *argv, char **argn);

static int std_abs(int argc, unbounded_int *argv, char **argn);

static int std_fact(int argc, unbounded_int *argv, char **argn);



/* #####################################################################################################################
 * Storage tree's functions.
 */

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
static int tree_add(Tree *t, unbounded_int n, char *string);

/**
 * Gets the value of the given variable.
 *
 * @param t The storage tree.
 * @param string The variable's name
 * @return the data value if the value is found. <br>
 *          0 otherwise
 */
static unbounded_int tree_getValue(Tree *t, char *string);

/**
 * Variables storage node.
 */
typedef struct Node Node;

/**
 * Create a node in a tree.
 *
 * @param id the node's id
 * @return the pointer of the created node.
 */
static Node *node_new(char id);

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
static int node_add(Node **pNode, unbounded_int n, const char *string);

/**
 * Gets the value of the given variable.
 *
 * @param node The current node.
 * @param string The variable's name.
 * @return the data value if the value is found. <br>
 *          0 otherwise.
 */
static unbounded_int node_getValue(Node **node, char *string);

/**
 * Open files according to the arguments given to the program. By default, in = stdin and out = stdout
 *
 * @param in The input file stream.
 * @param out The output file stream.
 * @param argc The argv length.
 * @param argv The args which will be treated.
 */
static void connect(FILE **in, FILE **out, int argc, char **argv);

/**
 * Disconnect the input and output file stream.
 *
 * @param in the input file stream.
 * @param out The output file stream.
 */
static void disconnect(FILE **in, FILE **out);

/**
 * Check is the strings given in arguments are equals.
 *
 * @param s1 The first string
 * @param s2 The second string
 * @return true if the string are equals, false otherwise.
 */
static int str_equals(const char *s1, const char *s2);

/**
 * Parse the input file stream, do the instruction and print the result on the output file stream.
 *
 * @param in The input file stream.
 * @param out The output file stream.
 * @param ast The AST.
 * @param storage The storage tree.
 * @return true if everything went fine, false otherwise.
 */
static int parseFile(FILE *in, AST *ast, Tree *storage, HashMap *map);

static int parseString(char *in, AST *ast, Tree *storage, HashMap *map, unbounded_int *astResult);

/**
 * Treats the token stored in the buffer.
 *
 * @param pBuffer The parser's buffer.
 * @param ast The AST.
 * @param storage The storage tree.
 * @param type The token's type
 * @param func An address to a int
 * @param out The output file stream.
 * @return 0 if an error occurred. <br>
 *         -1 if something was printed on the output file stream. <br>
 *         -2 if the token is empty or the token's type is VOID <br>
 *         1 otherwise.
 */
static int
treatment(Buffer *pBuffer, AST *ast, Tree *storage, TokenType type, int *func, HashMap *map, Function *function,
          int *argsStart);


/* #####################################################################################################################
 * Other functions.
 */

static int function_apply(HashMap *map, char *name, ASN *node);

static Function buildCalledFunction(HashMapData data, char *buffer);

static int functionTreatment(int c, Buffer *buffer, AST *ast, Tree *storage, int *isFunc, int *argsStart, HashMap *map,
                             Function *function);

/* #####################################################################################################################
 * struct definitions
 */
struct Node {
    char id;
    unbounded_int data;
    Node *left;
    Node *middle;
    Node *right;
};

struct Tree {
    Node *root;
};

void load_stdlib(HashMap *map) {
    Function print = function_new("print", VOID_TYPE, std_print, 1);
    Function pow = function_new("pow", NUM_TYPE, std_pow, 2);
    Function exit = function_new("exit", VOID_TYPE, std_exit, 0);
    Function abs = function_new("abs", NUM_TYPE, std_abs, 1);
    Function fact = function_new("fact", NUM_TYPE, std_fact, 1);
    hashMap_put(map, print.name,&print);
    hashMap_put(map, pow.name,&pow);
    hashMap_put(map, exit.name,&exit);
    hashMap_put(map,abs.name, &abs);
    hashMap_put(map, fact.name,&fact);
}

/* #####################################################################################################################
 * Main function.
 */
int main(int argc, char **argv) {
    FILE *in = NULL, *out = NULL;
    connect(&in, &out, argc, argv);
    OUT = out;
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
    HashMap *map = HashMap_new();
    if (map == NULL) {
        printErr("");
        AST_free(ast);
        free(storage);
        disconnect(&in, &out);
        exit(EXIT_FAILURE);
    }
    load_stdlib(map);
    int err = parseFile(in, ast, storage, map);
    AST_free(ast);
    tree_free(storage);
    hashMap_free(map);
    disconnect(&in, &out);
    if (EXIT_REQUEST == -1) exit(EXIT_FAILURE);
    exit((EXIT_REQUEST == 1 || err) ? EXIT_SUCCESS : EXIT_FAILURE);
}


/* #####################################################################################################################
 *  parser's functions.
 */

static int
parse(int c, AST *ast, Tree *storage, HashMap *map, TokenType *last, TokenType *current, Buffer *stack, int *isFunc,
      int *argStart, Function *function, AST *argAst, unbounded_int *astResult) {
    if (c == 4) {
        EXIT_REQUEST = 1;
        return 0;
    }
    if ((char) c == '\n' || (char) c == '\r') {
        int val = treatment(stack, ast, storage, *current, isFunc, map, function, argStart);
        if (*isFunc) {
            functionTreatment(c, stack, argAst, storage, isFunc, argStart, map, function);
        }
        if (val == 0 || (val > 0 && !AST_apply(storage, ast, map))) {
            return 0;
        }
        stack = buffer_clear(stack);
        if (astResult != NULL) *astResult = ast->root->result;
        ast = AST_clear(ast);
        if (stack == NULL) {
            return 0;
        }
        FILE_LINE++;
        *last = *current = VOID;
        isFunc = 0;
    } else if (*isFunc) {
        functionTreatment(c, stack, argAst, storage, isFunc, argStart, map, function);
    } else if (isspace(c)) {
        *current = VOID;
    } else if (isalpha(c)) {
        *current = VAR;
    } else if (isdigit(c) && (*current == VOID || *current == OPERATOR)) {
        *last = *current = NUMBER;
    } else if (isAnOperator((char) c)) {
        if (*last == OPERATOR) {
            perror_file(INVALID_SYNTAX,FILE_NAME,FILE_LINE);
            return 0;
        }
        *current = OPERATOR;
    }
    if (*current != *last && *last != VOID) {
        if (!treatment(stack, ast, storage, *last, isFunc, map, function, argStart)) {
            return 0;
        }
        stack = buffer_clear(stack);
        if (stack == NULL) {
            return 0;
        }
    }
    if (*current != VOID) {
        if (!buffer_add(stack, (char) c)) {
            return 0;
        }
    }
    return 1;
}

static int functionTreatment(int c, Buffer *buffer, AST *ast, Tree *storage, int *isFunc, int *argsStart, HashMap *map,
                             Function *function) {
    if (function == NULL) {
        perror_file(INTERNAL, FILE_NAME,FILE_LINE);
        return -1;
    }
    if (function->argc > function->requested) {
        return 0;
    }
    if (buffer->length == 0 && (c == '(')) {
        *argsStart = 1;
        return 1;
    }
    if ((*argsStart == 1 && c == ')') || ((char) c == '\n') || ((char) c == '\r')) {
        *argsStart = 0; //End of function
        *isFunc = 0;
        c = ',';
    }
    if (c == '(') {
        buffer_add(buffer, (char) c);
        *argsStart += 1;
        return 1;
    }
    if (c == ')') {
        buffer_add(buffer, (char) c);
        *argsStart -= 1;
        return 1;
    }
    if (c == ',') {
        unbounded_int astResult = ll2unbounded_int(0);
        char *in = trim(buffer->buffer, buffer->length);
        function->argn[function->argc] = in;
        int val = parseString(in, ast, storage, map, &astResult);
        if (!val) return 1;
        function->argc++;
        function->argv[function->argc - 1] = astResult;
        buffer = buffer_clear(buffer);
        ast = AST_clear(ast);
        if (buffer == NULL || ast == NULL) {
            printErr("");
            return 0;
        }
        if (!*isFunc) {
            hashMap_put(map, function, CALL);
            *function = FUNCTION_NULL;
        }
        return 1;
    }
    buffer_add(buffer, ((char) c));
    return 1;
}

static int parseString(char *in, AST *ast, Tree *storage, HashMap *map, unbounded_int *astResult) {
    int len = (int) strlen(in);
    if (len == 0) {
        return 0;
    }
    int isFunc = 0;
    int argsStart = 0;
    TokenType last, current;
    last = current = VOID;
    Buffer *stack = buffer_new();
    if (stack == NULL) {
        printErr("");
        return 0;
    }
    AST *argAST = AST_new();
    if (argAST == NULL) {
        buffer_free(stack);
        printErr("");
        return 0;
    }
    Function *function = malloc(sizeof(Function));
    if (function == NULL) {
        buffer_free(stack);
        AST_free(ast);
        printErr("");
        return 0;
    }
    for (unsigned int i = 0; i < len; i++) {
        int c = (int) (in[i]);
        int b = parse(c, ast, storage, map, &last, &current, stack, &isFunc, &argsStart, function, argAST, astResult);
        if (!b) {
            buffer_free(stack);
            AST_free(argAST);
            free(function);
            return 0;
        }
        last = current;
        if (EXIT_REQUEST != 0) break;
    }
    int val = treatment(stack, ast, storage, current, &isFunc, map, function, &argsStart);
    if (val == 0 || (val > 0 && !AST_apply(storage, ast, map))) {
        buffer_free(stack);
        AST_free(argAST);
        free(function);
        return 0;
    }
    if (astResult != NULL && ast->root != NULL) *astResult = ast->root->result;
    buffer_free(stack);
    AST_free(argAST);
    free(function);
    return 1;
}

static int parseFile(FILE *in, AST *ast, Tree *storage, HashMap *map) {
    int isFunc = 0;
    int argsStart = 0;
    TokenType last, current;
    last = current = VOID;
    Buffer *stack = buffer_new();
    if (stack == NULL) {
        printErr("");
        return 0;
    }
    AST *argAST = AST_new();
    if (argAST == NULL) {
        buffer_free(stack);
        printErr("");
        return 0;
    }
    Function *function = malloc(sizeof(Function));
    if (function == NULL) {
        buffer_free(stack);
        AST_free(ast);
        printErr("");
        return 0;
    }
    int c;
    while ((c = fgetc(in)) != EOF) {
        if (!parse(c, ast, storage, map, &last, &current, stack, &isFunc, &argsStart, function, argAST, NULL)) {
            buffer_free(stack);
            AST_free(argAST);
            free(function);
            return 0;
        }
        last = current;
        if (EXIT_REQUEST != 0) break;
    }
    int val = treatment(stack, ast, storage, current, &isFunc, map, function, &argsStart);
    if (val == 0 || (val > 0 && !AST_apply(storage, ast, map))) {
        buffer_free(stack);
        AST_free(argAST);
        free(function);
        return 0;
    }
    buffer_free(stack);
    AST_free(argAST);
    free(function);
    return 1;
}

static int treatment
        (Buffer *pBuffer, AST *ast, Tree *storage, TokenType type, int *func, HashMap *map, Function *function,
         int *argsStart) {
    char *buffer = pBuffer->buffer;
    size_t len = pBuffer->length;
    if (type == VOID || buffer[0] == '\0') {
        return (AST_hasFunction(ast)) ? 1 : -2;  //void value
    }
    HashMapData data = hashMap_get(map, buffer);
    if (data.mType != NONE) {
        *func = 1;
        Function new = buildCalledFunction(data, trim(buffer, len));
        *function = new;
        hashMap_put(map, function, CALL);
        Token token = token_new(new.name, strlen(new.name), FUNCTION);
        if (!AST_add(ast, storage, token)) {
            token_free(token);
            return 0; //error value
        }
        *argsStart = 1;
        buffer_clear(pBuffer);
        return 1;
    }
    if (type == NUMBER && buffer[0] == '=') {
        Token token = token_new(buffer, 1, OPERATOR);
        if (!AST_add(ast, storage, token)) {
            token_free(token);
            return 0; //error value
        }
        buffer++;
        len--;
    }
    if ((type == NUMBER) && !isSignOrNumber(buffer[0])) {
        perror_file(MISSING_BLANK,FILE_NAME,FILE_LINE);
        return 0;   //error value
    }
    Token token = token_new(buffer, len, type);
    if (!AST_add(ast, storage, token)) {
        token_free(token);
        return 0; //error value
    }
    return 1; //add value
}

static Function buildCalledFunction(HashMapData data, char *buffer) {
    Function result = FUNCTION_NULL;
    Function f = *(Function *) data.mData;
    int dLen = (int) strlen(f.name);
    INCR_CALL;
    char *r = intToString(CALL_ID);
    int rLen = (int) strlen(r);
    char *line = intToString((int) FILE_LINE);
    int len = (int) strlen(line);
    char *newName = malloc((len + dLen + rLen + 1) * sizeof(char));
    if (newName == NULL) {
        return result;
    }
    char *tmp = memmove(newName, f.name, dLen * sizeof(char));
    if (tmp == NULL) {
        free(newName);
        free(line);
        free(r);
        return result;
    }
    newName = tmp;
    tmp = memmove(&newName[dLen], line, len * sizeof(char));
    if (tmp == NULL) {
        free(newName);
        free(line);
        free(r);
        return result;
    }
    tmp = memmove(&newName[dLen + len], r, rLen * sizeof(char));
    if (tmp == NULL) {
        free(newName);
        free(line);
        free(r);
        return result;
    }
    newName[dLen + len + rLen] = '\0';
    result = function_new(newName, f.retType, f.func, f.requested);
    return result;
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

static Buffer *buffer_new() {
    Buffer *buffer = malloc(sizeof(Buffer));
    if (buffer == NULL) {
        printErr("");
        return NULL;
    }
    buffer->buffer = calloc(BUFFER_SIZE, sizeof(char));
    if (buffer->buffer == NULL) {
        printErr("");
        free(buffer);
        return NULL;
    }
    buffer->capacity = BUFFER_SIZE;
    buffer->length = 0;
    return buffer;
}

static Buffer *buffer_clear(Buffer *buffer) {
    free(buffer->buffer);
    buffer->buffer = calloc(BUFFER_SIZE, sizeof(char));
    if (buffer->buffer == NULL) {
        printErr("");
        free(buffer);
        return NULL;
    }
    buffer->capacity = BUFFER_SIZE;
    buffer->length = 0;
    return buffer;
}

static Buffer *buffer_free(Buffer *buffer) {
    free(buffer->buffer);
    free(buffer);
    return NULL;
}

static int buffer_add(Buffer *buffer, const char e) {
    if (buffer == NULL) {
        printErr("");
        return 0;
    }
    if (buffer->length + 1 >= buffer->capacity) {
        char *tmp = realloc(buffer->buffer, 2 * buffer->capacity);
        if (tmp == NULL) {
            printErr("");
            return 0;
        }
        buffer->capacity *= 2;
        buffer->buffer = tmp;
    }
    buffer->buffer[buffer->length++] = e;
    return 1;
}

/* ####################################################################################################################
 * STD functions
 */

static int std_print(int argc, unbounded_int *argv, char **argn) {
    char *result = unbounded_int2string(argv[0]);
    fprintf(OUT, "%s = %s \n", argn[0], result);
    argv[argc] = UNBOUNDED_INT_ERROR;
    //free(result);
    //unbounded_int_free(argv[0]);
    //free(argn[0]);
    return 0;
}

static int std_pow(int argc, unbounded_int *argv, char **argn) {
    argv[argc] = unbounded_int_pow(argv[0], argv[1]);
    return 1;

}

static int std_abs(int argc, unbounded_int *argv, char **argn) {
    argv[argc] = unbounded_int_abs(argv[0]);
    //free(argn[0]);
    //unbounded_int_free(argv[0]);
    return 1;
}

static int std_exit(int argc, unbounded_int *argv, char **argn) {
    argv[argc] = UNBOUNDED_INT_ERROR;
    EXIT_REQUEST = 1;
    return 1;
}

static int std_fact(int argc, unbounded_int *argv, char **argn) {
    argv[argc] = unbounded_int_fact(argv[0]);
    //(argv[0]);
    //free(argn[0]);
    return 1;
}


/* #####################################################################################################################
 * AST functions
 */

/* #####################################################################################################################
 * Storage tree's functions.
 */
static Tree *tree_new() {
    Tree *t = malloc(sizeof(Tree));
    if (t == NULL) return NULL;
    t->root = NULL;
    return t;
}

static Node *node_new(char id) {
    Node *n = malloc(sizeof(Node));
    if (n == NULL) {
        return NULL;
    }
    n->id = id;
    n->data = ll2unbounded_int(0);
    n->left = n->middle = n->right = NULL;
    return n;
}

static int tree_add(Tree *t, unbounded_int n, char *string) {
    if (t == NULL) return -2;
    int len = (int) strlen(string);
    if (len < 1) return -1;
    node_add(&t->root, n, string);
    return 0;
}

static int node_add(Node **pNode, const unbounded_int n, const char *string) {
    int len = (int) strlen(string);
    if (len < 1) return -1;
    if (*pNode == NULL) {
        *pNode = node_new(string[0]);
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

static unbounded_int tree_getValue(Tree *t, char *string) {
    int len = (int) strlen(string);
    if (len < 1) return ll2unbounded_int(0);
    return node_getValue(&t->root, string);
}

static unbounded_int node_getValue(Node **node, char *string) {
    int len = (int) strlen(string);
    if (len < 1) return ll2unbounded_int(0);
    if (node == NULL || *node == NULL) return ll2unbounded_int(0);
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

static void tree_free(Tree *t) {
    node_free(t->root);
    free(t);
}

static void node_free(Node *n) {
    if (n == NULL) return;
    node_free(n->left);
    node_free(n->middle);
    node_free(n->right);
    //unbounded_int_free(n->data);
    //free(n);
}

/* #####################################################################################################################
 * Function's functions
 */
static int function_apply(HashMap *map, char *name, ASN *node) {
    HashMapData data = hashMap_get(map, name);
    if (data.mType == NONE) return 0;
    Function f = *(Function *) data.mData;
    if (f.requested > f.argc) {
        if (!EXIT_REQUEST) perror_file(MISSING_ARGUMENTS,FILE_NAME,FILE_LINE);
        EXIT_REQUEST = -1;
        return 0;
    } else if (f.requested < f.argc) {
        if (!EXIT_REQUEST) perror_file(TOO_MANY_ARGUMENTS, FILE_NAME, FILE_LINE);
        EXIT_REQUEST = -1;
        return 0;
    }
    f.func(f.argc, f.argv, f.argn);
    if (node != NULL && f.retType != VOID_TYPE) node->result = f.argv[f.argc];
    HashMap_remove(map, f.name);
    return 1;
}


/* #####################################################################################################################
 * HashMap's function
 */


/* #####################################################################################################################
 * Other's functions
 */
