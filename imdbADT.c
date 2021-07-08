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
static void query3(yearInfo* year, char* titleType, char* primaryTitle, float rollsRoyce, size_t votes);

static void freeList(TList list){
    if(list==NULL)
        return;
    freeList(list->tail);
    free(list->genre);
    free(list);
}

static void freeVector(yearInfo ** vector, size_t until){
    for(int i=0; i<until; i++){
        if(vector[i] != NULL){
            freeList(vector[i]->firstGenre);
            free(vector[i]->bestSeries.name);
            free(vector[i]->bestMovie.name);
            free(vector[i]);
        }
    }
}

void freeImdb(imdbADT imdb){
    freeVector(imdb->yearsAfter, imdb->sizeAfter);
    freeVector(imdb->yearsBefore, imdb->sizeBefore);
    free(imdb->yearsAfter);
    free(imdb->yearsBefore);
    free(imdb);
}

imdbADT newImdb(){
    imdbADT new = calloc(1, sizeof(imdbCDT));
    CHECK_ALLOC(new);
    return new;
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
        CHECK_ALLOC(*yearsVec);
        memset(*yearsVec + *currentSize, 0, sizeof(yearInfo*) * (yearIdx + 1 - *currentSize));
        *currentSize = yearIdx + 1;
    }
    if((*yearsVec)[yearIdx] == NULL)
    {
        (*yearsVec)[yearIdx] = calloc(1, sizeof(yearInfo));
        CHECK_ALLOC((*yearsVec)[yearIdx]);
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
        CHECK_ALLOC(new);
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
    //hacer un char** con los distintos generos y ordenarlo
    //que addGenre no pare cuando agrega 1, sigue hasta que agregó todos los q estaban en el char**
    //hay q pasarle el char** por parámetro y la dimension, para que le pueda ir restando uno
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
        free(bestInfo->name);
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
size_t next(imdbADT imdb, char * string){
    if(!hasNext(imdb)){
        return 0;
    }
    strcpy(string, imdb->currentGenre->genre);
    size_t count = imdb->currentGenre->count;
    imdb->currentGenre = imdb->currentGenre->tail;
    return count;
}

size_t getLastYear(imdbADT db){
    if(db->yearZero==0) return 0;
    return db->sizeAfter + db->yearZero - 1;
}

size_t getFirstYear(imdbADT db){
    if(db->yearZero==0) return 0;
    return db->yearZero - db->sizeBefore + 1;
}

size_t getAmount(imdbADT db, char * titleType, size_t year){
    size_t yearIdx;
    yearInfo** yearsVec;
    if(year >= db->yearZero){
        yearsVec = db->yearsAfter;
        yearIdx = year - db->yearZero;
    }
    else{
        yearsVec = db->yearsBefore;
        yearIdx = db->yearZero - year;
    }
    if(yearsVec[yearIdx]==NULL)
        return 0;
    if(strcmp(titleType, "movie") == 0)
    {
        return yearsVec[yearIdx]->amountMovies;
    }
    else if (strcmp(titleType, "tvSeries") == 0)
    {
        return yearsVec[yearIdx]->amountSeries;
    }
    return 0;
}


char* getBest(imdbADT db, char* titleType, size_t year, float* rating, size_t* votes){
    size_t yearIdx;
    yearInfo** yearsVec;
    if(year >= db->yearZero){
        yearsVec = db->yearsAfter;
        yearIdx = year - db->yearZero;
    }
    else{
        yearsVec = db->yearsBefore;
        yearIdx = db->yearZero - year;
    }
    if(yearsVec[yearIdx]==NULL)
        return NULL;
    char * aux = NULL;
    if(strcmp(titleType, "movie") == 0)
    {
        if(yearsVec[yearIdx]->bestMovie.name==NULL)
            return NULL;
        *rating = yearsVec[yearIdx]->bestMovie.rating;
        *votes = yearsVec[yearIdx]->bestMovie.votes;
        aux = copyString(yearsVec[yearIdx]->bestMovie.name); //hay que hacerle free, o cambiar la función directamente
    }
    else if (strcmp(titleType, "tvSeries") == 0)
    {
        if(yearsVec[yearIdx]->bestSeries.name==NULL)
            return NULL;
        *rating = yearsVec[yearIdx]->bestSeries.rating;
        *votes = yearsVec[yearIdx]->bestSeries.votes;
        aux = copyString(yearsVec[yearIdx]->bestSeries.name);
    }
    return aux;
}
