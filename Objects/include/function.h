//
// Created by Kevin on 05/07/2022.
//
#ifndef C_PROJECT_FUNCTION_H
#define C_PROJECT_FUNCTION_H

#include "unbounded_int.h"
#include "ast.h"
#include "hashmap.h"
#include <string.h>

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
    UnboundedInt *argv;
    char **argn;

    int (*func)(int, UnboundedInt *, char **);
};

Function function_new(char *name, RetType type, int (*function)(int, UnboundedInt *, char **),
                      unsigned short requestedArguments);

void function_free(Function f);

int function_apply(HashMap *map, char *name, ASN *node);


#define FUNCTION_NULL (Function) {.name = NULL, .requested = 0, .argc = 0, .retType= VOID_TYPE, .argv = NULL, .argn = NULL}
#endif //C_PROJECT_FUNCTION_H
