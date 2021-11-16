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

    /* Array of ints where index 0 represents the total remaining empty 
    spaces in the puzzle and the 1-9 indexes represent the remaining empty
    spaces for that number */
    int remainingNums[10];

    /* 2x2 array of shorts where the 9 least significant bits
    represent the 1-9 possible entries in the sudoku puzzle */
    short** possibilities; 
}Puzzle;

Puzzle* instantiatePuzzle();
void cleanUp(FILE* fp, Puzzle* p);
int getPuzzle(FILE* fp, Puzzle* p);
void solve(Puzzle* p);
short compareRow(Puzzle* p, short possibilities, int row);
short compareCol(Puzzle* p, short possibilities, int col);
void printPuzzle(Puzzle* p);
void ppPuzzle(Puzzle* p);