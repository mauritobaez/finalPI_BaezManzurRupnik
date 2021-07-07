#ifndef TP_FINAL_PI_FILEREADING_H
#define TP_FINAL_PI_FILEREADING_H
#define MAX_LONG 128
#define DELIM ";"
#define DIM 8
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "auxFunctions.h"

int readLine(FILE* input, char line[DIM][MAX_LONG]);
void writeLine(FILE* output, size_t dim, char** info);
void numToText(size_t num, char * string);
void floatToText(float num, char * ans);

#endif //TP_FINAL_PI_FILEREADING_H
