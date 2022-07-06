
typedef struct chiffre {
    struct chiffre *suivant;
    char c; //Pour stocker un chiffre ('0', '1', '2', ..., '9')
    struct chiffre *precedent;
} chiffre;


typedef struct {
    char signe; //soit + ou -
    unsigned long long len; //Taille de la chaine
    chiffre *premier; //chiffre le plus élévé
    chiffre *dernier; //Chiffre des unités
} unbounded_int;

#define UNBOUNDED_INT_ERROR ((unbounded_int) {.len = 0, .dernier = NULL, .premier = NULL, .signe = '*'})

/**
 * Prototypes de fonctions (headers)
 */

unbounded_int unbounded_int_free(unbounded_int u);
unbounded_int string2unbounded_int(char *e);

unbounded_int ll2unbounded_int(long long i);

char *unbounded_int2string(unbounded_int ui);

int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b);

int unbounded_int_cmp_ll(unbounded_int a, long long b);

unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b);

unbounded_int unbounded_int_difference(unbounded_int a, unbounded_int b);

unbounded_int unbounded_int_produit(unbounded_int a, unbounded_int b);

__attribute__((unused)) unbounded_int unbounded_int_division(unbounded_int a, unbounded_int b);

unbounded_int unbounded_int_pow(unbounded_int x, unbounded_int n);

unbounded_int unbounded_int_abs(unbounded_int x);

unbounded_int unbounded_int_fact(unbounded_int n);

int isError(unbounded_int i);

void print_unbounded_int(unbounded_int ui);

