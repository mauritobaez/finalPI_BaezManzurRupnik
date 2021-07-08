#include "imdbADT.h"

#define BLOCK 10
#define CHECK_ALLOC(x) {    if((x)==NULL || errno==ENOMEM)\
                            {  fprintf(stderr, "Error. No more memory available.\n");\
                                exit(3);\
                            }\
                        }
//Lista para generos (query2)
typedef struct node{
    char * genre;
    struct node * tail;
    size_t count;
}TNode;

typedef TNode * TList;

//Para query3
typedef struct best{
    char * name;
    float rating;
    size_t votes;
}best;

//Información del año
typedef struct yearInfo{
    size_t amountSeries;
    size_t amountMovies;
    struct best bestMovie;
    struct best bestSeries;
    TList firstGenre;
}yearInfo;

// Nuestro CDT
typedef struct imdbCDT{
    yearInfo ** yearsAfter;
    size_t sizeAfter;
    yearInfo ** yearsBefore;
    size_t sizeBefore;
    size_t yearZero;
    //Se itera un año a la vez
    TList currentGenre;
}imdbCDT;

static char * copyString(const char * string){//Funcion auxiliar para copiar strings en el heap en forma de bloques
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
//Genera un nuevo ADT
imdbADT newImdb(){
    imdbADT new = calloc(1, sizeof(imdbCDT));
    CHECK_ALLOC(new);
    return new;
}

///ADD

static void query1(yearInfo* year, char * titleType){//Se diferencia si es una pelicula o una serie
    if(strcmp(titleType, "movie") == 0)             //Y se aumenta el contador respectivo dentro del año
        year->amountMovies++;
    else if (strcmp(titleType, "tvSeries") == 0)
        year->amountSeries++;
}

static TList addGenre(TList first, char* genres[MAX_GENRES], size_t dim){
    int c;
    if(dim==0)//Si se llega a dim==0 entonces no hay mas generos por los cuales se deba iterar en la lista
    {
        return first;
    }
    if(first==NULL || (c=strcmp(first->genre, genres[0])) >0){//Se crea el genero y se agrega a la lista pero se sigue iterando
        TList new = malloc(sizeof(TNode));
        CHECK_ALLOC(new);
        new->genre = copyString(genres[0]);
        new->count = 1;
        new->tail = addGenre(first, genres+1, dim-1);
        return new;
    }
    else if(c == 0){//Se aumenta el contador del genero respectivo y se sigue iterando
        first->count++;
        first->tail = addGenre(first->tail, genres+1, dim-1);
    }
    else//No es el genero indicado entonces sigue buscando
        first->tail = addGenre(first->tail, genres, dim);
    return first;
}

static void query2(yearInfo* year, char * genreText){
    char* genres[MAX_GENRES];   // Se crea un vector de generos respectivos a la pelicula on una Dim fija
    int c = 0;
    genres[c] = strtok(genreText, SEPARATOR);//Se los separa en Tokens delimitados por una ","
    if(genres[c]!=NULL)
        c++;
    while(c<MAX_GENRES && (genres[c] = strtok(NULL, SEPARATOR)) != NULL)//Se cargan los Tokens mientras
    {                                                                      //NO se pase el limite fijado
        c++;
    }

    qsort(genres, c, sizeof(char*), (int (*)(const void *, const void*)) strcmp);//Se ordenan los generos alfabeticamente

    year->firstGenre = addGenre(year->firstGenre, genres, c);//Se envia el vector a la lista correspondiente para iterar
}

static void query3(yearInfo* year, char* titleType, char* primaryTitle, float rollsRoyce, size_t votes){
    best* bestInfo;
    if(strcmp(titleType, "movie") == 0)//Se separa en el caso de ser una serie o una pelicula
    {
        bestInfo = &year->bestMovie;
    }
    else if (strcmp(titleType, "tvSeries") == 0)
    {
        bestInfo = &year->bestSeries;
    }
    else//Si no es ni una pelicula o ni una serie no hace nada
    {
        return;
    }
    if(bestInfo->name == NULL || votes > bestInfo->votes)//Si en el año no hay mejor pelicula/serie
    {                                                    //O tiene una mayor cantidad de votos se agregan al año respectivo
        free(bestInfo->name);
        bestInfo->name = copyString(primaryTitle);
        bestInfo->votes = votes;
        bestInfo->rating = rollsRoyce;
    }
}

//Añade la película o serie al ADT, sí no lo puedo agregar porque el "year" == 0, retorna 0, sino 1
int add(imdbADT db, char* titleType, char* primaryTitle, size_t year, char* genre, float rating, size_t votes){
    if(year==0)
        return 0;
    if(db->yearZero == 0){      //Si es el primer año que se procesa se lo fija como en año cero que divide a los años
        db->yearZero = year;    //En antes o despues del año cero
    }
    yearInfo*** yearsVec;
    size_t yearIdx;
    size_t* currentSize;
    if(year >= db->yearZero){//Si el año es mayor o igual al año cero se lo usan en años despues
        yearsVec = &db->yearsAfter;
        yearIdx = year - db->yearZero;
        currentSize = &db->sizeAfter;
    }
    else{//Viceversa con los años antes pero el año despues [0] se lo deja vacio por razones de comodidad
        yearsVec = &db->yearsBefore;
        yearIdx = db->yearZero - year;
        currentSize = &db->sizeBefore;
    }
    if( yearIdx >= *currentSize ){//Si no hay espacio suficiente para ese año se crean espacios hasta ese año respectivo
        *yearsVec = realloc(*yearsVec, sizeof(yearInfo*) * (yearIdx + 1));
        CHECK_ALLOC(*yearsVec);
        memset(*yearsVec + *currentSize, 0, sizeof(yearInfo*) * (yearIdx + 1 - *currentSize));
        *currentSize = yearIdx + 1;
    }
    if((*yearsVec)[yearIdx] == NULL)//Si el año no esta inicializado==NULL(Vacio) se crea el espacio correspondiente
    {
        (*yearsVec)[yearIdx] = calloc(1, sizeof(yearInfo));
        CHECK_ALLOC((*yearsVec)[yearIdx]);
    }
    query1((*yearsVec)[yearIdx] ,titleType);//Se aumenta el contador de la Query1
    if(strcmp(titleType, "movie") == 0)//Solo si es una pelicula se lo envia a la query2 ya que tiene generos respectivos
        query2((*yearsVec)[yearIdx] ,genre);
    query3((*yearsVec)[yearIdx] ,titleType,primaryTitle,rating,votes);//Se lo envia a la Query3
    return 1;
}


size_t getLastYear(imdbADT db){//Devuelve el ultimo año que se guardo ej los años van de [1970-2021] devuelve 2021
    if(db->yearZero==0) return 0;
    return db->sizeAfter + db->yearZero - 1;
}

size_t getFirstYear(imdbADT db){//Viceversa con lo anterior si es de [1970-2021] devuelve 1970
    if(db->yearZero==0) return 0;
    return db->yearZero - db->sizeBefore + 1;//Se tiene en cuenta que el añoDespues[0] esta vacio
}

///QUERY 1
size_t getAmount(imdbADT db, char * titleType, size_t year){//Devuelve la cantidad de peliculas o series en una año respectivo
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
    if(yearsVec[yearIdx]==NULL)//Si el año esta vacio==NULL no hubo peliculas/series
        return 0;
    if(strcmp(titleType, "movie") == 0)//Dependiendo de si es una serie o pelicula lo que se pide, devuelve uno o lo otro
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

int hasNext(imdbADT imdb){//Si no tiene genero siguiente en la lista entonces sera NULL
    return imdb->currentGenre !=NULL;
}

// devuelve una copia del current
size_t next(imdbADT imdb, char * string){//Devuelve el genero respectivo del iterador en un parametro dado
    if(!hasNext(imdb)){                  //Y la cantidad de veces que aparece el mismo en su nombre
        return 0;
    }
    strcpy(string, imdb->currentGenre->genre);
    size_t count = imdb->currentGenre->count;
    imdb->currentGenre = imdb->currentGenre->tail;//Se pasa al siguiente genero en la lista
    return count;
}

///QUERY 3
char* getBest(imdbADT db, char* titleType, size_t year, float* rating, size_t* votes){//Devuelve la mejor pelicula
    size_t yearIdx;                                                                   //O serie y sus respectivos datos
    yearInfo** yearsVec;
    if(year >= db->yearZero){
        yearsVec = db->yearsAfter;
        yearIdx = year - db->yearZero;
    }
    else{
        yearsVec = db->yearsBefore;
        yearIdx = db->yearZero - year;
    }
    if(yearsVec[yearIdx]==NULL)//Si no hubo peliculas/series ese año se lo ignora y no se hace nada
        return NULL;
    char * aux = NULL;
    if(strcmp(titleType, "movie") == 0)//Si es una pelicula o serie lo que se pide, devuelve uno o el otro
    {
        if(yearsVec[yearIdx]->bestMovie.name==NULL)
            return NULL;
        *rating = yearsVec[yearIdx]->bestMovie.rating;//Los ratings y votos se devuelven por parametros para dar mas
        *votes = yearsVec[yearIdx]->bestMovie.votes;  //libertad a la hora de implementarlo al Front-End
        aux = copyString(yearsVec[yearIdx]->bestMovie.name);// Es necesario liberar este string luego de usarlo
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
static void freeList(TList list){//Se libera la lista usada y los espacios de los generos usados
    if(list==NULL)
        return;
    freeList(list->tail);
    free(list->genre);
    free(list);
}

static void freeVector(yearInfo ** vector, size_t until){//Se liberan todos los recursos usados en cada año
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
