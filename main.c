#include <stdlib.h>
#include <stdio.h>
#include "imdbADT.h"
#include "fileReading.h"
#define DIM 8
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
    char **line;
    while((line=readLine(input,DIM))!=NULL){
        add(line[0],line[1],textToNum(line[2]),line[4],textToFloat(line[5]), textToNum(line[6]));
    }
    fclose(input);
    return 0;
}