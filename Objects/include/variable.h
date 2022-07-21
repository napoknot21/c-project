#ifndef VARIABLE
#define VARIABLE
#include "unbounded_int.h"

typedef struct Variable Variable;
typedef enum VarType VarType;
union Value {
	UnboundedInt ui;
	char* string;
	char character;
	void* null;
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

Variable Variable_new(char* name, void* value, VarType type);

void Variable_free(Variable var);
#endif // VARIABLE
