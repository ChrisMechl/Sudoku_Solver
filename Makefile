SHELL := /bin/bash
CC=gcc
DEBUGFLAGS=-g
CFLAGS=
INCLUDE=include/puzzle.h
PUZZLES=/home/christian/Documents/C_Projects/Sudoku_Solver/Puzzles/
EASY=easy/
MED=medium/
HARD=hard/
SOLVER=./build/solver

.PHONY: average

default: solver.c main.c
	$(CC) -o build/solver main.c $(INCLUDE) $(CFLAGS)

debug: solver.c
	$(CC) -o build/solver main.c $(INCLUDE) $(DEBUGFLAGS)

PUZ?=1
runEasy: default
	$(SOLVER) $(PUZZLES)$(EASY)test$(PUZ).txt

runEasySteps: default
	$(SOLVER) $(PUZZLES)$(EASY)test$(PUZ).txt -s

runEasyTime: default
	$(SOLVER) $(PUZZLES)$(EASY)test$(PUZ).txt -t

runMedium: default
	$(SOLVER) $(PUZZLES)$(MED)test$(PUZ).txt

runMediumSteps: default
	$(SOLVER) $(PUZZLES)$(MED)test$(PUZ).txt -s

runMediumTime: default
	$(SOLVER) $(PUZZLES)$(HARD)test$(PUZ).txt -t

runHard: default
	$(SOLVER) $(PUZZLES)$(HARD)test$(PUZ).txt

runHardSteps: default
	$(SOLVER) $(PUZZLES)$(HARD)test$(PUZ).txt -s

runHardTime: default
	$(SOLVER) $(PUZZLES)$(HARD)test$(PUZ).txt -t


valgrind: default
	valgrind $(SOLVER) $(PUZZLES)$(HARD)test1.txt

testPossibilities: Tests/testPossibilities.c solver.c
	$(CC) -o build/testPossibilities Tests/testPossibilities.c $(INCLUDE); ./build/testPossibilities

clean:
	rm -f build/time.txt;

ifdef LOOPS
loop: default clean
	SUM=0; echo Average for ${LOOPS} loops: > build/time.txt;
	for ((i=1; i <= ${LOOPS}; ++i)) do make runEasyTime | grep -oP "\d+\.{1}\d+" >> build/temp.txt; done; 
	awk '{ sum += $$1; count+=1 } END { result=sum/count; printf "%.08f", result }' build/temp.txt >> build/time.txt;
	rm -f build/temp.txt;
endif

grepTime: default
	make runTime | grep -oP "\d+\.{1}\d+" >> build/time.txt
