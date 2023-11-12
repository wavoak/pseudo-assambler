#include "helpers.h"

/*converts char to string, returns the string*/
char* ctos(char c){
	char *s = (char*)malloc(sizeof(char));
	sprintf(s,"%c",c);
	return s;
}

/*returns False if name is found in names (all words in names must be delimited by ',')
or True otherwise*/
int checkIName(char *name, char *names){
	int nameErr = True, len = strlen(names), i;
	char *operator = (char*)malloc(sizeof(char)*5);
	strcpy(operator,"");
	for(i=0;i<len;i++){
		if(names[i]==','){
			if(strcmp(operator,name)==0)
				nameErr = False;
			operator = (char*)malloc(sizeof(char)*5);
			strcpy(operator,"");
		}
		else
			strcat(operator,ctos(names[i]));
	}
	return nameErr;
}

/*return True if word is r# when # is int between 0 and 15 inclusively
return False otherwise*/
int isR(char *word){
	char *reg = (char*)malloc(sizeof(char)*3);
	int i;
	for(i=0;i<16;i++){
		sprintf(reg,"r%d",i);
		if(strcmp(word,reg)==0)
			return True;
	}
	return False;
}

/*receives label data, gets base and offset and saves them in given cbase, coffset
(label data structure: "adress,base,offset,code/data/external,entry")*/
void getAddress(char *ldata, char **cbase, char **coffset){
	int i, field = 0, len = strlen(ldata);
	char *cnum = (char*)malloc(sizeof(char)*len);
	strcpy(cnum,"");
	for(i=0;i<len;i++){
		if(ldata[i]==','){
			field++;
			if(field==2)
				strcpy(*cbase,cnum);
			if(field==3)
				strcpy(*coffset,cnum);
			strcpy(cnum,"");
		}
		else
			strcat(cnum,ctos(ldata[i]));
	}
}

/*returns a pointer to the filename.as file if found,
or NULL if filename.as doesn't exist*/
FILE* open(char *filename, char *type){
	char *filepath = (char*)malloc(sizeof(char)*(strlen(filename)+1));
	FILE *f;
	strcpy(filepath,filename);
	strcat(filepath,type);
	f = fopen(filepath,"r");
	return f;
}

/*creates filename.type file and returns "a" format stream for the created file,
or NULL if filename.type creation has failed*/
FILE* create(char *filename, char *type){
	char *filepath = (char*)malloc(sizeof(char)*(strlen(filename)+1));
	FILE *f;
	strcpy(filepath,filename);
	strcat(filepath,type);
	f = fopen(filepath,"a");
	return f;
}
