#include <stdio.h>
#include <time.h>

#include <test.h>

void test(void (*f)(), char *name) {
	printf("[TEST] %s\n", name);
	const clock_t tic = clock();
	f();
	const time_t toc = clock();
	const double delta = ((double) toc - tic) * 1000.0 / CLOCKS_PER_SEC;
	printf("[PASSED] %s in %f ms \n\n", name, delta);
}
