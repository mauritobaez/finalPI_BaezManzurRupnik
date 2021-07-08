#include "imdbADT.h"

#define BLOCK 10
#define CHECK_ALLOC(x) {    if((x)==NULL || errno==ENOMEM)\
                            {  fprintf(stderr, "Error. No more memory available.");\
                                exit(3);\
                            }\
                        }
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

static char * copyString(const char * string){
    char * aux=NULL;
    int dim=0;
    while(*string != '\0'){
        if(dim%BLOCK == 0){
            aux = realloc(aux, dim + BLOCK);
            CHECK_ALLOC(aux);
        }
        aux[dim++] = *string;
        string++;
    }
    aux = realloc(aux, dim+1);
    CHECK_ALLOC(aux);
    aux[dim] = '\0';
    return aux;
}

///NEW
//genera un nuevo ADT
imdbADT newImdb(){
    imdbADT new = calloc(1, sizeof(imdbCDT));
    CHECK_ALLOC(new);
    return new;
}

///ADD

static void query1(yearInfo* year, char * titleType){
    if(strcmp(titleType, "movie") == 0)
        year->amountMovies++;
    else if (strcmp(titleType, "tvSeries") == 0)
        year->amountSeries++;
}

static TList addGenre(TList first, char* genres[MAX_GENRES], size_t dim){
    int c;
    if(dim==0)
    {
        return first;
    }
    if(first==NULL || (c=strcmp(first->genre, genres[0])) >0){
        TList new = malloc(sizeof(TNode));
        CHECK_ALLOC(new);
        new->genre = copyString(genres[0]);
        new->count = 1;
        new->tail = addGenre(first, genres+1, dim-1);
        return new;
    }
    else if(c == 0){
        first->count++;
        first->tail = addGenre(first->tail, genres+1, dim-1);
    }
    else
        first->tail = addGenre(first->tail, genres, dim);
    return first;
}

static void query2(yearInfo* year, char * genreText){
    char* genres[MAX_GENRES];
    int c = 0;
    genres[c] = strtok(genreText, SEPARATOR);
    if(genres[c]!=NULL)
        c++;
    while(c<MAX_GENRES && (genres[c] = strtok(NULL, SEPARATOR)) != NULL)
    {
        c++;
    }

    qsort(genres, c, sizeof(char*), (int (*)(const void *, const void*)) strcmp);

    year->firstGenre = addGenre(year->firstGenre, genres, c);
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

//añade la película o serie al ADT, sí no lo puedo agregar porque el "year" == 0, retorna 0, sino 1
int add(imdbADT db, char* titleType, char* primaryTitle, size_t year, char* genre, float rating, size_t votes){
    if(year==0)
        return 0;
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


size_t getLastYear(imdbADT db){
    if(db->yearZero==0) return 0;
    return db->sizeAfter + db->yearZero - 1;
}

size_t getFirstYear(imdbADT db){
    if(db->yearZero==0) return 0;
    return db->yearZero - db->sizeBefore + 1;
}

///QUERY 1
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

///QUERY 2
//iterador por genero
void toBeginGenre(imdbADT imdb, size_t year){
    if(year>=imdb->yearZero) {
        imdb->currentGenre = imdb->yearsAfter[year - imdb->yearZero]->firstGenre;
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

///QUERY 3
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

///FREE
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
//libera el ADT
void freeImdb(imdbADT imdb){
    freeVector(imdb->yearsAfter, imdb->sizeAfter);
    freeVector(imdb->yearsBefore, imdb->sizeBefore);
    free(imdb->yearsAfter);
    free(imdb->yearsBefore);
    free(imdb);
}
