
#include "unbounded_int.h"

#include <stdlib.h>

#include "lib.h"
#include <string.h>
#include <ctype.h>

#include "exec_error.h"
#define NUM (48)

static Number *number_new(char c) {
	if (!isdigit(c)) {
		return NULL;
	}
	Number *n = malloc(sizeof(Number));
	if (n == NULL) {
		perror_src("");
		return NULL;
	}
	n->mNext = NULL;
	n->mLast = NULL;
	n->mVal = c;
	return n;
}

static UnboundedInt addFirst(UnboundedInt u, char c) {
	Number *new = number_new(c);
	if (new == NULL) {
		perror_src("Memory insufficient");
		return u;
	}

	if (u.mLength == 0) {
		u.mFirst = new;
		u.mLast = new;
		u.mLength++;
		return u;
	}

	new->mNext = u.mFirst;
	u.mFirst->mLast = new;
	u.mFirst = new;
	u.mLength++;
	return u;

}

static UnboundedInt addLast(UnboundedInt u, char c) {
	Number *new = number_new(c);
	if (new == NULL) {
		perror_src("Memory insufficient");
		return u;
	}

	if (u.mLength == 0) {
		u.mFirst = new;
		u.mLength++;
		return u;
	}

	new->mLast = u.mLast;
	u.mLast->mNext = new;
	u.mLast = new;
	u.mLength++;
	return u;

}

UnboundedInt unboundedInt_free(UnboundedInt u) {
	Number *c = u.mFirst;
	if (c == NULL) return UNBOUNDED_INT_ERROR;
	while (c->mNext != NULL) {
		Number *n = c->mNext;
		free(c);
		c = n;
	}
	free(c);
	return UNBOUNDED_INT_ERROR;
}

static char *buildString(UnboundedInt ui, char *new, size_t length) {
	Number *current = ui.mFirst;
	for (size_t i = 0; i < length; i++) {
		new[i] = current->mVal;
	}
	new[length] = '\0';
	return new;
}

static UnboundedInt unboundedInt_cpy(UnboundedInt u) {
	UnboundedInt cpy = UNBOUNDED_INT_ERROR;
	cpy.mSign = u.mSign;
	Number *current = u.mFirst;
	while (current->mNext != NULL) {
		addLast(cpy, current->mVal);
	}
	return cpy;
}

UnboundedInt unboundedInt_newString(char *e) {
	if (isAStringNum(e) == 0) return UNBOUNDED_INT_ERROR;
	const char *str = cleanNumber(e);
	UnboundedInt res = UNBOUNDED_INT_ERROR;
	int i;
	res.mLength = 0;
	if (*str == '-' || *str == '+') {
		if (*str == '-') {
			res.mSign = '-';
		}
		else {
			res.mSign = '+';
		}
		i = 1;
	}
	else {
		res.mSign = '+';
		i = 0;
	}
	for (int j = i; j < strlen(str); j++) {
		res = addFirst(res, *(str + j));
	}
	return res;
}

UnboundedInt unboundedInt_newll(long long i) {
	UnboundedInt new = UNBOUNDED_INT_ERROR;
	int base = 10;
	new.mSign = (i >= 0) ? '+' : '-';
	long long n = llabs(i);
	while (n > 0) {
		int d = (int) (n % base);
		addFirst(new, (char) (NUM + d));
		n /= base;
	}
	return new;
}

char *unboundedInt_toString(UnboundedInt ui) {
	if (ui.mLength == 0) {
		char *new = malloc(sizeof(char));
		if (new == NULL) {
			perror_src("");
			return NULL;
		}
		new[0] = '\0';
		return new;
	}
	size_t length = ui.mLength;
	char *new;
	if (ui.mSign == '-') {
		length++;
		new = malloc((length + 1) * sizeof(char));
		if (new == NULL) {
			perror_src("");
			return NULL;
		}
		new[0] = '-';
		buildString(ui, new + 1, length - 1);
		return new;

	}
	new = malloc((length + 1) * sizeof(char));
	if (new == NULL) {
		perror_src("");
		return NULL;
	}
	return buildString(ui, new, length);
}

int unboundedInt_cmpUnboundedInt(UnboundedInt a, UnboundedInt b) {
	if (
		a.mSign == '+' && b.mSign == '-' || (a.mSign == b.mSign && ((a.mSign == '+' && a.mLength > b.mLength)
			|| (a.mSign == '-' && a.mLength < b.mLength)))
		)
	{
		return -1;
	}
	if (
		a.mSign == '-' && b.mSign == '+' || (a.mSign == b.mSign && ((a.mSign == '+' && a.mLength < b.mLength)
			|| (a.mSign == '-' && a.mLength > b.mLength)))
		)
	{
		return 1;
	}

	Number *ac = a.mFirst;
	Number *bc = a.mFirst;
	while (ac->mNext != NULL) {
		if (ac->mVal > bc->mVal) {
			return -1;
		}
		if (bc->mVal > ac->mVal) {
			return 1;
		}
		ac = ac->mNext;
		bc = bc->mNext;
	}
	return 0;
}

int unboundedInt_cmpll(UnboundedInt a, long long b) {
	UnboundedInt tmp = unboundedInt_newll(b);
	int const bool = unboundedInt_cmpUnboundedInt(a, tmp);
	unboundedInt_free(tmp);
	return bool;
}

UnboundedInt unboundedInt_add(UnboundedInt a, UnboundedInt b) {
	int base = 10;
	if (a.mLength == 0) {
		return unboundedInt_cpy(b);
	}
	if (b.mLength == 0) {
		return unboundedInt_cpy(a);
	}
	if (a.mSign == '+' && b.mSign == '-') {
		UnboundedInt b2 = { .mSign = '+', .mLength = b.mLength, .mFirst = b.mFirst, .mLast = b.mLast };
		return unboundedInt_subtract(a, b2);
	}
	if (a.mSign == '-' && b.mSign == '+') {
		UnboundedInt a2 = { .mSign = '+', .mLength = a.mLength, .mFirst = a.mFirst, .mLast = a.mLast };
		return unboundedInt_subtract(b, a2);
	}
	UnboundedInt result = UNBOUNDED_INT_ERROR;
	result.mSign = a.mSign;
	int r = 0;
	Number *ac = a.mLast;
	Number *bc = b.mLast;
	while (ac->mLast != NULL && bc->mLast != NULL) {
		int res = r + (ac->mVal - NUM) + (bc->mVal - NUM);
		if (res >= base) {
			r = res / base;
		}
		char c = (char) NUM + (res % base);
		addFirst(result, c);
		ac = ac->mLast;
		bc = bc->mLast;
	}
	while (ac->mLast != NULL) {
		int res = r + ac->mVal - NUM;
		if (res >= base) {
			r = res / base;
		}
		char c = (char) (NUM + (res % base));
		addFirst(result, c);
		ac = ac->mLast;
	}
	while (bc->mLast != NULL) {
		int res = r + bc->mVal - NUM;
		if (res >= base) {
			r = res / base;
		}
		char c = (char) (NUM + (res % base));
		addFirst(result, c);
		bc = bc->mLast;
	}
	return result;
}

UnboundedInt unboundedInt_subtract(UnboundedInt a, UnboundedInt b) {
	int base = 10;
	if (a.mLength == 0) {
		return unboundedInt_cpy(b);
	}
	if (b.mLength == 0) {
		return unboundedInt_cpy(a);
	}

	if (a.mSign == '-' && b.mSign == '+') {
		UnboundedInt b2 = { .mSign = '-', .mLength = b.mLength, .mFirst = b.mFirst, .mLast = b.mLast };
		return unboundedInt_add(a, b2);
	}

	if (a.mSign == '+' && b.mSign == '-') {
		UnboundedInt b2 = { .mSign = '+', .mLength = b.mLength, .mFirst = b.mFirst, .mLast = b.mLast };
		return unboundedInt_add(a, b2);
	}

	if (a.mSign == '-' && b.mSign == '-') {
		a = (UnboundedInt){.mSign = '+', .mLength = b.mLength, .mFirst = b.mFirst, .mLast = b.mLast};
		b = (UnboundedInt){ .mSign = '+', .mLength = a.mLength, .mFirst = a.mFirst, .mLast = a.mLast };
	}

	UnboundedInt result = UNBOUNDED_INT_ERROR;
	result.mSign = '+';
	int r = 0;
	Number *ac = a.mLast;
	Number *bc = b.mLast;
	while (ac->mLast != NULL && bc->mLast != NULL) {
		int res = (ac->mVal - NUM) - (bc->mVal - NUM) - r;
		if (res < 0) {
			r = 1;
			res += 10;
		}
		char c = (char) NUM + (res % base);
		addFirst(result, c);
		ac = ac->mLast;
		bc = bc->mLast;
	}
	while (ac->mLast != NULL) {
		int res = ac->mVal - NUM - r;
		if (res < 0) {
			r = 1;
			res += 10;
		}
		char c = (char) (NUM + (res % base));
		addFirst(result, c);
		ac = ac->mLast;
	}
	while (bc->mLast != NULL) {
		int res = r + bc->mVal - NUM;
		if (res >= base) {
			r = res / base;
		}
		char c = (char) (NUM + (res % base));
		addFirst(result, c);
		bc = bc->mLast;
	}
	if (r != 0 || a.mLength < b.mLength) {
		result.mSign = -'-';
	}
	return result;
}

UnboundedInt unboundedInt_multiply(UnboundedInt a, UnboundedInt b) {
	return UNBOUNDED_INT_ERROR;
}

UnboundedInt unboundedInt_divide(UnboundedInt a, UnboundedInt b) {
	return UNBOUNDED_INT_ERROR;
}

UnboundedInt unboundedInt_pow(UnboundedInt x, UnboundedInt n) {
	//UnboundedInt result = unboundedInt_newll(1);
	//UnboundedInt decr = unboundedInt_newll(1);
	//UnboundedInt mod = unboundedInt_newll(2);
	//if (unboundedInt_cmpll(n, 0) == 0) {
	//	return result;
	//}
	/*while (unboundedInt_cmpll(n, 0) == 1) {
		//UnboundedInt modulo = fun(n,mod);
		if (unboundedInt_cmpll(MODULO(n,mod),1) == 0) { //modulo
			result = unboundedInt_multiply(result,x);
			n = unboundedInt_subtract(n,decr);
		}
		x = unboundedInt_multiply(x,x);
		n = unboundedInt_multiply(n,mod);
	}*/
	return UNBOUNDED_INT_ERROR;
}

UnboundedInt unboundedInt_abs(UnboundedInt x) {
	UnboundedInt result = unboundedInt_cpy(x);
	result.mSign = '+';
	return result;
}

UnboundedInt unboundedInt_fact(UnboundedInt n) {
	UnboundedInt decr = unboundedInt_newll(1);
	UnboundedInt result = unboundedInt_newll(1);
	while (unboundedInt_cmpll(n, 0) > 0) {
		UnboundedInt tmp = unboundedInt_multiply(result, n);
		unboundedInt_free(result);
		result = tmp;
		tmp = unboundedInt_subtract(n, decr);
		unboundedInt_free(n); //access to desallocated memory
		n = tmp;
	}
	unboundedInt_free(decr);
	return result;
}

int unboundedInt_isError(UnboundedInt i) {
	return i.mSign == UNBOUNDED_INT_ERROR.mSign;
}

void unboundedInt_print(UnboundedInt ui) {
	if (unboundedInt_isError(ui)) {
		printf("The unbounded int is empty!\n");
		return;
	}
	if (ui.mSign == '-') {
		printf("-");
	}
	Number *p = ui.mFirst;
	for (int i = 0; i < ui.mLength; i++) {
		printf("%c", p->mVal);
		p = p->mNext;
	}
	printf("\n");
}