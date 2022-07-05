//
// Created by Kevin on 05/07/2022.
//

#ifndef C_PROJECT_EXECERROR_H
#define C_PROJECT_EXECERROR_H

#define pERROR(error)(fprintf(stderr, "%s in file %s in line %d\n", error_getMessage(error), FILE_NAME, FILE_LINE))
#define printErr(c)(fprintf(stderr, "%s in file %s in line %d\n %s\n", strerror(errno), __FILE__, __LINE__, (c)))
/**
 * Parser and lexer errors.
 */
enum ERROR {
    INVALID_OPERATOR, INVALID_SYNTAX, MISSING_BLANK, UNKNOWN_CHARACTER, INTERNAL, TOO_MANY_ARGUMENTS, MISSING_ARGUMENTS
};

/**
 * Print on the stderr the corresponding message according to the code given in argument.
 * @param error The error's code.
 * @return The error's message.
 */
static char *error_getMessage(enum ERROR error);
#endif //C_PROJECT_EXECERROR_H
