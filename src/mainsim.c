#include <stdio.h>
#include <string.h>
void main(int argc,char *argv[] ){
	char source[] = "../tests/"; //hard coded sorce for mem
	char buffer[32];
	int address;
	int unsigned inst;
	char *token;
	int unsigned pc = 0;
	int sp;
	if ( (strcmp(argv[1],"-mem") == 0) && (strcmp(argv[3],"-pc") == 0) && (strcmp(argv[5],"-sp") == 0)){
		strcat(source, argv[2]);
		pc = (int)strtol(argv[4], NULL, 16);
		sp = (int)strtol(argv[6], NULL, 10);
		printf("File Name:%s\n",argv[2]); 
	}
	else if ((strcmp(argv[1],"-mem") == 0) && (strcmp(argv[5],"-pc") == 0) && (strcmp(argv[3],"-sp") == 0)){
		strcat(source, argv[2]);
		pc = (int)strtol(argv[6], NULL, 16);
		sp = (int)strtol(argv[4], NULL, 10); 
		printf("File Name:%s\n",argv[2]);
	}
	else if ((strcmp(argv[5],"-mem") == 0) && (strcmp(argv[1],"-pc") == 0) && (strcmp(argv[3],"-sp") == 0)){
		strcat(source, argv[6]);
		pc = (int)strtol(argv[2], NULL, 16);
		sp = (int)strtol(argv[4], NULL, 10); 
		printf("File Name:%s\n",argv[6]);
	}
	else if ((strcmp(argv[5],"-mem") == 0) && (strcmp(argv[3],"-pc") == 0) && (strcmp(argv[1],"-sp") == 0)){
		strcat(source, argv[6]);
		pc = (int)strtol(argv[4], NULL, 16);
		sp = (int)strtol(argv[2], NULL, 10);
		printf("File Name:%s\n",argv[6]); 
	}
	else if ((strcmp(argv[3],"-mem") == 0) && (strcmp(argv[5],"-pc") == 0) && (strcmp(argv[1],"-sp") == 0)){
		strcat(source, argv[4]);
		pc = (int)strtol(argv[6], NULL, 16);
		sp = (int)strtol(argv[2], NULL, 10); 
		printf("File Name:%s\n",argv[4]);
	}
	else if ((strcmp(argv[3],"-mem") == 0) && (strcmp(argv[1],"-pc") == 0) && (strcmp(argv[5],"-sp") == 0)){
		strcat(source, argv[4]);
		pc = (int)strtol(argv[2], NULL, 16);
		sp = (int)strtol(argv[6], NULL, 10); 
		printf("File Name:%s\n",argv[4]);
	}
	else{
		printf("No conditions passed\n");
	}
	FILE *fp = fopen(source,"r");
	printf("PC Value(represented in hex):%x\n", pc);
	printf("Stack pointer Value(represented in decimal):%d\n",sp );
	printf("Number of arguments(represented in decimal):%d\n",(argc-1)); //not needed

	while ( fgets(buffer,32,fp) != NULL){
		printf("%s",buffer);
		token  = strtok(buffer,":");
		printf("Expecting address:%s\n",token);
		address = (int)strtol(token, NULL, 16);
		printf ("from add : %x\n",address);
		
		token = strtok(NULL," ");
		printf("Expecting Inst:%s",token);
		inst = (int)strtol(token, NULL, 16);
		printf("From inst : %x\n\n",inst);
	}
	fclose(fp);
}
