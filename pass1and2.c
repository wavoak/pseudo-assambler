#include "pass1and2.h"

/*receives decimal int
returns binary int*/
int decToBin(int dec){
	if(dec==1||dec==-1||dec==0)
		return dec;
	return (dec%2) + (10*decToBin(dec/2));
}

/*receives negative binary int
returns unsigned (2's compliment) binary int, or -1 if error occured*/
int unsign(int negative, int flag){
	if(negative==0)
		return 0;
	if(flag==False){
		if(negative%10==-1)
			return 1 + 10*unsign(-1*negative/10,True);
		if(negative%10==0)
			return 10*unsign(negative/10,False);
	}
	else{
		if(negative%10==0)
			return 1 + 10*unsign(negative/10,True);
		if(negative%10==1)
			return 10*unsign(negative/10,True);
	}
	return -1;
}

/*returns len of num*/
int numLen(int num){
	if(num!=0)
		return 1 + numLen(num/10);
	return 0;
}

/*receives num as string
returns binary num as string in an unsigned %016 format*/
char* numToCode(char *cnum){
	char *word = (char*)malloc(sizeof(char)*17);
	int inum = atoi(cnum), bnum = decToBin(inum), len;
	if(bnum<0){/*binary form is signed, unsign and handle*/
		len = numLen(bnum);
		bnum = unsign(bnum, False);
		sprintf(word, "%016d", bnum);
		memset(word,'1',16-len);
		
	}
	else
		sprintf(word, "%016d", bnum);
	return word;
}

/*receives char
returns binary ascii as string in an unsigned %016 format*/
char* charToCode(int c){
	char *word = (char*)malloc(sizeof(char)*17);
	int bchar = decToBin(c);/*binary form of the ascii*/
	sprintf(word, "%016d", bchar);
	return word;
}

/*receives register as string
returns binary reg num as string in an unsigned %04 format*/
char* regToCode(char *reg){
	int len = strlen(reg), i, inum, bnum;
	char *word = (char*)malloc(sizeof(char)*5);
	char *cnum = (char*)malloc(sizeof(char)*3);
	for(i=1;i<len;i++)
		strcat(cnum,ctos(reg[i]));
	inum = atoi(cnum);
	bnum = decToBin(inum);
	sprintf(word, "%04d", bnum);
	return word;
}

/*receives 00 addressed operand as string (#int)
returns binary addressed num as string in an unsigned %016 format*/
char* operand00ToCode(char *operand){
	int len = strlen(operand), i, inum, bnum;
	char *word = (char*)malloc(sizeof(char)*17);
	char *cnum = (char*)malloc(sizeof(char)*100);
	for(i=1;i<len;i++)/*remove #*/
		strcat(cnum,ctos(operand[i]));
	inum = atoi(cnum);
	bnum = decToBin(inum);
	sprintf(word, "%016d", bnum);
	return word;
}

/*receives label info and converts it into a string in the format: "address,base,offset,type"*/
char* makeInfo(char *type, int IC){
	int i=0, base, offset;
	
	char *info = (char*)malloc(sizeof(char)*100);
	char *Value = (char*)malloc(sizeof(char)*100);
	char *Base = (char*)malloc(sizeof(char)*100);
	char *Offset = (char*)malloc(sizeof(char)*100);
	
	while(i<=IC)
		i+=16;
	i-=16;
	base = i;
	offset = IC - i;
	
	sprintf(Value,"%d",IC);
	sprintf(Base,"%d",base);
	sprintf(Offset,"%d",offset);
	strcpy(info,Value);
	strcat(info,",");
	strcat(info,Base);
	strcat(info,",");
	strcat(info,Offset);
	strcat(info,",");
	strcat(info,type);
	return info;
}

/*returns True if word is a legal label name, False otherwise*/
int checkLName(char *word){
	int len = strlen(word), i;
	for(i=0;i<len;i++)
		if(!((word[i]>='a'&&word[i]<='z')||(word[i]>='A'&&word[i]<='Z')||(word[i]>='0'&&word[i]<='9')))
			return False;
	return True;
}

/*create 2 operands form operands string
give a message and mark the error if such occures*/
void create2operands(int row, int len, int *skipFlag, char *operands, char **operand1, char **operand2){
	char *operand = (char*)malloc(sizeof(char)*len);
	int i;
	strcpy(operand,"");
	strcpy(*operand1," ");/*mark operand1 as unfilled*/
	strcpy(*operand2," ");/*mark operand2 as unfilled*/
	for(i=0;i<len;i++){
		if(operands[i]==','){/*field ended*/
			if(strcmp(*operand1," ")==0){/*operand is operand1*/
				*operand1 = (char*)malloc(sizeof(char)*strlen(operand));
				strcpy(*operand1,operand);
			}
			else if(strcmp(*operand2," ")==0){/*operand is operand2*/
				*operand2 = (char*)malloc(sizeof(char)*strlen(operand));
				strcpy(*operand2,operand);
			}
			else{/*more than 2 fields detacted*/
				printf("error in row %d: operator accepts 2 operands\n",row);
				i = len;
				*skipFlag = True;
			}
			operand = (char*)malloc(sizeof(char)*len);
			strcpy(operand,"");
		}
		else
			strcat(operand,ctos(operands[i]));
	}
	if(*skipFlag==False&&(strcmp(*operand1," ")==0||strcmp(*operand2," ")==0||strcmp(*operand1,"")==0||strcmp(*operand2,"")==0)){/*less than
	2 fields detacted*/
		printf("error in row %d: operator accepts 2 operands\n",row);
		*skipFlag = True;
	}
}

/*create 1 operand1 form operands string
give a message and mark the error if such occures
(based on create2operands)*/
void create1operand(int row, int len, int *skipFlag, char *operands, char **operand1){
	char *operand = (char*)malloc(sizeof(char)*len);
	int i;
	strcpy(*operand1," ");
	for(i=0;i<len;i++){
		if(operands[i]==','){
			if(strcmp(*operand1," ")==0){
				*operand1 = (char*)malloc(sizeof(char)*strlen(operand));
				strcpy(*operand1,operand);
			}
			else{
				printf("error in row %d: operator accepts 1 operand\n",row);
				i = len;
				*skipFlag = True;
			}
			operand = (char*)malloc(sizeof(char)*len);
		}
		else
			strcat(operand,ctos(operands[i]));
	}
	if(*skipFlag==False&&(strcmp(*operand1," ")==0||strcmp(*operand1,"")==0)){
		printf("error in row %d: operator accepts 1 operand\n",row);
		*skipFlag = True;
	}
}

/*return the operand's addressing method as binary word
handle L counter and return "-1" if an error occures*/
char* operandType(char *operand, int *L, int row, char *names){
	char *type = (char*)malloc(sizeof(char)*3);
	int i, len = strlen(operand);
	char *label = (char*)malloc(sizeof(char)*81);/*for method 10*/
	char *reg = (char*)malloc(sizeof(char)*81);/*for method 10*/
	char *help = (char*)malloc(sizeof(char)*81);/*for method 10*/
	strcpy(type,"-1");
	if(operand[0]=='#'){/*addressing method is 00 (#int)*/
		strcpy(type,"00");
		if(operand[1]!='-'&&(operand[1]<'0'||operand[1]>'9'))
			strcpy(type,"-1");
		for(i=2;i<len;i++)
			if(operand[i]<'0'||operand[i]>'9')
				strcpy(type,"-1");
		if(strcmp(type,"-1")==0)
			printf("error in row %d: # accepts int\n",row);
		*L = (*L) + 1;
	}
	else if(checkLName(operand)==True&&len<=31&&checkIName(operand,names)==True&&isR(operand)==False){/*addressing method is 01 (label)*/
		strcpy(type,"01");
		*L = (*L) + 2;
	}
	else if(isR(operand)==True)/*addressing method is 11 (r0-15)*/
		strcpy(type,"11");
	else{
		for(i=0;i<len;i++){
			if(operand[i]=='['){
				strcpy(label,help);
				help = (char*)malloc(sizeof(char)*81);
			}
			else if(operand[i]==']'){
				strcpy(reg,help);
				help = (char*)malloc(sizeof(char)*81);
			}
			else
				strcat(help,ctos(operand[i]));
		}
		if(checkLName(label)==True&&strlen(label)<=31&&checkIName(label,names)==True&&isR(reg))/*addressing method is 10 (label[r0-15])*/
			strcpy(type,"10");
		if(strcmp(type,"10")==0)
			*L = (*L) + 2;
	}
	return type;
}

/*adds ICF to addresses of all data labels in labels*/
void addICF(list **labels, int ICF){
	/*update the labels list to have the correct addresses stored*/
	int iaddress = 0, i, len;
	node *ptr = getHead(*labels);
	char *info = (char*)malloc(sizeof(char)*100);
	char *caddress = (char*)malloc(sizeof(char)*100);
	char *type = (char*)malloc(sizeof(char)*5);
	while(ptr!=NULL){
		info = (char*)malloc(sizeof(char)*100);
		caddress = (char*)malloc(sizeof(char)*100);
		type = (char*)malloc(sizeof(char)*5);
		iaddress = 0;
		strcpy(info,getData(ptr));
		len = strlen(info);
		for(i=0;i<4;i++){
			strcat(type,ctos(info[len-4+i]));
		}
		if(strcmp(type,"data")==0){
			for(i=0;i<len;i++){
				if(info[i]==','){
					iaddress = atoi(caddress);
					i = len;
				}
				else
					strcat(caddress,ctos(info[i]));
			}
			iaddress += ICF;
			strcpy(info,makeInfo("data",iaddress));
			updateData(ptr,info);
		}
		ptr = getNext(ptr);
	}
}

/*core function of the first pass.
return False if no errors occured, True otherwise*/
int pass1(char *filename, list **code, list **data, list **labels, char *names, int *ICF, int *DCF){
	int errFlag = False;
	FILE *input = NULL;
	input = open(filename,".am");
	if(input==NULL){
		printf("error: file %s.am failed to open\n",filename);
		errFlag = True;
	}
	else{
		int IC = 100, DC = 0, row = 0, binRow = 100, len;
		int fieldCount, labelFlag, skipFlag, noteFlag;
		char *binIC = (char*)malloc(sizeof(char)*100);/*IC alter counter used purely for saving with the binary code format*/
		char line[81];
		const char *delim = "	 \n";
		char *label = (char*)malloc(sizeof(char)*32);
		char *token = (char*)malloc(sizeof(char)*81);/*used to iterate through the file lines delimited by white spaces*/
		char type[8];/*used to hold type funct (.data/.string/.entry/.external)*/
		while(fgets(line,81,(FILE*)input)!=NULL){
			row++;
			fieldCount = 0;
			labelFlag = False;
			skipFlag = False;
			noteFlag = False;
			token = (char*)malloc(sizeof(char)*81);
			strcpy(type,"");
			token = strtok(line,delim);
			while(token!=NULL){/*iterate through line*/
				fieldCount++;
				if(token[0]==';')/*the rest of the line is a note, skip to next line*/
					noteFlag = True;
				else if(token[strlen(token)-1]==':'){/*token is a potential label init*/
					len = strlen(token);
					label = (char*)malloc(sizeof(char)*32);
					strcpy(label,"");
					strncat(label,token,len-1);
					if(fieldCount==1){/*token is a valid label init, mark label init flag*/
						if(checkLName(label)==True&&checkIName(label,names)==True&&isR(label)==False&&len!=1)
							labelFlag = True;
						else{/*label name invalid: handle error*/
							printf("error in row %d: invalid label name (%s)\n",row,label);
							skipFlag = True;
							errFlag = True;
						}
					}
					else{/*label init not in first field: handle error*/
						printf("error in row %d: invalid label definition (%s)\n",row,label);
						skipFlag = True;
						errFlag = True;
					}
				}
				else if(strcmp(token,".data")==0 || strcmp(token,".string")==0){/*.data/.string function, save function type*/
					strcpy(type,token);
					if(labelFlag==True){/*handle label init*/
						char *info = makeInfo("data",DC);
						insert(*labels,label,info);
						labelFlag = False;
					}
				}
				else if(strcmp(token,".entry")==0 || strcmp(token,".extern")==0){/*.entry/.extern function, save function type*/
					strcpy(type,token);
				}
				else if(strcmp(type,".data")==0){/*handle token data*/
					char *operands = (char*)malloc(sizeof(char)*81);
					strcpy(operands,"");
					while(token!=NULL){/*the rest of this line is operands for .data*/
						strcat(operands,token);
						token = strtok(NULL,delim);
					}
					strcat(operands,",");/*dummy for treatment of last operand*/
					int i, len = strlen(operands);
					char *operand = (char*)malloc(sizeof(char)*len);
					strcpy(operand,"");
					for(i=0;i<len;i++){
						if(operands[i]==','){/*operand now holds an operand*/
							if(strcspn(operand,"-0123456789")!=0||strcmp(operand,"")==0){/*operand is not a decimal number,
							handle error*/
								printf("error in row %d: invalid data (%s)\n",row,operand);
								errFlag = True;
							}
							else{/*operand is a decimal number, store in data*/
								char word[21] = "0100";
								strcat(word,numToCode(operand));
								insert(*data,operand,word);
								DC++;
							}
							strcpy(operand,"");
						}
						else
							strcat(operand,ctos(operands[i]));
					}
				}
				else if(strcmp(type,".string")==0){/*handle token data*/
					char *operand = (char*)malloc(sizeof(char)*81);
					strcpy(operand,"");
					while(token!=NULL){/*the rest of this line is operands for .string*/
						strcat(operand,token);
						token = strtok(NULL,delim);
					}
					int i,len=strlen(operand);
					if(operand[0]=='"'&&operand[len-1]=='"'){/*operand is a legal string, store in data*/
						for(i=1;i<len-1;i++){
							char word[21] = "0100";
							strcat(word,charToCode(operand[i]));
							insert(*data,ctos(operand[i]),word);
							DC++;
						}
						char word[21] = "0100";
						strcat(word,charToCode(0));
						insert(*data,"0",word);
						DC++;
					}
					else{/*operand isnt a legal string, handle error*/
						printf("error in row %d: invalid data (%s)\n",row,operand);
						errFlag = True;
					}
				}
				else if(strcmp(type,".entry")==0){/*handle .entry lines in pass2*/
					skipFlag = True;
				}
				else if(strcmp(type,".extern")==0){/*handle external label*/
					char *info = makeInfo("external",0);
					if(checkLName(token)==True&&checkIName(token,names)==True&&isR(token)==False)
						insert(*labels,token,info);
					else{
						printf("error in row %d: invalid label name (%s)\n",row,token);
						errFlag = True;
					}
				}
				else{/*this is a instruction code line*/
					if(labelFlag==True){/*handle label init*/
						char *info = makeInfo("code",IC);
						insert(*labels,label,info);
						labelFlag = False;
						fieldCount = 1;
					}
					if(fieldCount==1&&checkIName(token,names)==True){/*token is an invalid operator name*/
						printf("error in row %d: operator (%s) doesn't exist\n",row,token);
						skipFlag = True;
						errFlag = True;
					}
					else{/*handle encoding and compilation of function and operands. most cases are handled alike*/
						int L = 0;
						if(strcmp(token,"mov")==0){/*function is mov*/
							L = 2;
							char *operands = (char*)malloc(sizeof(char)*81);
							strcpy(operands,"");
							token = strtok(NULL,delim);
							while(token!=NULL){/*the rest of this line is operands for mov*/
								strcat(operands,token);
								token = strtok(NULL,delim);
							}
							strcat(operands,",");/*dummy for treatment of last operand*/
							int len = strlen(operands);
							char *operand1 = (char*)malloc(sizeof(char)*len);
							char *operand2 = (char*)malloc(sizeof(char)*len);
							create2operands(row,len,&skipFlag,operands,&operand1,&operand2);/*stores the operands*/
							if(skipFlag==False){/*the 2 operands were stored successfully*/
								char type1[3];
								char type2[3];
								strcpy(type1,operandType(operand1,&L,row,names));/*operand1's addressing methos*/
								strcpy(type2,operandType(operand2,&L,row,names));/*operand2's addressing methos*/
								if(strcmp(type1,"-1")==0){/*addressing is unrecognized*/
									printf("error in row %d: invalid addressing syntax in (%s)\n",row,operand1);
									errFlag = True;
								}
								if(strcmp(type2,"-1")==0){/*addressing is unrecognized*/
									printf("error in row %d: invalid addressing syntax in (%s)\n",row,operand2);
									errFlag = True;
								}
								char *word1 = (char*)malloc(sizeof(char)*21);/*1st code word of mov*/
								char *word2 = (char*)malloc(sizeof(char)*21);/*2nd code word of mov*/
								strcpy(word1,"01000000000000000001");
								strcpy(word2,"01000000");
								/*build mov word 2 with encoding of operand1's and operand2's addressing methods*/
								if(strcmp(type1,"10")==0){
									int i, oplen = strlen(operand1), regFlag = False;
									char *reg = (char*)malloc(sizeof(char)*oplen+1);
									strcpy(reg,"");
									for(i=0;i<oplen;i++){
										if(operand1[i]=='[')
											regFlag = True;
										else if(operand1[i]==']')
											regFlag = False;
										else if(regFlag==True)
											strcat(reg,ctos(operand1[i]));
									}
									strcat(word2,regToCode(reg));
								}
								else if(strcmp(type1,"11")==0)
									strcat(word2,regToCode(operand1));
								else
									strcat(word2,"0000");
								strcat(word2,type1);
								if(strcmp(type2,"10")==0){
									int i, oplen = strlen(operand2), regFlag = False;
									char *reg = (char*)malloc(sizeof(char)*oplen+1);
									strcpy(reg,"");
									for(i=0;i<oplen;i++){
										if(operand2[i]=='[')
											regFlag = True;
										else if(operand2[i]==']')
											regFlag = False;
										else if(regFlag==True)
											strcat(reg,ctos(operand2[i]));
									}
									strcat(word2,regToCode(reg));
								}
								else if(strcmp(type2,"11")==0)
									strcat(word2,regToCode(operand2));
								else
									strcat(word2,"0000");
								strcat(word2,type2);
								sprintf(binIC,"%d",binRow);
								insert(*code,binIC,word1);/*store completed mov word1 in code*/
								binRow++;
								sprintf(binIC,"%d",binRow);
								insert(*code,binIC,word2);/*store completed mov word2 in code*/
								binRow++;
								/*create any additional code words required by the addressing methods*/
								if(strcmp(type1,"00")==0){
									char *opWord = (char*)malloc(sizeof(char)*21);
									strcat(opWord,"0100");
									strcat(opWord,operand00ToCode(operand1));
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
								}
								if(strcmp(type1,"01")==0||strcmp(type1,"10")==0){
									char *opWord = (char*)malloc(sizeof(char)*21);
									strcpy(opWord,"????????????????????");
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
								}
								if(strcmp(type2,"00")==0){
									printf("error in row %d: invalid addressing (%s)\n",row,operand2);
									errFlag = True;
								}
								if(strcmp(type2,"01")==0||strcmp(type2,"10")==0){
									char *opWord = (char*)malloc(sizeof(char)*21);
									strcpy(opWord,"????????????????????");
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
								}
							}
							else/*error occured while storing operands, mark error*/
								errFlag = True;
						}
						else if(strcmp(token,"cmp")==0){
							L = 2;
							char *operands = (char*)malloc(sizeof(char)*81);
							strcpy(operands,"");
							token = strtok(NULL,delim);
							while(token!=NULL){/*the rest of this line is operands for cmp*/
								strcat(operands,token);
								token = strtok(NULL,delim);
							}
							strcat(operands,",");/*dummy for treatment of last operand*/
							int len = strlen(operands);
							char *operand1 = (char*)malloc(sizeof(char)*len+1);
							char *operand2 = (char*)malloc(sizeof(char)*len+1);
							create2operands(row,len,&skipFlag,operands,&operand1,&operand2);
							if(skipFlag==False){
								char type1[3];
								char type2[3];
								strcpy(type1,operandType(operand1,&L,row,names));
								strcpy(type2,operandType(operand2,&L,row,names));
								if(strcmp(type1,"-1")==0){
									printf("error in row %d: invalid addressing syntax in (%s)\n",row,operand1);
									errFlag = True;
								}
								if(strcmp(type2,"-1")==0){
									printf("error in row %d: invalid addressing syntax in (%s)\n",row,operand2);
									errFlag = True;
								}
								char *word1 = (char*)malloc(sizeof(char)*21);
								char *word2 = (char*)malloc(sizeof(char)*21);
								strcpy(word1,"01000000000000000010");
								strcpy(word2,"01000000");
								if(strcmp(type1,"10")==0){
									int i, oplen = strlen(operand1), regFlag = False;
									char *reg = (char*)malloc(sizeof(char)*oplen+1);
									strcpy(reg,"");
									for(i=0;i<oplen;i++){
										if(operand1[i]=='[')
											regFlag = True;
										else if(operand1[i]==']')
											regFlag = False;
										else if(regFlag==True)
											strcat(reg,ctos(operand1[i]));
									}
									strcat(word2,regToCode(reg));
								}
								else if(strcmp(type1,"11")==0)
									strcat(word2,regToCode(operand1));
								else
									strcat(word2,"0000");
								strcat(word2,type1);
								if(strcmp(type2,"10")==0){
									int i, oplen = strlen(operand2), regFlag = False;
									char *reg = (char*)malloc(sizeof(char)*oplen+1);
									strcpy(reg,"");
									for(i=0;i<oplen;i++){
										if(operand2[i]=='[')
											regFlag = True;
										else if(operand2[i]==']')
											regFlag = False;
										else if(regFlag==True)
											strcat(reg,ctos(operand2[i]));
									}
									strcat(word2,regToCode(reg));
								}
								else if(strcmp(type2,"11")==0)
									strcat(word2,regToCode(operand2));
								else
									strcat(word2,"0000");
								strcat(word2,type2);
								sprintf(binIC,"%d",binRow);
								insert(*code,binIC,word1);
								binRow++;
								sprintf(binIC,"%d",binRow);
								insert(*code,binIC,word2);
								binRow++;
								if(strcmp(type1,"00")==0){
									char *opWord = (char*)malloc(sizeof(char)*21);
									strcat(opWord,"0100");
									strcat(opWord,operand00ToCode(operand1));
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
								}
								if(strcmp(type1,"01")==0||strcmp(type1,"10")==0){
									char *opWord = (char*)malloc(sizeof(char)*21);
									strcpy(opWord,"????????????????????");
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
								}
								if(strcmp(type2,"00")==0){
									char *opWord = (char*)malloc(sizeof(char)*21);
									strcat(opWord,"0100");
									strcat(opWord,operand00ToCode(operand1));
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
								}
								if(strcmp(type2,"01")==0||strcmp(type2,"10")==0){
									char *opWord = (char*)malloc(sizeof(char)*21);
									strcpy(opWord,"????????????????????");
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
								}
							}
							else
								errFlag = True;
						}
						else if(strcmp(token,"add")==0){
							L = 2;
							char *operands = (char*)malloc(sizeof(char)*81);
							strcpy(operands,"");
							token = strtok(NULL,delim);
							while(token!=NULL){/*the rest of this line is operands for add*/
								strcat(operands,token);
								token = strtok(NULL,delim);
							}
							strcat(operands,",");/*dummy for treatment of last operand*/
							int len = strlen(operands);
							char *operand1 = (char*)malloc(sizeof(char)*len);
							char *operand2 = (char*)malloc(sizeof(char)*len);
							create2operands(row,len,&skipFlag,operands,&operand1,&operand2);
							if(skipFlag==False){
								char type1[3];
								char type2[3];
								strcpy(type1,operandType(operand1,&L,row,names));
								strcpy(type2,operandType(operand2,&L,row,names));
								if(strcmp(type1,"-1")==0){
									printf("error in row %d: invalid addressing syntax in (%s)\n",row,operand1);
									errFlag = True;
								}
								if(strcmp(type2,"-1")==0){
									printf("error in row %d: invalid addressing syntax in (%s)\n",row,operand2);
									errFlag = True;
								}
								char *word1 = (char*)malloc(sizeof(char)*21);
								char *word2 = (char*)malloc(sizeof(char)*21);
								strcpy(word1,"01000000000000000100");
								strcpy(word2,"01001010");
								if(strcmp(type1,"10")==0){
									int i, oplen = strlen(operand1), regFlag = False;
									char *reg = (char*)malloc(sizeof(char)*oplen+1);
									strcpy(reg,"");
									for(i=0;i<oplen;i++){
										if(operand1[i]=='[')
											regFlag = True;
										else if(operand1[i]==']')
											regFlag = False;
										else if(regFlag==True)
											strcat(reg,ctos(operand1[i]));
									}
									strcat(word2,regToCode(reg));
								}
								else if(strcmp(type1,"11")==0)
									strcat(word2,regToCode(operand1));
								else
									strcat(word2,"0000");
								strcat(word2,type1);
								if(strcmp(type2,"10")==0){
									int i, oplen = strlen(operand2), regFlag = False;
									char *reg = (char*)malloc(sizeof(char)*oplen+1);
									strcpy(reg,"");
									for(i=0;i<oplen;i++){
										if(operand2[i]=='[')
											regFlag = True;
										else if(operand2[i]==']')
											regFlag = False;
										else if(regFlag==True)
											strcat(reg,ctos(operand2[i]));
									}
									strcat(word2,regToCode(reg));
								}
								else if(strcmp(type2,"11")==0)
									strcat(word2,regToCode(operand2));
								else
									strcat(word2,"0000");
								strcat(word2,type2);
								sprintf(binIC,"%d",binRow);
								insert(*code,binIC,word1);
								binRow++;
								sprintf(binIC,"%d",binRow);
								insert(*code,binIC,word2);
								binRow++;
								if(strcmp(type1,"00")==0){
									char *opWord = (char*)malloc(sizeof(char)*21);
									strcat(opWord,"0100");
									strcat(opWord,operand00ToCode(operand1));
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
								}
								if(strcmp(type1,"01")==0||strcmp(type1,"10")==0){
									char *opWord = (char*)malloc(sizeof(char)*21);
									strcpy(opWord,"????????????????????");
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
								}
								if(strcmp(type2,"00")==0){
									printf("error in row %d: invalid addressing (%s)\n",row,operand2);
									errFlag = True;
								}
								if(strcmp(type2,"01")==0||strcmp(type2,"10")==0){
									char *opWord = (char*)malloc(sizeof(char)*21);
									strcpy(opWord,"????????????????????");
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
								}
							}
							else
								errFlag = True;
						}
						else if(strcmp(token,"sub")==0){
							L = 2;
							char *operands = (char*)malloc(sizeof(char)*81);
							strcpy(operands,"");
							token = strtok(NULL,delim);
							while(token!=NULL){/*the rest of this line is operands for sub*/
								strcat(operands,token);
								token = strtok(NULL,delim);
							}
							strcat(operands,",");/*dummy for treatment of last operand*/
							int len = strlen(operands);
							char *operand1 = (char*)malloc(sizeof(char)*len);
							char *operand2 = (char*)malloc(sizeof(char)*len);
							create2operands(row,len,&skipFlag,operands,&operand1,&operand2);
							if(skipFlag==False){
								char type1[3];
								char type2[3];
								strcpy(type1,operandType(operand1,&L,row,names));
								strcpy(type2,operandType(operand2,&L,row,names));
								if(strcmp(type1,"-1")==0){
									printf("error in row %d: invalid addressing syntax in (%s)\n",row,operand1);
									errFlag = True;
								}
								if(strcmp(type2,"-1")==0){
									printf("error in row %d: invalid addressing syntax in (%s)\n",row,operand2);
									errFlag = True;
								}
								char *word1 = (char*)malloc(sizeof(char)*21);
								char *word2 = (char*)malloc(sizeof(char)*21);
								strcpy(word1,"01000000000000000100");
								strcpy(word2,"01001011");
								if(strcmp(type1,"10")==0){
									int i, oplen = strlen(operand1), regFlag = False;
									char *reg = (char*)malloc(sizeof(char)*oplen+1);
									strcpy(reg,"");
									for(i=0;i<oplen;i++){
										if(operand1[i]=='[')
											regFlag = True;
										else if(operand1[i]==']')
											regFlag = False;
										else if(regFlag==True)
											strcat(reg,ctos(operand1[i]));
									}
									strcat(word2,regToCode(reg));
								}
								else if(strcmp(type1,"11")==0)
									strcat(word2,regToCode(operand1));
								else
									strcat(word2,"0000");
								strcat(word2,type1);
								if(strcmp(type2,"10")==0){
									int i, oplen = strlen(operand2), regFlag = False;
									char *reg = (char*)malloc(sizeof(char)*oplen+1);
									strcpy(reg,"");
									for(i=0;i<oplen;i++){
										if(operand2[i]=='[')
											regFlag = True;
										else if(operand2[i]==']')
											regFlag = False;
										else if(regFlag==True)
											strcat(reg,ctos(operand2[i]));
									}
									strcat(word2,regToCode(reg));
								}
								else if(strcmp(type2,"11")==0)
									strcat(word2,regToCode(operand2));
								else
									strcat(word2,"0000");
								strcat(word2,type2);
								sprintf(binIC,"%d",binRow);
								insert(*code,binIC,word1);
								binRow++;
								sprintf(binIC,"%d",binRow);
								insert(*code,binIC,word2);
								binRow++;
								if(strcmp(type1,"00")==0){
									char *opWord = (char*)malloc(sizeof(char)*21);
									strcat(opWord,"0100");
									strcat(opWord,operand00ToCode(operand1));
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
								}
								if(strcmp(type1,"01")==0||strcmp(type1,"10")==0){
									char *opWord = (char*)malloc(sizeof(char)*21);
									strcpy(opWord,"????????????????????");
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
								}
								if(strcmp(type2,"00")==0){
									printf("error in row %d: invalid addressing (%s)\n",row,operand2);
									errFlag = True;
								}
								if(strcmp(type2,"01")==0||strcmp(type2,"10")==0){
									char *opWord = (char*)malloc(sizeof(char)*21);
									strcpy(opWord,"????????????????????");
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
								}
							}
							else
								errFlag = True;
						}
						else if(strcmp(token,"lea")==0){
							L = 2;
							char *operands = (char*)malloc(sizeof(char)*81);
							strcpy(operands,"");
							token = strtok(NULL,delim);
							while(token!=NULL){/*the rest of this line is operands for lea*/
								strcat(operands,token);
								token = strtok(NULL,delim);
							}
							strcat(operands,",");/*dummy for treatment of last operand*/
							int len = strlen(operands);
							char *operand1 = (char*)malloc(sizeof(char)*len);
							char *operand2 = (char*)malloc(sizeof(char)*len);
							create2operands(row,len,&skipFlag,operands,&operand1,&operand2);
							if(skipFlag==False){
								char type1[3];
								char type2[3];
								strcpy(type1,operandType(operand1,&L,row,names));
								strcpy(type2,operandType(operand2,&L,row,names));
								if(strcmp(type1,"-1")==0){
									printf("error in row %d: invalid addressing syntax in (%s)\n",row,operand1);
									errFlag = True;
								}
								if(strcmp(type2,"-1")==0){
									printf("error in row %d: invalid addressing syntax in (%s)\n",row,operand2);
									errFlag = True;
								}
								char *word1 = (char*)malloc(sizeof(char)*21);
								char *word2 = (char*)malloc(sizeof(char)*21);
								strcpy(word1,"01000000000000010000");
								strcpy(word2,"01000000");
								if(strcmp(type1,"10")==0){
									int i, oplen = strlen(operand1), regFlag = False;
									char *reg = (char*)malloc(sizeof(char)*oplen+1);
									strcpy(reg,"");
									for(i=0;i<oplen;i++){
										if(operand1[i]=='[')
											regFlag = True;
										else if(operand1[i]==']')
											regFlag = False;
										else if(regFlag==True)
											strcat(reg,ctos(operand1[i]));
									}
									strcat(word2,regToCode(reg));
								}
								else if(strcmp(type1,"11")==0){
									printf("error in row %d: invalid addressing (%s)\n",row,operand1);
									errFlag = True;
								}
								else
									strcat(word2,"0000");
								strcat(word2,type1);
								if(strcmp(type2,"10")==0){
									int i, oplen = strlen(operand2), regFlag = False;
									char *reg = (char*)malloc(sizeof(char)*oplen+1);
									strcpy(reg,"");
									for(i=0;i<oplen;i++){
										if(operand2[i]=='[')
											regFlag = True;
										else if(operand2[i]==']')
											regFlag = False;
										else if(regFlag==True)
											strcat(reg,ctos(operand2[i]));
									}
									strcat(word2,regToCode(reg));
								}
								else if(strcmp(type2,"11")==0)
									strcat(word2,regToCode(operand2));
								else
									strcat(word2,"0000");
								strcat(word2,type2);
								sprintf(binIC,"%d",binRow);
								insert(*code,binIC,word1);
								binRow++;
								sprintf(binIC,"%d",binRow);
								insert(*code,binIC,word2);
								binRow++;
								if(strcmp(type1,"00")==0){
									printf("error in row %d: invalid addressing (%s)\n",row,operand1);
									errFlag = True;
								}
								if(strcmp(type1,"01")==0||strcmp(type1,"10")==0){
									char *opWord = (char*)malloc(sizeof(char)*21);
									strcpy(opWord,"????????????????????");
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
								}
								if(strcmp(type2,"00")==0){
									printf("error in row %d: invalid addressing (%s)\n",row,operand2);
									errFlag = True;
								}
								if(strcmp(type2,"01")==0||strcmp(type2,"10")==0){
									char *opWord = (char*)malloc(sizeof(char)*21);
									strcpy(opWord,"????????????????????");
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
								}
							}
							else
								errFlag = True;
						}
						else if(strcmp(token,"clr")==0){
							L = 2;
							char *operands = (char*)malloc(sizeof(char)*81);
							strcpy(operands,"");
							token = strtok(NULL,delim);
							while(token!=NULL){/*the rest of this line is operands for clr*/
								strcat(operands,token);
								token = strtok(NULL,delim);
							}
							strcat(operands,",");/*dummy for treatment of last operand*/
							int len = strlen(operands);
							char *operand1 = (char*)malloc(sizeof(char)*len);
							create1operand(row,len,&skipFlag,operands,&operand1);
							if(skipFlag==False){
								char type1[3];
								strcpy(type1,operandType(operand1,&L,row,names));
								if(strcmp(type1,"-1")==0){
									printf("error in row %d: invalid addressing syntax in (%s)\n",row,operand1);
									errFlag = True;
								}
								char *word1 = (char*)malloc(sizeof(char)*21);
								char *word2 = (char*)malloc(sizeof(char)*21);
								strcpy(word1,"01000000000000100000");
								strcpy(word2,"01001010000000");
								if(strcmp(type1,"10")==0){
									int i, oplen = strlen(operand1), regFlag = False;
									char *reg = (char*)malloc(sizeof(char)*oplen+1);
									strcpy(reg,"");
									for(i=0;i<oplen;i++){
										if(operand1[i]=='[')
											regFlag = True;
										else if(operand1[i]==']')
											regFlag = False;
										else if(regFlag==True)
											strcat(reg,ctos(operand1[i]));
									}
									strcat(word2,regToCode(reg));
								}
								else if(strcmp(type1,"11")==0)
									strcat(word2,regToCode(operand1));
								else
									strcat(word2,"0000");
								strcat(word2,type1);
								sprintf(binIC,"%d",binRow);
								insert(*code,binIC,word1);
								binRow++;
								sprintf(binIC,"%d",binRow);
								insert(*code,binIC,word2);
								binRow++;
								if(strcmp(type1,"00")==0){
									printf("error in row %d: invalid addressing (%s)\n",row,operand1);
									errFlag = True;
								}
								if(strcmp(type1,"01")==0||strcmp(type1,"10")==0){
									char *opWord = (char*)malloc(sizeof(char)*21);
									strcpy(opWord,"????????????????????");
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
								}
							}
							else
								errFlag = True;
						}
						else if(strcmp(token,"not")==0){
							L = 2;
							char *operands = (char*)malloc(sizeof(char)*81);
							strcpy(operands,"");
							token = strtok(NULL,delim);
							while(token!=NULL){/*the rest of this line is operands for not*/
								strcat(operands,token);
								token = strtok(NULL,delim);
							}
							strcat(operands,",");/*dummy for treatment of last operand*/
							int len = strlen(operands);
							char *operand1 = (char*)malloc(sizeof(char)*len);
							create1operand(row,len,&skipFlag,operands,&operand1);
							if(skipFlag==False){
								char type1[3];
								strcpy(type1,operandType(operand1,&L,row,names));
								if(strcmp(type1,"-1")==0){
									printf("error in row %d: invalid addressing syntax in (%s)\n",row,operand1);
									errFlag = True;
								}
								char *word1 = (char*)malloc(sizeof(char)*21);
								char *word2 = (char*)malloc(sizeof(char)*21);
								strcpy(word1,"01000000000000100000");
								strcpy(word2,"01001011000000");
								if(strcmp(type1,"10")==0){
									int i, oplen = strlen(operand1), regFlag = False;
									char *reg = (char*)malloc(sizeof(char)*oplen+1);
									strcpy(reg,"");
									for(i=0;i<oplen;i++){
										if(operand1[i]=='[')
											regFlag = True;
										else if(operand1[i]==']')
											regFlag = False;
										else if(regFlag==True)
											strcat(reg,ctos(operand1[i]));
									}
									strcat(word2,regToCode(reg));
								}
								else if(strcmp(type1,"11")==0)
									strcat(word2,regToCode(operand1));
								else
									strcat(word2,"0000");
								strcat(word2,type1);
								sprintf(binIC,"%d",binRow);
								insert(*code,binIC,word1);
								binRow++;
								sprintf(binIC,"%d",binRow);
								insert(*code,binIC,word2);
								binRow++;
								if(strcmp(type1,"00")==0){
									printf("error in row %d: invalid addressing (%s)\n",row,operand1);
									errFlag = True;
								}
								if(strcmp(type1,"01")==0||strcmp(type1,"10")==0){
									char *opWord = (char*)malloc(sizeof(char)*21);
									strcpy(opWord,"????????????????????");
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
								}
							}
							else
								errFlag = True;
						}
						else if(strcmp(token,"inc")==0){
							L = 2;
							char *operands = (char*)malloc(sizeof(char)*81);
							strcpy(operands,"");
							token = strtok(NULL,delim);
							while(token!=NULL){/*the rest of this line is operands for inc*/
								strcat(operands,token);
								token = strtok(NULL,delim);
							}
							strcat(operands,",");/*dummy for treatment of last operand*/
							int len = strlen(operands);
							char *operand1 = (char*)malloc(sizeof(char)*len);
							create1operand(row,len,&skipFlag,operands,&operand1);
							if(skipFlag==False){
								char type1[3];
								strcpy(type1,operandType(operand1,&L,row,names));
								if(strcmp(type1,"-1")==0){
									printf("error in row %d: invalid addressing syntax in (%s)\n",row,operand1);
									errFlag = True;
								}
								char *word1 = (char*)malloc(sizeof(char)*21);
								char *word2 = (char*)malloc(sizeof(char)*21);
								strcpy(word1,"01000000000000100000");
								strcpy(word2,"01001100000000");
								if(strcmp(type1,"10")==0){
									int i, oplen = strlen(operand1), regFlag = False;
									char *reg = (char*)malloc(sizeof(char)*oplen+1);
									strcpy(reg,"");
									for(i=0;i<oplen;i++){
										if(operand1[i]=='[')
											regFlag = True;
										else if(operand1[i]==']')
											regFlag = False;
										else if(regFlag==True)
											strcat(reg,ctos(operand1[i]));
									}
									strcat(word2,regToCode(reg));
								}
								else if(strcmp(type1,"11")==0)
									strcat(word2,regToCode(operand1));
								else
									strcat(word2,"0000");
								strcat(word2,type1);
								sprintf(binIC,"%d",binRow);
								insert(*code,binIC,word1);
								binRow++;
								sprintf(binIC,"%d",binRow);
								insert(*code,binIC,word2);
								binRow++;
								if(strcmp(type1,"00")==0){
									printf("error in row %d: invalid addressing (%s)\n",row,operand1);
									errFlag = True;
								}
								if(strcmp(type1,"01")==0||strcmp(type1,"10")==0){
									char *opWord = (char*)malloc(sizeof(char)*21);
									strcpy(opWord,"????????????????????");
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
								}
							}
							else
								errFlag = True;
						}
						else if(strcmp(token,"dec")==0){
							L = 2;
							char *operands = (char*)malloc(sizeof(char)*81);
							strcpy(operands,"");
							token = strtok(NULL,delim);
							while(token!=NULL){/*the rest of this line is operands for dec*/
								strcat(operands,token);
								token = strtok(NULL,delim);
							}
							strcat(operands,",");/*dummy for treatment of last operand*/
							int len = strlen(operands);
							char *operand1 = (char*)malloc(sizeof(char)*len);
							create1operand(row,len,&skipFlag,operands,&operand1);
							if(skipFlag==False){
								char type1[3];
								strcpy(type1,operandType(operand1,&L,row,names));
								if(strcmp(type1,"-1")==0){
									printf("error in row %d: invalid addressing syntax in (%s)\n",row,operand1);
									errFlag = True;
								}
								char *word1 = (char*)malloc(sizeof(char)*21);
								char *word2 = (char*)malloc(sizeof(char)*21);
								strcpy(word1,"01000000000000100000");
								strcpy(word2,"01001101000000");
								if(strcmp(type1,"10")==0){
									int i, oplen = strlen(operand1), regFlag = False;
									char *reg = (char*)malloc(sizeof(char)*oplen+1);
									strcpy(reg,"");
									for(i=0;i<oplen;i++){
										if(operand1[i]=='[')
											regFlag = True;
										else if(operand1[i]==']')
											regFlag = False;
										else if(regFlag==True)
											strcat(reg,ctos(operand1[i]));
									}
									strcat(word2,regToCode(reg));
								}
								else if(strcmp(type1,"11")==0)
									strcat(word2,regToCode(operand1));
								else
									strcat(word2,"0000");
								strcat(word2,type1);
								sprintf(binIC,"%d",binRow);
								insert(*code,binIC,word1);
								binRow++;
								sprintf(binIC,"%d",binRow);
								insert(*code,binIC,word2);
								binRow++;
								if(strcmp(type1,"00")==0){
									printf("error in row %d: invalid addressing (%s)\n",row,operand1);
									errFlag = True;
								}
								if(strcmp(type1,"01")==0||strcmp(type1,"10")==0){
									char *opWord = (char*)malloc(sizeof(char)*21);
									strcpy(opWord,"????????????????????");
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
								}
							}
							else
								errFlag = True;
						}
						else if(strcmp(token,"jmp")==0){
							L = 2;
							char *operands = (char*)malloc(sizeof(char)*81);
							strcpy(operands,"");
							token = strtok(NULL,delim);
							while(token!=NULL){/*the rest of this line is operands for jmp*/
								strcat(operands,token);
								token = strtok(NULL,delim);
							}
							strcat(operands,",");/*dummy for treatment of last operand*/
							int len = strlen(operands);
							char *operand1 = (char*)malloc(sizeof(char)*len);
							create1operand(row,len,&skipFlag,operands,&operand1);
							if(skipFlag==False){
								char type1[3];
								strcpy(type1,operandType(operand1,&L,row,names));
								if(strcmp(type1,"-1")==0){
									printf("error in row %d: invalid addressing syntax in (%s)\n",row,operand1);
									errFlag = True;
								}
								char *word1 = (char*)malloc(sizeof(char)*21);
								char *word2 = (char*)malloc(sizeof(char)*21);
								strcpy(word1,"01000000001000000000");
								strcpy(word2,"01001010000000");
								if(strcmp(type1,"10")==0){
									int i, oplen = strlen(operand1), regFlag = False;
									char *reg = (char*)malloc(sizeof(char)*oplen+1);
									strcpy(reg,"");
									for(i=0;i<oplen;i++){
										if(operand1[i]=='[')
											regFlag = True;
										else if(operand1[i]==']')
											regFlag = False;
										else if(regFlag==True)
											strcat(reg,ctos(operand1[i]));
									}
									strcat(word2,regToCode(reg));
								}
								else if(strcmp(type1,"11")==0){
									printf("error in row %d: invalid addressing (%s)\n",row,operand1);
									errFlag = True;
								}
								else
									strcat(word2,"0000");
								strcat(word2,type1);
								sprintf(binIC,"%d",binRow);
								insert(*code,binIC,word1);
								binRow++;
								sprintf(binIC,"%d",binRow);
								insert(*code,binIC,word2);
								binRow++;
								if(strcmp(type1,"00")==0){
									printf("error in row %d: invalid addressing (%s)\n",row,operand1);
									errFlag = True;
								}
								if(strcmp(type1,"01")==0||strcmp(type1,"10")==0){
									char *opWord = (char*)malloc(sizeof(char)*21);
									strcpy(opWord,"????????????????????");
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
								}
							}
							else
								errFlag = True;
						}
						else if(strcmp(token,"bne")==0){
							L = 2;
							char *operands = (char*)malloc(sizeof(char)*81);
							strcpy(operands,"");
							token = strtok(NULL,delim);
							while(token!=NULL){/*the rest of this line is operands for bne*/
								strcat(operands,token);
								token = strtok(NULL,delim);
							}
							strcat(operands,",");/*dummy for treatment of last operand*/
							int len = strlen(operands);
							char *operand1 = (char*)malloc(sizeof(char)*len);
							create1operand(row,len,&skipFlag,operands,&operand1);
							if(skipFlag==False){
								char type1[3];
								strcpy(type1,operandType(operand1,&L,row,names));
								if(strcmp(type1,"-1")==0){
									printf("error in row %d: invalid addressing syntax in (%s)\n",row,operand1);
									errFlag = True;
								}
								char *word1 = (char*)malloc(sizeof(char)*21);
								char *word2 = (char*)malloc(sizeof(char)*21);
								strcpy(word1,"01000000001000000000");
								strcpy(word2,"01001011000000");
								if(strcmp(type1,"10")==0){
									int i, oplen = strlen(operand1), regFlag = False;
									char *reg = (char*)malloc(sizeof(char)*oplen+1);
									strcpy(reg,"");
									for(i=0;i<oplen;i++){
										if(operand1[i]=='[')
											regFlag = True;
										else if(operand1[i]==']')
											regFlag = False;
										else if(regFlag==True)
											strcat(reg,ctos(operand1[i]));
									}
									strcat(word2,regToCode(reg));
								}
								else if(strcmp(type1,"11")==0){
									printf("error in row %d: invalid addressing (%s)\n",row,operand1);
									errFlag = True;
								}
								else
									strcat(word2,"0000");
								strcat(word2,type1);
								sprintf(binIC,"%d",binRow);
								insert(*code,binIC,word1);
								binRow++;
								sprintf(binIC,"%d",binRow);
								insert(*code,binIC,word2);
								binRow++;
								if(strcmp(type1,"00")==0){
									printf("error in row %d: invalid addressing (%s)\n",row,operand1);
									errFlag = True;
								}
								if(strcmp(type1,"01")==0||strcmp(type1,"10")==0){
									char *opWord = (char*)malloc(sizeof(char)*21);
									strcpy(opWord,"????????????????????");
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
								}
							}
							else
								errFlag = True;
						}
						else if(strcmp(token,"jsr")==0){
							L = 2;
							char *operands = (char*)malloc(sizeof(char)*81);
							strcpy(operands,"");
							token = strtok(NULL,delim);
							while(token!=NULL){/*the rest of this line is operands for jsr*/
								strcat(operands,token);
								token = strtok(NULL,delim);
							}
							strcat(operands,",");/*dummy for treatment of last operand*/
							int len = strlen(operands);
							char *operand1 = (char*)malloc(sizeof(char)*len);
							create1operand(row,len,&skipFlag,operands,&operand1);
							if(skipFlag==False){
								char type1[3];
								strcpy(type1,operandType(operand1,&L,row,names));
								if(strcmp(type1,"-1")==0){
									printf("error in row %d: invalid addressing syntax in (%s)\n",row,operand1);
									errFlag = True;
								}
								char *word1 = (char*)malloc(sizeof(char)*21);
								char *word2 = (char*)malloc(sizeof(char)*21);
								strcpy(word1,"01000000001000000000");
								strcpy(word2,"01001100000000");
								if(strcmp(type1,"10")==0){
									int i, oplen = strlen(operand1), regFlag = False;
									char *reg = (char*)malloc(sizeof(char)*oplen+1);
									strcpy(reg,"");
									for(i=0;i<oplen;i++){
										if(operand1[i]=='[')
											regFlag = True;
										else if(operand1[i]==']')
											regFlag = False;
										else if(regFlag==True)
											strcat(reg,ctos(operand1[i]));
									}
									strcat(word2,regToCode(reg));
								}
								else if(strcmp(type1,"11")==0){
									printf("error in row %d: invalid addressing (%s)\n",row,operand1);
									errFlag = True;
								}
								else
									strcat(word2,"0000");
								strcat(word2,type1);
								sprintf(binIC,"%d",binRow);
								insert(*code,binIC,word1);
								binRow++;
								sprintf(binIC,"%d",binRow);
								insert(*code,binIC,word2);
								binRow++;
								if(strcmp(type1,"00")==0){
									printf("error in row %d: invalid addressing (%s)\n",row,operand1);
									errFlag = True;
								}
								if(strcmp(type1,"01")==0||strcmp(type1,"10")==0){
									char *opWord = (char*)malloc(sizeof(char)*21);
									strcpy(opWord,"????????????????????");
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
								}
							}
							else
								errFlag = True;
						}
						else if(strcmp(token,"red")==0){
							L = 2;
							char *operands = (char*)malloc(sizeof(char)*81);
							strcpy(operands,"");
							token = strtok(NULL,delim);
							while(token!=NULL){/*the rest of this line is operands for red*/
								strcat(operands,token);
								token = strtok(NULL,delim);
							}
							strcat(operands,",");/*dummy for treatment of last operand*/
							int len = strlen(operands);
							char *operand1 = (char*)malloc(sizeof(char)*len);
							create1operand(row,len,&skipFlag,operands,&operand1);
							if(skipFlag==False){
								char type1[3];
								strcpy(type1,operandType(operand1,&L,row,names));
								if(strcmp(type1,"-1")==0){
									printf("error in row %d: invalid addressing syntax in (%s)\n",row,operand1);
									errFlag = True;
								}
								char *word1 = (char*)malloc(sizeof(char)*21);
								char *word2 = (char*)malloc(sizeof(char)*21);
								strcpy(word1,"01000001000000000000");
								strcpy(word2,"01000000000000");
								if(strcmp(type1,"10")==0){
									int i, oplen = strlen(operand1), regFlag = False;
									char *reg = (char*)malloc(sizeof(char)*oplen+1);
									strcpy(reg,"");
									for(i=0;i<oplen;i++){
										if(operand1[i]=='[')
											regFlag = True;
										else if(operand1[i]==']')
											regFlag = False;
										else if(regFlag==True)
											strcat(reg,ctos(operand1[i]));
									}
									strcat(word2,regToCode(reg));
								}
								else if(strcmp(type1,"11")==0)
									strcat(word2,regToCode(operand1));
								else
									strcat(word2,"0000");
								strcat(word2,type1);
								sprintf(binIC,"%d",binRow);
								insert(*code,binIC,word1);
								binRow++;
								sprintf(binIC,"%d",binRow);
								insert(*code,binIC,word2);
								binRow++;
								if(strcmp(type1,"00")==0){
									printf("error in row %d: invalid addressing (%s)\n",row,operand1);
									errFlag = True;
								}
								if(strcmp(type1,"01")==0||strcmp(type1,"10")==0){
									char *opWord = (char*)malloc(sizeof(char)*21);
									strcpy(opWord,"????????????????????");
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
								}
							}
							else
								errFlag = True;
						}
						else if(strcmp(token,"prn")==0){
							L = 2;
							char *operands = (char*)malloc(sizeof(char)*81);
							strcpy(operands,"");
							token = strtok(NULL,delim);
							while(token!=NULL){/*the rest of this line is operands for prn*/
								strcat(operands,token);
								token = strtok(NULL,delim);
							}
							strcat(operands,",");/*dummy for treatment of last operand*/
							int len = strlen(operands);
							char *operand1 = (char*)malloc(sizeof(char)*len);
							create1operand(row,len,&skipFlag,operands,&operand1);
							if(skipFlag==False){
								char type1[3];
								strcpy(type1,operandType(operand1,&L,row,names));
								if(strcmp(type1,"-1")==0){
									printf("error in row %d: invalid addressing syntax in (%s)\n",row,operand1);
									errFlag = True;
								}
								char *word1 = (char*)malloc(sizeof(char)*21);
								char *word2 = (char*)malloc(sizeof(char)*21);
								strcpy(word1,"01000010000000000000");
								strcpy(word2,"01000000000000");
								if(strcmp(type1,"10")==0){
									int i, oplen = strlen(operand1), regFlag = False;
									char *reg = (char*)malloc(sizeof(char)*oplen+1);
									strcpy(reg,"");
									for(i=0;i<oplen;i++){
										if(operand1[i]=='[')
											regFlag = True;
										else if(operand1[i]==']')
											regFlag = False;
										else if(regFlag==True)
											strcat(reg,ctos(operand1[i]));
									}
									strcat(word2,regToCode(reg));
								}
								else if(strcmp(type1,"11")==0)
									strcat(word2,regToCode(operand1));
								else
									strcat(word2,"0000");
								strcat(word2,type1);
								sprintf(binIC,"%d",binRow);
								insert(*code,binIC,word1);
								binRow++;
								sprintf(binIC,"%d",binRow);
								insert(*code,binIC,word2);
								binRow++;
								if(strcmp(type1,"00")==0){
									char *opWord = (char*)malloc(sizeof(char)*21);
									strcat(opWord,"0100");
									strcat(opWord,operand00ToCode(operand1));
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
								}
								if(strcmp(type1,"01")==0||strcmp(type1,"10")==0){
									char *opWord = (char*)malloc(sizeof(char)*21);
									strcpy(opWord,"????????????????????");
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
									sprintf(binIC,"%d",binRow);
									insert(*code,binIC,opWord);
									binRow++;
								}
							}
							else
								errFlag = True;
						}
						else if(strcmp(token,"rts")==0){
							L = 1;
							char *operands = (char*)malloc(sizeof(char)*81);
							strcpy(operands,"");
							token = strtok(NULL,delim);
							while(token!=NULL){/*the rest of this line is illegal operands*/
								strcat(operands,token);
								token = strtok(NULL,delim);
							}
							if(strlen(operands)!=0){
								printf("error in row %d: operator (rts) accepts 0 operands\n",row);
								skipFlag = True;
								errFlag = True;
							}
							if(skipFlag==False){
								sprintf(binIC,"%d",binRow);
								insert(*code,binIC,"01000100000000000000");
								binRow++;
							}
						}
						else if(strcmp(token,"stop")==0){
							L = 1;
							char *operands = (char*)malloc(sizeof(char)*81);
							strcpy(operands,"");
							token = strtok(NULL,delim);
							while(token!=NULL){/*the rest of this line is illegal operands*/
								strcat(operands,token);
								token = strtok(NULL,delim);
							}
							if(strlen(operands)!=0){
								printf("error in row %d: operator (stop) accepts 0 operands\n",row);
								skipFlag = True;
								errFlag = True;
							}
							if(skipFlag==False){
								sprintf(binIC,"%d",binRow);
								insert(*code,binIC,"01001000000000000000");
								binRow++;
							}
						}
						IC += L;
					}
				}
				if(noteFlag==True||skipFlag==True)/*skip the rest of line*/
					token = NULL;
				else{
					token = strtok(NULL,delim);
				}
			}
		}
		if(errFlag==False){/*no errors occured in pass1, handle ICF,DCF,labels*/
			*ICF = IC;
			*DCF = DC;
			addICF(labels,*ICF);/*update all data labels to be stored at the end of the code*/
		}
	}
	return errFlag;
}

/*core function of the second pass.
return False if no errors occured, True otherwise*/
int pass2(char *filename, list **code, list **data, list **labels, list **externals, char *names){
	int errFlag = False, row, fieldCount, skipFlag, noteFlag;
	char line[81];
	char *token = (char*)malloc(sizeof(char)*81);
	const char *delim = "	 \n";
	FILE *input = NULL;
	input = open(filename,".am");
	if(input==NULL)
		printf("error: file %s.am failed to open\n",filename);
	else{
		row = 0;
		while(fgets(line,81,(FILE*)input)!=NULL){
			row++;
			fieldCount = 0;
			skipFlag = False;
			noteFlag = False;
			token = (char*)malloc(sizeof(char)*81);
			token = strtok(line,delim);
			while(token!=NULL){
				fieldCount++;
				if(token[0]==';')/*the rest of line is a note, skip to next line*/
					noteFlag = True;
				else if(token[strlen(token)-1]==':'&&fieldCount==1)/*skip label inits*/
					fieldCount--;/*fieldCount skips labels*/
				else if(strcmp(token,".data")==0||strcmp(token,".string")==0||strcmp(token,".extern")==0){/*skip .data/.string/.extern*/
					skipFlag = True;
				}
				else if(strcmp(token,".entry")==0){/*handle .entry function*/
					char *operands = (char*)malloc(sizeof(char)*81);
					token = strtok(NULL,delim);
					while(token!=NULL){/*the rest of this line is operands for mov*/
						strcat(operands,token);
						token = strtok(NULL,delim);
					}
					strcat(operands,",");/*dummy for treatment of last operand*/
					int len = strlen(operands);
					char *operand1 = (char*)malloc(sizeof(char)*len);
					create1operand(row,len,&skipFlag,operands,&operand1);/*stores the operands*/
					if(skipFlag==False){/*operand1 was stored successfully*/
						node *label = find(*labels,operand1);/*search operand1 in labels*/
						if(label!=NULL)/*label is operand1 label node*/
							addDataTo(label,",entry");/*update operand1*/
						else{/*operand1 isn't a label, handle error*/
							printf("error in row %d: label (%s) doesn't exist\n",row,operand1);
							errFlag = True;
						}
					}/*an error occured while handling operand1, mark error*/
					else
						errFlag = True;
				}
				else{/*handle label operands in instruction code lines*/
					if(strcmp(token,"mov")==0||strcmp(token,"cmp")==0||strcmp(token,"add")==0||strcmp(token,"sub")==0||strcmp(token,"lea")==0){
						char *operands = (char*)malloc(sizeof(char)*81);
						token = strtok(NULL,delim);
						while(token!=NULL){/*the rest of this line is operands for mov*/
							strcat(operands,token);
							token = strtok(NULL,delim);
						}
						strcat(operands,",");/*dummy for treatment of last operand*/
						int len = strlen(operands);
						char *operand1 = (char*)malloc(sizeof(char)*len);
						char *operand2 = (char*)malloc(sizeof(char)*len);
						create2operands(row,len,&skipFlag,operands,&operand1,&operand2);/*stores the operands*/
						if(skipFlag==False){/*operand1 and operand2 were stored successfully*/
							char type1[3];
							char type2[3];
							strcpy(type1,"");
							strcpy(type2,"");
							int temp = 0;
							strcpy(type1,operandType(operand1,&temp,row,names));
							strcpy(type2,operandType(operand2,&temp,row,names));
							if(strcmp(type1,"01")==0){/*operand1 should be a label*/
								node *label = find(*labels,operand1);
								if(label!=NULL){/*label is operand1 in labels*/
									char *word1 = (char*)malloc(sizeof(char)*21);
									char *word2 = (char*)malloc(sizeof(char)*21);
									char *ldata = getData(label);
									if(strstr(ldata,"external")==NULL){/*operand1 isn't external*/
										strcpy(word1,"0010");
										strcpy(word2,"0010");
									}
									else{/*operand1 is external, store in externals*/
										strcpy(word1,"0001");
										strcpy(word2,"0001");
										char *IC = getKey(findData(*code,"????????????????????"));
										insert(*externals,operand1,IC);
										temp = atoi(IC);
										temp++;
										sprintf(IC,"%d",temp);
										insert(*externals,operand1,IC);
									}
									char *cbase = (char*)malloc(sizeof(char)*len);
									char *coffset = (char*)malloc(sizeof(char)*len);
									getAddress(ldata,&cbase,&coffset);/*retreive base and offset from label's data*/
									strcat(word1,numToCode(cbase));
									strcat(word2,numToCode(coffset));
									updateData(findData(*code,"????????????????????"),word1);/*store operand1 base*/
									updateData(findData(*code,"????????????????????"),word2);/*store operand1 offset*/
								}
								else{/*operand1 isn't a label, handle error*/
									printf("error in row %d: label (%s) doesn't exist\n",row,operand1);
									errFlag = True;
								}
							}
							if(strcmp(type1,"10")==0){/*same as method 01 after retreiving the label from operand1*/
								len = strlen(operand1);
								char *labelName = (char*)malloc(sizeof(char)*len);
								strcpy(labelName,"");
								int i;
								for(i=0;i<len;i++){/*retreive label from operand1, store in labelName*/
									if(operand1[i]=='[')
										i = len;
									else
										strcat(labelName,ctos(operand1[i]));
								}
								node *label = find(*labels,labelName);
								if(label!=NULL){
									char *word1 = (char*)malloc(sizeof(char)*21);
									char *word2 = (char*)malloc(sizeof(char)*21);
									char *ldata = getData(label);
									if(strstr(ldata,"external")==NULL){
										strcpy(word1,"0010");
										strcpy(word2,"0010");
									}
									else{
										strcpy(word1,"0001");
										strcpy(word2,"0001");
										char *IC = getKey(findData(*code,"????????????????????"));
										insert(*externals,operand1,IC);
										temp = atoi(IC);
										temp++;
										sprintf(IC,"%d",temp);
										insert(*externals,operand1,IC);
									}
									char *cbase = (char*)malloc(sizeof(char)*len);
									char *coffset = (char*)malloc(sizeof(char)*len);
									getAddress(ldata,&cbase,&coffset);/*retreive base and offset from label's data*/
									strcat(word1,numToCode(cbase));
									strcat(word2,numToCode(coffset));
									updateData(findData(*code,"????????????????????"),word1);
									updateData(findData(*code,"????????????????????"),word2);
								}
								else{
									printf("error in row %d: label (%s) doesn't exist\n",row,labelName);
									errFlag = True;
								}
							}
							if(strcmp(type2,"01")==0){
								node *label = find(*labels,operand2);
								if(label!=NULL){
									char *word1 = (char*)malloc(sizeof(char)*21);
									char *word2 = (char*)malloc(sizeof(char)*21);
									char *ldata = getData(label);
									if(strstr(ldata,"external")==NULL){
										strcpy(word1,"0010");
										strcpy(word2,"0010");
									}
									else{
										strcpy(word1,"0001");
										strcpy(word2,"0001");
										char *IC = getKey(findData(*code,"????????????????????"));
										/*the key (address) of the next 2 unfilled code is the required IC base and
										offset*/
										insert(*externals,operand2,IC);
										temp = atoi(IC);
										temp++;
										sprintf(IC,"%d",temp);
										insert(*externals,operand2,IC);
									}
									char *cbase = (char*)malloc(sizeof(char)*len);
									char *coffset = (char*)malloc(sizeof(char)*len);
									getAddress(ldata,&cbase,&coffset);/*retreive base and offset from label's data*/
									strcat(word1,numToCode(cbase));
									strcat(word2,numToCode(coffset));
									updateData(findData(*code,"????????????????????"),word1);
									updateData(findData(*code,"????????????????????"),word2);
								}
								else{
									printf("error in row %d: label (%s) doesn't exist\n",row,operand2);
									errFlag = True;
								}
							}
							if(strcmp(type2,"10")==0){
								len = strlen(operand2);
								char *labelName = (char*)malloc(sizeof(char)*len);
								strcpy(labelName,"");
								int i;
								for(i=0;i<len;i++){
									if(operand2[i]=='[')
										i = len;
									else
										strcat(labelName,ctos(operand2[i]));
								}
								node *label = find(*labels,labelName);
								if(label!=NULL){
									char *word1 = (char*)malloc(sizeof(char)*21);
									char *word2 = (char*)malloc(sizeof(char)*21);
									char *ldata = getData(label);
									if(strstr(ldata,"external")==NULL){
										strcpy(word1,"0010");
										strcpy(word2,"0010");
									}
									else{
										strcpy(word1,"0001");
										strcpy(word2,"0001");
										char *IC = getKey(findData(*code,"????????????????????"));
										/*the key (address) of the next 2 unfilled code is the required IC base and
										offset*/
										insert(*externals,operand2,IC);
										temp = atoi(IC);
										temp++;
										sprintf(IC,"%d",temp);
										insert(*externals,operand2,IC);
									}
									char *cbase = (char*)malloc(sizeof(char)*len);
									char *coffset = (char*)malloc(sizeof(char)*len);
									getAddress(ldata,&cbase,&coffset);/*retreive base and offset from label's data*/
									strcat(word1,numToCode(cbase));
									strcat(word2,numToCode(coffset));
									updateData(findData(*code,"????????????????????"),word1);
									updateData(findData(*code,"????????????????????"),word2);
								}
								else{
									printf("error in row %d: label (%s) doesn't exist\n",row,labelName);
									errFlag = True;
								}
							}
						}
						else
							errFlag = True;
					}
					else if(strcmp(token,"clr")==0||strcmp(token,"not")==0||strcmp(token,"inc")==0||strcmp(token,"dec")==0||strcmp(token,"jmp")==0||strcmp(token,"bne")==0||strcmp(token,"jsr")==0||strcmp(token,"red")==0||strcmp(token,"prn")==0){
						char *operands = (char*)malloc(sizeof(char)*81);
						token = strtok(NULL,delim);
						while(token!=NULL){/*the rest of this line is operands for mov*/
							strcat(operands,token);
							token = strtok(NULL,delim);
						}
						strcat(operands,",");/*dummy for treatment of last operand*/
						int len = strlen(operands);
						char *operand1 = (char*)malloc(sizeof(char)*len);
						create1operand(row,len,&skipFlag,operands,&operand1);/*stores the operands*/
						if(skipFlag==False){
							char type1[3];
							strcpy(type1,"");
							int temp = 0;
							strcpy(type1,operandType(operand1,&temp,row,names));
							if(strcmp(type1,"01")==0){
								node *label = find(*labels,operand1);
								if(label!=NULL){
									char *word1 = (char*)malloc(sizeof(char)*21);
									char *word2 = (char*)malloc(sizeof(char)*21);
									char *ldata = getData(label);
									if(strstr(ldata,"external")==NULL){
										strcpy(word1,"0010");
										strcpy(word2,"0010");
									}
									else{
										strcpy(word1,"0001");
										strcpy(word2,"0001");
										char *IC = getKey(findData(*code,"????????????????????"));
										/*the key (address) of the next 2 unfilled code is the required IC base and
										offset*/
										insert(*externals,operand1,IC);
										temp = atoi(IC);
										temp++;
										sprintf(IC,"%d",temp);
										insert(*externals,operand1,IC);
									}
									char *cbase = (char*)malloc(sizeof(char)*len);
									char *coffset = (char*)malloc(sizeof(char)*len);
									getAddress(ldata,&cbase,&coffset);
									strcat(word1,numToCode(cbase));
									strcat(word2,numToCode(coffset));
									updateData(findData(*code,"????????????????????"),word1);
									updateData(findData(*code,"????????????????????"),word2);
								}
								else{
									printf("error in row %d: label (%s) doesn't exist\n",row,operand1);
									errFlag = True;
								}
							}
							if(strcmp(type1,"10")==0){
								len = strlen(operand1);
								char *labelName = (char*)malloc(sizeof(char)*len);
								strcpy(labelName,"");
								int i;
								for(i=0;i<len;i++){
									if(operand1[i]=='[')
										i = len;
									else
										strcat(labelName,ctos(operand1[i]));
								}
								node *label = find(*labels,labelName);
								if(label!=NULL){
									char *word1 = (char*)malloc(sizeof(char)*21);
									char *word2 = (char*)malloc(sizeof(char)*21);
									char *ldata = getData(label);
									if(strstr(ldata,"external")==NULL){
										strcpy(word1,"0010");
										strcpy(word2,"0010");
									}
									else{
										strcpy(word1,"0001");
										strcpy(word2,"0001");
										char *IC = getKey(findData(*code,"????????????????????"));
										/*the key (address) of the next 2 unfilled code is the required IC base and
										offset*/
										insert(*externals,operand1,IC);
										temp = atoi(IC);
										temp++;
										sprintf(IC,"%d",temp);
										insert(*externals,operand1,IC);
									}
									char *cbase = (char*)malloc(sizeof(char)*len);
									char *coffset = (char*)malloc(sizeof(char)*len);
									getAddress(ldata,&cbase,&coffset);
									strcat(word1,numToCode(cbase));
									strcat(word2,numToCode(coffset));
									updateData(findData(*code,"????????????????????"),word1);
									updateData(findData(*code,"????????????????????"),word2);
								}
								else{
									printf("error in row %d: label (%s) doesn't exist\n",row,labelName);
									errFlag = True;
								}
							}
						}
						else
							errFlag = True;	
					}
				}
				if(noteFlag==True||skipFlag==True)
					token = NULL;
				else{
					token = strtok(NULL,delim);
				}
			}
		}
		fclose(input);
	}
	return errFlag;
}

