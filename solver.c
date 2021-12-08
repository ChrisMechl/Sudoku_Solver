#include "include/puzzle.h"
//TODO hard getting wrong answers
short ands[9] = {ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE};

Puzzle* instantiatePuzzle(bool steps){
    /* Create Puzzle */
    Puzzle* p = malloc(sizeof(Puzzle));

    /* Create puzzle array */
    int** array = malloc(sizeof(int*) * COL);
    /* Each column gets ROW number of ints */
    for(int i = 0; i < ROW; i++){
        array[i] = malloc(sizeof(int) * ROW);
        memset(array[i], 0, (ROW * sizeof(int)));
    }

    /* Create possibilities array */
    short** possibilities = malloc(sizeof(short* ) * ROW);
    /* Each column gets ROW number of shorts */
    for(int i = 0; i < ROW; i++){
        possibilities[i] = malloc(sizeof(short) * ROW);
        memset(possibilities[i], 0xff, (ROW * sizeof(short)));
    }

    /* Attach to Puzzle struct */
    p->array = array;
    p->possibilities = possibilities;

    /* Assign remaining nums to default values */
    p->remainingNums[0] = 0;
    for(int i = 1; i < 10; i++){
        p->remainingNums[i] = 9;
    }

    p->printSteps = steps;
    p->iterations = 0;
    p->guess = false;
    p->g = 0;

    return p;
}

void instantiateGuess(Puzzle* p){
    /* Create guess puzzle */
    Guess* g = malloc(sizeof(Guess));
    /* Create puzzle array */
    int** array = malloc(sizeof(int*) * COL);
    /* Each column gets ROW number of ints */
    for(int i = 0; i < ROW; i++){
        array[i] = malloc(sizeof(int) * ROW);
        memset(array[i], 0, (ROW * sizeof(int)));
    }
    /* Create possibilities array */
    short** possibilities = malloc(sizeof(short* ) * ROW);
    /* Each column gets ROW number of shorts */
    for(int i = 0; i < ROW; i++){
        possibilities[i] = malloc(sizeof(short) * ROW);
        memset(possibilities[i], 0xff, (ROW * sizeof(short)));
    }

    /* Attach to Puzzle struct */
    g->array = array;
    g->possibilities = possibilities;

    /* Assign remaining nums to default values */
    g->remainingNums[0] = 0;
    for(int i = 1; i < 10; i++){
        g->remainingNums[i] = 9;
    }

    /* Attach to end of list */
    if(p->last != 0){
        p->last->next = g;
        g->prev = p->last;
        g->next = 0;
        p->last = g;
    }
    else{
        p->first = g;
        p->last = g;
        g->prev = 0;
        g->next = 0;
    }
}

void removeLastGuess(Puzzle* p){
    Guess* cur;
    if(p->last != 0){
        cur = p->last;
        p->last = p->last->prev;
        p->last->next = 0;
        for(int i = 0; i < ROW; i++){
            free(cur->array[i]);
            free(cur->possibilities[i]);
        }
        free(cur->array);
        free(cur->possibilities);
        free(cur);
    }
}

void cleanUp(FILE* fp, Puzzle* p){
    if(p == 0){
        fclose(fp);
        return;
    }
    for(int i = 0; i < ROW; i++){
        free(p->array[i]);
        free(p->possibilities[i]);
    }

    if(p->g != 0){
        for(int i = 0; i < ROW; i++){
            free(p->g->array[i]);
            free(p->g->possibilities[i]);
        }
        free(p->g->array);
        free(p->g->possibilities);
        free(p->g);
    }

    free(p->array);
    free(p->possibilities);
    free(p);
    fclose(fp);
}

int getPuzzle(FILE* fp, Puzzle* p){
    char cur;
    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            cur = fgetc(fp);
            if(cur == EOF){
                return -1;
            }
            /* If char is newline, get next value */
            else if(cur == '\n'){
                j--;
                continue;
            }
            /* If char is not between 1-9 or newline, set it to zero */
            else if(cur < 49 || cur > 57){
                p->array[i][j] = 0;
                p->remainingNums[0]++;
            }
            /* If char is 1-9, set the array position to it */
            else{
                int num = cur - 48;
                p->array[i][j] = num;
                p->remainingNums[num]--;
                p->possibilities[i][j] = 0;
                /* Remove possibilities for num - 1 because the functions were 
                designed to take index values for the ands array which starts at
                zero */
                clearRowPossibility(p, i, num - 1);
                clearColPossibility(p, j, num - 1);
                clear3x3Possibility(p, i, j, num - 1);
            }
        }
    }
}

void solve(Puzzle* p){
    checkPossibilities(p);
    while(p->remainingNums[0] > 0){
        /* If we have gone through the puzzle and made no changes for an
        entire iteration, time to make a guess */
        if(p->iterations > 0 && p->g == 0){
            instantiateGuess(p);
            findGuess(p);
            copyPuzzleToGuess(p);
            makeGuess(p);
        }
        /* If we have gone through the puzzle and made no changes for an
        entire iteration AFTER already making a guess, revert to old puzzle
        pre-guess and update possibilities because the guessed value is no
        longer considered legal. We proved it can't exist in the place we
        guessed it to be */
        else if(p->iterations > 0 && p->g != 0){
            /* Revert to pre-guess puzzle */
            copyGuessToPuzzle(p);
            short guess = getNextPossibility(p->g->xyPossibilities);
            /* Remove possibility of guess from guessed spot */
            p->possibilities[p->g->x][p->g->y] &= ~(ands[guess - 1]);
            p->g->xyPossibilities &= ~(ands[guess - 1]);

            /* If there's only one possibility left, fill it in and remove guess as 
            it's no longer accurate */
            if(getNumberOfPossibilities(p->g->xyPossibilities) == 1){
                guess = getNextPossibility(p->g->xyPossibilities);
                p->array[p->g->x][p->g->y] = guess;
                /* Set possibilities at newly placed number to zero */
                p->possibilities[p->g->x][p->g->y] = 0;
                /* Clear possibilities of row, col, 3x3 for the newly placed number.
                Must use guess - 1 to use correct index in ands array */
                clearRowPossibility(p, p->g->x, guess - 1);
                clearColPossibility(p, p->g->y, guess - 1);
                clear3x3Possibility(p, p->g->x, p->g->y, guess -1);
                /* Update remainingNums */
                p->remainingNums[0]--;
                p->remainingNums[guess]--;

                p->iterations = 0;
                

                if(p->printSteps){
                    printf("Previous guess was wrong, but only one possible number left of %d at i = %d, j = %d\n", guess, p->g->x, p->g->y);
                    ppPuzzle(p);
                }
                removeLastGuess(p);
                p->g = 0;
            }
            else{
                /* Create new guess */
                makeGuess(p);
            }
            


        }
        p->iterations++;
        int col = -1;
        for(int i = 0; i < ROW; i++){
            for(int j = 0; j < COL; j++){
                /* if a number is already there, possibilities = 0 */
                if(p->array[i][j] != 0){
                    p->possibilities[i][j] = 0;
                    continue;
                }
                p->possibilities[i][j] = compareRow(p, p->possibilities[i][j], i);
                p->possibilities[i][j] = compareCol(p, p->possibilities[i][j], j);
                p->possibilities[i][j] = compare3x3(p, p->possibilities[i][j], i, j);
                
            }
            col++;
            checkRow(p, i);
            checkCol(p, col);
            if(i == 2 || i == 6 || i == 8){
                check3x3(p, i, col);
            }
        }
        checkPossibilities(p);
        // printPossibilities(p);
    }

    printf("\nPuzzle Solution\n");
    ppPuzzle(p);
}

/* Accepts the puzzle, a single short representing to the 
short in the current position of the possibilities array, and the row
of the puzzle to compare against. Returns a possibility short with
the bits zeroed out at the indexes in which there are the corresponding
number of that index in the row */
short compareRow(Puzzle* p, short possibilities, int row){
    for(int j = 0; j < COL; j++){
        if(p->array[row][j] == 0){
            continue;
        }
        possibilities &= ~(1 << (p->array[row][j] - 1));
    }
    return possibilities;
}

/* Accepts the puzzle, a single short representing to the 
short in the current position of the possibilities array, and the col
of the puzzle to compare against. Returns a possibility short with
the bits zeroed out at the indexes in which there are the corresponding
number of that index in the col */
short compareCol(Puzzle* p, short possibilities, int col){
    for(int i = 0; i < ROW; i++){
        if(p->array[i][col] == 0){
            continue;
        }
        possibilities &= ~(1 << (p->array[i][col] - 1));
    }
    return possibilities;
}

short compare3x3(Puzzle* p, short possibilities, int row, int col){
    for(int i = (row / 3) * 3; i < ((row / 3) * 3) + 3; i++){
        for(int j = (col / 3) * 3; j < ((col / 3) * 3) + 3; j++){
            if(p->array[i][j] == 0){
                continue;
            }
            possibilities &= ~(1 << (p->array[i][j] - 1));
        }
    }
    return possibilities;
}

/*Checks all numbers' (1-9) availability in the current row to see
if there's a single location that any number can legally reside in */
void checkRow(Puzzle* p, int row){
    int possibleIdx = -1;
    /* Go through 1-9 */
    for(int i = 0; i < 9; i++){
        /* Go through each col in the row */
        for(int j = 0; j < COL; j++){
            if(p->possibilities[row][j] == 0){
                continue;
            }
            else if((p->possibilities[row][j] & ands[i]) != 0){
                /* If multiple indexes can contain the current checked value,
                we won't be able to find a single spot for that number in
                this row. Reset it and go to the next number */
                if(possibleIdx > -1){
                    possibleIdx = -1;
                    break;
                }
                else{
                    possibleIdx = j;
                }
            }
        }
        /* If we make it here, only one index can hold the current i value
        so we set it in array, clear the possibilities for that index,
        and clear that bit from all other possibilities in the row, col,
        and 3x3 */
        if(possibleIdx > -1){
            p->array[row][possibleIdx] = i + 1;
            p->possibilities[row][possibleIdx] = 0;
            p->remainingNums[0]--;
            p->remainingNums[i + 1]--;
            p->iterations = 0;
            if(p->printSteps){
                ppPuzzle(p);
            }
            clearColPossibility(p, possibleIdx, i);
            clear3x3Possibility(p, row, possibleIdx, i);
            possibleIdx = -1;
        }
    }
}

/*Checks all numbers' (1-9) availability in the current col to see
if there's a single location that any number can legally reside in */
void checkCol(Puzzle* p, int col){
    int possibleIdx = -1;
    /* Go through 1-9 */
    for(int i = 0; i < 9; i++){
        /* Go through each row in the col */
        for(int j = 0; j < ROW; j++){
            if(p->possibilities[j][col] == 0){
                continue;
            }
            else if((p->possibilities[j][col] & ands[i]) != 0){
                /* If multiple indexes can contain the current checked value,
                we won't be able to find a single spot for that number in
                this col. Reset it and go to the next number */
                if(possibleIdx > -1){
                    possibleIdx = -1;
                    break;
                }
                else{
                    possibleIdx = j;
                }
            }
        }
        /* If we make it here, only one index can hold the current i value
        so we set it in array, clear the possibilities for that index,
        and clear that bit from all other possibilities in the row, col,
        and 3x3 */
        if(possibleIdx > -1){
            p->array[possibleIdx][col] = i + 1;
            p->possibilities[possibleIdx][col] = 0;
            p->remainingNums[0]--;
            p->remainingNums[i + 1]--;
            p->iterations = 0;
            if(p->printSteps){
                ppPuzzle(p);
            }
            clearRowPossibility(p, possibleIdx, i);
            clear3x3Possibility(p, possibleIdx, col, i);
            possibleIdx = -1;
        }
    }
}

/*Checks all numbers' (1-9) availability in the current 3x3 to see
if there's a single location that any number can legally reside in */
void check3x3(Puzzle* p, int row, int col){
    int possibleXIdx = -1;
    int possibleYIdx = -1;
    for(int k = 0; k < 9; k++){
        for(int i = (row / 3) * 3; i < ((row / 3) * 3) + 3; i++){
            for(int j = (col / 3) * 3; j < ((col / 3) * 3) + 3; j++){
                if(p->possibilities[i][j] == 0){
                    continue;
                }
                else if((p->possibilities[i][j] & ands[k]) != 0){
                    /* If multiple indexes can contain the current checked value,
                    we won't be able to find a single spot for that number in
                    this col. Reset it and go to the next number */
                    if(possibleXIdx > -1){
                        possibleXIdx = -1;
                        possibleYIdx = -1;
                        i = 99;
                        j = 99;
                        break;
                    }
                    else{
                        possibleXIdx = i;
                        possibleYIdx = j;
                    }
                }
            }
        }
        /* If we make it here, only one index can hold the current i value
        so we set it in array, clear the possibilities for that index,
        and clear that bit from all other possibilities in the row, col,
        and 3x3 */
        if(possibleXIdx > -1){
            p->array[possibleXIdx][possibleYIdx] = k + 1;
            p->possibilities[possibleXIdx][possibleYIdx] = 0;
            p->remainingNums[0]--;
            p->remainingNums[k + 1]--;
            p->iterations = 0;
            if(p->printSteps){
                ppPuzzle(p);
            }
            clearRowPossibility(p, possibleXIdx, k);
            clearColPossibility(p, possibleYIdx, k);
            possibleXIdx = -1;
            possibleYIdx = -1;
        }
    }
}

void findGuess(Puzzle* p){
    /* Starts looking for possibilities with only 2 possibilities. 
    If it can't find a possibility with only 2 possibilities, increase 
    the number of possibilities to look for until we reach 9 */
    for(int poss = 2; poss < 10; poss++){
        for(int i = 0; i < ROW; i++){
            for(int j = 0; j < COL; j++){
                if(p->possibilities[i][j] == 0){
                    continue;
                }
                if(getNumberOfPossibilities(p->possibilities[i][j]) == poss){
                    p->g->x = i;
                    p->g->y = j;
                    p->g->xyPossibilities = p->possibilities[i][j];
                    poss = 99;
                    i = 99;
                    j = 99;
                    break;
                }
            }
        }
    }
}



void makeGuess(Puzzle* p){
    int guess = -1;
    /* Set number in array to guess */
    guess = getNextPossibility(p->g->xyPossibilities);
    p->array[p->g->x][p->g->y] = guess;
    /* Set possibilities at newly placed number to zero */
    p->possibilities[p->g->x][p->g->y] = 0;
    /* Clear possibilities of row, col, 3x3 for the newly placed number.
    Must use guess - 1 to use correct index in ands array */
    clearRowPossibility(p, p->g->x, guess - 1);
    clearColPossibility(p, p->g->y, guess - 1);
    clear3x3Possibility(p, p->g->x, p->g->y, guess -1);
    /* Update remainingNums */
    p->remainingNums[0]--;
    p->remainingNums[guess]--;

    p->iterations = 0;

    if(p->printSteps){
        printf("Making guess of %d at i = %d, j = %d\n", guess, p->g->x, p->g->y);
        ppPuzzle(p);
    }
}

/* For a given possibility, return the number of possibilities
by bit shifting 9 times and counting the 1's in the lsb */
int getNumberOfPossibilities(short possibility){
    int count = 0;
    for(int k = 0; k < 9; k++){
        if((possibility >> k) & 1 == 1){
            count++;
        }
    }
    return count;
}

/* Gets the next possible number from a possibility by bit
shifting, then ANDing with 1 */
int getNextPossibility(short possibility){
    for(int i = 0; i < 9; i++){
        if((possibility >> i) & 1 == 1){
            return i + 1;
        }
    }
}

/* Copies data from Puzzle to Guess prior to making the guess
in order to be able to revert back to the pre-guess state */
void copyPuzzleToGuess(Puzzle* p){
    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            p->g->possibilities[i][j] = p->possibilities[i][j];
            p->g->array[i][j] = p->array[i][j];
        }
    }

    for(int i = 0; i < 10; i++){
        p->g->remainingNums[i] = p->remainingNums[i];
    }
}

/* Copies data from Guess to Puzzle after the guess has been made
and found to be wrong. Reverts back to pre-guess state */
void copyGuessToPuzzle(Puzzle* p){
    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            p->possibilities[i][j] = p->g->possibilities[i][j];
            p->array[i][j] = p->g->array[i][j];
        }
    }

    for(int i = 0; i < 10; i++){
        p->remainingNums[i] = p->g->remainingNums[i];
    }
}

/* Goes through possibilities array and fills indexes that only have 
one possibility in the puzzle array */
void checkPossibilities(Puzzle* p){
    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            /* If the possibilities only contains one number,
            set the array at the same index to the number and 
            set the possibilities of that index to zero */
            switch(p->possibilities[i][j]){
                case (short)0xFE01:
                    p->array[i][j] = 1;
                    p->possibilities[i][j] = 0;
                    p->remainingNums[0]--;
                    p->remainingNums[1]--;
                    p->iterations = 0;
                    clearRowPossibility(p, i, 0);
                    clearColPossibility(p, j, 0);
                    clear3x3Possibility(p, i, j, 0);
                    if(p->printSteps){
                        ppPuzzle(p);
                    }
                    break;
                case (short)0xFE02:
                    p->array[i][j] = 2;
                    p->possibilities[i][j] = 0;
                    p->remainingNums[0]--;
                    p->remainingNums[2]--;
                    p->iterations = 0;
                    clearRowPossibility(p, i, 1);
                    clearColPossibility(p, j, 1);
                    clear3x3Possibility(p, i, j, 1);
                    if(p->printSteps){
                        ppPuzzle(p);
                    }
                    break;
                case (short)0xFE04:
                    p->array[i][j] = 3;
                    p->possibilities[i][j] = 0;
                    p->remainingNums[0]--;
                    p->remainingNums[3]--;
                    p->iterations = 0;
                    clearRowPossibility(p, i, 2);
                    clearColPossibility(p, j, 2);
                    clear3x3Possibility(p, i, j, 2);
                    if(p->printSteps){
                        ppPuzzle(p);
                    }
                    break;
                case (short)0xFE08:
                    p->array[i][j] = 4;
                    p->possibilities[i][j] = 0;
                    p->remainingNums[0]--;
                    p->remainingNums[4]--;
                    p->iterations = 0;
                    clearRowPossibility(p, i, 3);
                    clearColPossibility(p, j, 3);
                    clear3x3Possibility(p, i, j, 3);
                    if(p->printSteps){
                        ppPuzzle(p);
                    }
                    break;
                case (short)0xFE10:
                    p->array[i][j] = 5;
                    p->possibilities[i][j] = 0;
                    p->remainingNums[0]--;
                    p->remainingNums[5]--;
                    p->iterations = 0;
                    clearRowPossibility(p, i, 4);
                    clearColPossibility(p, j, 4);
                    clear3x3Possibility(p, i, j, 4);
                    if(p->printSteps){
                        ppPuzzle(p);
                    }
                    break;
                case (short)0xFE20:
                    p->array[i][j] = 6;
                    p->possibilities[i][j] = 0;
                    p->remainingNums[0]--;
                    p->remainingNums[6]--;
                    p->iterations = 0;
                    clearRowPossibility(p, i, 5);
                    clearColPossibility(p, j, 5);
                    clear3x3Possibility(p, i, j, 5);
                    if(p->printSteps){
                        ppPuzzle(p);
                    }
                    break;
                case (short)0xFE40:
                    p->array[i][j] = 7;
                    p->possibilities[i][j] = 0;
                    p->remainingNums[0]--;
                    p->remainingNums[7]--;
                    p->iterations = 0;
                    clearRowPossibility(p, i, 6);
                    clearColPossibility(p, j, 6);
                    clear3x3Possibility(p, i, j, 6);
                    if(p->printSteps){
                        ppPuzzle(p);
                    }
                    break;
                case (short)0xFE80:
                    p->array[i][j] = 8;
                    p->possibilities[i][j] = 0;
                    p->remainingNums[0]--;
                    p->remainingNums[8]--;
                    p->iterations = 0;
                    clearRowPossibility(p, i, 7);
                    clearColPossibility(p, j, 7);
                    clear3x3Possibility(p, i, j, 7);
                    if(p->printSteps){
                        ppPuzzle(p);
                    }
                    break;
                case (short)0xFF00:
                    p->array[i][j] = 9;
                    p->possibilities[i][j] = 0;
                    p->remainingNums[0]--;
                    p->remainingNums[9]--;
                    p->iterations = 0;
                    clearRowPossibility(p, i, 8);
                    clearColPossibility(p, j, 8);
                    clear3x3Possibility(p, i, j, 8);
                    if(p->printSteps){
                        ppPuzzle(p);
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

/* Clears the row defined in row of the possibility referenced in the ands
array by index i */
void clearRowPossibility(Puzzle* p, int row, int i){
    for(int j = 0; j < COL; j++){
        p->possibilities[row][j] = p->possibilities[row][j] & ~(ands[i]);
    }
}

/* Clears the row defined in row of the possibility referenced in the ands
array by index i */
void clearColPossibility(Puzzle* p, int col, int i){
    for(int j = 0; j < ROW; j++){
        p->possibilities[j][col] = p->possibilities[j][col] & ~(ands[i]);
    }
}

/* Clears the 3x3 defined by row and col of the possibility referenced in the ands
array by index i */
void clear3x3Possibility(Puzzle* p, int row, int col, int i){
    for(int k = (row / 3) * 3; k < ((row / 3) * 3) + 3; k++){
        for(int j = (col / 3) * 3; j < ((col / 3) * 3) + 3; j++){
            p->possibilities[k][j] = p->possibilities[k][j] & ~(ands[i]);
        }
    }
}

/* Prints puzzle */
void printPuzzle(Puzzle* p){
    for(int i = 0; i < ROW; i++){
        printf("\n");
        for(int j = 0; j < COL; j++){
            if(p->array[i][j] == 0){
                printf("_");
            }
            else{
                printf("%d", p->array[i][j]);
            }
        }
    }
    printf("\n");
}

/* Prints puzzle with bars to format it like a written puzzle */
void ppPuzzle(Puzzle* p){
    printf("\n%d", p->remainingNums[0]);
    for(int i = 0; i < ROW; i++){
        printf("\n");
        if(i % 3 == 0){
            printf("%s\n", HORZ_BAR);
        }
        for(int j = 0; j < COL; j++){
            if(j % 3 == 0){
                printf("|");
            }

            if(p->array[i][j] == 0){
                printf("_");
            }
            else{
                printf("%d", p->array[i][j]);
            }

            if(j == COL - 1){
                printf("|");
            }
        }
    }
    printf("\n%s\n", HORZ_BAR);
}


void printPossibilities(Puzzle* p){
    for(int i = 0; i < ROW; i++){
        printf("\n");
        if(i % 3 == 0){
            printf("%s\n", HORZ_BAR);
        }
        for(int j = 0; j < COL; j++){
            if(j % 3 == 0){
                printf("|");
            }

            printf("%d", p->possibilities[i][j]);

            if(j == COL -1){
                printf("|");
            }
        }
    }
}