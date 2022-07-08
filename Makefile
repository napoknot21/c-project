CC=gcc
CFLAGS= 	-Wall -g -pedantic -std=c11
LDLIBS=		-lm
EXEC= calc


calc: build build/calc_UnboundedInt.o build/UnboundedInt.o
	$(CC) $(CFLAGS) $(LDLIBS) build/calc_UnboundedInt.o build/UnboundedInt.o -o $(EXEC)

test: build build/test_unbounded.o build/UnboundedInt.o
	$(CC) $(CFLAGS) $(LDLIBS)  build/UnboundedInt.o build/test_unbounded.o -o $@

build/test_unbounded.o: build/UnboundedInt.o library
	$(CC) $(CFLAGS) $(LDLIBS)  -c test_unbounded.c -o $@

build/UnboundedInt.o: library library
	$(CC) $(CFLAGS) $(LDLIBS) -c UnboundedInt.c -o $@

build/calc_UnboundedInt.o: src/calc_UnboundedInt.c library
	$(CC) $(CFLAGS) $(LDLIBS) -c calc_UnboundedInt.c -o $@



clean: build
	rm -rf build
build:
	mkdir -p build

cleanall:
	rm -rf ./build ./calc

