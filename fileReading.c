#include "fileReading.h"

int readLine(FILE* input, char output[DIM][MAX_LONG]){
    char line[MAX_LONG * DIM];
    size_t i = 0;
    if(fgets(line,MAX_LONG,input) == NULL){
        return 0;
    }
    char * token;
    token = strtok(line,DELIM);
    while(token != NULL){
        strcpy(output[i],token);
        i++;
        token = strtok(NULL,DELIM);
    }
    return 1;
}

void writeLine(FILE* output, size_t dim, char** info)
{
    int i;
    for(i=0 ; i < dim-1 ; i++)
    {
        fprintf(output, "%s;", info[i]);
    }
    fprintf(output, "%s\n", info[i]);
}
/*
int textToNum(char * text){
    return atoi(text);
}

float textToFloat(char *text){
    return (float)atof(text);
}
*/ //estan al pedo¿
void numToText(size_t num, char * string){
    sprintf(string, "%d", (int)num);
}

void floatToText(float num, char * ans){
    sprintf(ans, "%.1f", num);
}
