#ifndef VARIABLE_H
#define VARIABLE_H

#include "unbounded_int.h"

union Value {
	UnboundedInt ui;
	char *string;
	char character;
	void *null;
};

typedef enum VarType {
	VARTYPE_STRING,
	VARTYPE_INT,
	VARTYPE_CHARACTER,
	VARTYPE_NULL
} VarType;

typedef struct Variable {
	char *mName;
	union Value mValue;
	VarType mType;
} Variable;

#define VAR_NULL ((Variable){.mName = "", .mValue.null = NULL, .mType = VARTYPE_NULL})

int Variable_equals(Variable a, Variable b);

int Variable_isCopy(Variable a, Variable b);

int Variable_hashMapUtil_cmp(const char *name, void *var);

void Variable_hashMapUtil_free(void *var);

Variable Variable_new(char *name, void *value, VarType type);

void Variable_free(Variable var);

Variable Variable_cpy(Variable var);
#endif // VARIABLE_H
