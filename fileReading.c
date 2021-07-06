#include "fileReading.h"

char ** readLine(FILE* input,size_t dim){
    char line[MAX_LONG];
    size_t i=0;
    if(fgets(line,MAX_LONG,input)==NULL){
        return NULL;
    }
    char * token;
    char ** output;
    output=calloc(dim,sizeof(char *));
    token=strtok(line,DELIM);
    while(token!=NULL){
        output[i]=copyString(token);
        i++;
        token=strtok(NULL,DELIM);
    }
    return output;
}
size_t textToNum(char * text){
    return (size_t) atoi(text);
}
float textToFloat(char *text){
    return (float)atof(text);
}