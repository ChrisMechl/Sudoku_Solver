#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


#define ROW 9
#define COL 9


int getPuzzle(FILE* fp, int** array);


typedef struct Puzzle{
    int** array;
    int remaining;
}Puzzle;