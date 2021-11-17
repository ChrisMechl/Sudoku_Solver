SHELL := /bin/bash
CC=gcc
DEBUGFLAGS=-g
CFLAGS=
INCLUDE=include/puzzle.h
TESTS=Number of tests: 

.PHONY: average

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
	rm -f build/time.txt;

ifdef LOOPS
loop: default clean
	SUM=0; echo Average for ${LOOPS} loops: > build/time.txt;
	for ((i=1; i <= ${LOOPS}; ++i)) do make runTime | grep -oP "\d+\.{1}\d+" >> build/temp.txt; done; 
	awk '{ sum += $$1; count+=1 } END { result=sum/count; print result }' build/temp.txt >> build/time.txt;
	rm -f build/temp.txt;
endif

test:
	awk '{ sum += $$1 } END { print sum }' /home/christian/Documents/C_Projects/Sudoku_Solver/build/time.txt

grepTime: default
	make runTime | grep -oP "\d+\.{1}\d+" >> build/time.txt
