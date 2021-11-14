#include <assert.h>

#include "../solver.c"

int main(int argv, char** argc){
    Puzzle* p = instantiatePuzzle();
    FILE* fp = fopen("/home/christian/Documents/C_Projects/Sudoku_Solver/Puzzles/test1.txt", "r");

    assert(getPuzzle(fp, p) != -1);
    
    solve(p);

    assert(p->array[0][0] == 0xFF1C);

    cleanUp(fp, p);
}