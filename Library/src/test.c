#include <stdio.h>
#include <time.h>

#include <test.h>

void test(void(*f)(), char *name) {
    printf("[TEST] %s\n",name);
    clock_t const tic = clock();
    f();
    time_t const toc = clock();
    double const delta = ((double) toc - tic) * 1000.0 / CLOCKS_PER_SEC;
    printf("[PASSED] %s in %f ms \n\n", name, delta);
}
