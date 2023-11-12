#ifndef preprocessorh
#define preprocessorh
#include "list.h"
#include "helpers.h"
/*list.h includes all of the imports*/

node* getMacro(list *macros, char *name);
void putMacro(node *macro, FILE *output);
void preprocessor(int argc, char **argv, char *names);
#endif
