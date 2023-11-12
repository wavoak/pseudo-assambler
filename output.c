#include "output.h"

/*receives data line and 5 obj file bases and stores the fields*/
void dataToNums(char *data, int *Anum, int *Bnum, int *Cnum, int *Dnum, int *Enum){
	int i;
	int current = 0;
	for(i=0;i<20;i++){
		if(i==4){/*current is now the A field number as int*/
			*Anum = current;
			current = 0;
		}
		if(i==8){/*current is now the B field number as int*/
			*Bnum = current;
			current = 0;
		}
		if(i==12){/*current is now the C field number as int*/
			*Cnum = current;
			current = 0;
		}
		if(i==16){/*current is now the D field number as int*/
			*Dnum = current;
			current = 0;
		}
		if(data[i]=='1'){
			current += pow(2,(20-i-1)%4);
		}
	}
	/*current is now the E field number as int*/
	*Enum = current;
}

/*create filename.ob using code,data,ICF and DCF*/
void objCreate(char *filename, list *code, list *data, int ICF, int DCF){
	FILE *f;
	char line[21];
	node *ptr;
	int Anum = 0, Bnum = 0, Cnum = 0, Dnum = 0, Enum = 0, address = 100;
	f = create(filename,".ob");
	if(f!=NULL){
		sprintf(line,"%d %d",ICF-100,DCF);
		fputs(line,f);
		fputc('\n',f);
		ptr = getHead(code);
		while(ptr!=NULL){
			strcpy(line,getData(ptr));
			dataToNums(line,&Anum,&Bnum,&Cnum,&Dnum,&Enum);
			sprintf(line,"%04d A%x-B%x-C%x-D%x-E%x",address,Anum,Bnum,Cnum,Dnum,Enum);
			fputs(line,f);
			fputc('\n',f);
			address++;
			ptr = getNext(ptr);
		}
		ptr = getHead(data);
		while(ptr!=NULL){
			strcpy(line,getData(ptr));
			dataToNums(line,&Anum,&Bnum,&Cnum,&Dnum,&Enum);
			sprintf(line,"%04d A%x-B%x-C%x-D%x-E%x",address,Anum,Bnum,Cnum,Dnum,Enum);
			fputs(line,f);
			fputc('\n',f);
			address++;
			ptr = getNext(ptr);
		}
		fclose(f);
	}
	else
		printf("error: %s.ob failed to open\n",filename);
}

/*create filename.ext using externals*/
void extCreate(char *filename, list *externals){
	FILE *f;
	node *ptr;
	char line[44];/*label<=31  +  2*' '  +  BASE/OFFSET  +  address(%04)*/
	int address;
	f = create(filename,".ext");
	if(f!=NULL){
		ptr = getHead(externals);
		while(ptr!=NULL){
			address = atoi(getData(ptr));
			sprintf(line,"%s BASE %04d",getKey(ptr),address);
			fputs(line,f);
			fputc('\n',f);
			ptr = getNext(ptr);
			address = atoi(getData(ptr));
			sprintf(line,"%s OFFSET %04d",getKey(ptr),address);
			fputs(line,f);
			fputc('\n',f);
			ptr = getNext(ptr);
		}
		fclose(f);
	}
	else
		printf("error: %s.ent failed to open\n",filename);
}

/*create filename.ent using labels*/
void entCreate(char *filename, list *labels){
	FILE *f;
	node *ptr;
	char *lname, *ldata, *cbase, *coffset;
	lname = (char*)malloc(sizeof(char)*32);
	ldata = (char*)malloc(sizeof(char)*21);
	cbase = (char*)malloc(sizeof(char)*21);
	coffset = (char*)malloc(sizeof(char)*21);
	f = create(filename,".ent");
	if(f!=NULL){
		ptr = getHead(labels);
		while(ptr!=NULL){
			strcpy(lname,getKey(ptr));
			strcpy(ldata,getData(ptr));
			if(strstr(ldata,"entry")!=NULL){
				getAddress(ldata,&cbase,&coffset);
				sprintf(ldata,"%s,%s,%s",lname,cbase,coffset);
				fputs(ldata,f);
				fputc('\n',f);
			}
			ptr = getNext(ptr);
		}
		fclose(f);
	}
	else
		printf("error: %s.ent failed to open\n",filename);
}

