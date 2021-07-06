#ifndef TP_FINAL_PI_IMDBADT_H
#define TP_FINAL_PI_IMDBADT_H
#include <stdlib.h>
#include "auxFunctions.h"
#include <stdio.h>
#include <string.h>

typedef struct imdbCDT * imdbADT;

imdbADT newImdb();
int add(char* titleType, char* primaryTitle, size_t year, char* genre, float rating, size_t votes);

#endif //TP_FINAL_PI_IMDBADT_H
