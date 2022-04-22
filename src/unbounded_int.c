#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "../headers/unbounded_int.h"

#define NUM (48)
#define UNBOUNDED_INT_ERROR ((unbounded_int) {.len = 0, .dernier = NULL, .premier = NULL, .signe = '*'})


/**
 * Convertis un string en type unbounded_int
 * @param e : le string à convertir
 * @return unbounded_int la paramètre comme nouvelle structure
 */
unbounded_int string2unbounded_int(const char *e) {
    if (isAStringNum(e) == 0) return UNBOUNDED_INT_ERROR;
    unbounded_int *res = initUnboundedEmpty();
    int i;
    if (*e == '-') {
        res->len = strlen(e) - 2;
        res->signe = '-';
        i = 1;
    } else {
        res->len = strlen(e)-1;
        res->signe = '+';
        i = 0;
    }
    for (int j = i; j < res->len; j++) {
        res = pushBack(res,*(e+j));
    }
    return *res;
}


/**
 * Convertis un long en type unbounded_int
 * @param i variable à convertir
 * @return unbounded_int : paramètre comme nouvelle struct
 */
unbounded_int ll2unbounded_int (long long i) {
    char buffer[100];
    return string2unbounded_int(lltoa(i,buffer,10));
}


/**
 * Convertis un type unbounded_int en string
 * @param i struct unbounded_int
 * @return char* le paramètre sous en format string
 */
char * unbounded_int2string (unbounded_int i) {
    if (i.signe == '*') {
        return NULL;
    }
    char *e;
    initCharOfUnbounded(e,i);
    int j;
    if (i.signe == '-') {
        j = 1;
    } else {
        j = 0;
    }
    chiffre *pElem = i.premier;
    while (pElem != NULL) {
        *(e+j) = pElem->c;
        pElem = pElem->suivant;
        j++;
    }
    *(e+j) = '\0';
    return e;
}


/**
 * Vérifie si la struct a est plus grande que celle de b (ou à l'inverse)
 * @param a struct unbounded_int
 * @param b struct unbounded_int
 * @return int : −1 si a < b; 0 si a == b et 1 sinon  
 */
int unbounded_int_cmp_unbounded_int (unbounded_int a, unbounded_int b) {
    /*
    if (a.len > b.len && a.signe == '+') {
        return 1;
    } else if (b.len > a.len && b.signe == '+') {
        return -1;
    }
    */
    return 0;
}


/**
 * Vérifie si la struct a est plus grande que long b (ou à l'inverse)
 * @param a struct unbounded_int
 * @param b long 
 * @return int : −1 si a < b; 0 si a == b et 1 sinon  
 */
int unbounded_int_cmp_ll (unbounded_int a, long long b) {
    unbounded_int b_ll = ll2unbounded_int(b);
    return unbounded_int_cmp_unbounded_int(a,b_ll);
}


/**
 * Fais la somme de deux struct unbounded_int
 * @param a struct unbounded_int
 * @param b struct unbounded_int
 * @return la somme sous forme de struct unbounded_int 
 */
unbounded_int unbounded_int_somme (unbounded_int a, unbounded_int b) {
    if (a.signe == '+' && b.signe == '+' || a.signe == '-' && b.signe == '-') {
        return UNBOUNDED_INT_ERROR;
    }
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
 * fonction main (tests)
 */
int main (int argc, char *argv[]) {

    //isNum() test
    char a = '2';
    char b = 'm';

    printf("%c => is a a number ? %s\n",a, isNum(a) ? "True" : "False");
    printf("%c => is a a number ? %s\n",b, isNum(b) ? "True" : "False");

    printf("===========\n");

    //isAStringNum() test
    char *st_n = malloc(sizeof(char)*6+1);
    char *st_p = malloc(sizeof(char)*5+1);
    char *stn_err = malloc(sizeof(char)*6+1);
    char *stp_err = malloc(sizeof(char)*5+1);
    st_n = strcpy(st_n,"-45612");
    st_p = strcpy(st_p,"31245");
    stn_err = strcpy(stn_err,"-548ff");
    stp_err = strcpy(stp_err,"542df");

    printf("%s => is a a number ? %s\n",st_n, isAStringNum(st_n) ? "True" : "False");
    printf("%s => is a a number ? %s\n",st_p, isAStringNum(st_p) ? "True" : "False");
    printf("%s => is a a number ? %s\n",stn_err, isAStringNum(stn_err) ? "True" : "False");
    printf("%s => is a a number ? %s\n",stp_err, isAStringNum(stp_err) ? "True" : "False");


    //free(st_n);
    //free(st_p);
    //free(stn_err);
    //free(stp_err)


    printf("===========\n");

    //lltoa() test
    char *buffer [100];
    long long i = 45526229262;
    long long j = -893636383;
    printf("%s\n", lltoa(i,buffer,10));
    printf("%s\n", lltoa(j,buffer,10));

    //print_unbounded_int() test
    unbounded_int ui1 = string2unbounded_int(st_n);
    unbounded_int ui2 = string2unbounded_int(st_p);

    //print_Unbounded_int(ui1);
    //print_Unbounded_int(ui2);


    return 0;
}



/**
 * vérifie si le code ASCII du char est un nombre (entre 48 et 57)
 * @param c char à tester
 * @return 1 si c est un nombre, 0 sinon
 */
static int isNum(char c) {
    return (int) c >= NUM && (int) c <= NUM + 9;
}


/**
 * Vérifie que le string est bien composé de nombres
 * @param c String à tester
 * @return 1 = true, et 0 sinon
 */
static int isAStringNum (const char *c) {
    int i;
    if (*(c+0) == '-') {
        i = 1;
    } else {
        i = 0;
    }
    for (int j = i; j < strlen(c); j++) {
        if (!isNum(*(c+j))) {
            return 0;
        }
    }
    return 1;
}


static int isUnboundedIntEmpty () {
    return 0;
}


/**
 * Fonction qui permet de transformer un long long en String
 * @param value long long à transformer
 * @param buffer allocation du string
 * @param base base de conversion
 * @return char* 
 */
 static char * lltoa (long long value, char *buffer, int base) {
     if (base < 2 || base > 32) {
        return buffer;
    }
    long long n = abs(value);
    int i = 0;
    while (n) {
        int r = n % base;
        if (r >= 10) buffer[i++] = 65 + (r - 10);
        else buffer[i++] = 48 + r;
        n = n / base;
    }
    if (i == 0) buffer[i++] = '0';
    if (value < 0 && base == 10) buffer[i++] = '-';
    buffer[i] = '\0'; // null terminate string
    return reverse(buffer, 0, i - 1);
 }


/**
 * Echange la valeur de deux pointeurs
 * @param x pointeur 1
 * @param y pointeur 2
 */
static void swap(char *x, char *y) {
    char t = *x; *x = *y; *y = t;
}

/**
 * 
 * @param buffer allocationo du string
 * @param i 
 * @param j 
 * @return char* 
 */
static char* reverse(char *buffer, int i, int j) {
    while (i < j) {
        swap(&buffer[i++], &buffer[j--]);
    }
    return buffer;
}


/**
 * Initialise un *char (signe et allocation) à partir d'un unbounded_int
 * @param e char à initialiser
 * @param i unbounded_int source
 */
static void initCharOfUnbounded (char *e, unbounded_int i) {
    if (i.signe == '-') {
        e = malloc(i.len + 2);
        if (e == NULL) exit(EXIT_FAILURE);
        *(e+0) = '-';
    } else {
        e = malloc(i.len + 1);
        if (e == NULL) exit(EXIT_FAILURE);
    }
}


/**
 * Initialise une struct chiffre à partir d'un char
 * @param c valeur de l'argument c de la struct chiffre
 * @return une nouvelle instance de chiffre
 */
static chiffre * initChiffre (const char c) {
    if (isNum(c) == 0) return NULL;
    chiffre *ch = malloc(sizeof (chiffre));
    if (ch == NULL) {
        perror("Pas de mémoire pour allouer la struct chiffre\n");
        return NULL;
    }
    ch->suivant = NULL;
    ch->precedent = NULL;
    ch->c = c;
    return ch;
}


/**
 * On initialise les pointeurs d'un unbounded_int à NULL
 * @param ui structure affectée
 */
static unbounded_int * initUnboundedEmpty () {
    unbounded_int *ui = malloc (sizeof (unbounded_int));
    if (ui == NULL) {
        perror("Pas de mémoire pour allouer la struct unbounded_int\n");
        exit(EXIT_FAILURE);
    }
    ui->premier = NULL;
    ui->dernier = NULL;
    ui->signe = '*';
    ui->len = 0;
    return ui;
}


/**
 * Ajoute d'une nouvelle struct Chiffre à la fin de la liste
 * @param ui struct source
 * @param c valeur de la nouvelle struct chiffre ajouté
 */
static unbounded_int * pushFront (unbounded_int *ui, const char c) {
    chiffre *ch = initChiffre(c);
    ch->precedent = ui->dernier;
    ch->suivant = NULL;
    if (ui->dernier != NULL) {
        ui->dernier->suivant = ch;
    } else if (ui->dernier == NULL && ui->premier == NULL) {
        ui->premier = ch;
    }
    ui->dernier = ch;
    return ui;
}


/**
 * Ajoute d'une nouvelle struct Chiffre au début de la liste
 * @param ui struct source
 * @param c valeur de la nouvelle struct chiffre ajouté
 */
static unbounded_int * pushBack (unbounded_int *ui, const char c) {
    chiffre *ch = initChiffre(c);
    ch->precedent = NULL;
    ch->suivant = ui->premier;
    if (ui->premier != NULL) {
        ui->premier->precedent = ch;
    } else if (ui->dernier == NULL && ui->premier == NULL) {
       ui->dernier = ch;
    }
    ui->premier = ch;
    return ui;
}


/*
static void print_unbounded_int (unbounded_int ui) {
    unbounded_int tmp = UNBOUNDED_INT_ERROR;
    if (ui.signe == tmp.signe && ui.len == tmp.len && ui.premier == tmp.premier && ui.dernier == tmp.dernier) {
        printf("L'unbounded_int est vide\n");
        return;
    }
    if (ui.signe == '-') {
        printf("-");
    }
    chiffre *p = ui.premier;
    for (int i = 0; i < ui.len; i++) {
        if (p == ui.dernier) {
            printf("%c\n", p->c);
            return;
        }
        printf("%c", p->c);
        p = p->suivant;
    }
}
*/