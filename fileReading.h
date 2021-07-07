#ifndef TP_FINAL_PI_FILEREADING_H
#define TP_FINAL_PI_FILEREADING_H
#define MAX_LONG 256
#define DELIM ";"
#define DIM 8
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "auxFunctions.h"

int readLine(FILE* input,size_t dim,char line[DIM][MAX_LONG]);
void writeLine(FILE* output, size_t dim, char** info);
size_t textToNum(char * text);
float textToFloat(char *text);
char* numToText(size_t num);
char* floatToText(float num);

#endif //TP_FINAL_PI_FILEREADING_H
