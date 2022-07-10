
#include <stdio.h>
#include "unbounded_int.h"
#include <time.h>
#include <assert.h>
#include <string.h>

#include "test.h"

static int check_UnboundedInt_form(char *src, UnboundedInt result) {
    if (strlen(src) != result.mLength) {
        return 0;
    }
    Number *current = ((Number *) result.mFirst);
    for (size_t i = 0; i < result.mLength; i++) {
        if (current->mVal != src[i]) {
            return 0;
        }
        current = current->mNext;
    }
    return 1;
}

static void test_unboundedInt_newString() {
    char *strPos = "1123534876543";
    UnboundedInt uPos = unboundedInt_newString(strPos);
    assert(check_UnboundedInt_form(strPos, uPos));
    unboundedInt_free(uPos);
    char *strAdd = "+936419645";
    UnboundedInt uAdd = unboundedInt_newString(strAdd);
    assert(check_UnboundedInt_form(&strAdd[1], uAdd));
    unboundedInt_free(uAdd);
}

static void test_unboundedInt_newll() {
    char *strPos = "1123534876543";
    long long llPos = 1123534876543;
    UnboundedInt uPos = unboundedInt_newll(llPos);
    assert(check_UnboundedInt_form(strPos, uPos));
    unboundedInt_free(uPos);
    char *strAdd = "936419645";
    long long llAdd = +936419645;
    UnboundedInt uAdd = unboundedInt_newll(llAdd);
    assert(check_UnboundedInt_form(strAdd, uAdd));
    unboundedInt_free(uAdd);
}

static void test_UnboundedInt_toString() {
    //todo: Construction d'un UnboundedInt (positif, negetif, bug ? et comparaison)
}

static void test_UnboundedInt_cmpUnboundedInt() {
}

static void test_UnboundedInt_cmpll() {
}

static void test_UnboundedInt_add() {
    long long i1 = 645248;
    long long i2 = 543124;
    long long i3 = -46872;
    long long i4 = -13246;
    long long i5 = 5724;
    long long i6 = -6578;
    long long i7 = 0;
    UnboundedInt u1 = unboundedInt_newll(645248);
    UnboundedInt u2 = unboundedInt_newll(543124);
    UnboundedInt u3 = unboundedInt_newll(-46872);
    UnboundedInt u4 = unboundedInt_newll(-13246);
    UnboundedInt u5 = unboundedInt_newll(5724);
    UnboundedInt u6 = unboundedInt_newll(-6578);
    UnboundedInt u7 = unboundedInt_newll(0);

    UnboundedInt res1 = unboundedInt_add(u1, u2);
    UnboundedInt res2 = unboundedInt_add(u1, u7);
    UnboundedInt res3 = unboundedInt_add(u3, u4);
    UnboundedInt res4 = unboundedInt_add(u5, u7);
    UnboundedInt res5 = unboundedInt_add(u5, u1);
    UnboundedInt res6 = unboundedInt_add(u3, u5);
    UnboundedInt res7 = unboundedInt_add(u5, u6);
    int b1 = unboundedInt_cmpll(res1, i1 + i2);
    int b2 = unboundedInt_cmpll(res2, i1 + i7);
    int b3 = unboundedInt_cmpll(res3, i3 + i4);
    int b4 = unboundedInt_cmpll(res4, i5 + i7);
    int b5 = unboundedInt_cmpll(res5, i5 + i1);
    int b6 = unboundedInt_cmpll(res6, i3 + i5);
    int b7 = unboundedInt_cmpll(res7, i5 + i6);
    unboundedInt_free(u1);
    unboundedInt_free(u2);
    unboundedInt_free(u3);
    unboundedInt_free(u4);
    unboundedInt_free(u5);
    unboundedInt_free(u6);
    unboundedInt_free(u7);
    unboundedInt_free(res1);
    unboundedInt_free(res2);
    unboundedInt_free(res3);
    unboundedInt_free(res4);
    unboundedInt_free(res5);
    unboundedInt_free(res6);
    unboundedInt_free(res7);
    assert(b1 == 0);
    assert(b2 == 0);
    assert(b3 == 0);
    assert(b4 == 0);
    assert(b5 == 0);
    assert(b6 == 0);
    assert(b7 == 0);
}

static void test_UnboundedInt_subtract() { //todo: fix

    long long i1 = 645248;
    long long i2 = 543124;
    long long i3 = -46872;
    long long i4 = -13246;
    long long i5 = 5724;
    long long i6 = -6578;
    long long i7 = 0;
    UnboundedInt u1 = unboundedInt_newll(645248);
    UnboundedInt u2 = unboundedInt_newll(543124);
    UnboundedInt u3 = unboundedInt_newll(-46872);
    UnboundedInt u4 = unboundedInt_newll(-13246);
    UnboundedInt u5 = unboundedInt_newll(5724);
    UnboundedInt u6 = unboundedInt_newll(-6578);
    UnboundedInt u7 = unboundedInt_newll(0);
    UnboundedInt res1 = unboundedInt_subtract(u3, u1); // -a - (+b)
    UnboundedInt res2 = unboundedInt_subtract(u1, u4); // +a - (-b)
    UnboundedInt res3 = unboundedInt_subtract(u7, u6);
    UnboundedInt res4 = unboundedInt_subtract(u7, u5);
    UnboundedInt res5 = unboundedInt_subtract(u6, u7);
    UnboundedInt res6 = unboundedInt_subtract(u1, u2);
    UnboundedInt res7 = unboundedInt_subtract(u3, u4);
    int b1 = unboundedInt_cmpll(res1, i3 - i1);
    int b2 = unboundedInt_cmpll(res2, i1 - i4);
    int b3 = unboundedInt_cmpll(res3, i7 - i6); //Inversion 8756 vs 6578
    int b4 = unboundedInt_cmpll(res4, i7 - i5);
    int b5 = unboundedInt_cmpll(res5, i6 - i7);
    int b6 = unboundedInt_cmpll(res6, i1 - i2);
    int b7 = unboundedInt_cmpll(res7, i3 - i4);
    unboundedInt_free(u1);
    unboundedInt_free(u2);
    unboundedInt_free(u3);
    unboundedInt_free(u4);
    unboundedInt_free(u5);
    unboundedInt_free(u6);
    unboundedInt_free(u7);
    unboundedInt_free(res1);
    unboundedInt_free(res2);
    unboundedInt_free(res3);
    unboundedInt_free(res4);
    unboundedInt_free(res5);
    unboundedInt_free(res6);
    unboundedInt_free(res7);
    assert(b1 == 0);
    assert(b2 == 0);
    assert(b3 == 0);
    assert(b4 == 0);
    assert(b5 == 0);
    assert(b6 == 0);
    assert(b7 == 0);
}

static void test_UnboundedInt_multiply() {
    long long i1 = 645248;
    long long i2 = -13246;
    long long i3 = 5724;
    long long i4 = 0;

    UnboundedInt u1 = unboundedInt_newll(645248);
    UnboundedInt u2 = unboundedInt_newll(-13246);
    UnboundedInt u3 = unboundedInt_newll(5724);
    UnboundedInt u4 = unboundedInt_newll(0);

    UnboundedInt res1 = unboundedInt_multiply(u4, u1);
    UnboundedInt res2 = unboundedInt_multiply(u1, u4);
    UnboundedInt res3 = unboundedInt_multiply(u3, u3);
    UnboundedInt res4 = unboundedInt_multiply(u2, u2);
    //Todo: fix access to desallocated memory
    UnboundedInt res5 = unboundedInt_multiply(u3, u2);

    int b1 = unboundedInt_cmpll(res1, i4 * i1);
    int b2 = unboundedInt_cmpll(res2, i1 * i4);
    int b3 = unboundedInt_cmpll(res3, i3 * i3);
    int b4 = unboundedInt_cmpll(res4, i2 * i2);
    int b5 = unboundedInt_cmpll(res5, i3 + i2);
    unboundedInt_free(u1);
    unboundedInt_free(u2);
    unboundedInt_free(u3);
    unboundedInt_free(u4);
    unboundedInt_free(res1);
    unboundedInt_free(res2);
    unboundedInt_free(res3);
    unboundedInt_free(res4);
    unboundedInt_free(res5);
    assert(b1 == 0);
    assert(b2 == 0);
    assert(b3 == 0);
    assert(b4 == 0);
    assert(b5 == 0);
}

static void test_UnboundedInt_divide() {}

static void test_UnboundedInt_pow() {}

static void test_UnboundedInt_abs() {
    long long i1 = -834873642;
    long long i2 = 834873642;
    long long i3 = -0;
    long long i4 = 0;
    UnboundedInt u1 = unboundedInt_newll(i1);
    UnboundedInt u2 = unboundedInt_newll(i2);
    UnboundedInt u3 = unboundedInt_newll(i3);

    UnboundedInt res1 = unboundedInt_abs(u1);
    UnboundedInt res2 = unboundedInt_abs(u2);
    UnboundedInt res3 = unboundedInt_abs(u3);
    int b1 = unboundedInt_cmpll(res1, i2);
    int b2 = unboundedInt_cmpll(res2, i2);
    int b3 = unboundedInt_cmpll(res3, i4);
    unboundedInt_free(u1);
    unboundedInt_free(u2);
    unboundedInt_free(u3);
	unboundedInt_free(res1);
    unboundedInt_free(res2);
    unboundedInt_free(res3);
    assert(b1 == 0);
    assert(b2 == 0);
    assert(b3 == 0);
}

static long long fact(long long n) {
    if (n <= 0) return 1;
    long long val = 1;
    for (long long i = 1; i < n; i++) val *= i;
    return val;
}

static void test_UnboundedInt_fact() {
    long long i1 = fact(5);
    long long i2 = 1;
    long long i3 = fact(20);
    UnboundedInt u1 = unboundedInt_newll(5);
    UnboundedInt u2 = unboundedInt_newll(0);
    UnboundedInt u3 = unboundedInt_newll(20);
    UnboundedInt res1 = unboundedInt_fact(u1);
    UnboundedInt res2 = unboundedInt_fact(u2);
    UnboundedInt res3 = unboundedInt_fact(u3);
    int b1 = unboundedInt_cmpll(res1, i1);
    int b2 = unboundedInt_cmpll(res2, i2);
    int b3 = unboundedInt_cmpll(res3, i3);
    unboundedInt_free(u1);
    unboundedInt_free(u2);
    unboundedInt_free(u3);
    unboundedInt_free(res1);
    unboundedInt_free(res2);
    unboundedInt_free(res3);
    assert(b1);
    assert(b2);
    assert(b3);
}

int main() {
    test(test_unboundedInt_newll, "unboundedInt_newll()");
    test(test_unboundedInt_newString, "unboundedInt_newString()");
    test(test_UnboundedInt_toString, "UnboundedInt_toString()");
    test(test_UnboundedInt_cmpll, "UnboundedInt_cmpll()");
    test(test_UnboundedInt_cmpUnboundedInt, "UnboundedInt_cmpUnboundedInt()");
	test(test_UnboundedInt_abs, "UnboundedInt_abs()");
    test(test_UnboundedInt_add, "UnboundedInt_add()");
    test(test_UnboundedInt_subtract, "UnboundedInt_subtract()");
    //test(test_UnboundedInt_multiply, "UnboundedInt_multiply");
    //test(test_UnboundedInt_divide, "UnboundedInt_divide");
    //test(test_UnboundedInt_fact, "UnboundedInt_fact");
    //test(test_UnboundedInt_pow, "UnboundedInt_pow");
    return 0;
}