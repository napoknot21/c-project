//
// Created by Kevin on 05/07/2022.
//
#include "function.h"
#include "lib.h"
#include <stdlib.h>
#include <string.h>

#include "exec_error.h"

Function Function_new(char *name, RetType type, int (*function)(int, Variable *, char **),
                      unsigned short requestedArguments) {


	Function f = {
		f.mName = trim(name, strlen(name)),
		f.mRequested = requestedArguments,
		f.mArgc = 0,
		f.mRetType = type,
		f.mArgv = calloc(requestedArguments + 1, sizeof(UnboundedInt)),
		f.mArgn = malloc(requestedArguments * sizeof(char *)),
		f.mFunc = function
	};
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
	size_t len = strlen(fSrc->mName);
	char *name = malloc(len * sizeof(char));
	if (name == NULL) {
		perror_src("");
		return NULL;
	}
	char *tmp = strncpy(name, fSrc->mName, len);
	if (tmp == NULL) {
		free(name);
		return NULL;
	}
	tmp[len] = '\0';
	name = tmp;

	cpy->mName = name;
	cpy->mArgc = fSrc->mArgc;
	cpy->mRequested = fSrc->mRequested;
	cpy->mRetType = fSrc->mRetType;
	cpy->mFunc = fSrc->mFunc;
	cpy->mArgv = calloc(fSrc->mRequested + 1, sizeof(UnboundedInt));
	cpy->mArgn = malloc(fSrc->mRequested * sizeof(char *));
	return cpy;
}

void Function_free(Function f) {
	//free(f.mArgv);
	//free(f.mArgn);
	//free(f.mName);
}
