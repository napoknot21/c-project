//
// Created by Kevin on 05/07/2022.
//
#include "function.h"
#include <stdlib.h>

static Function
function_new(char *name, RetType type, int (*function)(int, UnboundedInt *, char **), 
            unsigned short requestedArguments) {
    Function f = {
            .name = trim(name, strlen(name)),
            .requested = requestedArguments,
            .func = function,
            .argc = 0,
            .retType = type,
            .argv = calloc(requestedArguments + 1, sizeof(UnboundedInt)),
            .argn = malloc(requestedArguments * sizeof(char *))
    };
    return f;
}

static void function_free(Function f) {
    //free(f.argv);
    //free(f.argn);
    free(f.name);
}

