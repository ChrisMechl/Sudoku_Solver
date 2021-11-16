#include "solver.c"

int main(int argc, char** argv){
    int pid = getpid();
    // printf("%d\n", pid);

    if(argc != 2 && argc != 3){
        fprintf(stderr, "Invalid arguments: Please provide sudoku file location and -s flag if you wish to see steps\n");
        return EXIT_FAILURE;
    }

    bool steps = false;
    bool timer = false;

    if(argc == 3){
        if(strcmp(argv[2], "-s") == 0){
            steps = true;
        }
        else if(strcmp(argv[2], "-t") == 0){
            timer = true;
        }
    }
    if(argc == 4){
        if(strcmp(argv[3], "-s") == 0){
            steps = true;
        }
        else if(strcmp(argv[3], "-t") == 0){
            timer = true;
        }
    }

    FILE* fp = fopen(argv[1], "r");
    if(!fp){
        fprintf(stderr, "Invalid file location\n");
        return EXIT_FAILURE;
    }

    clock_t begin = clock();

    Puzzle* p = instantiatePuzzle(steps);
    
    /* Fill matrix */
    if(getPuzzle(fp, p) == -1){
        fprintf(stderr, "Invalid sudoku file format. Not enough rows or columns\n");
        cleanUp(fp, p);
        return EXIT_FAILURE;
    }

    printf("\nStarting Puzzle\n");
    ppPuzzle(p);

    solve(p);

    clock_t end = clock();
    if(timer){
        printf("Time spent solving %f\n", (double)(end - begin) / CLOCKS_PER_SEC);
    }
    cleanUp(fp, p);

    return EXIT_SUCCESS;

}