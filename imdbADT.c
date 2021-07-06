#include <stdlib.h>
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
