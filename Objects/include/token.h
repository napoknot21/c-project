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
	NUMBER,
	VAR,
	OPERATOR,
	VOID,
	FUNCTION
} TokenType;

/**
 * Lexers Tokens.
 */
typedef struct Token {
	char *mData;
	TokenType mType;
} Token;

/**
 * Free a token.
 * @param t The token which will be free
 */
void Token_free(Token t);

/**
 * Create a new token according to the string given in argument and the mType.
 * @param s The token' data.
 * @param type The token's type.
 * @return The created Token.
 */
Token Token_new(char *s, size_t len, TokenType type);

#endif //C_PROJECT_TOKEN_H
