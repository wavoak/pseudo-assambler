#include "memory.h"
#include "preprocessor.h"
#include "pass1and2.h"
#include "output.h"

int main(int argc, char **argv){
	
	char *names = "mov,cmp,add,sub,lea,clr,not,inc,dec,jmp,bne,jsr,red,prn,rts,stop,";/*for this project, names will be a string holding all function names*/
	int ICF, DCF;
	list *code = (list*)malloc(sizeof(list));/*dynamic list for code memory*/
	list *data = (list*)malloc(sizeof(list));/*dynamic list for data memory*/
	list *labels = (list*)malloc(sizeof(list));/*dynamic list for labels*/
	list *externals = (list*)malloc(sizeof(list));/*dynamic list for external operands*/
	
	int i, errFlag;/*i index for pass1 and pass 2 with argv, errFlag indicates any error in pass1 or pass2*/
	
	preprocessor(argc,argv,names);/*macro spreading for all input files*/
	fflush(NULL);/*flush all streams to prevent unwanted stream contents in pass1*/
	
	/*pass1 and pass2 for all input files.
	i didn't make the pass1/2 an execution function due to the amount of parameters and integrations each file would require*/
	for(i=1;i<argc;i++){
		initlist(code);
		initlist(data);
		initlist(labels);
		initlist(externals);
		errFlag = pass1(argv[i],&code,&data,&labels,names, &ICF, &DCF);
		if(errFlag==True)
			printf("error occured in pass1 for %s\n",argv[i]);
		else{
			errFlag = pass2(argv[i],&code,&data,&labels,&externals,names);
			if(errFlag==True)
				printf("error occured in pass2 for %s\n",argv[i]);
			else{
				printf("no errors found for %s\n",argv[i]);
				objCreate(argv[i],code,data,ICF,DCF);
				entCreate(argv[i],labels);
				extCreate(argv[i],externals);
			}
		}
	}
	
	return 0;
}

