//
// Created by Kevin on 05/07/2022.
//
#include "lib.h"
#include <string.h>
#include<ctype.h>
#include <stdio.h>

#include "exec_error.h"

int isHigher(const char a, const char b) {
	if (!isAnOperator(a)) return 0;
	if (!isAnOperator(b)) return 1;
	return priority(a) > priority(b);
}

unsigned short priority(const char a) {
	switch (a) {
		case '=':
			return 1;
		case '+':
		case '-':
			return 2;
		//case '/':
		case '*':
			//case '%':
			return 3;
		default:
			return 0;
	}
}

char *trim(const char *s, size_t len) {
	int start = 0, end = 0;
	for (int i = 0; i < (int) len; i++) {
		if (!isspace(s[i])) {
			start = i;
			break;
		}
	}
	for (int i = (int)len - 1; i >= 0; i--) {
		if (!isspace(s[i])) {
			end = i;
			break;
		}
	}
	int size = end - start + 1;
	char *ret = malloc((size + 1) * sizeof(char));
	if (ret == NULL) {
		perror_src("");
		return NULL;
	}
	strncpy(ret, s, size);
	ret[size] = '\0';
	return ret;
}

int isSignOrNumber(char c) {
	return c == '+' || c == '-' || isdigit(c);
}

int nDigits(unsigned int i) {
	int n = 1;
	while (i > 9) {
		n++;
		i /= 10;
	}
	return n;

}

char *intToString(int n) {
	int len = nDigits(abs(n));
	char *s = malloc(len * sizeof(char));
	if (s == NULL) return NULL;
	snprintf(s, len + 1, "%d", n);
	return s;
}

int str_equals(const char *s1, const char *s2) {
	size_t l1 = strlen(s1);
	size_t l2 = strlen(s2);
	if (l1 != l2) return 0;
	for (size_t i = 0; i < l1; i++) {
		if (s1[i] != s2[i]) {
			return 0;
		}
	}
	return 1;
}

int isAnOperator(const char s) {
	return s == '*' || s == '+' || s == '-' || s == '=';
	//|| s == '%' || s == '/'
}

int isAStringNum(const char *c) {
	size_t mLength = strlen(c);
	if (mLength == 0) return 0;
	int i;
	if (*c == '-' || *c == '+') {
		if (mLength == 1) return 0;
		i = 1;
	}
	else {
		i = 0;
	}
	for (size_t j = i; j < strlen(c); j++) {
		if (!isdigit(c[j])) {
			return 0;
		}
	}
	return 1;
}

char *cleanNumber(char *str) {
	size_t length = strlen(str);
	char *zero = malloc(sizeof(char[2]));
	zero[0] = '0';
	zero[1] = '\0';
	int i;
	if (*str == '-' || *str == '+') {
		if (length == 2) {
			if (*(str + 1) == '0') {
				return zero;
			}
			free(zero);
			return str;
		}
		i = 1;
	}
	else {
		if (length == 1) {
			free(zero);
			return str;
		}
		i = 0;
	}
	int index = i;
	for (size_t j = i; j < length; j++) {
		if (*(str + j) != '0') {
			break;
		}
		index++;
	}
	if (index == i) {
		free(zero);
		return str;
	}
	if (index == length) return zero;
	free(zero);
	int newLen = length - index + 1;
	char *newStr = malloc(sizeof(char) * (newLen + 1));
	if (i == 1) newStr[0] = str[0];
	memmove(newStr, str + index, length);
	newStr[newLen] = '\0';
	return newStr;
}
