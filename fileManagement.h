#ifndef TP_FINAL_PI_FILEREADING_H
#define TP_FINAL_PI_FILEREADING_H
#define MAX_LONG 128    //Máxima longitud en caracteres de cada campo
#define DELIM ";"       //Delimitador entre campos del archivo csv
#define DIM 8           //Cantidad de campos del archivo csv por renglón
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Se lee una linea del archivo dado y se la tokeniza, devolviendo sus datos en la matriz indicada por parámtro
int readLine(FILE* input, char line[DIM][MAX_LONG]);
//Se escribe una línea dentro del archivo dado con los datos que haya en la matriz indicada por parámetro
void writeLine(FILE* output, size_t dim, char info[DIM][MAX_LONG]);
//Se carga el entero dado al string que se recibe como parámetro
void numToText(size_t num, char * string);
//Se carga el float dado al string que se recibe como parámetro
void floatToText(float num, char * ans);

#endif //TP_FINAL_PI_FILEREADING_H
