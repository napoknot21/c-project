//
// Created by Kevin on 05/07/2022.
//
#ifndef C_PROJECT_FUNCTION_H
#define C_PROJECT_FUNCTION_H

#include "variable.h"
#include "ast.h"
#include "hashmap.h"

typedef enum FunctionType {
	FUNCTION_STD,
	FUNCTION_DUMMY,
	FUNCTION_NONE,
	FUNCTION_CALL
} FunctionType;

typedef enum RetType {
	RETURN_NUM,
	RETURN_VOID
} RetType;


typedef struct Function Function;

struct Function {
	char *mName;
	unsigned short mRequested;
	unsigned short mArgc;
	RetType mRetType;
	Variable *mArgv;

	int (*mFunc)(int argc, Variable *argv);
};

Function Function_new(char *name, RetType type, int (*function)(int, Variable *),
                      unsigned short requestedArguments);

void Function_free(Function f);

int Function_apply(HashMap *map, char *name, ASN *node);

int Function_hashMapUtil_cmp(const char *name, void *func);

void Function_hashMapUtil_free(void *func);

void *Function_HashMapUtil_cpy(void *dst, void *src, size_t size);

#define FUNCTION_NULL (Function) {.mName = NULL, .mRequested = 0, .mArgc = 0, .mRetType= RETURN_VOID, .mArgv = NULL}
#endif //C_PROJECT_FUNCTION_H
