#include <stdlib.h>
#include <stdio.h>
#include "imdbADT.h"
#include "fileManagement.h"


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
    if(from != 0 && to != 0) //  Si no hubo ningún título con año identificado => from=to=0
    {
        for(size_t y = from ; y >= to ; y--) //Recorremos todos los años entre el último y el primero.
        {
            size_t movies = getAmount(db, "movie", y);
            size_t series = getAmount(db, "tvSeries", y);
            if( movies!=0 || series!=0 ) //Si hubo algún título ese año
            {
                ///query1
                numToText(y, buffer[0]);
                numToText(movies, buffer[1]);
                numToText(series, buffer[2]);
                writeLine(query1, 3, buffer);


                ///query2
                toBeginGenre(db, y);
                size_t count;
                while(hasNext(db)){
                    count = next(db, buffer[1]);
                    numToText(count, buffer[2]);
                    writeLine(query2, 3, buffer);
                }


                ///query3
                float rating;
                size_t votes;
                //El año ya está en buffer[0]
                char * aux = getBest(db, "movie", y, &rating, &votes);
                if(aux!=NULL) {
                    strcpy(buffer[1], aux);
                    numToText(votes, buffer[2]);
                    floatToText(rating, buffer[3]);
                }
                else //Si no hubo películas ese año
                {
                    for(int i=1; i<=3; i++){
                        strcpy(buffer[i], "\\N");
                    }
                }
                free(aux);

                aux = getBest(db, "tvSeries", y, &rating, &votes);
                if(aux!=NULL) {
                    strcpy(buffer[4], aux);
                    numToText(votes, buffer[5]);
                    floatToText(rating, buffer[6]);
                }
                else //Si no hubo series ese año
                {
                    for(int i=4; i<=6; i++){
                        strcpy(buffer[i], "\\N");
                    }
                }
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
