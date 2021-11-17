#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
    if(argc != 2){
        fprintf(stderr, "Invalid arguments: Please give path to text file\n");
    }

    FILE* fp = fopen(argv[1], "r");
    if(!fp){
        fprintf(stderr, "Invalid file location\n");
        return EXIT_FAILURE;
    }
}