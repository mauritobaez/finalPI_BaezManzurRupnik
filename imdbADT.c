#include "imdbADT.h"

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
}year;

// nuestro CDT
typedef struct imdbCDT{
    year ** yearsAfter;
    size_t sizeAfter;
    year ** yearsBefore;
    size_t sizeBefore;
    size_t yearZero;
    //se itera un año a la vez
    TList currentGenre;
}imdbCDT;

imdbADT newImdb(){
    return calloc(1, sizeof(imdbCDT));
}
int add(imdbADT db, char* titleType, char* primaryTitle, size_t year, char* genre, float rating, size_t votes){
    if(db->yearZero == 0){
        db->yearZero=year;
    }
    if(year > db->yearZero){
        if( year-db->yearZero > db->sizeAfter){
            db->sizeAfter=year-db->yearZero;
            db->yearsAfter=realloc(db->yearsAfter, sizeof(struct year)*(db->sizeAfter));
        }

    }else{
        if( db->yearZero-year+1 > db->sizeBefore){
            db->sizeBefore=(db->yearZero-year+1);
            db->yearsBefore=realloc(db->yearsBefore, sizeof(struct year)*(db->sizeBefore));
        }
    }
    if((strcmp(titleType,"movie"))==0){
    }
    else if((strcmp(titleType,"tvSeries"))==0){
    }
    return 0;
}
static TList addGenre(TList first, char * genre){
    int c;
    if(first==NULL || (c=strcmp(first->genre, genre)) >0){
        TList new = malloc(sizeof(TNode));
        new->genre = copyString(genre);
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
        addGenre(imdb->yearsBefore[imdb->yearZero- year]->firstGenre, genre);
    }
}

//iterador por genero
void toBeginGenre(imdbADT imdb, size_t year){
    if(year>imdb->yearZero){
        imdb->currentGenre = imdb->yearsAfter[year - imdb->yearZero-1]->firstGenre;
    }
    else
        imdb->currentGenre = imdb->yearsBefore[-year+imdb->yearZero]->firstGenre;
}

int hasNext(imdbADT imdb){
    return imdb->currentGenre !=NULL;
}

// devuelve una copia del current
char * next(imdbADT imdb){
    if(!hasNext(imdb)){
        //algo malo pasa
        // return NULL;
        // ?
    }
    char * aux = copyString(imdb->currentGenre->genre);
    imdb->currentGenre = imdb->currentGenre->tail;
    return aux;
}


static void query1(){

}
