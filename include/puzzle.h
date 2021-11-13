#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


#define ROW 9
#define COL 9

typedef struct Puzzle{
    int** array;
    int remaining;
}Puzzle;

int getPuzzle(FILE* fp, Puzzle* p);
void cleanUp(FILE* fp, Puzzle* p);


