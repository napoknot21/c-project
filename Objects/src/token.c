//
// Created by Kevin on 05/07/2022.
//
#include "token.h"
#include "lib.h"

Token Token_new(char *s, size_t len,TokenType type) {
    Token token = {.data = trim(s, len), .type = type};
    return token;
}

void Token_free(Token t) {
    if (t.type == FUNCTION) return;
    //free(t.data);
}

