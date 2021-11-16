#include "solver.c"

int main(int argc, char** argv){
    int pid = getpid();
    // printf("%d\n", pid);

    if(argc != 2 && argc != 3){
        fprintf(stderr, "Invalid arguments: Please provide sudoku file location and -s flag if you wish to see steps\n");
        return EXIT_FAILURE;
    }

    bool steps = false;

    if(argc == 3){
        if(strcmp(argv[2], "-s") == 0){
            steps = true;
        }
    }

    FILE* fp = fopen(argv[1], "r");
    if(!fp){
        fprintf(stderr, "Invalid file location\n");
        return EXIT_FAILURE;
    }

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

    cleanUp(fp, p);

    return EXIT_SUCCESS;

}