#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


#define ROW 9
#define COL 9
#define HORZ_BAR "-------------"
#define TOTAL_REMAINING 0;

typedef struct Puzzle{
    int** array;
    int remainingNums[9];
}Puzzle;

void cleanUp(FILE* fp, Puzzle* p);
int getPuzzle(FILE* fp, Puzzle* p);
void printPuzzle(Puzzle* p);
void ppPuzzle(Puzzle* p);



