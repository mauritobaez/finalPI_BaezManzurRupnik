#include "imdbADT.h"

#define BLOCK 10
//Macro para chequear que haya memoria suficiente cuando se realiza un malloc, calloc o realloc
#define CHECK_ALLOC(x,y) {    if((x)==NULL || errno==ENOMEM)\
                            {   errno=ENOMEM;\
                                return (y);\
                            }\
                        }

//Nodo de lista para generos (query2)
typedef struct node{
    char * genre;
    struct node * tail;
    size_t count;
}TNode;

typedef TNode * TList;

//Para query3 - Contiene información relevante de la mejor película o serie del año
typedef struct best{
    char * name;
    float rating;
    size_t votes;
}best;

//Información relevante del año
typedef struct yearInfo{
    size_t amountSeries;
    size_t amountMovies;
    struct best bestMovie;
    struct best bestSeries;
    TList firstGenre;
}yearInfo;

// Nuestro CDT
typedef struct imdbCDT{
    yearInfo ** yearsAfter;         //Son dos vectores dinámicos a partir del "año cero" (el año del primer título ingresado)
    size_t sizeAfter;               //Uno hacia adelante en el tiempo, y uno hacia atras
    yearInfo ** yearsBefore;        //Ejemplo: si el primer año leído es 1970 => el 1973 está en el elemento 3 de yearsAfter, el 1970 está en el elemento 0 de yearsAfter,
    size_t sizeBefore;              //y el 1969 estará en el elemento 1 de yearsBefore. La posición 0 de yearsBefore la dejamos sin usar para simplificar las cuentas.
    size_t yearZero;                //Si por ejemplo, el año 1958 no posee películas ni series, entonces yearsBefore[12]=NULL
    //Se itera por los géneros un año a la vez
    TList currentGenre;
}imdbCDT;

static char * copyString(const char * string){  //Funcion auxiliar para copiar strings en el heap en forma de bloques
    char * aux=NULL;                            //Si no alcanza la memoria, devuelve NULL y setea errno en ENOMEM
    int dim=0;
    while(*string != '\0'){
        if(dim%BLOCK == 0){
            errno = 0;
            aux = realloc(aux, dim + BLOCK);
            CHECK_ALLOC(aux, NULL);
        }
        aux[dim++] = *string;
        string++;
    }
    errno = 0;
    aux = realloc(aux, dim+1);
    CHECK_ALLOC(aux, NULL);
    aux[dim] = '\0';
    return aux;
}

///NEW
//Genera un nuevo ADT
//Si no alcanza la memoria, devuelve NULL y setea errno en ENOMEM
imdbADT newImdb(){
    errno = 0;
    imdbADT new = calloc(1, sizeof(imdbCDT));
    CHECK_ALLOC(new, NULL);
    return new;
}

///ADD

static void query1(yearInfo* year, char * titleType){   //Se diferencia si es una pelicula o una serie
    if(strcmp(titleType, "movie") == 0)                 //Y se aumenta el contador respectivo dentro del año
        year->amountMovies++;
    else if (strcmp(titleType, "tvSeries") == 0)
        year->amountSeries++;
}

static TList addGenre(TList first, char* genres[MAX_GENRES], size_t dim){   //Recibe todos los géneros (ordenados) a los que se les debe incrementar su contador
    int c;                                                                  //Si no alcanzó la memoria, setea errno en ENOMEM
    if(dim==0)  //Si se llega a dim==0 entonces no hay mas géneros por los cuales se deba iterar en la lista
    {
        return first;
    }
    if(first==NULL || (c=strcmp(first->genre, genres[0])) > 0){  //Se crea el género y se agrega a la lista pero se sigue iterando
        errno = 0;
        TList new = malloc(sizeof(TNode));
        CHECK_ALLOC(new, first);
        new->genre = copyString(genres[0]);
        new->count = 1;
        new->tail = addGenre(first, genres+1, dim-1);
        return new;
    }
    else if(c == 0){    //Se aumenta el contador del género respectivo y se sigue iterando
        first->count++;
        first->tail = addGenre(first->tail, genres+1, dim-1);
    }
    else    //No es el género indicado, entonces sigue buscando
    {
        first->tail = addGenre(first->tail, genres, dim);
    }
    return first;
}

static void query2(yearInfo* year, char * genreText){
    char* genres[MAX_GENRES];   // Se crea un vector de géneros respectivos a la película con una Dim fija
    int c = 0;
    genres[c] = strtok(genreText, SEPARATOR);   //Se los separa en Tokens delimitados por una ","
    if(genres[c]!=NULL)
        c++;
    while(c<MAX_GENRES && (genres[c] = strtok(NULL, SEPARATOR)) != NULL)    //Se cargan los Tokens mientras
    {                                                                          //NO se pase el límite fijado
        c++;
    }

    qsort(genres, c, sizeof(char*), (int (*)(const void *, const void*)) strcmp);   //Se ordenan los géneros alfabéticamente

    year->firstGenre = addGenre(year->firstGenre, genres, c);   //Se envía el vector con los géneros ordenados a la lista correspondiente
}

static void query3(yearInfo* year, char* titleType, char* primaryTitle, float rollsRoyce, size_t votes){
    best* bestInfo;
    if(strcmp(titleType, "movie") == 0) //Se separa en el caso de ser una serie o una película
    {
        bestInfo = &year->bestMovie;
    }
    else if (strcmp(titleType, "tvSeries") == 0)
    {
        bestInfo = &year->bestSeries;
    }
    else    //Si no es ni una película o ni una serie no hace nada
    {
        return;
    }
    if(bestInfo->name == NULL || votes > bestInfo->votes)   //Si es la primera película/serie actualmente en ese año
    {                                                       //O tiene una mayor cantidad de votos se agregan al año respectivo
        free(bestInfo->name);
        bestInfo->name = copyString(primaryTitle);
        bestInfo->votes = votes;
        bestInfo->rating = rollsRoyce;
    }
}

//Añade la película o serie al ADT, si no lo pudo agregar porque el "year" == 0, retorna 0, sino 1
int add(imdbADT db, char* titleType, char* primaryTitle, size_t year, char* genre, float rating, size_t votes){
    if(year==0)
        return 0;
    if(db->yearZero == 0){      //Si es el primer año que se procesa se lo fija como en año cero que divide a los años
        db->yearZero = year;    //En antes o después del año cero
    }
    yearInfo*** yearsVec;
    size_t yearIdx;
    size_t* currentSize;
    if(year >= db->yearZero){       //Si el año es mayor o igual al año cero se usa yearsAfter
        yearsVec = &db->yearsAfter;
        yearIdx = year - db->yearZero;
        currentSize = &db->sizeAfter;
    }
    else{       //Si el año es menor al año cero se usa yearsBefore. yearsBefore[0] se lo deja vacio por razones de comodidad
        yearsVec = &db->yearsBefore;
        yearIdx = db->yearZero - year;
        currentSize = &db->sizeBefore;
    }
    if( yearIdx >= *currentSize ){  //Si no hay espacio suficiente para ese año se crean espacios hasta ese año respectivo
        errno = 0;
        *yearsVec = realloc(*yearsVec, sizeof(yearInfo*) * (yearIdx + 1));
        CHECK_ALLOC(*yearsVec,0);
        memset(*yearsVec + *currentSize, 0, sizeof(yearInfo*) * (yearIdx + 1 - *currentSize));
        *currentSize = yearIdx + 1;
    }
    if((*yearsVec)[yearIdx] == NULL)    //Si en ese año no hubieron títulos hasta ahora (==NULL), se reserva el espacio correspondiente
    {
        errno = 0;
        (*yearsVec)[yearIdx] = calloc(1, sizeof(yearInfo));
        CHECK_ALLOC((*yearsVec)[yearIdx], 0);
    }
    query1((*yearsVec)[yearIdx] ,titleType);    //Se aumenta el contador de la Query1
    if(strcmp(titleType, "movie") == 0)         //Solo si es una película se lo envía a la query2
    {
        query2((*yearsVec)[yearIdx] ,genre);
        if(errno == ENOMEM) return 0;
    }
    query3((*yearsVec)[yearIdx] ,titleType,primaryTitle,rating,votes);  //Se lo envía a la Query3
    if(errno == ENOMEM) return 0;
    return 1;
}


size_t getLastYear(imdbADT db){     //Devuelve el año mas reciente que se guardó. Ej.: si los años van de [1980-2021], devuelve 2021
    if(db->yearZero==0) return 0;
    return db->sizeAfter + db->yearZero - 1;
}

size_t getFirstYear(imdbADT db){    //Devuelve el año mas antiguo que se guardó. Ej.: si los años van de [1980-2021], devuelve 1980
    if(db->yearZero==0) return 0;
    return db->yearZero - db->sizeBefore + 1;   //Se tiene en cuenta que el yearsBefore[0] esta vacio
}

///QUERY 1
size_t getAmount(imdbADT db, char * titleType, size_t year){    //Devuelve la cantidad de películas o series en una año respectivo
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
    if(yearsVec[yearIdx]==NULL) //Si el año está vacio(==NULL), no hubo películas/series
        return 0;
    if(strcmp(titleType, "movie") == 0)  //Dependiendo de si es una serie o película lo que se pide, devuelve uno o lo otro
    {
        return yearsVec[yearIdx]->amountMovies;
    }
    else if (strcmp(titleType, "tvSeries") == 0)
    {
        return yearsVec[yearIdx]->amountSeries;
    }
    return 0;   //Si en titleType se recibió algo distinto de "movie" o "tvSeries", devuelve 0
}

///QUERY 2
//Se inicializa el iterador por genero
void toBeginGenre(imdbADT imdb, size_t year){
    if(year>=imdb->yearZero) {
        imdb->currentGenre = imdb->yearsAfter[year - imdb->yearZero]->firstGenre;
    }
    else
        imdb->currentGenre = imdb->yearsBefore[-year+imdb->yearZero]->firstGenre;
}

int hasNext(imdbADT imdb){  //Si no hay más generos para leer en la lista, devuelve 0
    return imdb->currentGenre !=NULL;
}

//Devuelve la cantidad de películas y, por parámetro, el nombre del género
size_t next(imdbADT imdb, char * string){
    if(!hasNext(imdb)){
        return 0;
    }
    strcpy(string, imdb->currentGenre->genre);
    size_t count = imdb->currentGenre->count;
    imdb->currentGenre = imdb->currentGenre->tail;//Se pasa al siguiente género en la lista
    return count;
}

///QUERY 3
//Devuelve el nombre de la mejor película o serie y, por parámetro, el rating y la cantidad de votos de la misma
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
    if(yearsVec[yearIdx]==NULL) //Si no hubo películas ni series ese año se devuelve NULL
        return NULL;
    char * aux = NULL;
    if(strcmp(titleType, "movie") == 0) //Si es una película o serie lo que se pide, devuelve uno o el otro
    {
        if(yearsVec[yearIdx]->bestMovie.name==NULL)
            return NULL;
        *rating = yearsVec[yearIdx]->bestMovie.rating;          //Los ratings y votos se devuelven por parametros para dar mas
        *votes = yearsVec[yearIdx]->bestMovie.votes;            //libertad a la hora de implementar el Front-End
        aux = copyString(yearsVec[yearIdx]->bestMovie.name);    //Es necesario liberar este string luego de usarlo
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
static void freeList(TList list){   //Se libera la lista usada y los espacios de los géneros usados
    if(list==NULL)
        return;
    freeList(list->tail);
    free(list->genre);
    free(list);
}

static void freeVector(yearInfo ** vector, size_t until){   //Se liberan todos los recursos usados en cada año
    for(int i=0; i<until; i++){
        if(vector[i] != NULL){
            freeList(vector[i]->firstGenre);
            free(vector[i]->bestSeries.name);
            free(vector[i]->bestMovie.name);
            free(vector[i]);
        }
    }
}

//Libera el ADT
void freeImdb(imdbADT imdb){
    freeVector(imdb->yearsAfter, imdb->sizeAfter);
    freeVector(imdb->yearsBefore, imdb->sizeBefore);
    free(imdb->yearsAfter);
    free(imdb->yearsBefore);
    free(imdb);
}
