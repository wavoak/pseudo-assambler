#ifndef outputh
#define outputh
#include "list.h"
#include "helpers.h"
#include <math.h>
/*list.h includes all of the imports*/

void dataToNums(char *data, int *Anum, int *Bnum, int *Cnum, int *Dnum, int *Enum);
void objCreate(char *filename, list *code, list *data, int ICF, int DCF);
void extCreate(char *filename, list *externals);
void entCreate(char *filename, list *labels);
#endif
