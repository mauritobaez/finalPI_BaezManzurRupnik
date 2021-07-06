#include <stdlib.h>
#include <stdio.h>
#include "imdbADT.h"

int main(int argcount, char* args[])
{
    if(argcount != 2)
    {
        fprintf(stderr, "Error in amount of arguments expected. (one)");
        exit(1);
    }
    FILE * input;
    input= fopen(args[1],"rt");
    if(input==NULL){
        fprintf(stderr, "Error in path to file,must exist");
        exit(2);
    }
    fclose(input);
    return 0;
}