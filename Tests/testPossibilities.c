#include <assert.h>

#include "../solver.c"

int main(int argv, char** argc){
    Puzzle* p = instantiatePuzzle();
    FILE* fp = fopen("/home/christian/Documents/C_Projects/Sudoku_Solver/Puzzles/test1.txt", "r");

    assert(getPuzzle(fp, p) != -1);
    
    solve(p);

    assert(p->possibilities[0][0] == (short)0xFF1C);
    assert(p->possibilities[0][1] == (short)0xFE04);
    assert(p->possibilities[0][2] == (short)0xFE90);
    assert(p->possibilities[0][3] == (short)0);
    assert(p->possibilities[0][4] == (short)0);
    assert(p->possibilities[0][5] == (short)0xFF10);
    assert(p->possibilities[0][6] == (short)0);
    assert(p->possibilities[0][7] == (short)0xFE90);
    assert(p->possibilities[0][8] == (short)0);

    assert(p->possibilities[1][0] == (short)0);
    assert(p->possibilities[1][1] == (short)0);
    assert(p->possibilities[1][2] == (short)0xFE13);
    assert(p->possibilities[1][3] == (short)0xFE18);
    assert(p->possibilities[1][4] == (short)0);
    assert(p->possibilities[1][5] == (short)0xFE11);
    assert(p->possibilities[1][6] == (short)0xFE0F);
    assert(p->possibilities[1][7] == (short)0);
    assert(p->possibilities[1][8] == (short)0xFE16);

    assert(p->possibilities[2][0] == (short)0);
    assert(p->possibilities[2][1] == (short)0);
    assert(p->possibilities[2][2] == (short)0xFEE2);
    assert(p->possibilities[2][3] == (short)0xFEC0);
    assert(p->possibilities[2][4] == (short)0xFE86);
    assert(p->possibilities[2][5] == (short)0);
    assert(p->possibilities[2][6] == (short)0);
    assert(p->possibilities[2][7] == (short)0xFEA0);
    assert(p->possibilities[2][8] == (short)0xFE46);

    assert(p->possibilities[3][0] == (short)0);
    assert(p->possibilities[3][1] == (short)0);
    assert(p->possibilities[3][2] == (short)0xFE70);
    assert(p->possibilities[3][3] == (short)0);
    assert(p->possibilities[3][4] == (short)0xFF04);
    assert(p->possibilities[3][5] == (short)0xFF70);
    assert(p->possibilities[3][6] == (short)0xFE24);
    assert(p->possibilities[3][7] == (short)0);
    assert(p->possibilities[3][8] == (short)0xFF54);

    assert(p->possibilities[4][0] == (short)0XFE14);
    assert(p->possibilities[4][1] == (short)0xFE45);
    assert(p->possibilities[4][2] == (short)0);
    assert(p->possibilities[4][3] == (short)0);
    assert(p->possibilities[4][4] == (short)0xFE84);
    assert(p->possibilities[4][5] == (short)0);
    assert(p->possibilities[4][6] == (short)0);
    assert(p->possibilities[4][7] == (short)0xFE91);
    assert(p->possibilities[4][8] == (short)0xFE54);

    assert(p->possibilities[5][0] == (short)0xFF08);
    assert(p->possibilities[5][1] == (short)0);
    assert(p->possibilities[5][2] == (short)0xFE61);
    assert(p->possibilities[5][3] == (short)0xFF48);
    assert(p->possibilities[5][4] == (short)0xFF08);
    assert(p->possibilities[5][5] == (short)0);
    assert(p->possibilities[5][6] == (short)0xFE29);
    assert(p->possibilities[5][7] == (short)0);
    assert(p->possibilities[5][8] == (short)0);

    assert(p->possibilities[6][0] == (short)0xFE12);
    assert(p->possibilities[6][1] == (short)0xFE21);
    assert(p->possibilities[6][2] == (short)0);
    assert(p->possibilities[6][3] == (short)0);
    assert(p->possibilities[6][4] == (short)0xFE82);
    assert(p->possibilities[6][5] == (short)0xFE31);
    assert(p->possibilities[6][6] == (short)0xFEA3);
    assert(p->possibilities[6][7] == (short)0);
    assert(p->possibilities[6][8] == (short)0);

    assert(p->possibilities[7][0] == (short)0xFF02);
    assert(p->possibilities[7][1] == (short)0);
    assert(p->possibilities[7][2] == (short)0xFEC3);
    assert(p->possibilities[7][3] == (short)0xFFC0);
    assert(p->possibilities[7][4] == (short)0);
    assert(p->possibilities[7][5] == (short)0xFF41);
    assert(p->possibilities[7][6] == (short)0xFE83);
    assert(p->possibilities[7][7] == (short)0);
    assert(p->possibilities[7][8] == (short)0);

    assert(p->possibilities[8][0] == (short)0);
    assert(p->possibilities[8][1] == (short)0xFE20);
    assert(p->possibilities[8][2] == (short)0);
    assert(p->possibilities[8][3] == (short)0xFF18);
    assert(p->possibilities[8][4] == (short)0);
    assert(p->possibilities[8][5] == (short)0);
    assert(p->possibilities[8][6] == (short)0xFE2A);
    assert(p->possibilities[8][7] == (short)0xFE30);
    // assert(p->possibilities[8][8] == (short)0xFF12);
    

    cleanUp(fp, p);
}