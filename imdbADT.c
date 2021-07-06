#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//sacar, tiene que ir en el .h
typedef struct imdbCDT * imdbADT;
//lista para generos (query2)
typedef struct node{
    char * genre;
    struct node * tail;
    size_t count;
}TNode;

typedef TNode * TList;

//para query3
typedef struct best{
    char * name;
    float rating;
    size_t votes;
}best;

//información del año
typedef struct year{
    size_t amountSeries;
    size_t amountMovies;
    struct best bestMovie;
    struct best bestSeries;
    TList firstGenre;
    TList currentGenre;
}year;

// nuestro CDT
typedef struct imdbCDT{
    year ** yearsAfter;
    year ** yearsBefore;
    size_t size;
    size_t yearZero;
}imdbCDT;

imdbADT newImdb(){
    return calloc(1, sizeof(imdbCDT));
}

static TList addGenre(TList first, char * genre){
    int c;
    if(first==NULL || (c=strcmp(first->genre, genre)) >0){
        TList new = malloc(sizeof(TNode));
       // new->genre = copyString(genre);
        new->count=1;
        new->tail=first;
        return new;
    }
    else if(!c){
        first->count++;
    }
    else
        first->tail = addGenre(first->tail, genre);
    return first;
}

static void query2(imdbADT imdb, size_t year, char * genre){
    if(year > imdb->yearZero){
        addGenre(imdb->yearsAfter[year -imdb->yearZero -1]->firstGenre, genre);
    }
    else{
        addGenre(imdb->yearsBefore[imdb->yearZero- year -1]->firstGenre, genre);
    }
}

