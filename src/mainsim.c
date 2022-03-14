#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commondefs.h"

char regnames[32][5] = {"zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};

int main(int argc,char *argv[] ){
	char source[] = "../";
	char buffer[32];
	int address;
	unsigned int inst;
	char *token;
	unsigned int inst_mem[16384] = {0}; //64K Mem
	unsigned int pc = 0;
	unsigned int sp = 65536;
	unsigned int x[32] = {0};
	unsigned int singlestep = 0;

	#ifdef __verbose__
		printf("Verbose mode triggered\n");
	#endif
	if (argc >= 7){
		printf("Only 3 arguments is passed\n");
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
	}
	else if (argc == 5){
		printf("Only two arguments is passed\n");
		if ( (strcmp(argv[1],"-mem") == 0) && (strcmp(argv[3],"-pc") == 0)){
			strcat(source, argv[2]);
			pc = (int)strtol(argv[4], NULL, 16);
			}
		else if ((strcmp(argv[1],"-mem") == 0) && (strcmp(argv[3],"-sp") == 0)){
			strcat(source, argv[2]);
			sp = (int)strtol(argv[4], NULL, 10);
			}
		else if ((strcmp(argv[1],"-pc") == 0) && (strcmp(argv[3],"-sp") == 0)){
			strcat(source,"tests/porg.mem");
			pc = (int)strtol(argv[2], NULL, 16);
			sp = (int)strtol(argv[4], NULL, 10);
			}
		else if ((strcmp(argv[1],"-pc") == 0) && (strcmp(argv[3],"-mem") == 0)){
			strcat(source, argv[4]);
			pc = (int)strtol(argv[2], NULL, 16);
			}
		else if ((strcmp(argv[1],"-sp") == 0) && (strcmp(argv[3],"-pc") == 0)){
			strcat(source,"tests/porg.mem");
			pc = (int)strtol(argv[4], NULL, 16);
			sp = (int)strtol(argv[2], NULL, 10);
			}
		else if ((strcmp(argv[1],"-sp") == 0) && (strcmp(argv[3],"-mem") == 0)){
			strcat(source, argv[2]);
			sp = (int)strtol(argv[4], NULL, 10);
			}
	}
	else if (argc == 3){
		printf("Only one arguments is passed\n");
		if ( (strcmp(argv[1],"-mem") == 0)){
			strcat(source, argv[2]);
			}
		else if ((strcmp(argv[1],"-pc") == 0)){
			pc = (int)strtol(argv[2], NULL, 16);
			strcat(source,"tests/porg.mem");
			}
		else if ((strcmp(argv[1],"-sp") == 0)){
			sp = (int)strtol(argv[2], NULL, 10);
			strcat(source,"tests/porg.mem");
			}
	}
	else {
		printf("No conditions passed Using Default value\n");
		strcat(source,"tests/porg.mem");
	}
	FILE *fp = fopen(source,"r");
	printf("Initial PC Value(represented in hex):%x\n", pc);
	printf("Initial Stack pointer Value(represented in decimal):%u\n",sp );
	//printf("Number of arguments(represented in decimal):%d\n",(argc-1)); //not needed

	//unsigned int breakpoint[2] = {-1,-1};
	//printf("argc argv[8] = %d %x\n",argc, (int)strtol(argv[8], NULL, 16));

	/*if(argc == 10)
	{
		breakpoint[0] = (int)strtol(argv[8], NULL, 16);
		breakpoint[1] = (int)strtol(argv[9], NULL, 16);
		printf("inside 0 = %x\n", breakpoint[0]);
	}*/
	//printf("breakpoint 0 = %x %x\n", breakpoint[0], breakpoint[1]);

	while ( fgets(buffer,32,fp) != NULL){
		token  = strtok(buffer,":");
		address = (int)strtol(token, NULL, 16);
		
		token = strtok(NULL," ");
		inst = strtoul(token, NULL, 16);
		inst_mem[address/4] = inst;
	}
	fclose(fp);
	unsigned int IR = 0;
	x[spindex] = sp;
	x[raindex] = 0;
	AllDecodeFields decodedall;

		//printf("Single Stepping entered: ");
		//getchar();

	while(!(inst_mem[pc/4] == 0x8067 && x[raindex]== 0))
	{
		x[zeroindex] = 0;
		if(pc%4 != 0){
			printf("Error PC is not 4 byte aligned: pc = %u\n", pc);
			exit(0);
		}
		if(pc > 65535){
			printf("Error PC is out of memory bound: pc = %u\n", pc);
			exit(0);
		}
		IR = inst_mem[pc/4];
		#ifdef __verbose__
			printf("Instruction  : 0x%08x\n", IR);
		#endif
		decodedall = DecodeInst(IR);
		pc = Execute(decodedall, inst_mem, x, pc);
		x[zeroindex] = 0;
		#ifdef __verbose__
			DisplayRegs(x, pc);
		#endif
		//printf("%x\n",decodedall.opcode);
		//if(decodedall.opcode == opralu || decodedall.opcode == opialu || decodedall.opcode == opload || decodedall.opcode == opstore || decodedall.opcode == oplui || decodedall.opcode == opauipc)
		pc = pc + 4;
		//printf("pc bp = %d %d\n",pc, breakpoint[0]);

			//getchar();
		
	}
	printf("\n\nAfter Final Instruction \n\n");
	DisplayRegs(x, pc);

	return(0);
}
