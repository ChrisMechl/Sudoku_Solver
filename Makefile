CC=gcc
DEBUGFLAGS=-g
CFLAGS=

default: solver.c
	$(CC) -o solver solver.c $(CFLAGS)

debug: solver.c
	$(CC) -o solver solver.c $(DEBUGFLAGS)

run: default
	./solver /home/christian/Documents/C_Projects/Sudoku_Solver/Tests/test1.txt

clean:
	rm solver
