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
                    //El primer argumento está en args[1]

    return 0;
}