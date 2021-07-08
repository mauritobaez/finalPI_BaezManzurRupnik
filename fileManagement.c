#include "fileManagement.h"

int readLine(FILE* input, char output[DIM][MAX_LONG]){//Se lee una linea del archivo dado y se la tokeniza
    char line[MAX_LONG * DIM];//Se crea una vector estatico donde se guarda temporalmente la linea completa del archivo
    size_t i = 0;
    if(fgets(line,MAX_LONG * DIM,input) == NULL){//Si fgets devuelve NULL entonces termino el archivo(=EOF)
        return 0;
    }
    char * token;
    token = strtok(line,DELIM);//Se tokeniza la línea del archivo con el delimitador ";"
    while(token != NULL){
        strcpy(output[i],token);//Se copia cada token a la matriz que se da por parametro y se pasa al siguiente lugar
        i++;
        token = strtok(NULL,DELIM);//Se pasa al siguiente token
    }
    return 1;
}

void writeLine(FILE* output, size_t dim, char info[DIM][MAX_LONG]){//Se escribe una línea dada dentro del archivo dado
    int i;
    if(dim>DIM)
        return;
    for(i=0 ; i < dim-1 ; i++)
    {
        fprintf(output, "%s;", info[i]);//Se escribe cada parte separada con una ";" en el archivo
    }
    fprintf(output, "%s\n", info[i]);//Cómo es el último se añade un ENTER('\n')
}

void numToText(size_t num, char * string){//Se carga el entero dado al string que se recibe como parámetro
    sprintf(string, "%d", (int)num);
}

void floatToText(float num, char * ans){//Se carga el float dado al string que se recibe como parámetro
    sprintf(ans, "%.1f", num);
}
