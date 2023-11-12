#ifndef helpersh
#define helpersh
#include "list.h"
/*list.h includes all of the imports*/

char* ctos(char c);
int checkIName(char *name, char *names);
int isR(char *word);
void getAddress(char *ldata, char **cbase, char **coffset);
FILE* open(char *filename, char *type);
FILE* create(char *filename, char *type);
#endif
