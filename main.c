#include <stdlib.h>
#include <stdio.h>
#include "imdbADT.h"
#include "fileReading.h"


int main(int argcount, char* args[])
{
    if(argcount != 2)
    {
        fprintf(stderr, "Error in amount of arguments expected. (one)");
        exit(1);
    }

    FILE * input;
    input = fopen(args[1],"rt");
    if(input==NULL){
        fprintf(stderr, "Error in path to file,must exist");
        exit(2);
    }

    char line[DIM][MAX_LONG] = {0};
    readLine(input, line); // leo la primera linea que es el encabezado
    int year;
    imdbADT db = newImdb();
    while(readLine(input, line)!=0){ //si el archivo está vacio? q pasa?
        year = atoi(line[2]);
        if(year > 0) { //si en el campo del año no hay un numero positivo, no lo tomamos en cuenta
            add(db, line[0], line[1], (size_t)year, line[4], (float)atof(line[5]), (size_t)atoi(line[6]));
        }
    }
    fclose(input);
    FILE * query1 = fopen("query1.csv","wt");
    FILE * query2 = fopen("query2.csv","wt");
    FILE * query3 = fopen("query3.csv","wt");

    char * titles[7] = {"year","films","series","ratingFilm","serie","votesSerie","ratingSerie"};
    writeLine(query1,3,titles);
    titles[1] = "genre";
    titles[2] = "films";
    writeLine(query2,3,titles);
    titles[0] = "startYear";
    titles[1] = "films";
    titles[2] = "votesFilm";
    writeLine(query3, 7, titles);

    size_t from = getLastYear(db);
    size_t to = getFirstYear(db);
    for(size_t y = from ; y >= to ; y--)
    {
        size_t movies = getAmount(db, "movie", y);
        size_t series = getAmount(db, "tvSeries", y);
        if( movies!=0 || series!=0 ) //si hay algo ese año
        {
            //query1
            char * vector[8] ={line[0], line[1], line[2], line[3], line[4],
                               line[5], line[6], line[7]};
            numToText(y, line[0]);
            numToText(movies, line[1]);
            numToText(series, line[2]);
            writeLine(query1, 3, vector);

            //query2
            toBeginGenre(db, y);
            size_t count;
            while(hasNext(db)){
                count = next(db, line[1]);
                numToText(count, line[2]);
                writeLine(query2, 3, vector);
            }

            //query3
            //char* q3[7]; Esto es lo que estaba antes
            //numToText(y, line[0]); Ya está puesto el año
            float ratingM;
            float ratingS;
            size_t votesM;
            size_t votesS;
            char * aux = getBest(db, "movie", y, &ratingM, &votesM);
            strcpy(line[1], aux);//podemos cambiarlo con la mejora de jamboard
            free(aux);
            if(line[1][0]=='\0')
            {
                for(int i=1; i<4; i++){
                    strcpy(line[i], "\\N");
                }
            }
            else
            {
                numToText(votesM, line[2]);
                floatToText(ratingM, line[3]);
            }
            aux = getBest(db, "tvSeries", y, &ratingS, &votesS);
            strcpy(line[4],aux);
            free(aux);
            if(line[4][0]=='\0')
            {
                for(int i=4; i<7; i++){
                    strcpy(line[i], "\\N");
                }
            }
            else
            {
                numToText(votesS, line[5]);
                floatToText(ratingS, line[6]);
            }
            writeLine(query3, 7, vector);
        }

    }
    fclose(query1);
    fclose(query2);
    fclose(query3);
    freeImdb(db);
    return 0;
}
