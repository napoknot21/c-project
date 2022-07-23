//
// Created by Kevin on 05/07/2022.
//
#include "function.h"
#include "lib.h"
#include <stdlib.h>
#include <string.h>

#include "exec_error.h"

Function *Function_new(char *name, RetType type, int (*function)(int, UnboundedInt *, char **),
                       unsigned short requestedArguments) {

	Function *f = malloc(sizeof(Function));
	if (f == NULL) {
		perror_src("");
		return NULL;
	}

	f->mName = trim(name, strlen(name));
	f->mRequested = requestedArguments;
	f->mFunc = function;
	f->mArgc = 0;
	f->mRetType = type;
	f->mArgv = calloc(requestedArguments + 1, sizeof(UnboundedInt));
	f->mArgn = malloc(requestedArguments * sizeof(char *));

	return f;
}

int Function_hashMapUtil_cmp(const char *name, void *func) {
	return str_equals(name, (*(Function *) func).mName);
}

void Function_hashMapUtil_free(void *func) {
	Function_free(*(Function *) func);
}

void Function_free(Function f) {
	//free(f.mArgv);
	//free(f.mArgn);
	free(f.mName);
}
