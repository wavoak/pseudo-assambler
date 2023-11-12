#include "preprocessor.h"

/*returns a pointer to the macro in the list if found,
or NULL if the macro doesn't exist*/
node* getMacro(list *macros, char *name){
	return find(macros,name);
}

/*puts macro in output file*/
void putMacro(node *macro, FILE *output){
	char *data = macro->data;
	char *copy = (char*)malloc(sizeof(char)*strlen(data));
	int i, len, endWhite = 0, startWhite = 0;
	strcpy(copy,data);
	len = strlen(copy);
	for(i=len-1;i>=0;i--)
		if(copy[i]==' '||copy[i]=='	'||copy[i]=='\n')
			endWhite++;
		else
			i = 0;
	for(i=0;i<len;i++)
		if(copy[i]==' '||copy[i]=='	'||copy[i]=='\n')
			startWhite++;
		else
			i = len;
	for(i=startWhite;i<len-endWhite;i++){
		fputc(copy[i],output);
		if(copy[i]=='\n')
			i++;
	}
}

/*the preprocessor core function, receives command line arguments and executes preprocessor procedure*/
void preprocessor(int argc, char **argv, char *names){
	int i;
	FILE *input = NULL;
	FILE *output = NULL;
	char *filename;
	char fileAMname[82];
	strcpy(fileAMname,"");
	for(i=1;i<argc;i++){
		filename = argv[i];
		input = open(filename,".as");
		if(input!=NULL){/*filename.as exists, built filename.am file*/
			output = create(filename,".am");
			char line[82];
			list *macros = (list*)malloc(sizeof(list));
			int macroFlag = False, defFlag = False;
			while(fgets(line,82,(FILE*)input)!=NULL){/*loop used to scan the input file line by line*/
				int j,len=strlen(line),wordCount=0;
				char *word = (char*)malloc(sizeof(char)*len+1),*startWhite=(char*)malloc(sizeof(char)*(len+1));
				for(j=0;j<len;j++){/*loop used to scan each line of the input file*/
					switch(line[j]){
						case ' ': case '	': case '\n':/*line[j] isn't part of a word, examine the created word*/
							if(strlen(word)!=0)/*word isnt empty: there were chars that aren't white chars*/
								wordCount++;
							if(defFlag==True){/*word is now the macro name*/
								if(checkIName(word,names)==False||isR(word)==True){/*macro name is illegal, terminate assambler*/
									printf("error: invalid macro name (%s) in file (%s)\n",word,filename);
									sprintf(fileAMname,"%s.am",filename);
									remove(fileAMname);
									return;
								}
								defFlag = False;
								updateKey(macros,word);
								j = len;
							}
							else if(macroFlag==True){/*word is data of a current macro*/
								if(strcmp(word,"endm")!=0){
									addData(macros,word);
									addData(macros,ctos(line[j]));
								}
								else{
									macroFlag = False;
									j = len;
								}
							}
							else if(strcmp(word,"macro")==0 && wordCount==1){/*macro init*/
								macroFlag = True;
								defFlag = True;
								insert(macros,"","");
							}
							else{/*word is not related to macro definition in any form*/
								if(wordCount==0)/*built the indentations for the output file*/
									strcat(startWhite,ctos(line[j]));
								else{
									if(wordCount==1){/*once 1st word is found the input indentation is inserted*/
										fputs(startWhite,output);
										wordCount++;/*corrupt wordCount to prevent reoccurences of this if*/
									}
									node *macro = getMacro(macros,word);
									if(macro!=NULL){/*word is a call for an existing macro*/
										putMacro(macro,output);
										fputc(line[j],output);
									}
									else{/*word is not related to macro in any form,insert to output without changes*/
										fputs(word,output);
										fputc(line[j],output);
									}
								}
							}
							word = (char*)malloc(sizeof(char)*(len+1));/*empty word param to allow storage of next word*/
							break;
						default:/*line[j] is part of a word, store to word and continue*/
							strcat(word,ctos(line[j]));
							break;
					}
				}
			}
		}
		else/*filename.as doesn't exist, give error message in terminal*/
			printf("error in preprocessor: %s.as is not found\n",filename);
	}
	/*closure of now unused file streams*/
	if(input!=NULL)
		fclose(input);
	if(output!=NULL)
		fclose(output);
}

