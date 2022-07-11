//
// Created by Kevin on 05/07/2022.
//

#ifndef C_PROJECT_LIB_H
#define C_PROJECT_LIB_H

#include <stdlib.h>

/**
 * Indicate if the char given in argument is an operator
 * @param s The char which will be tested.
 * @return return true if the char is an operator, false otherwise.
 */
int isAnOperator(char s);

int str_equals(const char *s1, const char *s2);

/**
 * Indicate if the char a has a higher precedence than b.
 * @param a The first operator.
 * @param b The second operator.
 * @return true if a has a higher precedence than b, false otherwise.
 */
int isHigher(char a, char b);

/**
 * Associate a priority with an operator.
 * @param a The operator.
 * @return The operator's priority
 */
unsigned short priority(char a);

/**
 * Trim the string with the given regex. <br>
 * For example, all the space before the other character is ignored, and all the space after are deleted.
 * @param s The string which will be treated.
 * @return return the trimmed string
 */
char *trim(const char *s, size_t len);

/**
 * Check is the given character is a sign symbol (+ or -) or is a number
 * @param c The character that will be tested.
 * @return true if its the case, false otherwise.
 */
int isSignOrNumber(char c);

char *intToString(int n);

/**
 * Vérifie que le string est bien composé de nombres
 * @param c String à tester
 * @return 1 = true, et 0 sinon
 */
int isAStringNum(const char* c);

/**
 * Fonction qui permet de "nettoyer" un nombre.
 * Free l'argument, s'il y a une copie.
 * @param str le nombre passé en paramètre sous forme de string
 * @return le nombre sans 0 ou d'autres characters de plus
 */
char* cleanNumber(char* str);

#endif //C_PROJECT_LIB_H
