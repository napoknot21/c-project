//
// Created by Kevin on 05/07/2022.
//

#include "exec_error.h"

#include <errno.h>
#include <stdio.h> 
#include <string.h>

#include "app_informations.h"

char *error_getMessage(enum Error error) {
    switch (error) {
        case INVALID_SYNTAX:
            return "An invalid syntax was found";
        case INVALID_OPERATOR:
            return "An unknown operator was found";
        case INTERNAL:
            return "Internal error occurred during the execution";
        case MISSING_BLANK:
            return "A blank is missing";
        case UNKNOWN_CHARACTER:
            return "An Unknown character was found";
        case TOO_MANY_ARGUMENTS:
            return "Too many arguments were given";
        case MISSING_ARGUMENTS:
            return "Some arguments are missing";
        default:
            return "";
    }
}

void perror_file(enum Error error) {
    fprintf(stderr, "%s in file %s in line %d\n", error_getMessage(error), FILE_NAME, FILE_LINE);
}

void perror_src(char *message) {
    fprintf(stderr, "%s in file %s in line %d\n %s\n", strerror(errno), __FILE__, __LINE__, message);
}
