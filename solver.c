#include "include/puzzle.h"

int main(int argc, char** argv){
    int pid = getpid();
    // printf("%d\n", pid);

    if(argc != 2){
        fprintf(stderr, "Invalid arguments: Please provide soduko file \n");
        return EXIT_FAILURE;
    }

    FILE* fp = fopen(argv[1], "r");
    if(!fp){
        fprintf(stderr, "Invalid file location\n");
        return EXIT_FAILURE;
    }

    /* Create Puzzle */
    Puzzle* p = malloc(sizeof(Puzzle));
    int** array = malloc(sizeof(int*) * ROW);
    p->array = array;
    p->remainingNums[0] = 0;
    for(int i = 1; i < 10; i++){
        p->remainingNums[i] = 9;
    }
    /* Create rows for array */
    for(int i = 0; i < ROW; i++){
        array[i] = malloc(sizeof(int) * COL);
        memset(array[0], 0, (ROW * sizeof(int)));
    }
    for(int i = 0; i < ROW; i++){
        array[i][0] = i+1;
    }
    
    /* Fill matrix */
    if(getPuzzle(fp, p) == -1){
        fprintf(stderr, "Invalid sudoku file format. Not enough rows or columns\n");
        fclose(fp);
        free(array);
        return EXIT_FAILURE;
    }
    
    ppPuzzle(p);

    cleanUp(fp, p);
    return EXIT_SUCCESS;

}

void cleanUp(FILE* fp, Puzzle* p){
    for(int i = 0; i < ROW; i++){
        free(p->array[i]);
    }

    free(p->array);
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

void printPuzzle(Puzzle* p){
    for(int i = 0; i < ROW; i++){
        printf("\n");
        for(int j = 0; j < COL; j++){
            printf("%d", p->array[i][j]);
        }
    }
    printf("\n");
}

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
