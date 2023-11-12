#ifndef pass1and2h
#define pass1and2h
#include "list.h"
#include "helpers.h"
/*list.h includes all of the imports*/

int decToBin(int dec);
int unsign(int negative, int flag);
int numLen(int num);
char* numToCode(char *cnum);
char* charToCode(int c);
char* regToCode(char *reg);
char* operand00ToCode(char *operand);
char* makeInfo(char *type, int IC);
int checkLName(char *word);
void create2operands(int row, int len, int *skipFlag, char *operands, char **operand1, char **operand2);
void create1operand(int row, int len, int *skipFlag, char *operands, char **operand1);
char* operandType(char *operand, int *L, int row, char *names);
void addICF(list **labels, int ICF);
int pass1(char *filename, list **code, list **data, list **labels, char *names, int *ICF, int *DCF);
int pass2(char *filename, list **code, list **data, list **labels, list **externals, char *names);
#endif
