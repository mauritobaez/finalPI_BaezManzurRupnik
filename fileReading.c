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

char* numToText(size_t num){
    size_t aux = num;
    size_t n = 0;
    while(aux!=0)
    {
        aux /= 10;
        n++;
    }
    char* ans = malloc(n+2);
    CHECK_ALLOC(ans);
    sprintf(ans, "%d", (int)num);
    return ans;
}

char* floatToText(float num){
    int aux = (int)num;
    size_t n = 0;
    while(aux!=0)
    {
        aux /= 10;
        n++;
    }
    char* ans = malloc(n+4);
    CHECK_ALLOC(ans);
    sprintf(ans, "%.1f", num);
    return ans;
}
