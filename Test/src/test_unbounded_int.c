
#include <stdio.h>
#include "unbounded_int.h"
#include <time.h>
#include <assert.h>
#include <string.h>

#include "test.h"
#include "lib.h"

static int check_UnboundedInt_form(char *src, UnboundedInt result) {
    if (strlen(src) != result.mLength) {
        return 0;
    }
    Number *current = result.mFirst;
    for (size_t i = 0; i < result.mLength; i++) {
        if (current->mVal != src[i]) {
            return 0;
        }
        current = current->mNext;
    }
    return 1;
}

static long long pow(long long a, long long b) {
    if (b < 0) {
        return 0;
    }
    if (b == 0) {
	    return 1;
    }
	long long tmp = pow(a, b / 2);
    long long square = tmp * tmp;
    if (b%2 == 0) {
        return square;
    }
    return a * square;
}

static void test_UnboundedInt_newString() {
    char *strPos = "1123534876543";
    UnboundedInt uPos = UnboundedInt_newString(strPos);
    assert(check_UnboundedInt_form(strPos, uPos));
    UnboundedInt_free(uPos);
    char *strAdd = "+936419645";
    UnboundedInt uAdd = UnboundedInt_newString(strAdd);
    assert(check_UnboundedInt_form(&strAdd[1], uAdd));
    UnboundedInt_free(uAdd);
    char *strNeg = "-936419645";
    UnboundedInt uNeg = UnboundedInt_newString(strNeg);
    assert(check_UnboundedInt_form(&strNeg[1], uNeg) && uNeg.mSign == '-');
    UnboundedInt_free(uNeg);
}

static void test_UnboundedInt_newll() {
    char *strPos = "1123534876543";
    long long llPos = 1123534876543;
    UnboundedInt uPos = UnboundedInt_newll(llPos);
    assert(check_UnboundedInt_form(strPos, uPos));
    UnboundedInt_free(uPos);
    char *strAdd = "936419645";
    long long llAdd = +936419645;
    UnboundedInt uAdd = UnboundedInt_newll(llAdd);
    assert(check_UnboundedInt_form(strAdd, uAdd));
    UnboundedInt_free(uAdd);
    char *strNeg = "-936419645";
    UnboundedInt uNeg = UnboundedInt_newString(strNeg);
    assert(check_UnboundedInt_form(&strNeg[1], uNeg) && uNeg.mSign == '-');
    UnboundedInt_free(uNeg);
}

static void test_UnboundedInt_toString() {
    char *strPos = "1123534876543";
    long long llPos = 1123534876543;
    UnboundedInt uPos = UnboundedInt_newll(llPos);
    char *res = UnboundedInt_toString(uPos);
    assert(str_equals(strPos, res));
    UnboundedInt_free(uPos);
    free(res);
    char *strAdd = "936419645";
    long long llAdd = +936419645;
    UnboundedInt uAdd = UnboundedInt_newll(llAdd);
    res = UnboundedInt_toString(uAdd);
    assert(str_equals(res,strAdd));
    UnboundedInt_free(uAdd);
    free(res);
    char *strNeg = "-936419645";
    UnboundedInt uNeg = UnboundedInt_newString(strNeg);
    res = UnboundedInt_toString(uNeg);
    assert(str_equals(res, strNeg));
    UnboundedInt_free(uNeg);
    free(res);
}

static void test_UnboundedInt_cmpUnboundedInt() {
    UnboundedInt u1 = UnboundedInt_newll(645248);
    UnboundedInt u2 = UnboundedInt_newll(543124);
    UnboundedInt u3 = UnboundedInt_newll(-46872);
    UnboundedInt u4 = UnboundedInt_newll(-13246);
    UnboundedInt u5 = UnboundedInt_newll(5724);
    UnboundedInt u6 = UnboundedInt_newll(-6578);
    UnboundedInt u7 = UnboundedInt_newll(0);
    int b1 = UnboundedInt_cmpUnboundedInt(u1, u7);
    int b2 = UnboundedInt_cmpUnboundedInt(u2, u1);
    int b3 = UnboundedInt_cmpUnboundedInt(u3, u3);
    int b4 = UnboundedInt_cmpUnboundedInt(u4, u5);
    int b5 = UnboundedInt_cmpUnboundedInt(u5, u7);
    int b6 = UnboundedInt_cmpUnboundedInt(u4, u3);
    int b7 = UnboundedInt_cmpUnboundedInt(u7, u6);
    UnboundedInt_free(u1);
    UnboundedInt_free(u2);
    UnboundedInt_free(u3);
    UnboundedInt_free(u4);
    UnboundedInt_free(u5);
    UnboundedInt_free(u6);
    UnboundedInt_free(u7);
    assert(b1 > 0);
    assert(b2 < 0);
    assert(b3 == 0);
    assert(b4 < 0);
    assert(b5 > 0);
    assert(b6 < 0);
    assert(b7 > 0);
}

static void test_UnboundedInt_cmpll() {
    long long i1 = 645248;
    long long i3 = -46872;
    long long i5 = 5724;
    long long i6 = -6578;
    long long i7 = 0;
    UnboundedInt u1 = UnboundedInt_newll(645248);
    UnboundedInt u2 = UnboundedInt_newll(543124);
    UnboundedInt u3 = UnboundedInt_newll(-46872);
    UnboundedInt u4 = UnboundedInt_newll(-13246);
    UnboundedInt u5 = UnboundedInt_newll(5724);
    UnboundedInt u6 = UnboundedInt_newll(-6578);
    UnboundedInt u7 = UnboundedInt_newll(0);
    int b1 = UnboundedInt_cmpll(u1, i7);
    int b2 = UnboundedInt_cmpll(u2, i1);
    int b3 = UnboundedInt_cmpll(u3, i3);
    int b4 = UnboundedInt_cmpll(u4, i5);
    int b5 = UnboundedInt_cmpll(u5, i7);
    int b6 = UnboundedInt_cmpll(u4, i3);
    int b7 = UnboundedInt_cmpll(u7, i6);
    UnboundedInt_free(u1);
    UnboundedInt_free(u2);
    UnboundedInt_free(u3);
    UnboundedInt_free(u4);
    UnboundedInt_free(u5);
    UnboundedInt_free(u6);
    UnboundedInt_free(u7);
    assert(b1 > 0);
    assert(b2 < 0);
    assert(b3 == 0);
    assert(b4 < 0);
    assert(b5 > 0);
    assert(b6 < 0);
    assert(b7 > 0);
}

static void test_UnboundedInt_add() {
    long long i1 = 645248;
    long long i2 = 543124;
    long long i3 = -46872;
    long long i4 = -13246;
    long long i5 = 5724;
    long long i6 = -6578;
    long long i7 = 0;
    UnboundedInt u1 = UnboundedInt_newll(645248);
    UnboundedInt u2 = UnboundedInt_newll(543124);
    UnboundedInt u3 = UnboundedInt_newll(-46872);
    UnboundedInt u4 = UnboundedInt_newll(-13246);
    UnboundedInt u5 = UnboundedInt_newll(5724);
    UnboundedInt u6 = UnboundedInt_newll(-6578);
    UnboundedInt u7 = UnboundedInt_newll(0);

    UnboundedInt res1 = UnboundedInt_add(u1, u2);
    UnboundedInt res2 = UnboundedInt_add(u1, u7);
    UnboundedInt res3 = UnboundedInt_add(u3, u4);
    UnboundedInt res4 = UnboundedInt_add(u5, u7);
    UnboundedInt res5 = UnboundedInt_add(u5, u1);
    UnboundedInt res6 = UnboundedInt_add(u3, u5);
    UnboundedInt res7 = UnboundedInt_add(u5, u6);
    int b1 = UnboundedInt_cmpll(res1, i1 + i2);
    int b2 = UnboundedInt_cmpll(res2, i1 + i7);
    int b3 = UnboundedInt_cmpll(res3, i3 + i4);
    int b4 = UnboundedInt_cmpll(res4, i5 + i7);
    int b5 = UnboundedInt_cmpll(res5, i5 + i1);
    int b6 = UnboundedInt_cmpll(res6, i3 + i5);
    int b7 = UnboundedInt_cmpll(res7, i5 + i6);
    UnboundedInt_free(u1);
    UnboundedInt_free(u2);
    UnboundedInt_free(u3);
    UnboundedInt_free(u4);
    UnboundedInt_free(u5);
    UnboundedInt_free(u6);
    UnboundedInt_free(u7);
    UnboundedInt_free(res1);
    UnboundedInt_free(res2);
    UnboundedInt_free(res3);
    UnboundedInt_free(res4);
    UnboundedInt_free(res5);
    UnboundedInt_free(res6);
    UnboundedInt_free(res7);
    assert(b1 == 0);
    assert(b2 == 0);
    assert(b3 == 0);
    assert(b4 == 0);
    assert(b5 == 0);
    assert(b6 == 0);
    assert(b7 == 0);
}

static void test_UnboundedInt_subtract() {
    long long i1 = 645248;
    long long i2 = 543124;
    long long i3 = -46872;
    long long i4 = -13246;
    long long i5 = 5724;
    long long i6 = -6578;
    long long i7 = 0;
    UnboundedInt u1 = UnboundedInt_newll(645248);
    UnboundedInt u2 = UnboundedInt_newll(543124);
    UnboundedInt u3 = UnboundedInt_newll(-46872);
    UnboundedInt u4 = UnboundedInt_newll(-13246);
    UnboundedInt u5 = UnboundedInt_newll(5724);
    UnboundedInt u6 = UnboundedInt_newll(-6578);
    UnboundedInt u7 = UnboundedInt_newll(0);
    UnboundedInt res1 = UnboundedInt_subtract(u3, u1); // -a - (+b)
    UnboundedInt res2 = UnboundedInt_subtract(u1, u4); // +a - (-b)
    UnboundedInt res3 = UnboundedInt_subtract(u7, u6);
    UnboundedInt res4 = UnboundedInt_subtract(u7, u5);
    UnboundedInt res5 = UnboundedInt_subtract(u6, u7);
    UnboundedInt res6 = UnboundedInt_subtract(u1, u2);
    UnboundedInt res7 = UnboundedInt_subtract(u3, u4);
    int b1 = UnboundedInt_cmpll(res1, i3 - i1);
    int b2 = UnboundedInt_cmpll(res2, i1 - i4);
    int b3 = UnboundedInt_cmpll(res3, i7 - i6); //Inversion 8756 vs 6578
    int b4 = UnboundedInt_cmpll(res4, i7 - i5);
    int b5 = UnboundedInt_cmpll(res5, i6 - i7);
    int b6 = UnboundedInt_cmpll(res6, i1 - i2);
    int b7 = UnboundedInt_cmpll(res7, i3 - i4);
    UnboundedInt_free(u1);
    UnboundedInt_free(u2);
    UnboundedInt_free(u3);
    UnboundedInt_free(u4);
    UnboundedInt_free(u5);
    UnboundedInt_free(u6);
    UnboundedInt_free(u7);
    UnboundedInt_free(res1);
    UnboundedInt_free(res2);
    UnboundedInt_free(res3);
    UnboundedInt_free(res4);
    UnboundedInt_free(res5);
    UnboundedInt_free(res6);
    UnboundedInt_free(res7);
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

    UnboundedInt u1 = UnboundedInt_newll(645248);
    UnboundedInt u2 = UnboundedInt_newll(-13246);
    UnboundedInt u3 = UnboundedInt_newll(5724);
    UnboundedInt u4 = UnboundedInt_newll(0);

    UnboundedInt res1 = UnboundedInt_multiply(u4, u1);
    UnboundedInt res2 = UnboundedInt_multiply(u1, u4);
    UnboundedInt res3 = UnboundedInt_multiply(u3, u3);
    UnboundedInt res4 = UnboundedInt_multiply(u2, u2);
    UnboundedInt res5 = UnboundedInt_multiply(u3, u2);
	int b1 = UnboundedInt_cmpll(res1, i4 * i1);
    int b2 = UnboundedInt_cmpll(res2, i1 * i4);
    int b3 = UnboundedInt_cmpll(res3, i3 * i3);
    int b4 = UnboundedInt_cmpll(res4, i2 * i2);
    int b5 = UnboundedInt_cmpll(res5, i3 * i2);
    UnboundedInt_free(u1);
    UnboundedInt_free(u2);
    UnboundedInt_free(u3);
    UnboundedInt_free(u4);
    UnboundedInt_free(res1);
    UnboundedInt_free(res2);
    UnboundedInt_free(res3);
    UnboundedInt_free(res4);
    UnboundedInt_free(res5);
    assert(b1 == 0);
    assert(b2 == 0);
    assert(b3 == 0);
    assert(b4 == 0);
    assert(b5 == 0);
}

static void test_UnboundedInt_abs() {
    long long i1 = -834873642;
    long long i2 = 834873642;
    long long i3 = -0;
    long long i4 = 0;
    UnboundedInt u1 = UnboundedInt_newll(i1);
    UnboundedInt u2 = UnboundedInt_newll(i2);
    UnboundedInt u3 = UnboundedInt_newll(i3);

    UnboundedInt res1 = UnboundedInt_abs(u1);
    UnboundedInt res2 = UnboundedInt_abs(u2);
    UnboundedInt res3 = UnboundedInt_abs(u3);
    int b1 = UnboundedInt_cmpll(res1, i2);
    int b2 = UnboundedInt_cmpll(res2, i2);
    int b3 = UnboundedInt_cmpll(res3, i4);
    UnboundedInt_free(u1);
    UnboundedInt_free(u2);
    UnboundedInt_free(u3);
	UnboundedInt_free(res1);
    UnboundedInt_free(res2);
    UnboundedInt_free(res3);
    assert(b1 == 0);
    assert(b2 == 0);
    assert(b3 == 0);
}

static long long fact(long long n) {
    if (n <= 0) return 1;
    long long val = 1;
    for (long long i = 1; i <= n; i++) val *= i;
    return val;
}

static void test_UnboundedInt_fact() {
    long long i1 = fact(5);
    long long i2 = 1;
    long long i3 = fact(20);
    UnboundedInt u1 = UnboundedInt_newll(5);
    UnboundedInt u2 = UnboundedInt_newll(0);
    UnboundedInt u3 = UnboundedInt_newll(20);
    UnboundedInt res1 = UnboundedInt_fact(u1);
    UnboundedInt res2 = UnboundedInt_fact(u2);
    UnboundedInt res3 = UnboundedInt_fact(u3);
    int b1 = UnboundedInt_cmpll(res1, i1);
    int b2 = UnboundedInt_cmpll(res2, i2);
    int b3 = UnboundedInt_cmpll(res3, i3);
    UnboundedInt_free(u1);
    UnboundedInt_free(u2);
    UnboundedInt_free(u3);
    UnboundedInt_free(res1);
    UnboundedInt_free(res2);
    UnboundedInt_free(res3);
    assert(b1 == 0);
    assert(b2 == 0);
    assert(b3 == 0);
}

static void test_UnboundedInt_divide() {
    long long i1 = 30;
    long long i2 = 2;
    long long i3 = -46872;
    long long i4 = -13246;
    long long i5 = 5724;
    long long i6 = -6578;
    long long i7 = 0;
    UnboundedInt u1 = UnboundedInt_newll(i1);
    UnboundedInt u2 = UnboundedInt_newll(i2);
    UnboundedInt u3 = UnboundedInt_newll(i3);
    UnboundedInt u4 = UnboundedInt_newll(i4);
    UnboundedInt u5 = UnboundedInt_newll(i5);
    UnboundedInt u6 = UnboundedInt_newll(i6);
    UnboundedInt u7 = UnboundedInt_newll(i7);
    UnboundedInt res1 = UnboundedInt_divide(u1, u2);
    UnboundedInt res2 = UnboundedInt_divide(u2, u1);
    UnboundedInt res3 = UnboundedInt_divide(u7, u6);
    UnboundedInt res4 = UnboundedInt_divide(u7, u5);
    UnboundedInt res5 = UnboundedInt_divide(u6, u7);
    UnboundedInt res6 = UnboundedInt_divide(u1, u2);
    UnboundedInt res7 = UnboundedInt_divide(u3, u4);
    UnboundedInt res8 = UnboundedInt_divide(u1, u4);
    int b1 = UnboundedInt_cmpll(res1, i1 / i2);
    int b2 = UnboundedInt_cmpll(res2, i2 / i1);
    int b3 = UnboundedInt_cmpll(res3, i7 / i6);
    int b4 = UnboundedInt_cmpll(res4, i7 / i5);
    int b5 = u5.mSign == '*';
    int b6 = UnboundedInt_cmpll(res6, i1 / i2);
    int b7 = UnboundedInt_cmpll(res7, i3 / i4);
    int b8 = UnboundedInt_cmpll(res8, i1 / i4);
    UnboundedInt_free(u1);
    UnboundedInt_free(u2);
    UnboundedInt_free(u3);
    UnboundedInt_free(u4);
    UnboundedInt_free(u5);
    UnboundedInt_free(u6);
    UnboundedInt_free(u7);
    UnboundedInt_free(res1);
    UnboundedInt_free(res2);
    UnboundedInt_free(res3);
    UnboundedInt_free(res4);
    UnboundedInt_free(res5);
    UnboundedInt_free(res6);
    UnboundedInt_free(res7);
    UnboundedInt_free(res8);
    assert(b1 == 0);
    assert(b2 == 0);
    assert(b3 == 0);
    assert(b4 == 0);
    assert(b5 == 0);
    assert(b6 == 0);
    assert(b7 == 0);
    assert(b8 == 0);

}

static void test_UnboundedInt_modulo() {
    long long i1 = 30;
    long long i2 = 2;
    long long i3 = -46872;
    long long i4 = -13246;
    long long i5 = 5724;
    long long i6 = -6578;
    long long i7 = 0;
    UnboundedInt u1 = UnboundedInt_newll(i1);
    UnboundedInt u2 = UnboundedInt_newll(i2);
    UnboundedInt u3 = UnboundedInt_newll(i3);
    UnboundedInt u4 = UnboundedInt_newll(i4);
    UnboundedInt u5 = UnboundedInt_newll(i5);
    UnboundedInt u6 = UnboundedInt_newll(i6);
    UnboundedInt u7 = UnboundedInt_newll(i7);
    UnboundedInt res1 = UnboundedInt_modulo(u1, u2);
    UnboundedInt res2 = UnboundedInt_modulo(u2, u1);
    UnboundedInt res3 = UnboundedInt_modulo(u7, u6);
    UnboundedInt res4 = UnboundedInt_modulo(u7, u5);
    UnboundedInt res5 = UnboundedInt_modulo(u6, u7);
    UnboundedInt res6 = UnboundedInt_modulo(u1, u2);
    UnboundedInt res7 = UnboundedInt_modulo(u3, u4);
    UnboundedInt res8 = UnboundedInt_modulo(u1, u4);
    int b1 = UnboundedInt_cmpll(res1, i1 % i2);
    int b2 = UnboundedInt_cmpll(res2, i2 % i1);
    int b3 = UnboundedInt_cmpll(res3, i7 % i6);
    int b4 = UnboundedInt_cmpll(res4, i7 % i5);
    int b5 = u5.mSign == '*';
    int b6 = UnboundedInt_cmpll(res6, i1 % i2);
    int b7 = UnboundedInt_cmpll(res7, i3 % i4);
    int b8 = UnboundedInt_cmpll(res8, i1 % i4);
    UnboundedInt_free(u1);
    UnboundedInt_free(u2);
    UnboundedInt_free(u3);
    UnboundedInt_free(u4);
    UnboundedInt_free(u5);
    UnboundedInt_free(u6);
    UnboundedInt_free(u7);
    UnboundedInt_free(res1);
    UnboundedInt_free(res2);
    UnboundedInt_free(res3);
    UnboundedInt_free(res4);
    UnboundedInt_free(res5);
    UnboundedInt_free(res6);
    UnboundedInt_free(res7);
    UnboundedInt_free(res8);
    assert(b1 == 0);
    assert(b2 == 0);
    assert(b3 == 0);
    assert(b4 == 0);
    assert(b5 == 0);
    assert(b6 == 0);
    assert(b7 == 0);
    assert(b8 == 0);

}

static void test_UnboundedInt_pow() {
    long long i1 = 2;
    long long i2 = 4;
    long long i3 = -46872;
    long long i4 = -13246;
    long long i5 = 5724;
    long long i6 = -6578;
    long long i7 = 0;
    UnboundedInt u1 = UnboundedInt_newll(i1);
    UnboundedInt u2 = UnboundedInt_newll(i2);
    UnboundedInt u3 = UnboundedInt_newll(i3);
    UnboundedInt u4 = UnboundedInt_newll(i4);
    UnboundedInt u5 = UnboundedInt_newll(i5);
    UnboundedInt u6 = UnboundedInt_newll(i6);
    UnboundedInt u7 = UnboundedInt_newll(i7);
    UnboundedInt res1 = UnboundedInt_pow(u1, u2);
    UnboundedInt res2 = UnboundedInt_pow(u2, u1);
    UnboundedInt res3 = UnboundedInt_pow(u7, u6);
    UnboundedInt res4 = UnboundedInt_pow(u7, u5);
    UnboundedInt res5 = UnboundedInt_pow(u6, u7);
    UnboundedInt res6 = UnboundedInt_pow(u4, u2);
    UnboundedInt res7 = UnboundedInt_pow(u3, u4);
    UnboundedInt res8 = UnboundedInt_pow(u1, u4);
    int b1 = UnboundedInt_cmpll(res1, pow(i1 , i2));
    int b2 = UnboundedInt_cmpll(res2, pow(i2 , i1));
    int b3 = UnboundedInt_cmpll(res3, pow(i7, i6));
    int b4 = UnboundedInt_cmpll(res4, pow(i7 , i5));
    int b5 = u5.mSign == '*';         
    int b6 = UnboundedInt_cmpll(res6, pow(i4 , i2));
    int b7 = UnboundedInt_cmpll(res7, pow(i3 , i4));
    int b8 = UnboundedInt_cmpll(res8, pow(i1 , i4));
    UnboundedInt_free(u1);
    UnboundedInt_free(u2);
    UnboundedInt_free(u3);
    UnboundedInt_free(u4);
    UnboundedInt_free(u5);
    UnboundedInt_free(u6);
    UnboundedInt_free(u7);
    UnboundedInt_free(res1);
    UnboundedInt_free(res2);
    UnboundedInt_free(res3);
    UnboundedInt_free(res4);
    UnboundedInt_free(res5);
    UnboundedInt_free(res6);
    UnboundedInt_free(res7);
    UnboundedInt_free(res8);
    assert(b1 == 0);
    assert(b2 == 0);
    assert(b3 == 0);
    assert(b4 == 0);
    assert(b5 == 0);
    assert(b6 == 0);
    assert(b7 == 0);
    assert(b8 == 0);
}

int main() {
    test(test_UnboundedInt_newll, "UnboundedInt_newll()");
    test(test_UnboundedInt_newString, "UnboundedInt_newString()");
    test(test_UnboundedInt_toString, "UnboundedInt_toString()");
    test(test_UnboundedInt_cmpll, "UnboundedInt_cmpll()");
    test(test_UnboundedInt_cmpUnboundedInt, "UnboundedInt_cmpUnboundedInt()");
	test(test_UnboundedInt_abs, "UnboundedInt_abs()");
    test(test_UnboundedInt_add, "UnboundedInt_add()");
    test(test_UnboundedInt_subtract, "UnboundedInt_subtract()");
    test(test_UnboundedInt_multiply, "UnboundedInt_multiply()");
    test(test_UnboundedInt_fact, "UnboundedInt_fact()");
    test(test_UnboundedInt_divide, "UnboundedInt_divide()");
    test(test_UnboundedInt_modulo, "UnboundedInt_modulo()");
    test(test_UnboundedInt_pow, "UnboundedInt_pow()");
    return 0;
}