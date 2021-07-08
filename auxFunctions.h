#ifndef TP_FINAL_PI_AUXFUNCTIONS_H
#define TP_FINAL_PI_AUXFUNCTIONS_H

#include <stdio.h>
#define BLOCK 10

#define CHECK_ALLOC(x) {    if((x)==NULL)\
                            {  fprintf(stderr, "Error. No more memory available.");\
                                exit(3);\
                            }\
                        }
char * copyString(const char * string);


#endif //TP_FINAL_PI_AUXFUNCTIONS_H