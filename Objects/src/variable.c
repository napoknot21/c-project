#include "variable.h"
#include "string.h"
#include "lib.h"
#include "stdlib.h"

#include "unbounded_int.h"

int Variable_isCopy(Variable a, Variable b) {
	if (a.mType != b.mType || !str_equals(a.mName, b.mName)) {
		return 0;
	}
	return Variable_equals(a, b);
}

int Variable_equals(Variable a, Variable b) {
	if (a.mType != b.mType) {
		return 0;
	}
	switch (a.mType) {
		case VARTYPE_INT:
			return UnboundedInt_cmpUnboundedInt(a.mValue.ui, b.mValue.ui) == 0;
		case VARTYPE_CHARACTER:
			return a.mValue.character == b.mValue.character;
		case VARTYPE_STRING:
			return str_equals(a.mValue.string, b.mValue.string);
		case VARTYPE_NULL:
			return 1;
		default:
			return 0;
	}
}

int Variable_hashMapUtil_cmp(const char *name, void *var) {
	return str_equals(name, (*(Variable *) var).mName);
}

void Variable_hashMapUtil_free(void *var) {
	Variable_free(*(Variable *) var);
}

Variable Variable_new(char *name, void *value, VarType type) {
	Variable new;

	size_t len = strlen(name);
	new.mName = malloc(sizeof(char) * (len + 1));
	strncpy(new.mName, name, len);
	new.mName[len] = '\0';

	new.mType = type;

	switch (type) {
		case VARTYPE_INT:
			new.mValue.ui = *(UnboundedInt *) value;
			break;
		case VARTYPE_CHARACTER:
			new.mValue.character = *(char *) value;
			break;
		case VARTYPE_NULL:
			new.mValue.null = NULL;
			break;
		case VARTYPE_STRING:
			new.mValue.string = (char *) value;
	}
	return new;
}

void Variable_free(Variable var) {
	if (var.mType == VARTYPE_NULL) {
		return;
	}
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

Variable Variable_cpy(Variable var) {
	if (var.mType == VARTYPE_NULL) {
		return VAR_NULL;
	}
	size_t len = strlen(var.mName);
	char *name = malloc(sizeof(char) * (len + 1));
	name = strncpy(name, var.mName, len);
	name[len] = '\0';
	switch (var.mType) {
		case VARTYPE_INT:
			UnboundedInt tmp = unboundedInt_cpy(var.mValue.ui);
			return Variable_new(name, &tmp, var.mType);
		case VARTYPE_STRING:
			len = strlen(var.mName);
			char *cpy = malloc(sizeof(char) * (len + 1));
			cpy = strncpy(cpy, var.mValue.string, len);
			cpy[len] = '\0';
			return Variable_new(name, cpy, var.mType);
		case VARTYPE_CHARACTER:
			char ccpy = var.mValue.character;
			return Variable_new(name, &ccpy, var.mType);
		default:
			return VAR_NULL;
	}
}
