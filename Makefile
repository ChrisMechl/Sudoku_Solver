CC=gcc
DEBUGFLAGS=-g
CFLAGS=
INCLUDE=include/puzzle.h

default: solver.c 
	$(CC) -o solver solver.c $(INCLUDE) $(CFLAGS)

debug: solver.c
	$(CC) -o solver solver.c $(INCLUDE) $(DEBUGFLAGS)

run: default
	./solver /home/christian/Documents/C_Projects/Sudoku_Solver/Tests/test1.txt

clean:
	rm solver
