#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>


#define ROW 9
#define COL 9
#define HORZ_BAR "-------------"
#define TOTAL_REMAINING 0
#define ONE 0x0001
#define TWO 0x0002
#define THREE 0x0004
#define FOUR 0x0008
#define FIVE 0x0010
#define SIX 0x0020
#define SEVEN 0x0040
#define EIGHT 0x0080
#define NINE 0x0100

/* Holds solved array, possibility array, and remaining nums array of the Puzzle
before a guess is made to allow for reversion back to pre-guess state. Also holds
the x,y coords of where the guess took place, and the possibilities at that index
to allow moving onto the next possible number */
typedef struct Guess{
    int** array;

    /* Array of ints where index 0 represents the total remaining empty 
    spaces in the puzzle and the 1-9 indexes represent the remaining empty
    spaces for that number */
    int remainingNums[10];

    /* 2x2 array of shorts where the 9 least significant bits
    represent the 1-9 possible entries in the sudoku puzzle */
    short** possibilities; 

    short x;
    short y;
    short xyPossibilities;
}Guess;

typedef struct Puzzle{
    int** array;

    /* Array of ints where index 0 represents the total remaining empty 
    spaces in the puzzle and the 1-9 indexes represent the remaining empty
    spaces for that number */
    int remainingNums[10];

    /* 2x2 array of shorts where the 9 least significant bits
    represent the 1-9 possible entries in the sudoku puzzle */
    short** possibilities; 

    bool printSteps;
    /* Iterations since last change in puzzle solution array */
    int iterations;
    /* Whether this puzzle is currently based on a guess */
    bool guess;

    Guess* g;
}Puzzle;



Puzzle* instantiatePuzzle(bool steps);
void instantiateGuess(Puzzle* p);
void cleanUp(FILE* fp, Puzzle* p);
int getPuzzle(FILE* fp, Puzzle* p);

void solve(Puzzle* p);

short compareRow(Puzzle* p, short possibilities, int row);
short compareCol(Puzzle* p, short possibilities, int col);
short compare3x3(Puzzle* p, short possibilities, int row, int col);
void checkRow(Puzzle* p, int row);
void checkCol(Puzzle* p, int col);
void check3x3(Puzzle* p, int row, int col);

void clearRowPossibility(Puzzle* p, int row, int i);
void clearColPossibility(Puzzle* p, int col, int i);
void clear3x3Possibility(Puzzle* p, int row, int col, int i);

void findGuess(Puzzle* p);
void makeGuess(Puzzle* p);
int getNumberOfPossibilities(short possibility);
int getNextPossibility(short possibility);
void copyPuzzleToGuess(Puzzle* p);
void copyGuessToPuzzle(Puzzle* p);

void checkPossibilities(Puzzle* p);

void printPuzzle(Puzzle* p);
void ppPuzzle(Puzzle* p);
void printPossibilities(Puzzle* p);