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

    char line[DIM][MAX_LONG];
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
            char* q1[3];
            q1[0] = numToText(y);
            q1[1] = numToText(movies);
            q1[2] = numToText(series);
            writeLine(query1, 3, q1);
            free(q1[0]);
            free(q1[1]);
            free(q1[2]);

            char* q3[7];
            q3[0] = numToText(y);
            float ratingM;
            float ratingS;
            size_t votesM;
            size_t votesS;
            q3[1] = getBest(db, "movie", y, &ratingM, &votesM);
            if(q3[1]==NULL)
            {
                q3[1] = copyString("\\N");
                q3[2] = copyString("\\N");
                q3[3] = copyString("\\N");
            }
            else
            {
                q3[2] = numToText(votesM);
                q3[3] = floatToText(ratingM);
            }
            q3[4] = getBest(db, "tvSeries", y, &ratingS, &votesS);
            if(q3[4]==NULL)
            {
                q3[4] = copyString("\\N");
                q3[5] = copyString("\\N");
                q3[6] = copyString("\\N");
            }
            else
            {
                q3[5] = numToText(votesS);
                q3[6] = floatToText(ratingS);
            }
            writeLine(query3, 7, q3);
            free(q3[0]);
            free(q3[1]);
            free(q3[2]);
            free(q3[3]);
            free(q3[4]);
            free(q3[5]);
            free(q3[6]);
        }
    }
    fclose(query1);
    fclose(query2);
    fclose(query3);
    freeImdb(db);
    return 0;
}
