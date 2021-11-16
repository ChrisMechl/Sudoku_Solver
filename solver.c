#include "include/puzzle.h"

Puzzle* instantiatePuzzle(){
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

    return p;
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
            }
        }
    }
}

void solve(Puzzle* p){
    while(p->remainingNums[0] > 0){
        for(int i = 0; i < ROW; i++){
            for(int j = 0; j < COL; j++){
                /* if a number is already there, possibilities = 0 */
                if(p->array[i][j] != 0){
                    p->possibilities[i][j] = 0;
                    continue;
                }
                p->possibilities[i][j] = compareRow(p, p->possibilities[i][j], i);
                p->possibilities[i][j] = compareCol(p, p->possibilities[i][j], j);
                checkPossibilities(p);
            }
        }
    }
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
                    ppPuzzle(p);
                    break;
                case (short)0xFE02:
                    p->array[i][j] = 2;
                    p->possibilities[i][j] = 0;
                    p->remainingNums[0]--;
                    p->remainingNums[2]--;
                    ppPuzzle(p);
                    break;
                case (short)0xFE04:
                    p->array[i][j] = 3;
                    p->possibilities[i][j] = 0;
                    p->remainingNums[0]--;
                    p->remainingNums[3]--;
                    ppPuzzle(p);
                    break;
                case (short)0xFE08:
                    p->array[i][j] = 4;
                    p->possibilities[i][j] = 0;
                    p->remainingNums[0]--;
                    p->remainingNums[4]--;
                    ppPuzzle(p);
                    break;
                case (short)0xFE10:
                    p->array[i][j] = 5;
                    p->possibilities[i][j] = 0;
                    p->remainingNums[0]--;
                    p->remainingNums[5]--;
                    ppPuzzle(p);
                    break;
                case (short)0xFE20:
                    p->array[i][j] = 6;
                    p->possibilities[i][j] = 0;
                    p->remainingNums[0]--;
                    p->remainingNums[6]--;
                    ppPuzzle(p);
                    break;
                case (short)0xFE40:
                    p->array[i][j] = 7;
                    p->possibilities[i][j] = 0;
                    p->remainingNums[0]--;
                    p->remainingNums[7]--;
                    ppPuzzle(p);
                    break;
                case (short)0xFE80:
                    p->array[i][j] = 8;
                    p->possibilities[i][j] = 0;
                    p->remainingNums[0]--;
                    p->remainingNums[8]--;
                    ppPuzzle(p);
                    break;
                case (short)0xFF00:
                    p->array[i][j] = 9;
                    p->possibilities[i][j] = 0;
                    p->remainingNums[0]--;
                    p->remainingNums[9]--;
                    ppPuzzle(p);
                    break;
                default:
                    break;
            }
        }
    }
}

/* Prints puzzle */
void printPuzzle(Puzzle* p){
    for(int i = 0; i < ROW; i++){
        printf("\n");
        for(int j = 0; j < COL; j++){
            printf("%d", p->array[i][j]);
        }
    }
    printf("\n");
}

/* Prints puzzle with bars to format it like a written puzzle */
void ppPuzzle(Puzzle* p){
    for(int i = 0; i < ROW; i++){
        printf("\n");
        if(i % 3 == 0){
            printf("%s\n", HORZ_BAR);
        }
        for(int j = 0; j < COL; j++){
            if(j % 3 == 0){
                printf("|");
            }
            printf("%d", p->array[i][j]);
            if(j == COL - 1){
                printf("|");
            }
        }
    }
    printf("\n%s\n", HORZ_BAR);
}
