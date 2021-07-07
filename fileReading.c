#include "fileReading.h"

char ** readLine(FILE* input,size_t dim){
    char line[MAX_LONG];
    size_t i = 0;
    if(fgets(line,MAX_LONG,input) == NULL){
        return NULL;
    }
    char * token;
    char ** output;
    output = calloc(dim,sizeof(char *));
    CHECK_ALLOC(output);
    token = strtok(line,DELIM);
    while(token != NULL){
        output[i] = copyString(token);
        i++;
        token = strtok(NULL,DELIM);
    }
    return output;
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

size_t textToNum(char * text){
    return (size_t)atoi(text);
}

float textToFloat(char *text){
    return (float)atof(text);
}