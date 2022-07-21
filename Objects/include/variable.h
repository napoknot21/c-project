#ifndef VARIABLE
#define VARIABLE
#include "function.h"
#include "unbounded_int.h"

typedef struct Variable Variable;
typedef enum VarType VarType;
union Value {
	UnboundedInt ui;
	char* string;
	char character;
	void* null;
	Function function;
};

enum VarType {
	VARTYPE_STRING, VARTYPE_INT, VARTYPE_CHARACTER, VARTYPE_NULL
};
struct Variable {
	char* mName;
	union Value mValue;
	VarType mType;
};

#define VAR_NULL ((Variable){.mName = "", .mValue.null = NULL, .mType = VARTYPE_NULL})

int Variable_equals(Variable a, Variable b);

int Variable_isCopy(Variable a, Variable b);

int Variable_hashMapUtil_cmp(const char *name, void *var);

void Variable_hashMapUtil_free(void *var);

Variable Variable_new(char *name, void *value, VarType type);

void Variable_free(Variable var);
#endif // VARIABLE
