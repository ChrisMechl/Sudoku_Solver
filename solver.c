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

    /* Create matrix */
    int** array = malloc(sizeof(int*) * ROW);
    for(int i = 0; i < ROW; i++){
        array[i] = malloc(sizeof(int) * COL);
        memset(array[0], 0, (ROW * sizeof(int)));
    }
    for(int i = 0; i < ROW; i++){
        array[i][0] = i+1;
    }
    
    /* Fill matrix */
    if(getPuzzle(fp, array) == -1){
        fprintf(stderr, "Invalid sudoku file format. Not enough rows or columns\n");
        fclose(fp);
        free(array);
        return EXIT_FAILURE;
    }

    for(int i = 0; i < ROW; i++){
        if(i != 0){
            printf("\n");
        }
        for(int j = 0; j < COL; j++){
            printf("%d", array[i][j]);
        }
    }
    printf("\n");
    free(array);
    fclose(fp);
    return EXIT_SUCCESS;

}

int getPuzzle(FILE* fp, int** array){
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
                array[i][j] = 0;
            }
            /* If char is 1-9, set the array position to it */
            else{
                array[i][j] = cur - 48;
            }
        }
    }
}
