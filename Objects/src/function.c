//
// Created by Kevin on 05/07/2022.
//
#include "function.h"
#include "lib.h"
#include <stdlib.h>
#include <string.h>

Function Function_new(char *name, RetType type, int (*function)(int, UnboundedInt *, char **),
                      unsigned short requestedArguments) {
    Function f = {
            .mName = trim(name, strlen(name)),
            .mRequested = requestedArguments,
            .mFunc = function,
            .mArgc = 0,
            .mRetType = type,
            .mArgv = calloc(requestedArguments + 1, sizeof(UnboundedInt)),
            .mArgn = malloc(requestedArguments * sizeof(char *))
    };
    return f;
}

int Function_hashMapUtil_cmp(const char* name, void * func) {
    return str_equals(name, (*(Function*)func).mName);
}

void Function_hashMapUtil_free(void * func) {
    Function_free(*(Function*) func);
}

void Function_free(Function f) {
    //free(f.mArgv);
    //free(f.mArgn);
    free(f.mName);
}

