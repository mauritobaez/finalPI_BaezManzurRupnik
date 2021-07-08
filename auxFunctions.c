#include "auxFunctions.h"
#include <stdlib.h>

#define BLOCK 10
char * copyString(const char * string){
    char * aux=NULL;
    int dim=0;
    while(*string != '\0'){
        if(dim%BLOCK == 0){
            aux = realloc(aux, dim + BLOCK);
            CHECK_ALLOC(aux);
        }
        aux[dim++] = *string;
        string++;
    }
    aux = realloc(aux, dim+1);
    CHECK_ALLOC(aux);
    aux[dim] = '\0';
    return aux;
}
