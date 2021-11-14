#include "solver.c"

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

    Puzzle* p = instantiatePuzzle();
    
    /* Fill matrix */
    if(getPuzzle(fp, p) == -1){
        fprintf(stderr, "Invalid sudoku file format. Not enough rows or columns\n");
        cleanUp(fp, p);
        return EXIT_FAILURE;
    }

    ppPuzzle(p);

    solve(p);

    cleanUp(fp, p);

    return EXIT_SUCCESS;

}