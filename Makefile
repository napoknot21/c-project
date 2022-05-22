CC=gcc
CFLAGS= 	-Wall -g -pedantic -ansi -std=c11
LDLIBS=		-lm
EXEC= calc

calc: build calc_unbounded_int.o unbounded_int.o
	$(CC) $(CFLAGS) $(LDLIBS) build/calc_unbounded_int.o build/unbounded_int.o -o $(EXEC)

unbounded_int.o: src/unbounded_int.c headers/unbounded_int.h
	$(CC) $(CFLAGS) $(LDLIBS) -c src/unbounded_int.c -o build/$@

calc_unbounded_int.o: src/calc_unbounded_int.c headers/unbounded_int.h
	$(CC) $(CFLAGS) $(LDLIBS) -c src/calc_unbounded_int.c -o build/$@


clean: build
	rm -r build
build:
	mkdir -p build

cleanall:
	rm -r ./build ./calc

