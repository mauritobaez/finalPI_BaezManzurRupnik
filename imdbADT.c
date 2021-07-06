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
typedef struct yearInfo{
    size_t amountSeries;
    size_t amountMovies;
    struct best bestMovie;
    struct best bestSeries;
    TList firstGenre;
}yearInfo;

// nuestro CDT
typedef struct imdbCDT{
    yearInfo ** yearsAfter;
    size_t sizeAfter;
    yearInfo ** yearsBefore;
    size_t sizeBefore;
    size_t yearZero;
    //se itera un año a la vez
    TList currentGenre;
}imdbCDT;

static void query1(yearInfo* year, char * titleType);
static void query2(yearInfo* year, char * genre);
static void query3(yearInfo* year, char* titleType, char* primaryTitle, float rating, size_t votes);

imdbADT newImdb(){
    return calloc(1, sizeof(imdbCDT));
}
int add(imdbADT db, char* titleType, char* primaryTitle, size_t year, char* genre, float rating, size_t votes){
    if(db->yearZero == 0){
        db->yearZero = year;
    }
    yearInfo*** yearsVec;
    size_t yearIdx;
    size_t* currentSize;
    if(year >= db->yearZero){
        yearsVec = &db->yearsAfter;
        yearIdx = year - db->yearZero;
        currentSize = &db->sizeAfter;
    }
    else{
        yearsVec = &db->yearsBefore;
        yearIdx = db->yearZero - year;
        currentSize = &db->sizeBefore;
    }
    if( yearIdx >= *currentSize ){
        *yearsVec = realloc(*yearsVec, sizeof(yearInfo*) * (yearIdx + 1));
        memset(*yearsVec + *currentSize, 0, sizeof(yearInfo*) * (yearIdx + 1 - *currentSize));
        *currentSize = yearIdx + 1;
    }
    if((*yearsVec)[yearIdx] == NULL)
    {
        (*yearsVec)[yearIdx] = calloc(1, sizeof(yearInfo));
    }
    query1((*yearsVec)[yearIdx] ,titleType);
    if(strcmp(titleType, "movie") == 0)
        query2((*yearsVec)[yearIdx] ,genre);
    query3((*yearsVec)[yearIdx] ,titleType,primaryTitle,rating,votes);
    return 1;
}

static void query1(yearInfo* year, char * titleType){
    if(strcmp(titleType, "movie") == 0)
        year->amountMovies++;
    else if (strcmp(titleType, "tvSeries") == 0)
        year->amountSeries++;
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

static void query2(yearInfo* year, char * genre){
    year->firstGenre = addGenre(year->firstGenre, genre);
}

static void query3(yearInfo* year, char* titleType, char* primaryTitle, float rollsRoyce, size_t votes){
    best* bestInfo;
    if(strcmp(titleType, "movie") == 0)
    {
        bestInfo = &year->bestMovie;
    }
    else if (strcmp(titleType, "tvSeries") == 0)
    {
        bestInfo = &year->bestSeries;
    }
    else
    {
        return;
    }
    if(bestInfo->name == NULL || votes > bestInfo->votes)
    {
        bestInfo->name = copyString(primaryTitle);
        bestInfo->votes = votes;
        bestInfo->rating = rollsRoyce;
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



