//
// Created by Kevin on 05/07/2022.
//

#ifndef C_PROJECT_EXECERROR_H
#define C_PROJECT_EXECERROR_H

/**
 * Parser and lexer errors.
 */
enum Error {
	INVALID_OPERATOR,
	INVALID_SYNTAX,
	MISSING_BLANK,
	UNKNOWN_CHARACTER,
	INTERNAL,
	TOO_MANY_ARGUMENTS,
	MISSING_ARGUMENTS
};


void perror_file(enum Error error);

void perror_src(char *message);

#endif //C_PROJECT_EXECERROR_H
