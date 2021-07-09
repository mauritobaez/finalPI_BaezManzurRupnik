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

//Devuelve el año mas reciente que se guardó. Ej.: si los años van de [1980-2021], devuelve 2021
size_t getLastYear(imdbADT db);
//Devuelve el año mas antiguo que se guardó. Ej.: si los años van de [1980-2021], devuelve 1980
size_t getFirstYear(imdbADT db);

//Devuelve la cantidad de películas o series en una año respectivo
size_t getAmount(imdbADT db, char* titleType, size_t year);

//Se inicializa el iterador por genero en el año recibido
void toBeginGenre(imdbADT imdb, size_t year);
//Si no hay más generos ese año, devuelve 0, sino 1
int hasNext(imdbADT imdb);
//Devuelve la cantidad de películas y, por parámetro, el nombre del género
size_t next(imdbADT imdb, char * string);

//Devuelve el nombre de la mejor película o serie y, por parámetro, el rating y la cantidad de votos de la misma. Si no hubo películas ni series ese año se devuelve NULL.
char* getBest(imdbADT db, char* titleType, size_t year, float* rating, size_t* votes);

//Libera el ADT
void freeImdb(imdbADT imdb);

#endif //TP_FINAL_PI_IMDBADT_H
