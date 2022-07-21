//
// Created by Kevin on 05/07/2022.
//
#ifndef C_PROJECT_FUNCTION_H
#define C_PROJECT_FUNCTION_H

#include "unbounded_int.h"
#include "ast.h"
#include "hashmap.h"

typedef enum FunctionType {
    FUNCTION_STD, FUNCTION_DUMMY, FUNCTION_NONE, FUNCTION_CALL
} FunctionType;

typedef enum RetType {
    RETURN_NUM, RETURN_VOID
} RetType;


typedef struct Function Function;

struct Function {
    char *mName;
    unsigned short mRequested;
    unsigned short mArgc;
    RetType mRetType;
    UnboundedInt *mArgv;
    char **mArgn;

    int (*mFunc)(int, UnboundedInt *, char **); //Todo: Build an Union in order to create different functions
};

Function Function_new(char *name, RetType type, int (*function)(int, UnboundedInt *, char **),
                      unsigned short requestedArguments);

void Function_free(Function f);

int Function_apply(HashMap *map, char *name, ASN *node);

int Function_hashMapUtil_cmp(const char* name, void* func);

void Function_hashMapUtil_free(void* func);

#define FUNCTION_NULL (Function) {.mName = NULL, .mRequested = 0, .mArgc = 0, .mRetType= RETURN_VOID, .mArgv = NULL, .mArgn = NULL}
#endif //C_PROJECT_FUNCTION_H
