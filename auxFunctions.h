#ifndef TP_FINAL_PI_AUXFUNCTIONS_H
#define TP_FINAL_PI_AUXFUNCTIONS_H

#include <stdio.h>
#include <errno.h>
#define BLOCK 10

#define CHECK_ALLOC(x) {    if((x)==NULL || errno==ENOMEM)\
                            {  fprintf(stderr, "Error. No more memory available.");\
                                exit(3);\
                            }\
                        }
char * copyString(const char * string);


#endif //TP_FINAL_PI_AUXFUNCTIONS_H
