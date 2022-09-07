//
// Created by Kevin on 05/07/2022.
//
#include "function.h"
#include "lib.h"
#include <stdlib.h>
#include <string.h>

#include "exec_error.h"

Function Function_new(char *name, RetType type, int (*function)(int, Variable *),
                      unsigned short requestedArguments) {


	Function f = {
		f.mName = trim(name, strlen(name)),
		f.mRequested = requestedArguments,
		f.mArgc = 0,
		f.mRetType = type,
		f.mArgv = malloc((requestedArguments + 1) * sizeof(Variable)),
		f.mFunc = function
	};
	for (size_t i = 0; i<= f.mRequested; i++) {
		f.mArgv[i] = VAR_NULL;
	}
	return f;
}

int Function_hashMapUtil_cmp(const char *name, void *func) {
	return str_equals(name, (*(Function *) func).mName);
}

void Function_hashMapUtil_free(void *func) {
	Function_free(*(Function *) func);
}

void *Function_HashMapUtil_cpy(void *dst, void *src, size_t size) {
	Function *fSrc = src;
	Function *cpy = dst;
	char* name = str_cpy(fSrc->mName);
	cpy->mName = name;
	cpy->mArgc = fSrc->mArgc;
	cpy->mRequested = fSrc->mRequested;
	cpy->mRetType = fSrc->mRetType;
	cpy->mFunc = fSrc->mFunc;
	cpy->mArgv = calloc(fSrc->mRequested + 1 , sizeof(Variable));
	for (size_t i = 0; i <= fSrc->mRequested; i++) {
		cpy->mArgv[i] = Variable_cpy(fSrc->mArgv[i]);
	}
	return cpy;
}

void Function_free(Function f) {
	if (f.mName == NULL) return;
	for (int i= 0; i< f.mArgc; i++) {
		Variable_free(f.mArgv[i]);
	}
	free(f.mArgv);
	free(f.mName);
}
