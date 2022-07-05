//
// Created by Kevin on 05/07/2022.
//
#include "token.h"
#include "lib.h"

static Token token_new(char *s, size_t len, enum TokenType type) {
    Token token = {.data = trim(s, len), .type = type};
    return token;
}

static void token_free(Token t) {
    if (t.type == FUNCTION) return;
    //free(t.data);
}

