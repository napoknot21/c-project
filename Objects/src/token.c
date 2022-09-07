//
// Created by Kevin on 05/07/2022.
//
#include "token.h"
#include "lib.h"

Token Token_new(char *s, size_t len, TokenType type) {
	Token token = {.mData = trim(s, len), .mType = type};
	return token;
}

void Token_free(Token t) {
	if (t.mType == FUNCTION) return;
	free(t.mData);
}
