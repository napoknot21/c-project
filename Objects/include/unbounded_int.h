#ifndef C_PROJECT_UNBOUNDED_INT_H
#define C_PROJECT_UNBOUNDED_INT_H

typedef struct Number {
    struct Number *mNext;
    char mVal; //Pour stocker un chiffre ('0', '1', '2', ..., '9')
    struct Number *mLast;
} Number;


typedef struct {
    char mSign; //soit + ou -
    size_t mLength; //Taille de la chaine
    Number *mFirst; //chiffre le plus élévé
    Number *mLast; //Chiffre des unités
} unbounded_int;

#define UNBOUNDED_INT_ERROR ((unbounded_int) {.mLength = 0, .mLast = NULL, .mFirst = NULL, .mSign = '*'})

/**
 * Prototypes de fonctions (headers)
 */

unbounded_int unbounded_int_free(unbounded_int u);

/**
 * Convertis un string en type unbounded_int
 * @param e : le string à convertir
 * @return unbounded_int la paramètre comme nouvelle structure
 */
unbounded_int string2unbounded_int(char *e);

/**
 * Convertit un long en type unbounded_int
 * @param i variable à convertir
 * @return unbounded_int : paramètre comme nouvelle struct
 */
unbounded_int ll2unbounded_int(long long i);

/**
 * Convertis un type unbounded_int en string
 * @param i struct unbounded_int
 * @return char* le paramètre sous en format string
 */
char *unbounded_int2string(unbounded_int ui);

/**
 * Vérifie si la struct a est plus grande que celle de b (ou à l'inverse)
 * On va supposer que les struct sont pas vides (UNBOUNDED_INT_ERROR) et qu'elles sont précises
 * @param a struct unbounded_int
 * @param b struct unbounded_int
 * @return int : −1 si a < b; 0 si a == b et 1 sinon
 */
int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b);

/**
 * Vérifie si la struct a est plus grande que long b (ou à l'inverse)
 * @param a struct unbounded_int
 * @param b long
 * @return int : −1 si a < b; 0 si a == b et 1 sinon
 */
int unbounded_int_cmp_ll(unbounded_int a, long long b);

/**
 * Fais la somme de deux struct unbounded_int
 * @param a struct unbounded_int
 * @param b struct unbounded_int
 * @return la somme sous forme de struct unbounded_int
 */
unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b);

/**
 * Fais la difference des deux struct struct unbounded_int
 * @param a struct unbounded_int
 * @param b struct unbounded_int
 * @return la différence sous forme de struct unbounded_int
 */
unbounded_int unbounded_int_difference(unbounded_int a, unbounded_int b);

/**
 * Fait la multiplication de deux structs unbounded_int
 * @param a struct unbounded_int
 * @param b struct unbounded_int
 * @return le produit sous forme de struct unbounded_int
 */
unbounded_int unbounded_int_produit(unbounded_int a, unbounded_int b);

/**
 * fais la division de deux structures unbounded-int
 * @param a struct unbounded_int
 * @param b struct unbounded_int
 * @return le quotient entier de la division
 */
unbounded_int unbounded_int_division(unbounded_int a, unbounded_int b);

unbounded_int unbounded_int_pow(unbounded_int x, unbounded_int n);

unbounded_int unbounded_int_abs(unbounded_int x);

unbounded_int unbounded_int_fact(unbounded_int n);

int isError(unbounded_int i);

/**
 * Fonction auxiliaire pour afficher une structure unbounded_int
 * @param ui structure unbounded_int
 */
void print_unbounded_int(unbounded_int ui);
#endif // !C_PROJECT_UNBOUNDED_INT_H
