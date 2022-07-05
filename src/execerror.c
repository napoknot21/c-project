//
// Created by Kevin on 05/07/2022.
//

#include "execerror.h"

static char *error_getMessage(enum ERROR error) {
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
