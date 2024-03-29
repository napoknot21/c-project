CC=gcc
CFLAGS= 	-Wall -g -pedantic -ansi -std=c11
LDLIBS=		-lm
EXEC= calc


calc: build calc_unbounded_int.o unbounded_int.o
	$(CC) $(CFLAGS) $(LDLIBS) build/calc_unbounded_int.o build/unbounded_int.o -o $(EXEC)

test: build test_unbounded.o unbounded_int.h
	$(CC) $(CFLAGS) $(LDLIBS)  build/unbounded_int.o build/test_unbounded.o -o $@

test_unbounded.o: unbounded_int.o unbounded_int.h
	$(CC) $(CFLAGS) $(LDLIBS)  -c test_unbounded.c -o build/$@

unbounded_int.o: unbounded_int.c unbounded_int.h
	$(CC) $(CFLAGS) $(LDLIBS) -c unbounded_int.c -o build/$@

calc_unbounded_int.o: calc_unbounded_int.c unbounded_int.h
	$(CC) $(CFLAGS) $(LDLIBS) -c calc_unbounded_int.c -o build/$@



clean: build
	rm -rf build
build:
	mkdir -p build

cleanall:
	rm -rf ./build ./calc

