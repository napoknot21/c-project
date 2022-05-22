#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "unbounded_int.h"

/**
 * fonction main (tests)
 */
int main(int argc, char *argv[]) {

    char *st_n = malloc(sizeof(char)*6+1);
    char *st_p = malloc(sizeof(char)*5+1);
    st_n = strcpy(st_n,"-45612");
    st_p = strcpy(st_p,"31245");

    long long i = 45526229262;
    long long j = -893636383;

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
}