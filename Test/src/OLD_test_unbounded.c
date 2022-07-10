#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "unbounded_int.h"

/**
 * fonction main (tests)
 */
int main(int argc, char *argv[]) {

    char *st_n = malloc(sizeof(char) * 6 + 1);
    char *st_p = malloc(sizeof(char) * 5 + 1);
    st_n = strcpy(st_n, "-45612");
    st_p = strcpy(st_p, "31245");

    long long i = 45526229262;
    long long j = -893636383;

    printf("===========\n");
    printf("=MAIN FUNCTIONS=\n");
    printf("===========\n");

    //STRING2UNBOUNDED_INT () TEST
    printf("==STRING2UNBOUNDED_INT() TEST==\n");
    UnboundedInt ui1 = unboundedInt_newString(st_n);
    UnboundedInt ui2 = unboundedInt_newString(st_p);

    print_UnboundedInt(ui1);
    print_UnboundedInt(ui2);

    printf("===========\n");

    //LL2UNBOUNDED_INT() TEST
    printf("==LL2UNBOUNDED_INT() TEST==\n");
    UnboundedInt ui3 = ll2UnboundedInt(i);
    UnboundedInt ui4 = ll2UnboundedInt(j);

    print_UnboundedInt(ui3);
    print_UnboundedInt(ui4);

    printf("===========\n");

    //UNBOUNDED_INT2STRING() TEST
    printf("==UNBOUNDED_INT2STRING() TEST==\n");
    printf("%s\n", UnboundedInt2string(ui1));
    printf("%s\n", UnboundedInt2string(ui2));
    printf("%s\n", UnboundedInt2string(ui3));
    printf("%s\n", UnboundedInt2string(ui4));

    printf("===========\n");

    //UNBOUNDED_INT_SOMME() TEST
    printf("==UNBOUNDED_INT_SOMME() TEST==\n");

    UnboundedInt ui5 = unboundedInt_newString("645248");
    UnboundedInt ui6 = unboundedInt_newString("543124");
    UnboundedInt ui7 = unboundedInt_newString("-46872");
    UnboundedInt ui8 = unboundedInt_newString("-13246");
    UnboundedInt ui9 = unboundedInt_newString("5724");
    UnboundedInt ui10 = unboundedInt_newString("-6578");
    UnboundedInt zero = unboundedInt_newString("0");

    UnboundedInt res1 = unboundedInt_add(ui5, ui6);
    UnboundedInt res2 = unboundedInt_add(ui5, zero);
    UnboundedInt res3 = unboundedInt_add(ui7, ui8);
    UnboundedInt res4 = unboundedInt_add(ui7, zero);
    UnboundedInt res5 = unboundedInt_add(ui9, ui5);
    UnboundedInt res6 = unboundedInt_add(ui7, ui10);
    UnboundedInt res6_5 = unboundedInt_add(ui9, ui10);


    printf("%s + %s = %s\n", UnboundedInt2string(ui5), UnboundedInt2string(ui6), UnboundedInt2string(res1));
    printf("%s + %s = %s\n", UnboundedInt2string(ui5), UnboundedInt2string(zero), UnboundedInt2string(res2));
    printf("%s + (%s) = %s\n", UnboundedInt2string(ui7), UnboundedInt2string(ui8), UnboundedInt2string(res3));
    printf("%s + %s = %s\n", UnboundedInt2string(ui7), UnboundedInt2string(zero), UnboundedInt2string(res4));
    printf("%s + %s = %s\n", UnboundedInt2string(ui9), UnboundedInt2string(ui5), UnboundedInt2string(res5));
    printf("%s + (%s) = %s\n", UnboundedInt2string(ui7), UnboundedInt2string(ui10), UnboundedInt2string(res6));
    printf("%s + (%s) = %s\n", UnboundedInt2string(ui9), UnboundedInt2string(ui10), UnboundedInt2string(res6_5));

    printf("===========\n");


    //UNBOUNDED_INT_SOMME() TEST
    printf("==UNBOUNDED_INT_DIFFERENCE() TEST==\n");

    UnboundedInt res7 = unboundedInt_subtract(ui7, ui5); // -a - (+b)
    UnboundedInt res8 = unboundedInt_subtract(ui5, ui8); // +a - (-b)
    UnboundedInt res9 = unboundedInt_subtract(zero, ui10);
    UnboundedInt res10 = unboundedInt_subtract(zero, ui9);
    UnboundedInt res11 = unboundedInt_subtract(ui10, ui10);
    UnboundedInt res12 = unboundedInt_subtract(ui5, ui6);
    UnboundedInt res13 = unboundedInt_subtract(ui7, ui8);


    printf("%s - %s = %s\n", UnboundedInt2string(ui7), UnboundedInt2string(ui5), UnboundedInt2string(res7));
    printf("%s - (%s) = %s\n", UnboundedInt2string(ui5), UnboundedInt2string(ui8), UnboundedInt2string(res8));
    printf("%s - (%s) = %s\n", UnboundedInt2string(zero), UnboundedInt2string(ui10), UnboundedInt2string(res9));
    printf("%s - %s = %s\n", UnboundedInt2string(zero), UnboundedInt2string(ui9), UnboundedInt2string(res10));
    printf("%s - (%s) = %s\n", UnboundedInt2string(ui10), UnboundedInt2string(ui10), UnboundedInt2string(res11));
    printf("%s - %s = %s\n", UnboundedInt2string(ui5), UnboundedInt2string(ui6), UnboundedInt2string(res12));
    printf("%s - (%s) = %s\n", UnboundedInt2string(ui7), UnboundedInt2string(ui8), UnboundedInt2string(res13));


    printf("===========\n");

    //UNBOUNDED_INT_PRODUIT() TEST
    printf("==UNBOUNDED_INT_PRODUIT() TEST==\n");

    UnboundedInt res14 = unboundedInt_multiply(zero, ui5);
    UnboundedInt res14_5 = unboundedInt_multiply(ui5, zero);
    UnboundedInt res15 = unboundedInt_multiply(ui9, ui9);
    UnboundedInt res16 = unboundedInt_multiply(ui8, ui8);
    UnboundedInt res17 = unboundedInt_multiply(ui9, ui8);

    printf("%s * (%s) = %s\n", UnboundedInt2string(zero), UnboundedInt2string(ui5), UnboundedInt2string(res14));
    printf("%s * (%s) = %s\n", UnboundedInt2string(ui5), UnboundedInt2string(zero), UnboundedInt2string(res14_5));
    printf("%s * (%s) = %s\n", UnboundedInt2string(ui9), UnboundedInt2string(ui9), UnboundedInt2string(res15));
    printf("%s * (%s) = %s\n", UnboundedInt2string(ui8), UnboundedInt2string(ui8), UnboundedInt2string(res16));
    printf("%s * (%s) = %s\n", UnboundedInt2string(ui9), UnboundedInt2string(ui8), UnboundedInt2string(res17));


    printf("===========\n");

    //UNBOUNDED_INT_DIVISION() TEST
    printf("==UNBOUNDED_INT_DIVISION() TEST==\n");


    return 0;
}