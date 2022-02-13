// Main function
#include <stdio.h>
#include <string.h>
void main(int argc,char *argv[] ){
	char source[] = "../tests/";
	strcat(source, argv[1]);
	FILE *fp = fopen(source,"r");
	char buffer[32];
	int address;
	int unsigned inst;
	char *token;
	int pc,sp;
	pc = (int)strtol(argv[2], NULL, 16);
	sp = (int)strtol(argv[3], NULL, 10);
	printf("File Name:%s\n",argv[1]);
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
