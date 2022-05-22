#include <stdio.h>

#include <malloc.h>

#include <string.h>

#include <stdlib.h>

#include <ctype.h>

#include <math.h>
#include <limits.h>
#include <errno.h>

#include "../headers/unbounded_int.h"

/**
 * Parser and lexer errors.
 */
enum ERROR {
    INVALID_OPERATOR, INVALID_SYNTAX, MISSING_BLANK, UNKNOWN_CHARACTER, INTERNAL, TOO_MANY_ARGUMENTS, MISSING_ARGUMENTS
};

/**
 * Print on the stderr the corresponding message according to the code given in argument.
 * @param error The error's code.
 * @return The error's message.
 */
static char *error_getMessage(enum ERROR error);

/**
 * Name of the open file.
 */
static char *FILE_NAME = "stdin";
/**
 * Line position of the reader in the open file.
 */
static size_t FILE_LINE = 1;
static int MALLOC_COUNTER = 0;
static FILE *OUT;
static int EXIT_REQUEST = 0;
static int CALL_ID = 0;

#define pERROR(error)(fprintf(stderr, "%s in file %s in line %li\n", error_getMessage(error), FILE_NAME, FILE_LINE))
#define printErr(c)(fprintf(stderr, "%s in file %s in line %d\n %s\n", strerror(errno), __FILE__, __LINE__, (c)))
#define DEFAULT_OP '\0'
#define MULTIPLICATION(a, b)(unbounded_int_produit(a,b))
#define ADDITION(a, b)(unbounded_int_somme(a,b))
#define SUBSTRACTION(a, b)(unbounded_int_difference(a,b))
#define DIVISION(a, b)(unbounded_int_division(a, b))
#define EQUALS(t, a, b)(tree_add((t), (a), (b)))
#define MODULO(a, b)((a) % (b))

#define BUFFER_SIZE 16
#define HASHMAP_INITIAL_SIZE 11
#define HASHMAP_MIN_RATIO ((float)0.25)
#define HASHMAP_MAX_RATIO ((float)0.75)
#define HASH_A ((sqrt(5)-1)/2)
#define INCR_CALL (CALL_ID = CALL_ID + 19 % INT_MAX)





/* #####################################################################################################################
 * Token's functions.
 */

/**
* Token types.
*/
typedef enum TokenType {
    NUMBER, VAR, OPERATOR, VOID, FUNCTION
} TokenType;

/**
 * Lexers Tokens.
 */
typedef struct Token {
    char *data;
    enum TokenType type;
} Token;

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




/* #####################################################################################################################
 * Parser's buffer's functions.
*/

/**
 * Parser Buffer.
*/
typedef struct Buffer {
    char *buffer;
    int capacity;
    int length;
} Buffer;

/**
 * Create a dynamic buffer.
 * @return The buffer pointer.
 */
static Buffer *buffer_new();

/**
 * Clear the buffer and puts its capacity to the BUFFER_SIZE.
 * @param buffer The buffer pointer.
 * @return the buffer if everything went fine, NULL otherwise
 */
static Buffer *buffer_clear(Buffer *buffer);

/**
 * Free the buffer.
 *
 * @param buffer The buffer which will be free.
 * @return NULL.
 */
static Buffer *buffer_free(Buffer *buffer);

/**
 * Add the given element in the given buffer
 * @param buffer The buffer
 * @param e The element which will be added to the buffer
 * @return 1 if the element is added to the buffer, 0 otherwise
 */
static int buffer_add(Buffer *buffer, char e);


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


/*#####################################################################################################################
 * Function's functions
 */
typedef enum FuncType {
    STD, DUMMY, NONE, CALL
} FuncType;

typedef enum RetType {
    NUM_TYPE, VOID_TYPE
} RetType;


typedef struct Function Function;

struct Function {
    char *name;
    unsigned short requested;
    unsigned short argc;
    RetType retType;
    unbounded_int *argv;
    char **argn;

    int (*func)(int, unbounded_int *, char **);
};

static Function
function_new(char *name, RetType type, int (*function)(int, unbounded_int *, char **),
             unsigned short requestedArguments);

static void function_free(Function f);

#define FUNCTION_NULL (Function) {.name = NULL, .requested = 0, .argc = 0, .retType= VOID_TYPE, .argv = NULL, .argn = NULL}

/* #####################################################################################################################
* funcHashMap's declaration
*/
#define DUMMY_DATA(h) ((HashMapData) {.hash = (h), .function = FUNCTION_NULL, .type=DUMMY})
#define NONE_DATA ((HashMapData) {.hash = 0,.function = FUNCTION_NULL, .type=NONE})

typedef struct HashMap HashMap;
typedef struct HashMapData HashMapData;

struct HashMapData {
    long long hash;
    Function function;
    FuncType type;
};
struct HashMap {
    HashMapData *data;
    size_t capacity;
    size_t keyNumber;
    size_t dummyNumber;
    float minRatio, maxRatio;
};

static void resize(HashMap *map, size_t new);

static HashMapData hashMap_get(HashMap *map, const char *name);

static HashMap *HashMap_new();

static void hashMapData_free(HashMapData data);

static long long hash(const char *name);

static long long hash1(double capacity, long long hash);

static long long hash2(long long hash);

static long long find(HashMap *map, const char *name, int flag);

static int hashMap_put(HashMap *map, Function function, FuncType type);

static void hashMap_free(HashMap *map);

__attribute__((unused)) static int HashMap_remove(HashMap *map, char *name);




/* #####################################################################################################################
 * AST functions.
 */

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


/* #####################################################################################################################
 * Parser functions.
 */
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
 * Check is the given character is a sign symbol (+ or -) or is a number
 * @param c The character that will be tested.
 * @return true if its the case, false otherwise.
 */
static int isSignOrNumber(char c);

static char *intToString(int n);


static Function buildCalledFunction(HashMapData data, char *buffer);

static int functionTreatment(int c, Buffer *buffer, AST *ast, Tree *storage, int *isFunc, int *argsStart, HashMap *map,
                             Function *function);

static int AST_hasFunction(AST *ast);

static int ASN_hasFunction(ASN *asn);

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

struct ASN {
    Token token;
    unbounded_int result;
    ASN *left;
    ASN *right;
};

struct AST {
    ASN *root;
};


void load_stdlib(HashMap *map) {
    Function print = function_new("print", VOID_TYPE, std_print, 1);
    Function pow = function_new("pow", NUM_TYPE, std_pow, 2);
    Function exit = function_new("exit", VOID_TYPE, std_exit, 0);
    Function abs = function_new("abs", NUM_TYPE, std_abs, 1);
    Function fact = function_new("fact", NUM_TYPE, std_fact, 1);
    hashMap_put(map, print, STD);
    hashMap_put(map, pow, STD);
    hashMap_put(map, exit, STD);
    hashMap_put(map, abs, STD);
    hashMap_put(map, fact, STD);
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
    printf("MALLOC_COUNTER = %d\n", MALLOC_COUNTER);
    printf("exit = %d \n", EXIT_REQUEST);
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
            pERROR(INVALID_SYNTAX);
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
        pERROR(INTERNAL);
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
        unbounded_int astResult = UNBOUNDED_INT_ERROR;
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
            hashMap_put(map, *function, CALL);
            *function = NONE_DATA.function;
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
            MALLOC_COUNTER--;
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
        MALLOC_COUNTER--;
        return 0;
    }
    if (astResult != NULL && ast->root != NULL) *astResult = ast->root->result;
    buffer_free(stack);
    AST_free(argAST);
    free(function);
    MALLOC_COUNTER--;
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
    MALLOC_COUNTER++;
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
            MALLOC_COUNTER--;
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
        MALLOC_COUNTER--;
        return 0;
    }
    buffer_free(stack);
    AST_free(argAST);
    free(function);
    MALLOC_COUNTER--;
    return 1;
}

static int treatment
(Buffer *pBuffer, AST *ast, Tree *storage, TokenType type, int *func, HashMap *map, Function *function,int *argsStart)

{
    char *buffer = pBuffer->buffer;
    size_t len = pBuffer->length;
    if (type == VOID || buffer[0] == '\0') {
        return (AST_hasFunction(ast)) ? 1 : -2;  //void value
    }
    HashMapData data = hashMap_get(map, buffer);
    if (data.type != NONE) {
        *func = 1;
        Function new = buildCalledFunction(data, trim(buffer, len));
        *function = new;
        hashMap_put(map, new, CALL);
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
        pERROR(MISSING_BLANK);
        return 0;   //error value
    }
    Token token = token_new(buffer, len, type);
    if (!AST_add(ast, storage, token)) {
        token_free(token);
        return 0; //error value
    }
    return 1; //add value
}

static int AST_hasFunction(AST *ast) {
    if (ast == NULL || ast->root == NULL) return 0;
    return ASN_hasFunction(ast->root);
}

static int ASN_hasFunction(ASN *asn) {
    if (asn->token.type == FUNCTION) return 1;
    int left = (asn->left == NULL)? 0 : ASN_hasFunction(asn->left);
    int right = (asn->right == NULL)? 0 : ASN_hasFunction(asn->right);
    return left || right;
}

static Function buildCalledFunction(HashMapData data, char *buffer) {
    Function result;
    result.func = NULL;
    int dLen = (int) strlen(data.function.name);
    INCR_CALL;
    char *r = intToString(CALL_ID);
    int rLen = (int) strlen(r);
    char *line = intToString((int) FILE_LINE);
    int len = (int) strlen(line);
    char *newName = malloc((len + dLen + rLen + 1) * sizeof(char));
    MALLOC_COUNTER++;
    if (newName == NULL) {
        return result;
    }
    char *tmp = memmove(newName, data.function.name, dLen * sizeof(char));
    if (tmp == NULL) {
        free(newName);
        free(line);
        free(r);
        MALLOC_COUNTER -= 3;
        return result;
    }
    newName = tmp;
    tmp = memmove(&newName[dLen], line, len * sizeof(char));
    if (tmp == NULL) {
        free(newName);
        free(line);
        free(r);
        MALLOC_COUNTER -= 3;
        return result;
    }
    tmp = memmove(&newName[dLen + len], r, rLen * sizeof(char));
    if (tmp == NULL) {
        free(newName);
        free(line);
        free(r);
        MALLOC_COUNTER -= 3;
        return result;
    }
    newName[dLen + len + rLen] = '\0';
    result = function_new(newName, data.function.retType, data.function.func, data.function.requested);
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

static char *error_getMessage(enum ERROR error) {
    switch (error) {
        case INVALID_SYNTAX:
            return "An invalid syntax was found";
        case INVALID_OPERATOR:
            return "An unknown operator was found";
        case INTERNAL:
            return "Internal error occurred during the execution";
        case MISSING_BLANK:
            return "A blank is missing";
        case UNKNOWN_CHARACTER:
            return "An Unknown character was found";
        case TOO_MANY_ARGUMENTS:
            return "Too many arguments were given";
        case MISSING_ARGUMENTS:
            return "Some arguments are missing";
        default:
            return "";
    }
}


/* #####################################################################################################################
 * Token's functions.
 */

static Token token_new(char *s, size_t len, enum TokenType type) {
    Token token = {.data = trim(s, len), .type = type};
    return token;
}

static void token_free(Token t) {
    if (t.type == FUNCTION) return;
    free(t.data);
    MALLOC_COUNTER--;
}


/* #####################################################################################################################
 * Parser buffer function
 */

static Buffer *buffer_new() {
    Buffer *buffer = malloc(sizeof(Buffer));
    MALLOC_COUNTER++;
    if (buffer == NULL) {
        printErr("");
        MALLOC_COUNTER--;
        return NULL;
    }
    buffer->buffer = calloc(BUFFER_SIZE, sizeof(char));
    MALLOC_COUNTER++;
    if (buffer->buffer == NULL) {
        printErr("");
        free(buffer);
        MALLOC_COUNTER -= 2;
        return NULL;
    }
    buffer->capacity = BUFFER_SIZE;
    buffer->length = 0;
    return buffer;
}

static Buffer *buffer_clear(Buffer *buffer) {
    free(buffer->buffer);
    MALLOC_COUNTER--;
    buffer->buffer = calloc(BUFFER_SIZE, sizeof(char));
    MALLOC_COUNTER++;
    if (buffer->buffer == NULL) {
        printErr("");
        free(buffer);
        MALLOC_COUNTER -= 2;
        return NULL;
    }
    buffer->capacity = BUFFER_SIZE;
    buffer->length = 0;
    return buffer;
}

static Buffer *buffer_free(Buffer *buffer) {
    free(buffer->buffer);
    free(buffer);
    MALLOC_COUNTER -= 2;
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
    fprintf(OUT, "%s = ", argn[0]);
    print_unbounded_int(argv[0]);
    return 0;
}

static int std_pow(int argc, unbounded_int *argv, char **argn) {
    argv[argc] = unbounded_int_pow(argv[0], argv[1]);
    return 1;

}

static int std_abs(int argc, unbounded_int *argv, char **argn) {
    argv[argc] = unbounded_int_abs(argv[0]);
    return 1;
}

static int std_exit(int argc, unbounded_int *argv, char **argn) {
    printf("%s \n", "here");
    argv[argc] = UNBOUNDED_INT_ERROR;
    EXIT_REQUEST = 1;
    return 1;
}

static int std_fact(int argc, unbounded_int *argv, char **argn) {
    argv[argc] = unbounded_int_fact(argv[0]);
    return 1;
}


/* #####################################################################################################################
 * AST functions
 */

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
            node->result = ll2unbounded_int(0);
            node->token = token;
            break;
        case NUMBER:
            node->token = token;
            node->result = string2unbounded_int(token.data);
            break;
        case VAR:
            node->result = tree_getValue(t, token.data);
            node->token = token;
            break;
            node->token = token;
        case FUNCTION:
            node->result = UNBOUNDED_INT_ERROR;

        default:
            node->result = UNBOUNDED_INT_ERROR;
            node->token = token;
            break;
    }

    node->right = NULL;
    node->left = NULL;
    return node;
}

static int AST_add(AST *ast, Tree *storage, Token token) {
    if (ast == NULL || storage == NULL || isspace(token.data[0]) || token.type == VOID) {
        pERROR(INTERNAL);
        return 0;
    }
    return ASN_add(storage, &ast->root, token);
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
        token_free(token);
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
    MALLOC_COUNTER--;
    return NULL;
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
        case '/':
            return DIVISION(left, right);
            /*case '%':
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
        return UNBOUNDED_INT_ERROR;
    }
    if (asn->token.data[0] == '=' && asn->left->token.type == NUMBER) { //Invalid assignation
        if (*err) pERROR(INVALID_SYNTAX);
        *err = 0;
        return UNBOUNDED_INT_ERROR;
    }
    if (asn->token.type == FUNCTION) {
        *err = function_apply(map, asn->token.data, asn);
        return asn->result;
    }
    return asn->result = op(asn, storage, left, right);
}


/* #####################################################################################################################
 * Storage tree's functions.
 */
static Tree *tree_new() {
    Tree *t = malloc(sizeof(Tree));
    MALLOC_COUNTER++;
    if (t == NULL) return NULL;
    t->root = NULL;
    return t;
}

static Node *node_new(char id) {
    Node *n = malloc(sizeof(Node));
    MALLOC_COUNTER++;
    if (n == NULL) {
        return NULL;
    }
    n->id = id;
    n->data = UNBOUNDED_INT_ERROR;
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
    if (len < 1) return UNBOUNDED_INT_ERROR;
    return node_getValue(&t->root, string);
}

static unbounded_int node_getValue(Node **node, char *string) {
    int len = (int) strlen(string);
    if (len < 1) return UNBOUNDED_INT_ERROR;
    if (node == NULL || *node == NULL) return UNBOUNDED_INT_ERROR;
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
    MALLOC_COUNTER--;
}

static void node_free(Node *n) {
    if (n == NULL) return;
    node_free(n->left);
    node_free(n->middle);
    node_free(n->right);
    free(n);
    MALLOC_COUNTER--;
}

/* #####################################################################################################################
 * Function's functions
 */

static int function_apply(HashMap *map, char *name, ASN *node) {
    HashMapData data = hashMap_get(map, name);
    if (data.type == NONE) return 0;
    Function f = data.function;
    if (f.requested > f.argc) {
        pERROR(MISSING_ARGUMENTS);
        EXIT_REQUEST = -1;
        return 0;
    } else if (f.requested < f.argc) {
        pERROR(TOO_MANY_ARGUMENTS);
        EXIT_REQUEST = -1;
        return 0;
    }
    f.func(f.argc, f.argv, f.argn);
    if (node != NULL) node->result = f.argv[f.argc];
    HashMap_remove(map, f.name);
    return 1;
}

static Function
function_new(char *name, RetType type, int (*function)(int, unbounded_int *, char **),
             unsigned short requestedArguments) {
    Function f = {.name = trim(name,
                               strlen(name)), .requested = requestedArguments, .func = function, .argc = 0, .retType = type, .argv = calloc(
            requestedArguments + 1, sizeof(int)), .argn = malloc(requestedArguments * sizeof(char *))};
    MALLOC_COUNTER += 2;
    return f;
}

static void function_free(Function f) {
    for (int i = 0; i < f.argc && i < f.requested; i++) {
        free(f.argn[i]);
        MALLOC_COUNTER--;
    }
    free(f.name);
    free(f.argn);
    free(f.argv);
    MALLOC_COUNTER -= 3;
}


/* #####################################################################################################################
 * HashMap's function
 */
static long long hash(const char *name) {
    double value = 0;
    double len = (double) strlen(name);
    for (int i = 0; i < len; i++) {
        value += (int) name[i] * (pow(31, len - i));
    }
    return (long long) value;
}

static long long hash1(double capacity, long long hash) {
    return (long long) (((double) (hash) / HASH_A) * capacity);
}

static long long hash2(long long hash) {
    return (long long) (2 * hash + 1) % LLONG_MAX;
}

static HashMap *HashMap_new() {
    HashMap *map = malloc(sizeof(HashMap));
    MALLOC_COUNTER++;
    if (map == NULL) {
        printErr("");
        return NULL;
    }
    map->minRatio = HASHMAP_MIN_RATIO;
    map->maxRatio = HASHMAP_MAX_RATIO;
    map->capacity = HASHMAP_INITIAL_SIZE;
    map->data = malloc(sizeof(HashMapData) * map->capacity);
    MALLOC_COUNTER++;
    for (int i = 0; i < map->capacity; i++) {
        map->data[i] = NONE_DATA;
    }
    MALLOC_COUNTER++;
    if (map->data == NULL) {
        printErr("");
        free(map);
        MALLOC_COUNTER -= 2;
        return NULL;
    }
    map->keyNumber = 0;
    map->dummyNumber = 0;
    return map;
}

static long long find(HashMap *map, const char *name, int flag) {
    long long hashVal = hash(name);
    long long ind = hash1((double) map->capacity, hashVal) % (long long) map->capacity;
    long long step = hash2(hashVal);
    long long dummy = -1;
    for (int i = 0; i < map->capacity; i++) {
        if (map->data[ind].type == NONE) {
            if (dummy == -1) dummy = ind;
            break;
        }
        if (map->data[ind].type == DUMMY) {
            if (dummy == -1) dummy = ind;
        } else if (map->data[ind].hash == hashVal && str_equals(map->data[ind].function.name, name)) {
            return ind;
        }
        ind = (ind + step) % (long long) map->capacity;
    }
    return (flag) ? dummy : -1;
}

static int hashMap_put(HashMap *map, Function function, FuncType type) {
    long long hashVal = hash(function.name);
    long long empty = find(map, function.name, 1);
    if (empty == -1) return 0;
    HashMapData new = {.function = function, .type = type, .hash = hashVal};
    if (map->data[empty].type == DUMMY) {
        map->dummyNumber--;
        map->keyNumber++;
    }
    if (map->data[empty].type == NONE) {
        map->keyNumber++;
    }
    map->data[empty] = new;
    if (map->dummyNumber + map->keyNumber >= (long long) ((double) map->capacity * map->maxRatio)) {
        int ratio = (map->dummyNumber < map->keyNumber) ? 2 : 1;
        resize(map, ratio * map->capacity);
    }
    return 1;
}

static HashMapData hashMap_get(HashMap *map, const char *name) {
    long long pos = find(map, name, 0);
    if (pos == -1) return NONE_DATA;
    return map->data[pos];
}

static void resize(HashMap *map, size_t newSize) {
    HashMapData *old = map->data;
    size_t oldLen = map->capacity;
    HashMapData *new = malloc(newSize * sizeof(HashMapData));
    for (int i = 0; i < newSize; i++) new[i] = NONE_DATA;
    MALLOC_COUNTER++;
    if (new == NULL) {
        printErr("");
        return;
    }
    map->data = new;
    map->capacity = newSize;
    map->keyNumber = 0;
    map->dummyNumber = 0;
    for (int i = 0; i < oldLen; i++) {
        if (old[i].type == NONE || old[i].type == DUMMY) continue;
        else hashMap_put(map, old[i].function, old[i].type);
    }
    free(old);
    MALLOC_COUNTER--;
}

static void hashMapData_free(HashMapData data) {
    if (data.type == NONE || data.type == DUMMY) return;
    function_free(data.function);
}

static int HashMap_remove(HashMap *map, char *name) {
    long long pos = find(map, name, 0);
    if (pos == -1) return -1;
    HashMapData del = map->data[pos];
    map->data[pos] = DUMMY_DATA(del.hash);
    hashMapData_free(del);
    map->dummyNumber++;
    map->keyNumber--;
    if (map->dummyNumber + map->keyNumber <= (long long) ((double) map->capacity * map->minRatio)) {
        resize(map, (map->capacity / 2));
    }
    return 1;
}

static void hashMap_free(HashMap *map) {
    if (map == NULL) return;
    for (int i = 0; i < map->capacity; i++) {
        hashMapData_free(map->data[i]);
    }
    free(map);
    MALLOC_COUNTER--;
}


/* #####################################################################################################################
 * Other's functions
 */
static int isHigher(const char a, const char b) {
    if (!isAnOperator(a)) return 0;
    if (!isAnOperator(b)) return 1;
    return priority(a) > priority(b);
}

static unsigned short priority(const char a) {
    switch (a) {
        case '=':
            return 1;
        case '+':
        case '-':
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
    int start = 0, end = 0;
    for (int i = 0; i < (int) len; i++) {
        if (!isspace(s[i])) {
            start = i;
            break;
        }
    }
    for (int i = (int) len; i >= 0; i--) {
        if (!isspace(s[i])) {
            end = i;
            break;
        }
    }
    int size = end - start;
    char *ret = malloc((size) * sizeof(char));
    MALLOC_COUNTER++;
    if (ret == NULL) {
        printErr("");
        return NULL;
    }
    char *tmp = memmove(ret, s + start, sizeof(char) * size);
    if (tmp == NULL) {
        printErr("");
        free(ret);
        MALLOC_COUNTER--;
        return NULL;
    }
    ret = tmp;
    ret[size] = '\0';
    return ret;
}

static int isSignOrNumber(char c) {
    return c == '+' || c == '-' || isdigit(c);
}

static char *intToString(int n) {
    int len = snprintf(NULL, 0, "%d", n);
    char *s = malloc(len * sizeof(char));
    if (s == NULL) return NULL;
    MALLOC_COUNTER++;
    snprintf(s, len + 1, "%d", n);
    return s;
}

static int str_equals(const char *s1, const char *s2) {
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

static int isAnOperator(const char s) {
    return s == '*' || s == '/' || s == '+' || s == '-' || s == '=' || s == '%';
}
