#ifndef TP_FINAL_PI_FILEREADING_H
#define TP_FINAL_PI_FILEREADING_H
#define MAX_LONG 256
#define DELIM ";"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "auxFunctions.h"

char ** readLine(FILE* input,size_t dim);
void writeLine(FILE* output, size_t dim, char** info);
size_t textToNum(char * text);
float textToFloat(char *text);
char* numToText(size_t num);
char* floatToText(float num);

#endif //TP_FINAL_PI_FILEREADING_H
