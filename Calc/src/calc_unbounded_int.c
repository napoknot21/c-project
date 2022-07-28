#include <stdio.h>

#include <malloc.h>

#include <string.h>

#include <stdlib.h>

#include <ctype.h>

#include <limits.h>

#include "unbounded_int.h"
#include "lib.h"
#include "hashmap.h"
#include "exec_error.h"
#include "function.h"
#include "token.h"
#include "ast.h"
#include "Buffer.h"
#include "app_informations.h"
#include "variable.h"

/**
 * Name of the open file.
 */

/**
 * Line position of the reader in the open file.
 */

static FILE *OUT;

static int CALL_ID = 0;

#define INCR_CALL (CALL_ID = CALL_ID + 19 % INT_MAX)

/* ####################################################################################################################
 * FUNCTION_STD functions
 */

static int std_print(int argc, Variable *argv, char **argn);

static int std_pow(int argc, Variable *argv, char **argn);

static int std_exit(int argc, Variable *argv, char **argn);

static int std_abs(int argc, Variable *argv, char **argn);

static int std_fact(int argc, Variable *argv, char **argn);


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
 * @param s1 The mFirst string
 * @param s2 The second string
 * @return true if the string are equals, false otherwise.
 */

/**
 * Parse the input file stream, do the instruction and print the result on the output file stream.
 *
 * @param in The input file stream.
 * @param ast The AST.
 * @param storage The storage tree.
 * @return true if everything went fine, false otherwise.
 */
static int parseFile(FILE *in, AST *ast, HashMap *storage, HashMap *functionsMap);

static int parseString(char *in, AST *ast, HashMap *storage, HashMap *map, Variable *astResult);

/**
 * Treats the token stored in the mBuffer.
 *
 * @param pBuffer The parser's buffer.
 * @param ast The AST.
 * @param storage The storage tree.
 * @param type The token's type
 * @param func An address to a int
 * @return 0 if an error occurred. <br>
 *         -1 if something was printed on the output file stream. <br>
 *         -2 if the token is empty or the token's type is VOID <br>
 *         1 otherwise.
 */
static int
treatment(Buffer *pBuffer, AST *ast, HashMap *storage, TokenType type, int *func, HashMap *functionsMap,
          Function *function, int *argsStart);


/* #####################################################################################################################
 * Other functions.
 */

static int function_apply(HashMap *map, char *name, ASN *node);

static Function buildCalledFunction(Function data);

static int functionTreatment(int c, Buffer *buffer, AST *ast, HashMap *storage, int *isFunc, int *argsStart,
                             HashMap *map,
                             Function *function);

void load_stdlib(HashMap *map) {
	Function print = Function_new("print", RETURN_VOID, std_print, 1);
	Function pow = Function_new("pow", RETURN_NUM, std_pow, 2);
	Function exit = Function_new("exit", RETURN_VOID, std_exit, 0);
	Function abs = Function_new("abs", RETURN_NUM, std_abs, 1);
	Function fact = Function_new("fact", RETURN_NUM, std_fact, 1);
	HashMap_put(map, print.mName, &print);
	HashMap_put(map, pow.mName, &pow);
	HashMap_put(map, exit.mName, &exit);
	HashMap_put(map, abs.mName, &abs);
	HashMap_put(map, fact.mName, &fact);

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
		perror_src("Can't open the source file");
		exit(EXIT_FAILURE);
	}

	if (out == NULL) {
		perror_src("Can't open the destination file");
		disconnect(&in, &out);
		exit(EXIT_FAILURE);
	}
	AST *ast = AST_new();
	if (ast == NULL) {
		perror_src("");
		disconnect(&in, &out);
		exit(EXIT_FAILURE);
	}
	HashMap *storage = HashMap_new(sizeof(Variable), Variable_hashMapUtil_cmp, Variable_hashMapUtil_free);
	if (storage == NULL) {
		perror_src("");
		AST_free(ast);
		disconnect(&in, &out);
		exit(EXIT_FAILURE);
	}
	HashMap *functionsMap = HashMap_new(sizeof(Function), Function_hashMapUtil_cmp, Function_hashMapUtil_free);
	if (functionsMap == NULL) {
		perror_src("");
		AST_free(ast);
		HashMap_free(storage);
		disconnect(&in, &out);
		exit(EXIT_FAILURE);
	}
	load_stdlib(functionsMap);
	int err = parseFile(in, ast, storage, functionsMap);
	AST_free(ast);
	HashMap_free(storage);
	HashMap_free(functionsMap);
	disconnect(&in, &out);
	if (EXIT_REQUEST == -1) exit(EXIT_FAILURE);
	exit((EXIT_REQUEST == 1 || err) ? EXIT_SUCCESS : EXIT_FAILURE);
}


/* #####################################################################################################################
 *  parser's functions.
 */

static int
parse(int c, AST *ast, HashMap *storage, HashMap *functionsMap, TokenType *last, TokenType *current, Buffer *stack,
      int *isFunc, int *argStart, Function *function, AST *argAst, Variable *astResult) {
	if (c == 4) {
		EXIT_REQUEST = 1;
		return 0;
	}
	if ((char) c == '\n' || (char) c == '\r') {
		int val = treatment(stack, ast, storage, *current, isFunc, functionsMap, function, argStart);
		if (*isFunc) {
			functionTreatment(c, stack, argAst, storage, isFunc, argStart, functionsMap, function);
		}
		if (val == 0 || (val > 0 && !AST_apply(storage, ast, functionsMap))) {
			return 0;
		}
		stack = Buffer_clear(stack);
		if (astResult != NULL) *astResult = ast->root->result;
		ast = AST_clear(ast);
		if (stack == NULL) {
			return 0;
		}
		FILE_LINE++;
		*last = *current = VOID;
		isFunc = 0;

	}
	else if (*isFunc) {
		functionTreatment(c, stack, argAst, storage, isFunc, argStart, functionsMap, function);
	}
	else if (isspace(c)) {
		*current = VOID;
	}
	else if (isalpha(c)) {
		*current = VAR;
	}
	else if (isdigit(c) && (*current == VOID || *current == OPERATOR)) {
		*last = *current = NUMBER;
	}
	else if (isAnOperator(c)) {
		if (*last == OPERATOR) {
			perror_file(INVALID_SYNTAX);
			return 0;
		}
		*current = OPERATOR;
	}
	if (*current != *last && *last != VOID) {
		if (!treatment(stack, ast, storage, *last, isFunc, functionsMap, function, argStart)) {
			return 0;
		}
		stack = Buffer_clear(stack);
		if (stack == NULL) {
			return 0;
		}
	}
	if (*current != VOID) {
		if (!Buffer_add(stack, c)) {
			return 0;
		}
	}
	return 1;
}

static int functionTreatment(int c, Buffer *buffer, AST *ast, HashMap *storage, int *isFunc, int *argsStart,
                             HashMap *map,
                             Function *function) {
	if (function == NULL) {
		perror_file(INTERNAL);
		return -1;
	}
	if (function->mArgc > function->mRequested) {
		return 0;
	}
	if (buffer->mLength == 0 && (c == '(')) {
		*argsStart = 1;
		return 1;
	}
	if ((*argsStart == 1 && c == ')') || ((char) c == '\n') || ((char) c == '\r')) {
		*argsStart = 0; //End of function
		*isFunc = 0;
		c = ',';
	}
	if (c == '(') {
		Buffer_add(buffer, c);
		*argsStart += 1;
		return 1;
	}
	if (c == ')') {
		Buffer_add(buffer, c);
		*argsStart -= 1;
		return 1;
	}
	if (c == ',') {
		Variable astResult = VAR_NULL;
		char *in = trim(buffer->mBuffer, buffer->mLength);
		function->mArgn[function->mArgc] = in;
		int val = parseString(in, ast, storage, map, &astResult);
		if (!val) return 1;
		function->mArgc++;
		function->mArgv[function->mArgc - 1] = astResult;
		buffer = Buffer_clear(buffer);
		ast = AST_clear(ast);
		if (buffer == NULL || ast == NULL) {
			perror_src("");
			return 0;
		}
		if (!*isFunc) {
			HashMap_put(map, function->mName, function);
			*function = FUNCTION_NULL;
		}
		return 1;
	}
	Buffer_add(buffer, c);
	return 1;
}

static int parseString(char *in, AST *ast, HashMap *storage, HashMap *map, Variable *astResult) {
	size_t len = strlen(in);
	if (len == 0) {
		return 0;
	}
	int isFunc = 0;
	int argsStart = 0;
	TokenType last, current;
	last = current = VOID;
	Buffer *stack = Buffer_new();
	if (stack == NULL) {
		perror_src("");
		return 0;
	}
	AST *argAST = AST_new();
	if (argAST == NULL) {
		Buffer_free(stack);
		perror_src("");
		return 0;
	}

	Function *function = malloc(sizeof(Function));
	if (function == NULL) {
		Buffer_free(stack);
		AST_free(ast);
		perror_src("");
		return 0;
	}
	for (size_t i = 0; i < len; i++) {
		int c = in[i];
		int b = parse(c, ast, storage, map, &last, &current, stack, &isFunc, &argsStart, function, argAST, astResult);
		if (!b) {
			Buffer_free(stack);
			AST_free(argAST);
			return 0;
		}
		last = current;
		if (EXIT_REQUEST != 0) break;
	}
	int val = treatment(stack, ast, storage, current, &isFunc, map, function, &argsStart);
	if (val == 0 || (val > 0 && !AST_apply(storage, ast, map))) {
		Buffer_free(stack);
		AST_free(argAST);
		return 0;
	}
	if (astResult != NULL && ast->root != NULL) *astResult = ast->root->result;
	Buffer_free(stack);
	AST_free(argAST);
	return 1;
}

static int parseFile(FILE *in, AST *ast, HashMap *storage, HashMap *functionsMap) {
	int isFunc = 0;
	int argsStart = 0;
	TokenType last, current;
	last = current = VOID;
	Buffer *stack = Buffer_new();
	if (stack == NULL) {
		perror_src("");
		return 0;
	}
	AST *argAST = AST_new();
	if (argAST == NULL) {
		Buffer_free(stack);
		perror_src("");
		return 0;
	}
	Function *function = malloc(sizeof(Function));
	if (function == NULL) {
		Buffer_free(stack);
		AST_free(ast);
		perror_src("");
		return 0;
	}
	int c;
	while ((c = fgetc(in)) != EOF) {
		if (!parse(c, ast, storage, functionsMap, &last, &current, stack, &isFunc, &argsStart, function, argAST,
		           NULL)) {
			Buffer_free(stack);
			AST_free(argAST);
			return 0;
		}
		last = current;
		if (EXIT_REQUEST != 0) break;
	}
	int val = treatment(stack, ast, storage, current, &isFunc, functionsMap, function, &argsStart);
	if (val == 0 || (val > 0 && !AST_apply(storage, ast, functionsMap))) {
		Buffer_free(stack);
		AST_free(argAST);
		return 0;
	}
	Buffer_free(stack);
	AST_free(argAST);
	return 1;
}

static int treatment
(
	Buffer *pBuffer, AST *ast, HashMap *storage, TokenType type,
	int *func, HashMap *functionsMap, Function *function, int *argsStart) {


	char *buffer = pBuffer->mBuffer;
	size_t len = pBuffer->mLength;
	if (type == VOID || buffer[0] == '\0') {
		return (AST_hasFunction(ast)) ? 1 : -2;  //void value
	}
	Function *data = HashMap_get(functionsMap, buffer);
	if (data != NULL) {
		*func = 1;
		Function new = buildCalledFunction(*data);
		*function = new;
		HashMap_put(functionsMap, new.mName, function);
		Token token = Token_new(new.mName, strlen(new.mName), FUNCTION);
		Variable value = VAR_NULL;
		if (!AST_add(ast, value, token)) {
			Token_free(token);
			return 0; //error value
		}
		*argsStart = 1;
		return 1;
	}
	if (type == NUMBER && buffer[0] == '=') {
		Token token = Token_new(buffer, 1, OPERATOR);
		Variable *tmp = HashMap_get(storage, token.mData);
		Variable value = (tmp == NULL) ? VAR_NULL : *tmp;
		if (!AST_add(ast, value, token)) {
			Token_free(token);
			return 0; //error value
		}
		buffer++;
		len--;
	}
	if ((type == NUMBER) && !isSignOrNumber(buffer[0])) {
		perror_file(MISSING_BLANK);
		return 0;   //error value
	}
	Token token = Token_new(buffer, len, type);
	Variable *tmp = HashMap_get(storage, token.mData);
	Variable value = (tmp == NULL) ? VAR_NULL : *tmp;
	if (!AST_add(ast, value, token)) {
		Token_free(token);
		return 0; //error value
	}
	return 1; //add value
}

static Function buildCalledFunction(Function f) {
	size_t dLen = strlen(f.mName);
	INCR_CALL;
	char *r = intToString(CALL_ID);
	size_t rLen = strlen(r);
	char *line = intToString(FILE_LINE);
	size_t len = strlen(line);
	char *newName = malloc((len + dLen + rLen + 1) * sizeof(char));
	if (newName == NULL) {
		return FUNCTION_NULL;
	}
	char *tmp = memmove(newName, f.mName, dLen * sizeof(char));
	if (tmp == NULL) {
		free(newName);
		free(line);
		free(r);
		return FUNCTION_NULL;
	}
	newName = tmp;
	tmp = memmove(&newName[dLen], line, len * sizeof(char));
	if (tmp == NULL) {
		free(newName);
		free(line);
		free(r);
		return FUNCTION_NULL;
	}
	tmp = memmove(&newName[dLen + len], r, rLen * sizeof(char));
	if (tmp == NULL) {
		free(newName);
		free(line);
		free(r);
		return FUNCTION_NULL;
	}
	newName[dLen + len + rLen] = '\0';
	return Function_new(newName, f.mRetType, f.mFunc, f.mRequested);
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
					perror_src("");
				}
				FILE_NAME = argv[i];
			}
			else if ((str_equals(argv[i], "-o")) && !(str_equals(argv[i + 1], "-i"))) {
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

/* ####################################################################################################################
 * FUNCTION_STD functions
 */

static int std_print(int argc, Variable *argv, char **argn) {
	argv[argc] = VAR_NULL;
	char *result;
	char *format;
	switch (argv[0].mType) {
		case VARTYPE_INT:
			format = "%s = %s \n";
			result = UnboundedInt_toString(argv[0].mValue.ui);
			break;
		case VARTYPE_STRING:
			format = "%s = %s \n";
			result = argv[0].mValue.string;
			break;
		case VARTYPE_CHARACTER:
			format = "%s = %c \n";
			result = &argv[0].mValue.character;
			break;
		default:
			perror_file(UNKNOWN_VARTYPE);
			return 0;
	}
	fprintf(OUT, format, argn[0], result);

	//free(result);
	//UnboundedInt_free(mArgv[0]);
	//free(mArgn[0]);
	return 0;
}

static int std_pow(int argc, Variable *argv, char **argn) {
	if (argv[0].mType == VARTYPE_INT && argv[1].mType == VARTYPE_INT) {
		UnboundedInt tmp = UnboundedInt_pow(argv[0].mValue.ui, argv[1].mValue.ui);
		argv[argc] = Variable_new("", &tmp, VARTYPE_INT);
		return 1;
	}
	return 0;
}

static int std_abs(int argc, Variable *argv, char **argn) {
	if (argv[0].mType == VARTYPE_INT) {
		UnboundedInt tmp = UnboundedInt_abs(argv[0].mValue.ui);
		argv[argc] = Variable_new("", &tmp, VARTYPE_INT);
		return 1;
	}
	//free(mArgn[0]);
	//Variable_free(mArgv[0]);
	return 0;
}

static int std_exit(int argc, Variable *argv, char **argn) {
	argv[argc] = VAR_NULL;
	EXIT_REQUEST = 1;
	return 1;
}

static int std_fact(int argc, Variable *argv, char **argn) {
	if (argv[0].mType == VARTYPE_INT) {
		UnboundedInt tmp = UnboundedInt_fact(argv[0].mValue.ui);
		argv[argc] = Variable_new("", &tmp, VARTYPE_INT);
		return 1;
	}
	return 1;
}
