SHELL := /bin/bash
CC=gcc
DEBUGFLAGS=-g
CFLAGS=
INCLUDE=include/puzzle.h

default: solver.c main.c
	$(CC) -o build/solver main.c $(INCLUDE) $(CFLAGS)

debug: solver.c
	$(CC) -o build/solver main.c $(INCLUDE) $(DEBUGFLAGS)

run: default
	./build/solver /home/christian/Documents/C_Projects/Sudoku_Solver/Puzzles/test1.txt

runSteps: default
	./build/solver /home/christian/Documents/C_Projects/Sudoku_Solver/Puzzles/test1.txt -s

runTime: default
	./build/solver /home/christian/Documents/C_Projects/Sudoku_Solver/Puzzles/test1.txt -t

valgrind: default
	valgrind ./build/solver /home/christian/Documents/C_Projects/Sudoku_Solver/Puzzles/test1.txt

testPossibilities: Tests/testPossibilities.c solver.c
	$(CC) -o build/testPossibilities Tests/testPossibilities.c $(INCLUDE); ./build/testPossibilities

clean:
	rm build/*;

ifdef LOOPS
loop: default clean
	for ((i=1; i <= ${LOOPS}; ++i)) do make runTime | grep -oP "\d+\.{1}\d+" >> build/time.txt; done
endif

grepTime: default
	make runTime | grep -oP "\d+\.{1}\d+" >> build/time.txt
