//
// Created by Kevin on 05/07/2022.
//
#ifndef C_PROJECT_FUNCTION_H
#define C_PROJECT_FUNCTION_H
#include "unbounded_int.h"
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
    unbounded_int *argv;
    char **argn;

    int (*func)(int, unbounded_int *, char **);
};

static Function
function_new(char *name, RetType type, int (*function)(int, unbounded_int *, char **),
             unsigned short requestedArguments);

static void function_free(Function f);


#define FUNCTION_NULL (Function) {.name = NULL, .requested = 0, .argc = 0, .retType= VOID_TYPE, .argv = NULL, .argn = NULL}
#endif //C_PROJECT_FUNCTION_H
