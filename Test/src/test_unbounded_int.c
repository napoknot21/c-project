
#include <stdio.h>
#include "unbounded_int.h"
#include <time.h>
#include <assert.h>
#include <string.h>

static void check_unbounded_int_form(char* src, unbounded_int result) {
	assert(strlen(src) == result.mLength);
	Number current = *((Number *) result.mFirst);
	for (size_t i = 0; i < result.mLength; i++) {
		assert(current.mVal == src[i]);
	}
}

static void test_string2unbounded_int() {
	printf("########## TEST STRING2UNBOUNDED_INT ##########\n");
	char* strPos = "1123534876543";
	unbounded_int uPos = string2unbounded_int(strPos);
	check_unbounded_int_form(strPos, uPos);
	unbounded_int_free(uPos);
	char* strAdd = "+936419645";
	unbounded_int uAdd = string2unbounded_int(strAdd);
	check_unbounded_int_form(&strAdd[1], uAdd);
	unbounded_int_free(uAdd);
}

static void test_ll2unbounded_int() {
	printf("########## TEST LL2UNBOUNDED_INT ##########\n");
	char* strPos = "1123534876543";
	long long llPos = 1123534876543;
	unbounded_int uPos = ll2unbounded_int(llPos);
	check_unbounded_int_form(strPos, uPos);
	unbounded_int_free(uPos);
	char* strAdd = "936419645";
	long long llAdd = +936419645;
	unbounded_int uAdd = ll2unbounded_int(llAdd);
	check_unbounded_int_form(strAdd, uAdd);
	unbounded_int_free(uAdd);
}

static void test_unbounded_int2string() {
	//todo: Construction d'un unbounded_int (positif, negetif, bug ? et comparaison)
}

static void test_unbounded_int_cmp_unbounded_int() {}

static void test_unbounded_int_cmp_ll() {}

static void test_unbounded_int_somme() {
	printf("############ TEST UNBOUNDED_INT_SOMME ###########\n");
	long long i1 = 645248;
	long long i2 = 543124;
	long long i3 = -46872;
	long long i4 = -13246;
	long long i5 = 5724;
	long long i6 = -6578;
	long long i7 = 0;
	unbounded_int u1 = ll2unbounded_int(645248);
	unbounded_int u2 = ll2unbounded_int(543124);
	unbounded_int u3 = ll2unbounded_int(-46872);
	unbounded_int u4 = ll2unbounded_int(-13246);
	unbounded_int u5 = ll2unbounded_int(5724);
	unbounded_int u6 = ll2unbounded_int(-6578);
	unbounded_int u7 = ll2unbounded_int(0);

	unbounded_int res1 = unbounded_int_somme(u1, u2);
	unbounded_int res2 = unbounded_int_somme(u1, u7);
	unbounded_int res3 = unbounded_int_somme(u3, u4);
	unbounded_int res4 = unbounded_int_somme(u5, u7);
	unbounded_int res5 = unbounded_int_somme(u5, u1);
	unbounded_int res6 = unbounded_int_somme(u3, u5);
	unbounded_int res7 = unbounded_int_somme(u5, u6);

	int b1 = unbounded_int_cmp_ll(res1, i1 + i2);
	int b2 = unbounded_int_cmp_ll(res2, i1 + i7);
	int b3 = unbounded_int_cmp_ll(res3, i3 + i4);
	int b4 = unbounded_int_cmp_ll(res4, i5 + i7);
	int b5 = unbounded_int_cmp_ll(res5, i5 + i1);
	int b6 = unbounded_int_cmp_ll(res6, i3 + i5);
	int b7 = unbounded_int_cmp_ll(res7, i5 + i6);
	unbounded_int_free(u1);
	unbounded_int_free(u2);
	unbounded_int_free(u3);
	unbounded_int_free(u4);
	unbounded_int_free(u5);
	unbounded_int_free(u6);
	unbounded_int_free(u7);
	unbounded_int_free(res1);
	unbounded_int_free(res2);
	unbounded_int_free(res3);
	unbounded_int_free(res4);
	unbounded_int_free(res5);
	unbounded_int_free(res6);
	unbounded_int_free(res7);
	assert(b1);
	assert(b2);
	assert(b3);
	assert(b4);
	assert(b5);
	assert(b6);
	assert(b7);
}

static void test_unbounded_int_difference() {
	printf("########## TEST UNBOUNDED_INT_DIFFERENCE ###########\n");

	long long i1 = 645248;
	long long i2 = 543124;
	long long i3 = -46872;
	long long i4 = -13246;
	long long i5 = 5724;
	long long i6 = -6578;
	long long i7 = 0;
	unbounded_int u1 = ll2unbounded_int(645248);
	unbounded_int u2 = ll2unbounded_int(543124);
	unbounded_int u3 = ll2unbounded_int(-46872);
	unbounded_int u4 = ll2unbounded_int(-13246);
	unbounded_int u5 = ll2unbounded_int(5724);
	unbounded_int u6 = ll2unbounded_int(-6578);
	unbounded_int u7 = ll2unbounded_int(0);
	unbounded_int res1 = unbounded_int_difference(u3, u1); // -a - (+b)
	unbounded_int res2 = unbounded_int_difference(u1, u4); // +a - (-b)
	unbounded_int res3 = unbounded_int_difference(u7, u6);
	unbounded_int res4 = unbounded_int_difference(u7, u5);
	unbounded_int res5 = unbounded_int_difference(u6, u7);
	unbounded_int res6 = unbounded_int_difference(u1, u2);
	unbounded_int res7 = unbounded_int_difference(u3, u4);
	int b1 = unbounded_int_cmp_ll(res1, i1 - i2);
	int b2 = unbounded_int_cmp_ll(res2, i1 - i7);
	int b3 = unbounded_int_cmp_ll(res3, i3 - i4);
	int b4 = unbounded_int_cmp_ll(res4, i5 - i7);
	int b5 = unbounded_int_cmp_ll(res5, i5 - i1);
	int b6 = unbounded_int_cmp_ll(res1, i1 - i2);
	int b7 = unbounded_int_cmp_ll(res7, i3 - i4);
	unbounded_int_free(u1);
	unbounded_int_free(u2);
	unbounded_int_free(u3);
	unbounded_int_free(u4);
	unbounded_int_free(u5);
	unbounded_int_free(u6);
	unbounded_int_free(u7);
	unbounded_int_free(res1);
	unbounded_int_free(res2);
	unbounded_int_free(res3);
	unbounded_int_free(res4);
	unbounded_int_free(res5);
	unbounded_int_free(res6);
	unbounded_int_free(res7);
	assert(b1);
	assert(b2);
	assert(b3);
	assert(b4);
	assert(b5);
	assert(b6);
	assert(b7);
}

static void test_unbounded_int_produit() {
	printf("########### TEST UNBOUNDED_INT_PRODUIT() ###########\n");
	long long i1 = 645248;
	long long i2 = -13246;
	long long i3 = 5724;
	long long i4 = 0;

	unbounded_int u1 = ll2unbounded_int(645248);
	unbounded_int u2 = ll2unbounded_int(-13246);
	unbounded_int u3 = ll2unbounded_int(5724);
	unbounded_int u4 = ll2unbounded_int(0);

	unbounded_int res1 = unbounded_int_produit(u4, u1);
	unbounded_int res2 = unbounded_int_produit(u1, u4);
	unbounded_int res3 = unbounded_int_produit(u3, u3);
	unbounded_int res4 = unbounded_int_produit(u2, u2);
	unbounded_int res5 = unbounded_int_produit(u3, u2);

	int b1 = unbounded_int_cmp_ll(res1, i4 * i1);
	int b2 = unbounded_int_cmp_ll(res2, i1 * i4);
	int b3 = unbounded_int_cmp_ll(res3, i3 * i3);
	int b4 = unbounded_int_cmp_ll(res4, i2 * i2);
	int b5 = unbounded_int_cmp_ll(res5, i3 + i2);
	unbounded_int_free(u1);
	unbounded_int_free(u2);
	unbounded_int_free(u3);
	unbounded_int_free(u4);
	unbounded_int_free(res1);
	unbounded_int_free(res2);
	unbounded_int_free(res3);
	unbounded_int_free(res4);
	unbounded_int_free(res5);
	assert(b1);
	assert(b2);
	assert(b3);
	assert(b4);
	assert(b5);
}

static void test_unbounded_int_division() {}

static void test_unbounded_int_pow() {}

static void test_unbounded_int_abs() {
	long long i1 = -834873642;
	long long i2 = 834873642;
	long long i3 = -0;
	long long i4 = 0;
	unbounded_int u1 = ll2unbounded_int(i1);
	unbounded_int u2 = ll2unbounded_int(i2);
	unbounded_int u3 = ll2unbounded_int(i3);

	unbounded_int res1 = unbounded_int_abs(u1);
	unbounded_int res2 = unbounded_int_abs(u2);
	unbounded_int res3 = unbounded_int_abs(u3);
	int b1 = unbounded_int_cmp_ll(res1, i2);
	int b2 = unbounded_int_cmp_ll(res2, i2);
	int b3 = unbounded_int_cmp_ll(res3, i4);
	unbounded_int_free(u1);
	unbounded_int_free(u2);
	unbounded_int_free(u3);
	unbounded_int_free(res1);
	unbounded_int_free(res2);
	unbounded_int_free(res3);
	assert(b1);
	assert(b2);
	assert(b3);
}

static long long fact(long long n) {
	if (n <= 0) return 1;
	long long val = 1;
	for (long long i = 1; i < n; i++) val *= i;
	return val;
}

static void test_unbounded_int_fact() {
	long long i1 = fact(5);
	long long i2 = 1;
	long long i3 = fact(20);
	unbounded_int u1 = ll2unbounded_int(5);
	unbounded_int u2 = ll2unbounded_int(0);
	unbounded_int u3 = ll2unbounded_int(20);
	unbounded_int res1 = unbounded_int_fact(u1);
	unbounded_int res2 = unbounded_int_fact(u2);
	unbounded_int res3 = unbounded_int_fact(u3);
	int b1 = unbounded_int_cmp_ll(res1, i1);
	int b2 = unbounded_int_cmp_ll(res2, i2);
	int b3 = unbounded_int_cmp_ll(res3, i3);
	unbounded_int_free(u1);
	unbounded_int_free(u2);
	unbounded_int_free(u3);
	unbounded_int_free(res1);
	unbounded_int_free(res2);
	unbounded_int_free(res3);
	assert(b1);
	assert(b2);
	assert(b3);
}

static void test(void(*f)()) {
	time_t begin = time(NULL);
	f();
	time_t end = time(NULL);
	double delta = difftime(end,begin);
	printf("[PASSED] in %f seconds \n", delta);


}

int main() {
	test(test_ll2unbounded_int);
	test(test_string2unbounded_int);
	test(test_unbounded_int2string);
	test(test_unbounded_int_cmp_ll);
	test(test_unbounded_int_abs);
	test(test_unbounded_int_cmp_unbounded_int);
	test(test_unbounded_int_difference);
	test(test_unbounded_int_division);
	test(test_unbounded_int_fact);
	test(test_unbounded_int_pow);
	test(test_unbounded_int_produit);
	test(test_unbounded_int_somme);
	return 0;
}