#include <stdlib.h>
#include <stdio.h>
#include "imdbADT.h"
#include "fileManagement.h"

//Se cargan los valores para escribir el query3
void loadQuery3(char * name, int fromColumn, char buffer[][MAX_LONG], size_t votes, float rating);

int main(int argcount, char* args[])
{
    ///LECTURA DE DATOS DEL CSV
    //Se fija si la cantidad de argumentos es correcta
    if(argcount != 2)
    {
        fprintf(stderr, "Error in amount of arguments expected. (one)\n");
        exit(1);
    }

    //Se fija que exista el archivo de input en el path indicado
    FILE * input;
    input = fopen(args[1],"rt");
    if(input==NULL){
        fprintf(stderr, "Error in path to file,must exist\n");
        exit(2);
    }


    char buffer[DIM][MAX_LONG] = {0}; //En principio, se guardará acá la información leída por cada línea

    //Se lee la primera línea, que es el encabezado
    readLine(input, buffer);

    int year;
    imdbADT db = newImdb();

    while(readLine(input, buffer)!=0){ //Cuando se haya terminado de leer el archivo, salimos del while
        year = atoi(buffer[2]); //Extraemos el año del renglón leído
        if(year > 0) { //Si en el campo del año no hay un numero positivo, no lo tomamos en cuenta
            //Le pasamos al ADT los datos relevantes que necesita sobre este renglón leído
            add(db, buffer[0], buffer[1], (size_t)year, buffer[4], (float)atof(buffer[5]), (size_t)atoi(buffer[6]));
        }
    }
    fclose(input);

    ///ESCRITURA DE ARCHIVOS DE SALIDA
    FILE * query1 = fopen("query1.csv","wt");
    FILE * query2 = fopen("query2.csv","wt");
    FILE * query3 = fopen("query3.csv","wt");

    //Para escribir lineas en los archivos usamos el mismo buffer que anteriormente con la función writeLine

    //Escribimos el encabezado de cada archivo
    strcpy(buffer[0], "year");
    strcpy(buffer[1], "films");
    strcpy(buffer[2], "series");
    writeLine(query1,3,buffer);

    strcpy(buffer[1], "genre");
    strcpy(buffer[2], "films");
    writeLine(query2,3, buffer);

    strcpy(buffer[0], "startYear");
    strcpy(buffer[1], "films");
    strcpy(buffer[2], "votesFilm");
    strcpy(buffer[3], "ratingFilm");
    strcpy(buffer[4], "serie");
    strcpy(buffer[5], "votesSerie");
    strcpy(buffer[6], "ratingSerie");
    writeLine(query3, 7, buffer);

    //Obtenemos los años más reciente y más antiguo
    size_t from = getLastYear(db);
    size_t to = getFirstYear(db);
    if(from != 0 && to != 0) //  Si no hubieron títulos con año identificado => from=to=0 => no hacemos nada
    {
        for(size_t y = from ; y >= to ; y--) //Recorremos todos los años entre el último y el primero, de manera descendente.
        {
            //Extraemos la cantidad de películas y series en ese año
            size_t movies = getAmount(db, "movie", y);
            size_t series = getAmount(db, "tvSeries", y);

            if( movies!=0 || series!=0 ) //Si hubo algún título ese año
            {
                //Escribimos el renglón de ese año en query1.csv
                numToText(y, buffer[0]);
                numToText(movies, buffer[1]);
                numToText(series, buffer[2]);
                writeLine(query1, 3, buffer);

                //El año ya está en buffer[0]

                //Escribimos los renglones con cada género de ese año en query2.csv
                toBeginGenre(db, y);
                size_t count;
                while(hasNext(db)){ //Cuando no hayan más géneros ese año, hasNext devuelve 0
                    count = next(db, buffer[1]);
                    numToText(count, buffer[2]);
                    writeLine(query2, 3, buffer);
                }


                //Escribimos el renglón de ese año en query3.csv
                float rating;
                size_t votes;
                //El año ya está en buffer[0]
                char * aux = getBest(db, "movie", y, &rating, &votes);
                loadQuery3(aux, 1, buffer, votes, rating);
                free(aux);

                aux = getBest(db, "tvSeries", y, &rating, &votes);
                loadQuery3(aux, 4, buffer, votes, rating);
                free(aux);

                writeLine(query3, 7, buffer);
            }
        }
    }
    //Se cierran los archivos ya escritos, y se liberan los recursos usados del ADT
    fclose(query1);
    fclose(query2);
    fclose(query3);
    freeImdb(db);

    return 0;
}

//Carga en el buffer los valores necesarios para el query3
void loadQuery3(char * name, int fromColumn, char buffer[][MAX_LONG], size_t votes, float rating){
    if(name!=NULL){
        strcpy(buffer[fromColumn], name);
        numToText(votes, buffer[fromColumn+1]);
        floatToText(rating, buffer[fromColumn+2]);
    }
        //si no hubo series/películas
    else{
        for(int i=0; i<=2; i++){
            strcpy(buffer[fromColumn+i],"\\N");
        }
    }
}
