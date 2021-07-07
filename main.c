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
    char **line=readLine(input,DIM); // leo la primera linea que es el encabezado
    size_t year;
    imdbADT db=newImdb();
    while((line=readLine(input,DIM))!=NULL){
        year=textToNum(line[2]);
        if(year!=0) {
            add(db, line[0], line[1], year, line[4], textToFloat(line[5]), textToNum(line[6]));
        }
    }
    fclose(input);
    FILE * query1 = fopen("./query1.csv","wt");
    FILE * query2 = fopen("./query2.csv","wt");
    FILE * query3 = fopen("./query3.csv","wt");

    return 0;
}