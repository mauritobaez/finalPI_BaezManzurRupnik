#ifndef TP_FINAL_PI_FILEREADING_H
#define TP_FINAL_PI_FILEREADING_H
#define MAX_LONG 256
#define DELIM ";"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
char ** readLine(FILE* input,size_t dim);
size_t textToNum(char * text);


#endif //TP_FINAL_PI_FILEREADING_H
