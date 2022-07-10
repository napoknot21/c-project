//
// Created by Kevin on 05/07/2022.
//

#ifndef C_PROJECT_EXECERROR_H
#define C_PROJECT_EXECERROR_H

/**
 * Parser and lexer errors.
 */
enum Error {
    INVALID_OPERATOR, INVALID_SYNTAX, MISSING_BLANK, UNKNOWN_CHARACTER, INTERNAL, TOO_MANY_ARGUMENTS, MISSING_ARGUMENTS
};

/**
 * Print on the stderr the corresponding message according to the code given in argument.
 * @param error The error's code.
 * @return The error's message.
 */
char *error_getMessage(enum Error error);

void perror_file(enum Error error);

void perror_src(char *message);

#endif //C_PROJECT_EXECERROR_H
