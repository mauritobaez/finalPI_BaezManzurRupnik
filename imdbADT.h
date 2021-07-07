#ifndef TP_FINAL_PI_IMDBADT_H
#define TP_FINAL_PI_IMDBADT_H
#include <stdlib.h>
#include "auxFunctions.h"
#include <stdio.h>
#include <string.h>

typedef struct imdbCDT * imdbADT;

imdbADT newImdb();

int add(imdbADT db, char* titleType, char* primaryTitle, size_t year, char* genre, float rating, size_t votes);

size_t getLastYear(imdbADT db);
size_t getFirstYear(imdbADT db);

size_t getAmountSeries(imdbADT db, size_t year);
size_t getAmountMovies(imdbADT db, size_t year);

void toBeginGenre(imdbADT imdb, size_t year);
int hasNext(imdbADT imdb);
char* next(imdbADT imdb, size_t* count);

char* getBest(imdbADT db, char* titleType, size_t year, float* rating, size_t* votes);

void freeImdb(imdbADT imdb);

#endif //TP_FINAL_PI_IMDBADT_H
