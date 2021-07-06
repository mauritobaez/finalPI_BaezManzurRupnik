#include "fileReading.h"
char ** readLine(FILE* input,size_t dim){
    char line[MAX_LONG];
    size_t i=0;
    fgets(line,MAX_LONG,input);
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
    size_t num=atoi(text);
    return num;
}
