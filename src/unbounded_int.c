#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "../headers/unbounded_int.h"

#define NUM (48)

/*************Headers**************/

static int isNum(char c);

static int isAStringNum(const char *c);

static int isUnboundedIntEmpty(unbounded_int ui);

static int isUnboundedZero(unbounded_int ui);

static int isUnboundedUnity(unbounded_int ui);

static char *cleanNumber(char *str);

static unbounded_int cleanUnbounded_int(unbounded_int ui);

static char *unbounded_int_lltoa(long long value, char *buffer, int base);

static void swap(char *x, char *y);

static char *reverse(char *buffer, int i, int j);

static chiffre *initChiffre(char c);

static unbounded_int pushBack(unbounded_int ui, char c);

static unbounded_int pushFront(unbounded_int ui, char c);

static unbounded_int deleteFirstElem(unbounded_int ui);

__attribute__((unused)) static unbounded_int deleteLastElem(unbounded_int ui);

static unbounded_int sumPositifUnbounded(unbounded_int a, unbounded_int b);

static unbounded_int sumNegatifUnbounded(unbounded_int a, unbounded_int b);

static int plusGrandAbs(unbounded_int a, unbounded_int b);

static unbounded_int multiplicationPourUnChiffre(char c, unbounded_int ui);

__attribute__((unused)) static long long puissance(int base, int puissance);


/**
 * Convertis un string en type unbounded_int
 * @param e : le string à convertir
 * @return unbounded_int la paramètre comme nouvelle structure
 */
unbounded_int string2unbounded_int(char *e) {
    if (isAStringNum(e) == 0) return UNBOUNDED_INT_ERROR;
    const char *str = cleanNumber(e);
    unbounded_int res = UNBOUNDED_INT_ERROR;
    int i;
    res.len = 0;
    if (*(str) == '-' || *str == '+') {
        if (*(str) == '-') res.signe = '-';
        else res.signe = '+';
        i = 1;
    } else {
        res.signe = '+';
        i = 0;
    }
    for (int j = i; j < strlen(str); j++) {
        res = pushFront(res, *(str + j));
    }
    return res;
}


/**
 * Convertit un long en type unbounded_int
 * @param i variable à convertir
 * @return unbounded_int : paramètre comme nouvelle struct
 */
unbounded_int ll2unbounded_int(long long i) {
    char buffer[100];
    return string2unbounded_int(unbounded_int_lltoa(i, buffer, 10));
}


/**
 * Convertis un type unbounded_int en string
 * @param i struct unbounded_int
 * @return char* le paramètre sous en format string
 */
char *unbounded_int2string(unbounded_int ui) {
    if (isUnboundedIntEmpty(ui)) return NULL;
    char *e = malloc(sizeof(char) * (ui.len + 2));
    if (e == NULL) return NULL;
    int i;
    if (ui.signe == '-') {
        e[0] = '-';
        i = 1;
    } else i = 0;
    chiffre *tmp = ui.premier;
    for (int j = 0; j < ui.len; j++) {
        *(e + i + j) = tmp->c;
        tmp = tmp->suivant;
    }
    e[ui.len] = '\0';
    return cleanNumber(e);
}


/**
 * Vérifie si la struct a est plus grande que celle de b (ou à l'inverse) 
 * On va supposer que les struct sont pas vides (UNBOUNDED_INT_ERROR) et qu'elles sont précises
 * @param a struct unbounded_int
 * @param b struct unbounded_int
 * @return int : −1 si a < b; 0 si a == b et 1 sinon  
 */
int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b) {
    if ((isUnboundedZero(a) && isUnboundedZero(b)) || (isUnboundedUnity(a) && isUnboundedUnity(b))) return 0;
    unbounded_int a_c = cleanUnbounded_int(a);
    unbounded_int b_c = cleanUnbounded_int(b);
    if (a_c.signe == b_c.signe) {
        if (a_c.len == b_c.len) {
            chiffre *tmp1 = a_c.premier;
            chiffre *tmp2 = b_c.premier;
            for (int i = 0; i < a_c.len; i++) {
                if (tmp1->c > tmp2->c) return 1;
                if (tmp1->c < tmp2->c) return -1;
                tmp1 = tmp1->suivant;
                tmp2 = tmp2->suivant;
            }
            unbounded_int_free(a_c);
            unbounded_int_free(b_c);
            return 0;
        } else {
            if (a_c.signe == '-') return (a_c.len > b_c.len) ? -1 : 1;
            int ret = (a_c.len > b_c.len) ? 1 : -1;
            unbounded_int_free(a_c);
            unbounded_int_free(b_c);
            return ret;
        }
    }
    int ret = (a_c.signe == '-') ? -1 : 1;
    unbounded_int_free(a_c);
    unbounded_int_free(b_c);
    return ret;
}


/**
 * Vérifie si la struct a est plus grande que long b (ou à l'inverse)
 * @param a struct unbounded_int
 * @param b long 
 * @return int : −1 si a < b; 0 si a == b et 1 sinon  
 */
int unbounded_int_cmp_ll(unbounded_int a, long long b) {
    unbounded_int num = ll2unbounded_int(b);
    int bool = unbounded_int_cmp_unbounded_int(a, num);
    unbounded_int_free(num);
    return bool;
}


/**
 * Fais la somme de deux struct unbounded_int
 * @param a struct unbounded_int
 * @param b struct unbounded_int
 * @return la somme sous forme de struct unbounded_int 
 */
unbounded_int unbounded_int_somme(unbounded_int a_ui, unbounded_int b_ui) {
    if (isUnboundedIntEmpty(a_ui) || isUnboundedIntEmpty(b_ui)) return UNBOUNDED_INT_ERROR;
    unbounded_int a = cleanUnbounded_int(a_ui);
    unbounded_int b = cleanUnbounded_int(b_ui);
    if (isUnboundedZero(a) || isUnboundedZero(b)) {
        if (isUnboundedZero(a) && !isUnboundedZero(b)) {
            unbounded_int_free(a);
            return b;
        } else {
            unbounded_int_free(b);
            return a;
        }
    }
    unbounded_int res = (a.signe == b.signe) ? sumPositifUnbounded(a, b) : sumNegatifUnbounded(a, b);
    unbounded_int_free(a);
    unbounded_int_free(b);
    return res;
}


/**
 * Fais la différence des deux struct struct unbounded_int
 * @param a struct unbounded_int
 * @param b struct unbounded_int
 * @return la différence sous forme de struct unbounded_int 
 */
unbounded_int unbounded_int_difference(unbounded_int a_ui, unbounded_int b_ui) {
    if (isUnboundedIntEmpty(a_ui) || isUnboundedIntEmpty(b_ui)) return UNBOUNDED_INT_ERROR;
    unbounded_int a = cleanUnbounded_int(a_ui);
    unbounded_int b = cleanUnbounded_int(b_ui);
    if (isUnboundedZero(a) || isUnboundedZero(b)) {
        if (isUnboundedZero(a) && isUnboundedZero(b)) return a;
        else if (isUnboundedZero(a)) {
            if (b.signe == '-') b.signe = '+';
            else b.signe = '-';
            return b;
        } else return a;
    }
    if ((a.signe == '+' && b.signe == '-') || (a.signe == '-' && b.signe == '+')) return sumPositifUnbounded(a, b);
    return sumNegatifUnbounded(a, b);
}


/**
 * Fais la multiplication de deux structs unbounded_int
 * @param a struct unbounded_int
 * @param b struct unbounded_int
 * @return le produit sous forme de struct unbounded_int 
 */
unbounded_int unbounded_int_produit(unbounded_int a_ui, unbounded_int b_ui) {
    if (isUnboundedIntEmpty(a_ui) || isUnboundedIntEmpty(b_ui)) return UNBOUNDED_INT_ERROR;
    unbounded_int a = cleanUnbounded_int(a_ui);
    unbounded_int b = cleanUnbounded_int(b_ui);
    unbounded_int res = UNBOUNDED_INT_ERROR;
    if (isUnboundedZero(a) || isUnboundedZero(b)) {
        chiffre *c = initChiffre('0');
        res.len = 1;
        res.premier = c;
        res.dernier = c;
        res.signe = '+';
        unbounded_int_free(a);
        unbounded_int_free(b);
        return res;
    }

    if (isUnboundedUnity(a) || isUnboundedUnity(b)) {
        if (isUnboundedUnity(a) && !isUnboundedUnity(b)) return b;
        else return a;
    }

    chiffre *tmp = a.dernier;

    res = multiplicationPourUnChiffre(tmp->c, b);
    for (int i = 1; i < a.len; i++) {
        tmp = tmp->precedent;
        unbounded_int tmp_p = multiplicationPourUnChiffre(tmp->c, b);

        for (int j = 0; j < i; j++) {
            tmp_p = pushFront(tmp_p, '0');
        }

        res = sumPositifUnbounded(res, tmp_p);
    }
    res.signe = (a.signe == b.signe) ? '+' : '-';
    unbounded_int_free(a);
    unbounded_int_free(b);
    return res;
}


/**
 * fais la division de deux structures unbounded-int
 * @param a struct unbounded_int
 * @param b struct unbounded_int
 * @return le quotient entier de la division
 */
unbounded_int unbounded_int_division(unbounded_int a, unbounded_int b) {
    return UNBOUNDED_INT_ERROR;
}

unbounded_int unbounded_int_pow(unbounded_int x, unbounded_int n) {
    unbounded_int result = ll2unbounded_int(1);
    //unbounded_int decr = ll2unbounded_int(1);
    //unbounded_int mod = ll2unbounded_int(2);
    if (unbounded_int_cmp_ll(n, 0) == 0) { ;
        return result;
    }
    /*while (unbounded_int_cmp_ll(n, 0) == 1) {
        //unbounded_int modulo = fun(n,mod);
        if (unbounded_int_cmp_ll(MODULO(n,mod),1) == 0) { //modulo
            result = unbounded_int_produit(result,x);
            n = unbounded_int_difference(n,decr);
        }
        x = unbounded_int_produit(x,x);
        n = unbounded_int_produit(n,mod);
    }*/
    return result;
}

unbounded_int unbounded_int_abs(unbounded_int x) {
    x.signe = '+';
    return x;
}

unbounded_int unbounded_int_fact(unbounded_int n) {
    unbounded_int decr = ll2unbounded_int(1);
    unbounded_int result = ll2unbounded_int(1);
    while (unbounded_int_cmp_ll(n, 0) > 0) {
        result = unbounded_int_produit(result, n);
        n = unbounded_int_difference(n, decr);
    }
    return result;
}

unbounded_int unbounded_int_free(unbounded_int u) {
    chiffre *c = u.premier;
    u.premier = NULL;
    u.dernier = NULL;
    while (c->suivant != NULL) {
        chiffre *n = c->suivant;
        free(c);
        c = n;
    }
    free(c);
    return UNBOUNDED_INT_ERROR;
}

/**
 * fonction main (tests)
 */
/*int main (int argc, char *argv[]) {
    
    printf("=AUXILIAR FUNCTIONS=\n");
    //isNum() test
    printf("==ISNUM() TEST==\n");
    char a = '2';
    char b = 'm';

    printf("%c => is a a number ? %s\n",a, isNum(a) ? "True" : "False");
    printf("%c => is a a number ? %s\n",b, isNum(b) ? "True" : "False");

    printf("===========\n");
    printf("==ISASTRINGNUM() TEST==\n");
    //isAStringNum() test
    char *st_n = malloc(sizeof(char)*6+1);
    char *st_p = malloc(sizeof(char)*5+1);
    char *stn_err = malloc(sizeof(char)*6+1);
    char *stp_err = malloc(sizeof(char)*5+1);
    char *st_zero = malloc(sizeof(char)*1+1);
    char *st_empty = malloc(sizeof(char)*1+1);
    st_n = strcpy(st_n,"-45612");
    st_p = strcpy(st_p,"31245");
    stn_err = strcpy(stn_err,"-548ff");
    stp_err = strcpy(stp_err,"542df");
    st_zero = strcpy(st_zero, "0");
    st_empty = strcpy(st_empty,"+");

    printf("%s => is a a number ? %s\n",st_n, isAStringNum(st_n) ? "True" : "False");
    printf("%s => is a a number ? %s\n",st_p, isAStringNum(st_p) ? "True" : "False");
    printf("%s => is a a number ? %s\n",stn_err, isAStringNum(stn_err) ? "True" : "False");
    printf("%s => is a a number ? %s\n",stp_err, isAStringNum(stp_err) ? "True" : "False");
    printf("%s => is a a number ? %s\n",st_zero, isAStringNum(st_zero) ? "True" : "False");
    printf("%s => is a a number ? %s\n",st_empty, isAStringNum(st_empty) ? "True" : "False");


    printf("===========\n");

    //lltoa() test
    printf("==LLTOA() TEST==\n");
    char buffer [100];
    long long i = 45526229262;
    long long j = -893636383;
    printf("format long long: %lld => format string: %s\n",i , unbounded_int_lltoa(i,buffer,10));
    printf("format long long: %lld => format string: %s\n",j ,unbounded_int_lltoa(j,buffer,10));


    //Puissance test
    printf("==PUISSANCE() TEST==\n");
    int base = 10;
    printf("%d puissance 5 = %lld \n",base, puissance(base, 5));

    printf("===========\n");
    printf("=MAIN FUNCTIONS=\n");
    printf("===========\n");

    //STRING2UNBOUNDED_INT () TEST
    printf("==STRING2UNBOUNDED_INT() TEST==\n");
    unbounded_int ui1 = string2unbounded_int(st_n);
    unbounded_int ui2 = string2unbounded_int(st_p);

    print_unbounded_int(ui1);
    print_unbounded_int(ui2);

    printf("===========\n");

    //LL2UNBOUNDED_INT() TEST
    printf("==LL2UNBOUNDED_INT() TEST==\n");
    unbounded_int ui3 = ll2unbounded_int(i);
    unbounded_int ui4 = ll2unbounded_int(j);

    print_unbounded_int(ui3);
    print_unbounded_int(ui4);

    printf("===========\n");

    //UNBOUNDED_INT2STRING() TEST
    printf("==UNBOUNDED_INT2STRING() TEST==\n");
    printf("%s\n",unbounded_int2string(ui1));
    printf("%s\n",unbounded_int2string(ui2));
    printf("%s\n",unbounded_int2string(ui3));
    printf("%s\n",unbounded_int2string(ui4));
    
    printf("===========\n");
    
    //UNBOUNDED_INT_SOMME() TEST
    printf("==UNBOUNDED_INT_SOMME() TEST==\n");
    
    unbounded_int ui5 = string2unbounded_int("645248");
    unbounded_int ui6 = string2unbounded_int("543124");
    unbounded_int ui7 = string2unbounded_int("-46872");
    unbounded_int ui8 = string2unbounded_int("-13246");
    unbounded_int ui9 = string2unbounded_int("5724");
    unbounded_int ui10 = string2unbounded_int("-6578");
    unbounded_int zero = string2unbounded_int("0");

    unbounded_int res1 = unbounded_int_somme(ui5, ui6);
    unbounded_int res2 = unbounded_int_somme(ui5, zero);
    unbounded_int res3 = unbounded_int_somme(ui7, ui8);
    unbounded_int res4 = unbounded_int_somme(ui7, zero);
    unbounded_int res5 = unbounded_int_somme(ui9, ui5);
    unbounded_int res6 = unbounded_int_somme(ui7, ui10);
    unbounded_int res6_5 = unbounded_int_somme(ui9, ui10);
    
    
    printf("%s + %s = %s\n",unbounded_int2string(ui5), unbounded_int2string(ui6), unbounded_int2string(res1));
    printf("%s + %s = %s\n",unbounded_int2string(ui5), unbounded_int2string(zero), unbounded_int2string(res2));
    printf("%s + (%s) = %s\n",unbounded_int2string(ui7), unbounded_int2string(ui8), unbounded_int2string(res3));
    printf("%s + %s = %s\n",unbounded_int2string(ui7), unbounded_int2string(zero), unbounded_int2string(res4));
    printf("%s + %s = %s\n",unbounded_int2string(ui9), unbounded_int2string(ui5), unbounded_int2string(res5));
    printf("%s + (%s) = %s\n",unbounded_int2string(ui7), unbounded_int2string(ui10), unbounded_int2string(res6));
    printf("%s + (%s) = %s\n",unbounded_int2string(ui9), unbounded_int2string(ui10), unbounded_int2string(res6_5));
    
    printf("===========\n");


    //UNBOUNDED_INT_SOMME() TEST
    printf("==UNBOUNDED_INT_DIFFERENCE() TEST==\n");
    
    unbounded_int res7 = unbounded_int_difference(ui7, ui5); // -a - (+b)
    unbounded_int res8 = unbounded_int_difference(ui5, ui8); // +a - (-b)
    unbounded_int res9 = unbounded_int_difference(zero, ui10);
    unbounded_int res10 = unbounded_int_difference(zero, ui9);
    unbounded_int res11 = unbounded_int_difference(ui10, ui10);
    unbounded_int res12 = unbounded_int_difference(ui5, ui6);
    unbounded_int res13 = unbounded_int_difference(ui7, ui8);


    printf("%s - %s = %s\n",unbounded_int2string(ui7), unbounded_int2string(ui5), unbounded_int2string(res7));
    printf("%s - (%s) = %s\n",unbounded_int2string(ui5), unbounded_int2string(ui8), unbounded_int2string(res8));
    printf("%s - (%s) = %s\n",unbounded_int2string(zero), unbounded_int2string(ui10), unbounded_int2string(res9));
    printf("%s - %s = %s\n",unbounded_int2string(zero), unbounded_int2string(ui9), unbounded_int2string(res10));
    printf("%s - (%s) = %s\n",unbounded_int2string(ui10), unbounded_int2string(ui10), unbounded_int2string(res11));
    printf("%s - %s = %s\n",unbounded_int2string(ui5), unbounded_int2string(ui6), unbounded_int2string(res12));
    printf("%s - (%s) = %s\n",unbounded_int2string(ui7), unbounded_int2string(ui8), unbounded_int2string(res13));
    

    
    printf("===========\n");

    //UNBOUNDED_INT_PRODUIT() TEST
    printf("==UNBOUNDED_INT_PRODUIT() TEST==\n");

    unbounded_int res14 = unbounded_int_produit(zero, ui5);
    unbounded_int res14_5 = unbounded_int_produit(ui5, zero);
    unbounded_int res15 = unbounded_int_produit(ui9, ui9);
    unbounded_int res16 = unbounded_int_produit(ui8, ui8);
    unbounded_int res17 = unbounded_int_produit(ui9, ui8);
    
    printf("%s * (%s) = %s\n",unbounded_int2string(zero), unbounded_int2string(ui5), unbounded_int2string(res14));
    printf("%s * (%s) = %s\n",unbounded_int2string(ui5), unbounded_int2string(zero), unbounded_int2string(res14_5));
    printf("%s * (%s) = %s\n",unbounded_int2string(ui9), unbounded_int2string(ui9), unbounded_int2string(res15));
    printf("%s * (%s) = %s\n",unbounded_int2string(ui8), unbounded_int2string(ui8), unbounded_int2string(res16));
    printf("%s * (%s) = %s\n",unbounded_int2string(ui9), unbounded_int2string(ui8), unbounded_int2string(res17));

    

    printf("===========\n");

    //UNBOUNDED_INT_DIVISION() TEST
    printf("==UNBOUNDED_INT_DIVISION() TEST==\n");


    return 0;
}*/



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
static int isAStringNum(const char *c) {
    size_t len = strlen(c);
    if (len == 0) return 0;
    int i;
    if (*c == '-' || *c == '+') {
        if (len == 1) return 0;
        i = 1;
    } else {
        i = 0;
    }
    for (size_t j = i; j < strlen(c); j++) {
        if (!isNum(*(c + j))) {
            return 0;
        }
    }
    return 1;
}


/**
 * Verifie si la struct est une UNBOUNDED_INT_ERROR
 * @param une struct unbounded_int
 * @return 0 si c'est le cas, et 1 sinon
 */
static int isUnboundedIntEmpty(unbounded_int ui) {
    return (ui.signe == '*' || ui.len == 0 || ui.premier == NULL || ui.dernier == NULL);
}


/**
 * Vérifie que la structure est bien un 0, c'est à dire, que len = 1, premier = dernier = (chiffre) {.suivant = null, .c = '0' , .precedant = null}
 * @param ui pointeur vers la structure 
 * @return True si la strcutre est un 0 false sinon
 */
static int isUnboundedZero(unbounded_int ui) {
    return ((ui.signe == '+' || ui.signe == '-') && ui.len == 1 && ui.premier == ui.dernier && ui.premier->c == '0');
}


/**
 * Vérifie que la structure est bien un 1 (l'unité), c'est à dire, que len = 1, premier = dernier = (chiffre) {.suivant = null, .c = '1' , .precedant = null}
 * @param ui pointeur vers la structure 
 * @return True si la strcutre est un 1 (unité) false sinon
 */
static int isUnboundedUnity(unbounded_int ui) {
    return (ui.len == 1 && ui.premier == ui.dernier && ui.premier->c == '1' && (ui.signe == '+' || ui.signe == '-'));
}


/**
 * Fonction qui permet de "nettoyer" un nombre
 * @param str le nombre passé en paramètre sous forme de string
 * @return le nombre sans 0 ou d'autres characters de plus 
 */
static char *cleanNumber(char *str) {
    size_t len = strlen(str);
    int i;
    if (*str == '-' || *str == '+') {
        if (len == 2) {
            return (*(str + 1) == '0') ? "0" : str;
        }
        i = 1;
    } else {
        if (len == 1) return str;
        i = 0;
    }
    int index = i;
    for (size_t j = i; j < len; j++) {
        if (*(str + j) != '0') {
            break;
        }
        index++;
    }
    if (index == i) return str;
    if (index == len) return "0";
    int newLen = len - index + 1;
    char *newStr = malloc(sizeof(char) * newLen + 1);
    if (i == 1) newStr[0] = str[0];
    for (size_t j = index; j < len; j++) {
        newStr[i] = str[j];
        i++;
    }
    return newStr;
}


/**
 * Fonction qui permet de "nettoyer" une structure unbounded_int
 * @param ui struct à néttoyer
 * @return un pointeur vers la structure ou une nouvelle
 */

static unbounded_int cleanUnbounded_int(unbounded_int ui) {

    if (ui.len <= 1) return ui;
    if (ui.premier->c != '0') return ui;
    int index = 0;
    chiffre *tmp;// = ui->premier;
    for (tmp = ui.premier; tmp != NULL; tmp = tmp->suivant) {
        if (tmp->c != '0') {
            break;
        }
        index += 1;
    }
    if (tmp == NULL) {
        for (int i = 0; i < ui.len - 1; i++) {
            ui = deleteFirstElem(ui);
        }
        return ui;
    }

    for (int i = 0; i < index; i++) {
        ui = deleteFirstElem(ui);
    }
    return ui;

}


/**
 * Fonction qui permet de transformer un long long en String
 * @param value long long à transformer
 * @param buffer allocation du string
 * @param base base de conversion
 * @return char* 
 */
static char *unbounded_int_lltoa(long long value, char *buffer, int base) {
    if (base < 2 || base > 32) {
        return buffer;
    }
    long long n = llabs(value);
    int i = 0;
    while (n) {
        int r = (int) ((long long) n % base);
        if (r >= 10) buffer[i++] = (char) (65 + (r - 10));
        else buffer[i++] = (char) (48 + r);
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
    char t = *x;
    *x = *y;
    *y = t;
}


/**
 * 
 * @param buffer allocation du string
 * @param i 
 * @param j 
 * @return char* 
 */
static char *reverse(char *buffer, int i, int j) {
    while (i < j) {
        swap(&buffer[i++], &buffer[j--]);
    }
    return buffer;
}


/**
 * Initialise une struct chiffre à partir d'un char
 * @param c valeur de l'argument c de la struct chiffre
 * @return une nouvelle instance de chiffre
 */
static chiffre *initChiffre(const char c) {
    if (isNum(c) == 0) return NULL;
    chiffre *ch = malloc(sizeof(chiffre));
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
 * Ajoute d'une nouvelle struct Chiffre à la fin de la liste
 * @param ui struct source
 * @param c valeur de la nouvelle struct chiffre ajouté
 */
static unbounded_int pushFront(unbounded_int ui, const char c) {
    chiffre *ch = initChiffre(c);
    ch->precedent = ui.dernier;
    ch->suivant = NULL;
    if (ui.dernier != NULL) {
        ui.dernier->suivant = ch;
    } else if (ui.dernier == NULL && ui.premier == NULL) {
        ui.premier = ch;
    }
    ui.dernier = ch;
    ui.len++;
    return ui;
}


/**
 * Ajoute d'une nouvelle struct Chiffre au début de la liste
 * @param ui struct source
 * @param c valeur de la nouvelle struct chiffre ajouté
 */
static unbounded_int pushBack(unbounded_int ui, const char c) {
    chiffre *ch = initChiffre(c);
    ch->precedent = NULL;
    ch->suivant = ui.premier;
    if (ui.premier != NULL && ui.len != 0) {
        ui.premier->precedent = ch;
    } else if (ui.dernier == NULL && ui.premier == NULL) {
        ui.dernier = ch;
    }
    ui.premier = ch;
    ui.len++;
    return ui;
}


/**
 * Supprime le premier élément
 * @param la structure à modifier
 */
static unbounded_int deleteFirstElem(unbounded_int ui) {
    if (ui.len <= 1) return UNBOUNDED_INT_ERROR;
    chiffre *tmp;
    tmp = ui.premier;
    ui.premier = ui.premier->suivant;
    ui.premier->precedent = NULL;
    free(tmp);
    ui.len--;
    return ui;
}


/**
 * Delete last elem
 *
 */
__attribute__((unused)) static unbounded_int deleteLastElem(unbounded_int ui) {
    if (ui.len <= 1) return UNBOUNDED_INT_ERROR;
    chiffre *tmp;
    tmp = ui.dernier;
    ui.dernier = ui.dernier->precedent;
    ui.dernier->suivant = NULL;
    free(tmp);
    ui.len--;
    return ui;
}


/**
 * Fonction auxiliaire pour afficher une structure unbounded_int
 * @param ui structure unbounded_int
 */
void print_unbounded_int(unbounded_int ui) {
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


/**
 *
 *
 *
 */
static unbounded_int sumPositifUnbounded(unbounded_int a, unbounded_int b) {
    unbounded_int res = UNBOUNDED_INT_ERROR;
    res.signe = a.signe;
    res.len = 0;
    int ret = 0;
    chiffre *tmp_a = a.dernier;
    chiffre *tmp_b = b.dernier;
    int max_len = (int) ((a.len < b.len) ? b.len : a.len);
    for (int i = 0; i < max_len; i++) {

        char ca;
        if (tmp_a == NULL) ca = '0';
        else ca = tmp_a->c;
        char cb;
        if (tmp_b == NULL) cb = '0';
        else cb = tmp_b->c;

        int c = (ca - '0') + (cb - '0') + ret;

        char unit = (char) ((c % 10) + '0');
        ret = c / 10;
        res = pushBack(res, unit);

        if (tmp_a != NULL) {
            tmp_a = tmp_a->precedent;
        }

        if (tmp_b != NULL) {
            tmp_b = tmp_b->precedent;
        }

    }

    if (ret >= 1) {
        res = pushBack(res, (char) (ret + (int) '0'));
    }
    return cleanUnbounded_int(res);

}


/**
 *
 */
static unbounded_int sumNegatifUnbounded(unbounded_int a, unbounded_int b) {
    int plusgrand = plusGrandAbs(a, b);
    unbounded_int res = UNBOUNDED_INT_ERROR;
    if (plusgrand == 0) {
        res = pushBack(res, '0');

        return res;
    }
    res.len = 0;
    int max_len = (int) ((a.len < b.len) ? b.len : a.len);
    chiffre *tmp_a;
    chiffre *tmp_b;
    if (plusgrand == -1) {

        res.signe = b.signe;
        tmp_a = b.dernier;
        tmp_b = a.dernier;
    } else {
        res.signe = a.signe;
        tmp_a = a.dernier;
        tmp_b = b.dernier;
    }
    int ret = 0;
    for (int i = 0; i < max_len; i++) {
        char ca;
        if (tmp_a == NULL) ca = '0';
        else ca = tmp_a->c;
        char cb;
        if (tmp_b == NULL) cb = '0';
        else cb = tmp_b->c;

        int c = (ca - '0') - (cb - '0') - ret;
        if (c < 0) {
            c += 10;
            ret = 1;
        } else ret = 0;
        char unit = (char) ((c % 10) + '0');
        res = pushBack(res, unit);

        if (tmp_a != NULL) {
            tmp_a = tmp_a->precedent;
        }

        if (tmp_b != NULL) {
            tmp_b = tmp_b->precedent;
        }

    }

    return cleanUnbounded_int(res);
}


/**
 * 
 * @return -1 si |a| < |b|, 0 si |a| = |b| et sinon 1 si |a| > |b| 
 */
static int plusGrandAbs(unbounded_int a, unbounded_int b) {
    a.signe = '+';
    b.signe = '+';
    return unbounded_int_cmp_unbounded_int(a, b);
}


/**
 *
 *
 */
static unbounded_int multiplicationPourUnChiffre(char c, unbounded_int ui) {
    unbounded_int p = UNBOUNDED_INT_ERROR;
    int a = c - '0';
    if (a == 1) return ui;
    int ret = 0;
    chiffre *tmp = ui.dernier;
    for (int i = 0; i < ui.len; i++) {
        int tmp_c = tmp->c - '0';
        int mul = (a * tmp_c) + ret;
        ret = mul / 10;
        char num = (char) ((mul % 10) + '0');
        p = pushBack(p, num);

        tmp = tmp->precedent;
    }
    if (ret >= 1) {
        p = pushBack(p, (char) (ret + '0'));
    }
    return p;
}


/**
 *
 *
 *
 */
__attribute__((unused)) static long long puissance(int base, int puissance) {
    int res = 1;
    int puiss = puissance / 2;
    for (int i = 0; i < puiss; i++) {
        res *= base;
    }
    res *= res;
    if (puissance % 2 == 0) {
        return res;
    }
    return res * base;
}
