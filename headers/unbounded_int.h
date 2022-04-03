typedef struct chiffre {
	struct chiffre *suivant;
	char c; //Pour stocker un chiffre ('0', '1', '2', ..., '9')
	struct chiffre *precedent;
} chiffre;


typedef struct {
	char signe; //soit + ou -
	size_t len; //Taille de la chaine 
    chiffre *premier; //chiffre le plus élévé
    chiffre *dernier; //Chiffre des unités
} unbounded_int;

/**
 * Prototypes de fonctions (headers)
 */
unbounded_int string2unbounded_int(const char *e);

unbounded_int ll2unbounded_int(long long i);

char *unbounded_int2string(unbounded_int i);

int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b);

int unbounded_int_cmp_ll(unbounded_int a, long long b);

unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b);

unbounded_int unbounded_int_difference(unbounded_int a, unbounded_int b);

unbounded_int unbounded_int_produit(unbounded_int a, unbounded_int b);

static int isNum(char c);

static int isAStringNum (const char *c);

static int isUnboundedIntEmpty ();

static unbounded_int * initUnboundedEmpty ();

static void initCharOfUnbounded (char *e, unbounded_int i);

static chiffre * initChiffre (const char c);

static unbounded_int * pushBack (unbounded_int * ui, char c);

static unbounded_int * pushFront (unbounded_int *ui, const char c);

static void print_unbounded_int (unbounded_int ui);