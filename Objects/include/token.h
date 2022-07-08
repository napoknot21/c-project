//
// Created by Kevin on 05/07/2022.
//

#ifndef C_PROJECT_TOKEN_H
#define C_PROJECT_TOKEN_H
#include <stdlib.h>
/**
* Token types.
*/
typedef enum TokenType {
    NUMBER, VAR, OPERATOR, VOID, FUNCTION
} TokenType;

/**
 * Lexers Tokens.
 */
typedef struct Token {
    char *data;
    enum TokenType type;
} Token;

/**
 * Free a token.
 * @param t The token which will be free
 */
static void token_free(Token t);

/**
 * Create a new token according to the string given in argument and the type.
 * @param s The token' data.
 * @param type The token's type.
 * @return The created Token.
 */
static Token token_new(char *s, size_t len, enum TokenType type);
#endif //C_PROJECT_TOKEN_H