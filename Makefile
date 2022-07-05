CC=gcc
CFLAGS= 	-Wall -g -pedantic -std=c11
LDLIBS=		-lm
EXEC= calc


calc: build build/calc_unbounded_int.o build/unbounded_int.o
	$(CC) $(CFLAGS) $(LDLIBS) build/calc_unbounded_int.o build/unbounded_int.o -o $(EXEC)

test: build build/test_unbounded.o build/unbounded_int.o
	$(CC) $(CFLAGS) $(LDLIBS)  build/unbounded_int.o build/test_unbounded.o -o $@

build/test_unbounded.o: build/unbounded_int.o src/unbounded_int.h
	$(CC) $(CFLAGS) $(LDLIBS)  -c test_unbounded.c -o $@

build/unbounded_int.o: src/unbounded_int.c src/unbounded_int.h
	$(CC) $(CFLAGS) $(LDLIBS) -c unbounded_int.c -o $@

build/calc_unbounded_int.o: src/calc_unbounded_int.c src/unbounded_int.h
	$(CC) $(CFLAGS) $(LDLIBS) -c calc_unbounded_int.c -o $@



clean: build
	rm -rf build
build:
	mkdir -p build

cleanall:
	rm -rf ./build ./calc

