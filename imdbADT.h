#ifndef TP_FINAL_PI_IMDBADT_H
#define TP_FINAL_PI_IMDBADT_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define SEPARATOR "," //Separador entre géneros
#define MAX_GENRES 9  //Máxima cantidad de géneros que se leeran por película


typedef struct imdbCDT * imdbADT;

//Genera un nuevo ADT
imdbADT newImdb();

//Añade la película o serie al ADT, si no lo pudo agregar retorna 0, sino 1
int add(imdbADT db, char* titleType, char* primaryTitle, size_t year, char* genre, float rating, size_t votes);

size_t getLastYear(imdbADT db);
size_t getFirstYear(imdbADT db);

size_t getAmount(imdbADT db, char* titleType, size_t year);

void toBeginGenre(imdbADT imdb, size_t year);
int hasNext(imdbADT imdb);
size_t next(imdbADT imdb, char * string);

char* getBest(imdbADT db, char* titleType, size_t year, float* rating, size_t* votes);

void freeImdb(imdbADT imdb);

#endif //TP_FINAL_PI_IMDBADT_H
