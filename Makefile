SHELL := /bin/bash
CC=gcc
DEBUGFLAGS=-g
CFLAGS=
INCLUDE=include/puzzle.h
PUZZLES=/home/christian/Documents/C_Projects/Sudoku_Solver/Puzzles/
SOLVER=./build/solver

.PHONY: average

default: solver.c main.c
	$(CC) -o build/solver main.c $(INCLUDE) $(CFLAGS)

debug: solver.c
	$(CC) -o build/solver main.c $(INCLUDE) $(DEBUGFLAGS)

run1: default
	$(SOLVER) $(PUZZLES)test1.txt

run1Steps: default
	$(SOLVER) $(PUZZLES)test1.txt -s

run1Time: default
	$(SOLVER) $(PUZZLES)test1.txt -t

run2: default
	$(SOLVER) $(PUZZLES)test2.txt

run2Steps: default
	$(SOLVER) $(PUZZLES)test2.txt -s

run2Time: default
	$(SOLVER) $(PUZZLES)test2.txt -t

valgrind: default
	valgrind $(SOLVER) $(PUZZLES)test1.txt

testPossibilities: Tests/testPossibilities.c solver.c
	$(CC) -o build/testPossibilities Tests/testPossibilities.c $(INCLUDE); ./build/testPossibilities

clean:
	rm -f build/time.txt;

ifdef LOOPS
loop: default clean
	SUM=0; echo Average for ${LOOPS} loops: > build/time.txt;
	for ((i=1; i <= ${LOOPS}; ++i)) do make run1Time | grep -oP "\d+\.{1}\d+" >> build/temp.txt; done; 
	awk '{ sum += $$1; count+=1 } END { result=sum/count; printf "%.08f", result }' build/temp.txt >> build/time.txt;
	rm -f build/temp.txt;
endif

grepTime: default
	make runTime | grep -oP "\d+\.{1}\d+" >> build/time.txt
