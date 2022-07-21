#include "variable.h"
#include "string.h"

#include "unbounded_int.h"



Variable Variable_new(char* name, void* value, VarType type) {
	Variable new;

	size_t len = strlen(name);
	new.mName = malloc(sizeof(char) * (len + 1));
	strncpy(new.mName, name, len);
	new.mName[len] = '\0';

	new.mType = type;

	switch (type) {
	case VARTYPE_INT:
		new.mValue.ui = *(UnboundedInt*) value;
		break;
	case VARTYPE_CHARACTER:
		new.mValue.character = *(char*) value;
		break;
	case VARTYPE_NULL:
		new.mValue.null = NULL;
		break;
	case VARTYPE_STRING:
		new.mValue.string = (char*) value;
	}
	return new;
}

void Variable_free(Variable var) {
	free(var.mName);
	switch (var.mType) {
	case VARTYPE_STRING:
		free(var.mValue.string);
		break;
	case VARTYPE_INT:
		UnboundedInt_free(var.mValue.ui);
		break;
	}
}
