CC=gcc
DEBUGFLAGS=-g
CFLAGS=
INCLUDE=include/puzzle.h

default: solver.c main.c
	$(CC) -o solver main.c $(INCLUDE) $(CFLAGS)

debug: solver.c
	$(CC) -o solver main.c $(INCLUDE) $(DEBUGFLAGS)

run: default
	./solver /home/christian/Documents/C_Projects/Sudoku_Solver/Puzzles/test1.txt

valgrind: default
	valgrind ./solver /home/christian/Documents/C_Projects/Sudoku_Solver/Puzzles/test1.txt

testPossibilities: Tests/testPossibilities.c solver.c
	$(CC) -o Tests/testPossibilities Tests/testPossibilities.c $(INCLUDE)

clean:
	rm solver
