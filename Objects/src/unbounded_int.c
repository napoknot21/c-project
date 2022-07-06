#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "unbounded_int.h"

#define NUM (48)

/*************Declarationss**************/

static int isNum(char c);

static int isAStringNum(const char *c);

static int isUnboundedIntEmpty(unbounded_int ui);

static int isUnboundedZero(unbounded_int *ui);

static int isUnboundedUnity(unbounded_int ui);

static char *cleanNumber(char *str);

static unbounded_int cleanUnbounded_int(unbounded_int ui);

static char *unbounded_int_lltoa(long long value, char *buffer, int base);

static void swap(char *x, char *y);

static char *reverse(char *buffer, int i, int j);

static Number *initChiffre(char c);

static unbounded_int pushBack(unbounded_int ui, char c);

static unbounded_int pushFront(unbounded_int ui, char c);

static unbounded_int deleteFirstElem(unbounded_int ui);

static unbounded_int deleteLastElem(unbounded_int ui);

static unbounded_int sumPositifUnbounded(unbounded_int a, unbounded_int b);

static unbounded_int sumNegatifUnbounded(unbounded_int a, unbounded_int b);

static int plusGrandAbs(unbounded_int a, unbounded_int b);

static unbounded_int multiplicationPourUnChiffre(char c, unbounded_int ui);

static long long puissance(int base, int puissance);

static unbounded_int unbounded_int_cpy (unbounded_int u);


unbounded_int string2unbounded_int(char *e) {
    if (isAStringNum(e) == 0) return UNBOUNDED_INT_ERROR;
    const char *str = cleanNumber(e);
    unbounded_int res = UNBOUNDED_INT_ERROR;
    int i;
    res.mLength = 0;
    if (*(str) == '-' || *str == '+') {
        if (*(str) == '-') res.mSign = '-';
        else res.mSign = '+';
        i = 1;
    } else {
        res.mSign = '+';
        i = 0;
    }
    for (int j = i; j < strlen(str); j++) {
        res = pushFront(res, *(str + j));
    }
    return res;
}



unbounded_int ll2unbounded_int(long long i) {
    char buffer[100];
    return string2unbounded_int(unbounded_int_lltoa(i, buffer, 10));
}



char *unbounded_int2string(unbounded_int ui) {
    if (isUnboundedIntEmpty(ui)) return NULL;
    char *e = malloc(sizeof(char) * (ui.mLength + 2));
    if (e == NULL) return NULL;
    int i;
    if (ui.mSign == '-') {
        e[0] = '-';
        i = 1;
    } else i = 0;
    Number *tmp = ui.mFirst;
    for (; i < ui.mLength; i++) {
        *(e + i) = tmp->mVal;
        tmp = tmp->mNext;
    }
    e[ui.mLength] = '\0';
    return cleanNumber(e);
}



int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b) {
    if ((isUnboundedZero(&a) && isUnboundedZero(&b)) || (isUnboundedUnity(a) && isUnboundedUnity(b))) return 0;
    unbounded_int a_c = cleanUnbounded_int(a);
    unbounded_int b_c = cleanUnbounded_int(b);
    if (a_c.mSign == b_c.mSign) {
        if (a_c.mLength == b_c.mLength) {
            Number *tmp1 = a_c.mFirst;
            Number *tmp2 = b_c.mFirst;
            for (int i = 0; i < a_c.mLength; i++) {
                if (tmp1->mVal > tmp2->mVal) return 1;
                if (tmp1->mVal < tmp2->mVal) return -1;
                tmp1 = tmp1->mNext;
                tmp2 = tmp2->mNext;
            }
            return 0;
        } else {
            if (a_c.mSign == '-') return (a_c.mLength > b_c.mLength) ? -1 : 1;
            int ret = (a_c.mLength > b_c.mLength) ? 1 : -1;
            return ret;
        }
    }
    int ret = (a_c.mSign == '-') ? -1 : 1;
    return ret;
}



int unbounded_int_cmp_ll(unbounded_int a, long long b) {
    unbounded_int num = ll2unbounded_int(b);
    int bool = unbounded_int_cmp_unbounded_int(a, num);
    unbounded_int_free(num);
    return bool;
}



unbounded_int unbounded_int_somme(unbounded_int a_ui, unbounded_int b_ui) {
    if (isUnboundedIntEmpty(a_ui) || isUnboundedIntEmpty(b_ui)) return UNBOUNDED_INT_ERROR;
    unbounded_int a = cleanUnbounded_int(a_ui);
    unbounded_int b = cleanUnbounded_int(b_ui);
    if (isUnboundedZero(&a) || isUnboundedZero(&b)) {
        if (isUnboundedZero(&a) && !isUnboundedZero(&b)) {
            return b;
        } else {
            return a;
        }
    }
    unbounded_int res = (a.mSign == b.mSign) ? sumPositifUnbounded(a, b) : sumNegatifUnbounded(a, b);
    return res;
}



unbounded_int unbounded_int_difference(unbounded_int a_ui, unbounded_int b_ui) {
    if (isUnboundedIntEmpty(a_ui) || isUnboundedIntEmpty(b_ui)) return UNBOUNDED_INT_ERROR;
    unbounded_int a = cleanUnbounded_int(a_ui);
    unbounded_int b = cleanUnbounded_int(b_ui);
    if (isUnboundedZero(&a) || isUnboundedZero(&b)) {
        if (isUnboundedZero(&a) && isUnboundedZero(&b)) return unbounded_int_cpy(a);
        else if (isUnboundedZero(&a)) {
            unbounded_int res = unbounded_int_cpy(b);
            res.mSign = (b.mSign == '-') ? '+' : '-';
            return res;
        } else {
            return unbounded_int_cpy(a);
        }
    }
    unbounded_int res;
    if ((a.mSign == '+' && b.mSign == '-') || (a.mSign == '-' && b.mSign == '+')) res = sumPositifUnbounded(a, b);
    else res = sumNegatifUnbounded(a, b);
    return res;
}



unbounded_int unbounded_int_produit(unbounded_int a_ui, unbounded_int b_ui) {
    if (isUnboundedIntEmpty(a_ui) || isUnboundedIntEmpty(b_ui)) return UNBOUNDED_INT_ERROR;
    unbounded_int a = cleanUnbounded_int(a_ui);
    unbounded_int b = cleanUnbounded_int(b_ui);
    unbounded_int res = UNBOUNDED_INT_ERROR;
    if (isUnboundedZero(&a) || isUnboundedZero(&b)) {
        Number *c = initChiffre('0');
        res.mLength = 1;
        res.mFirst = c;
        res.mLast = c;
        res.mSign = '+';
        return res;
    }

    if (isUnboundedUnity(a) || isUnboundedUnity(b)) {
        if (isUnboundedUnity(a) && !isUnboundedUnity(b)) return unbounded_int_cpy(b);
        else return unbounded_int_cpy(a);
    }

    Number tmp = *a.mLast;

    res = multiplicationPourUnChiffre(tmp.mVal, b);
    for (int i = 1; i < a.mLength; i++) {
        tmp = *tmp.mLast;
        unbounded_int tmp_p = multiplicationPourUnChiffre(tmp.mVal, b);

        for (int j = 0; j < i; j++) {
            tmp_p = pushFront(tmp_p, '0');
        }
        unbounded_int s = sumPositifUnbounded(res, tmp_p);
        unbounded_int_free(tmp_p);
        unbounded_int_free(res);
        res = s;
    }
    res.mSign = (a.mSign == b.mSign) ? '+' : '-';
    return res;
}


static unbounded_int unbounded_int_cpy (unbounded_int u) {
    unbounded_int res = UNBOUNDED_INT_ERROR;
    res.mSign = u.mSign;
    Number *c = u.mFirst;
    while (c != NULL) {
        res = pushBack(res,c->mVal);
        c = c->mNext;
    }
    return res;
}


unbounded_int unbounded_int_division(unbounded_int a_ui, unbounded_int b_ui) {
    if (isUnboundedIntEmpty(a_ui) || isUnboundedIntEmpty(b_ui)) return UNBOUNDED_INT_ERROR;
    unbounded_int a = cleanUnbounded_int(a_ui);
    unbounded_int b = cleanUnbounded_int(b_ui);
    unbounded_int res = UNBOUNDED_INT_ERROR;
    if (isUnboundedZero(&a) || isUnboundedZero(&b)) {
        if (isUnboundedZero(&b)) return UNBOUNDED_INT_ERROR;
        else {
            Number *c = initChiffre('0');
            res.mLength = 1;
            res.mFirst = c;
            res.mLast = c;
            res.mSign = '+';
            return res;
        }
    }
    if (isUnboundedUnity(a) || isUnboundedUnity(b)) {
        if (isUnboundedUnity(a) && isUnboundedUnity(b)) {
            Number *c = initChiffre('1');
            res.mLength = 1;
            res.mFirst = c;
            res.mLast = c;
            res.mSign = (a.mSign == b.mSign) ? '+' : '-'; 
            return res;
        } else if (isUnboundedUnity(b)) {
            a.mSign = (a.mSign == b.mSign) ? '+' : '-';
            return a;
        } 
        Number *c = initChiffre('0');
        res.mLength = 1;
        res.mFirst = c;
        res.mLast = c;
        res.mSign = (a.mSign == b.mSign) ? '+' : '-';
        return res;
    }

    if (plusGrandAbs(a, b) == -1) {
        Number *c = initChiffre('0');
        res.mLength = 1;
        res.mFirst = c;
        res.mLast = c;
        res.mSign = (a.mSign == b.mSign) ? '+' : '-';
        return res;
    } else if (plusGrandAbs(a, b) == 0) {
        Number *c = initChiffre('1');
        res.mLength = 1;
        res.mFirst = c;
        res.mLast = c;
        res.mSign = (a.mSign == b.mSign) ? '+' : '-';
        return res;
    }
    long long cmpt = 0;
    unbounded_int qo = unbounded_int_difference(a, b);
    while (unbounded_int_cmp_unbounded_int(qo, b) != -1) {
        cmpt++;
    }
    return ll2unbounded_int(cmpt);
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
    x.mSign = '+';
    return x;
}

unbounded_int unbounded_int_fact(unbounded_int n) {
    unbounded_int decr = ll2unbounded_int(1);
    unbounded_int result = ll2unbounded_int(1);
    while (unbounded_int_cmp_ll(n, 0) > 0) {
        unbounded_int tmp = unbounded_int_produit(result, n);
        unbounded_int_free(result);
        result = tmp;
        tmp = unbounded_int_difference(n, decr);
        unbounded_int_free(n);
        n = tmp;
    }
    unbounded_int_free(decr);
    return result;
}

unbounded_int unbounded_int_free(unbounded_int u) {
    Number *c = u.mFirst;
    if (c == NULL) return UNBOUNDED_INT_ERROR;
    u.mFirst = NULL;
    u.mLast = NULL;
    while (c->mNext != NULL) {
        Number *n = c->mNext;
        free(c);
        c = n;
    }
    free(c);
    return UNBOUNDED_INT_ERROR;
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
static int isAStringNum(const char *c) {
    size_t mLength = strlen(c);
    if (mLength == 0) return 0;
    int i;
    if (*c == '-' || *c == '+') {
        if (mLength == 1) return 0;
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
    return (ui.mSign == '*' || ui.mLength == 0 || ui.mFirst == NULL || ui.mLast == NULL);
}


/**
 * Vérifie que la structure est bien un 0, c'est à dire, que mLength = 1, premier = mLast = (Number) {.mNext = null, .mVal = '0' , .precedant = null}
 * @param ui pointeur vers la structure 
 * @return True si la strcutre est un 0 false sinon
 */
static int isUnboundedZero(unbounded_int *ui) {
    return ((ui->mSign == '+' || ui->mSign == '-') && ui->mLength == 1 && ui->mFirst == ui->mLast &&
            ui->mFirst->mVal == '0');
}


/**
 * Vérifie que la structure est bien un 1 (l'unité), c'est à dire, que mLength = 1, premier = mLast = (Number) {.mNext = null, .mVal = '1' , .precedant = null}
 * @param ui pointeur vers la structure 
 * @return True si la strcutre est un 1 (unité) false sinon
 */
static int isUnboundedUnity(unbounded_int ui) {
    return (ui.mLength == 1 && ui.mFirst == ui.mLast && ui.mFirst->mVal == '1' && (ui.mSign == '+' || ui.mSign == '-'));
}


/**
 * Fonction qui permet de "nettoyer" un nombre.
 * Free l'argument, s'il y a une copie.
 * @param str le nombre passé en paramètre sous forme de string
 * @return le nombre sans 0 ou d'autres characters de plus 
 */
static char *cleanNumber(char *str) { //todo: Retravailler le return de cette fonction pour qu'il soit homogene.
    size_t mLength = strlen(str);
    char * zero = malloc(sizeof (char[2]));
    zero[0] = '0';
    zero [1] = '\0';
    int i;
    if (*str == '-' || *str == '+') {
        if (mLength == 2) {
           if (*(str + 1) == '0'){
               return zero;
           }
           else {
               free(zero);
              return str;
           }
        }
        i = 1;
    } else {

        if (mLength == 1) {
            free(zero);
            return str;
        }
        i = 0;
    }
    int index = i;
    for (size_t j = i; j < mLength; j++) {
        if (*(str + j) != '0') {
            break;
        }
        index++;
    }
    if (index == i) {
        free(zero);
        return str;
    }
    if (index == mLength) return zero;
    free(zero);
    int newLen = mLength - index + 1;
    char *newStr = malloc(sizeof(char) * (newLen + 1));
    if (i == 1) newStr[0] = str[0];
    for (size_t j = index; j < mLength; j++) {
        newStr[i] = str[j];
        i++;
    }
    newStr[newLen] = '\0';
    return newStr;
}


/**
 * Fonction qui permet de "nettoyer" une structure unbounded_int
 * @param ui struct à nettoyer
 * @return un pointeur vers la structure ou une nouvelle
 */

static unbounded_int cleanUnbounded_int(unbounded_int ui) {

    if (ui.mLength <= 1) return ui;
    if (ui.mFirst->mVal != '0') return ui;
    int index = 0;
    Number *tmp;// = ui->premier;
    for (tmp = ui.mFirst; tmp != NULL; tmp = tmp->mNext) {
        if (tmp->mVal != '0') {
            break;
        }
        index += 1;
    }
    if (tmp == NULL) {
        for (int i = 0; i < ui.mLength - 1; i++) {
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
 * Initialise une struct Number à partir d'un char
 * @param c valeur de l'argument c de la struct Number
 * @return une nouvelle instance de Number
 */
static Number *initChiffre(const char c) {
    if (isNum(c) == 0) return NULL;
    Number *ch = malloc(sizeof(Number));
    if (ch == NULL) {
        perror("Pas de mémoire pour allouer la struct chiffre\n");
        return NULL;
    }
    ch->mNext = NULL;
    ch->mLast = NULL;
    ch->mVal = c;
    return ch;
}


/**
 * Ajoute d'une nouvelle struct Chiffre à la fin de la liste
 * @param ui struct source
 * @param c valeur de la nouvelle struct Number ajouté
 */
static unbounded_int pushFront(unbounded_int ui, const char c) {
    Number *ch = initChiffre(c);
    ch->mLast = ui.mLast;
    ch->mNext = NULL;
    if (ui.mLast != NULL) {
        ui.mLast->mNext = ch;
    } else if (ui.mLast == NULL && ui.mFirst == NULL) {
        ui.mFirst = ch;
    }
    ui.mLast = ch;
    ui.mLength++;
    return ui;
}


/**
 * Ajoute d'une nouvelle struct Chiffre au début de la liste
 * @param ui struct source
 * @param c valeur de la nouvelle struct Number ajouté
 */
static unbounded_int pushBack(unbounded_int ui, const char c) {
    Number *ch = initChiffre(c);
    ch->mLast = NULL;
    ch->mNext = ui.mFirst;
    if (ui.mFirst != NULL && ui.mLength != 0) {
        ui.mFirst->mLast = ch;
    } else if (ui.mLast == NULL && ui.mFirst == NULL) {
        ui.mLast = ch;
    }
    ui.mFirst = ch;
    ui.mLength++;
    return ui;
}


/**
 * Supprime le premier élément
 * @param la structure à modifier
 */
static unbounded_int deleteFirstElem(unbounded_int ui) {
    if (ui.mLength <= 1) return UNBOUNDED_INT_ERROR;
    Number *tmp;
    tmp = ui.mFirst;
    ui.mFirst = ui.mFirst->mNext;
    ui.mFirst->mLast = NULL;
    free(tmp);
    ui.mLength--;
    return ui;
}


/**
 * Delete mLast elem
 *
 */
static unbounded_int deleteLastElem(unbounded_int ui) {
    if (ui.mLength <= 1) return UNBOUNDED_INT_ERROR;
    Number *tmp;
    tmp = ui.mLast;
    ui.mLast = ui.mLast->mLast;
    ui.mLast->mNext = NULL;
    free(tmp);
    ui.mLength--;
    return ui;
}



void print_unbounded_int(unbounded_int ui) {
    unbounded_int tmp = UNBOUNDED_INT_ERROR;
    if (ui.mSign == tmp.mSign && ui.mLength == tmp.mLength && ui.mFirst == tmp.mFirst && ui.mLast == tmp.mLast) {
        printf("L'unbounded_int est vide\n");
        return;
    }
    if (ui.mSign == '-') {
        printf("-");
    }
    Number *p = ui.mFirst;
    for (int i = 0; i < ui.mLength; i++) {
        if (p == ui.mLast) {
            printf("%c\n", p->mVal);
            return;
        }
        printf("%c", p->mVal);
        p = p->mNext;
    }
}


/**
 *
 *
 *
 */
static unbounded_int sumPositifUnbounded(unbounded_int a, unbounded_int b) {
    unbounded_int res = UNBOUNDED_INT_ERROR;
    res.mSign = a.mSign;
    res.mLength = 0;
    int ret = 0;
    Number *tmp_a = a.mLast;
    Number *tmp_b = b.mLast;
    int max_len = (int) ((a.mLength < b.mLength) ? b.mLength : a.mLength);
    for (int i = 0; i < max_len; i++) {

        char ca;
        if (tmp_a == NULL) ca = '0';
        else ca = tmp_a->mVal;
        char cb;
        if (tmp_b == NULL) cb = '0';
        else cb = tmp_b->mVal;

        int c = (ca - '0') + (cb - '0') + ret;

        char unit = (char) ((c % 10) + '0');
        ret = c / 10;
        res = pushBack(res, unit);

        if (tmp_a != NULL) {
            tmp_a = tmp_a->mLast;
        }

        if (tmp_b != NULL) {
            tmp_b = tmp_b->mLast;
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
    int plusgrand = unbounded_int_cmp_unbounded_int(a,b);
    unbounded_int res = UNBOUNDED_INT_ERROR;
    if (plusgrand == 0) {
        res.mSign = '+';
        res = pushBack(res, '0');

        return res;
    }
    res.mLength = 0;
    int max_len = (int) ((a.mLength < b.mLength) ? b.mLength : a.mLength);
    Number *tmp_a;
    Number *tmp_b;
    if (plusgrand == -1) {

        res.mSign = (b.mSign == '+')? '-' : '+';
        tmp_a = b.mLast;
        tmp_b = a.mLast;
    } else {
        res.mSign = a.mSign;
        tmp_a = a.mLast;
        tmp_b = b.mLast;
    }
    int ret = 0;
    for (int i = 0; i < max_len; i++) {
        char ca;
        if (tmp_a == NULL) ca = '0';
        else ca = tmp_a->mVal;
        char cb;
        if (tmp_b == NULL) cb = '0';
        else cb = tmp_b->mVal;

        int c = (ca - '0') - (cb - '0') - ret;
        if (c < 0) {
            c += 10;
            ret = 1;
        } else ret = 0;
        char unit = (char) ((c % 10) + '0');
        res = pushBack(res, unit);

        if (tmp_a != NULL) {
            tmp_a = tmp_a->mLast;
        }

        if (tmp_b != NULL) {
            tmp_b = tmp_b->mLast;
        }

    }

    return cleanUnbounded_int(res);
}


/**
 * 
 * @return -1 si |a| < |b|, 0 si |a| = |b| et sinon 1 si |a| > |b| 
 */
static int plusGrandAbs(unbounded_int a, unbounded_int b) {
    a.mSign = '+';
    b.mSign = '+';
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
    Number *tmp = ui.mLast;
    for (int i = 0; i < ui.mLength; i++) {
        int tmp_c = tmp->mVal - '0';
        int mul = (a * tmp_c) + ret;
        ret = mul / 10;
        char num = (char) ((mul % 10) + '0');
        p = pushBack(p, num);

        tmp = tmp->mLast;
    }
    if (ret >= 1) {
        p = pushBack(p, (char) (ret + '0'));
    }
    p.mSign = ui.mSign;
    return p;
}


/**
 *
 *
 *
 */
static long long puissance(int base, int puissance) {
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

int isError(unbounded_int i) {
    return i.mSign == UNBOUNDED_INT_ERROR.mSign;
}
