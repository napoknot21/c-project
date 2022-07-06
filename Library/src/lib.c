//
// Created by Kevin on 05/07/2022.
//
#include "lib.h"
#include <string.h>
static int isHigher(const char a, const char b) {
    if (!isAnOperator(a)) return 0;
    if (!isAnOperator(b)) return 1;
    return priority(a) > priority(b);
}

static unsigned short priority(const char a) {
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

static char *trim(const char *s, size_t len) {
    int start = 0, end = 0;
    for (int i = 0; i < (int) len; i++) {
        if (!isspace((unsigned) s[i])) {
            start = i;
            break;
        }
    }
    for (int i = (int) len; i >= 0; i--) {
        if (!isspace((unsigned) s[i])) {
            end = i;
            break;
        }
    }
    int size = end - start;
    char *ret = malloc((size) * sizeof(char) + 1);
    if (ret == NULL) {
        printErr("");
        return NULL;
    }
    strncpy(ret, s, size);
    ret [size] = '\0';
    return ret;
}

static int isSignOrNumber(char c) {
    return c == '+' || c == '-' || isdigit((unsigned)c);
}

static int nDigits(unsigned int i) {
    int n = 1;
    while (i > 9) {
        n++;
        i /= 10;
    }
    return n;
}

static char *intToString(int n) {
    int len = nDigits(abs(n));
    char *s = malloc(len * sizeof(char));
    if (s == NULL) return NULL;
    snprintf(s, len + 1, "%d", n);
    return s;
}

static int str_equals(const char *s1, const char *s2) {
    int l1 = (int) strlen(s1);
    int l2 = (int) strlen(s2);
    if (l1 != l2) return 0;
    for (int i = 0; i < l1; i++) {
        if (s1[i] != s2[i]) {
            return 0;
        }
    }
    return 1;
}

static int isAnOperator(const char s) {
    return s == '*' || s == '+' || s == '-' || s == '=';
    //|| s == '%' || s == '/'

