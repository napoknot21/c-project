#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "unbounded_int.h"

#define NUM (48)
#define UNBOUNDED_INT_ERROR ((unbounded_int) {.len = 0, .dernier = NULL, .premier = NULL, .signe = '*'})


static int isNum(char c) {
    return (int) c >= NUM && (int) c <= NUM + 9;
}

/**
 * Convertis un string en type unbounded_int
 * @param e : le string à convertir
 * @return unbounded_int la paramètre comme nouvelle structure
 */
unbounded_int string2unbounded_int(const char *e) {
    return UNBOUNDED_INT_ERROR;
}

/**
 * Convertis un long en type unbounded_int
 * @param i variable à convertir
 * @return unbounded_int : paramètre comme nouvelle struct
 */
unbounded_int ll2unbounded_int (long long i) {
    return UNBOUNDED_INT_ERROR;
}

/**
 * Convertis un type unbounded_int en string
 * @param i struct unbounded_int
 * @return char* le paramètre sous en format string
 */
char * unbounded_int2string (unbounded_int i) {
    return NULL;
}

/**
 * Vérifie si la struct a est plus grande que celle de b (ou à l'inverse)
 * @param a struct unbounded_int
 * @param b struct unbounded_int
 * @return int : −1 si a < b; 0 si a == b et 1 sinon  
 */
int unbounded_int_cmp_unbounded_int (unbounded_int a, unbounded_int b) {
    return 0;
}

/**
 * Vérifie si la struct a est plus grande que long b (ou à l'inverse)
 * @param a struct unbounded_int
 * @param b long 
 * @return int : −1 si a < b; 0 si a == b et 1 sinon  
 */
int unbounded_int_cmp_ll (unbounded_int a, long long b) {
    return 0;
}

/**
 * Fais la somme de deux struct unbounded_int
 * @param a struct unbounded_int
 * @param b struct unbounded_int
 * @return la somme sous forme de struct unbounded_int 
 */
unbounded_int unbounded_int_somme (unbounded_int a, unbounded_int b) {
    return UNBOUNDED_INT_ERROR;
}

/**
 * Fais la différence des deux struct struct unbounded_int
 * @param a struct unbounded_int
 * @param b struct unbounded_int
 * @return la différence sous forme de struct unbounded_int 
 */
unbounded_int unbounded_int_difference (unbounded_int a, unbounded_int b) {
    return UNBOUNDED_INT_ERROR;
}

/**
 * Fais la multiplication de deux structs unbounded_int
 * @param a struct unbounded_int
 * @param b struct unbounded_int
 * @return le produit sous forme de struct unbounded_int 
 */
unbounded_int unbounded_int_produit (unbounded_int a, unbounded_int b) {
    return UNBOUNDED_INT_ERROR;
}

/**
 * fonction main
 * @param argc taille du tableau de argv
 * @param argv tableau de string
 * @return int etat de sortie
 */
int main (int argc, char *argv[]) {
    return 0;
}