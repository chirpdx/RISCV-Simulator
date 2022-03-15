#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commondefs.h"

char regnames[32][5] = {"zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};

int main(int argc,char *argv[] ){
	char source[] = "../tests/all_alu.mem";
	char buffer[32];
	int address;
	unsigned int inst;
	char *token;
	unsigned int inst_mem[16384] = {0}; //64K Mem
	unsigned int pc = 0;
	unsigned int sp = 65536;
	unsigned int x[32] = {0};

	#ifdef __verbose__
		printf("Verbose mode triggered\n");
	#endif

	if(argc < 3) {
		printf("No conditions passed Using Default value\n");
		}
	else
	{
		for (int i = 1; i < argc; i+=2){
			if  ((strcmp(argv[i],"-mem") == 0))
				strcpy(source,argv[i+1]);
			else if ((strcmp(argv[i],"-pc") == 0))
				pc = (int) strtol(argv[i+1],NULL,16);
			else if ((strcmp(argv[i],"-sp") == 0))
				sp = (int) strtol(argv[i+1],NULL,10);
			}
		}
	FILE *fp = fopen(source,"r");
	#ifdef __verbose__
		printf("Initial PC Value(represented in hex):%x\n", pc);
		printf("Initial Stack pointer Value(represented in decimal):%u\n",sp );
		//printf("Number of arguments(represented in decimal):%d\n",(argc-1)); //not needed
	#endif

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
	unsigned int singlestep = 0;
	char single[1] ;
	unsigned int pcjump = 0;
	unsigned int bpset = 0;
	#ifdef __VerboseDebug__
		singlestep = 1;
	#endif

	while(!(inst_mem[pc/4] == 0x8067 && x[raindex]== 0) && !(inst_mem[pc/4] == 0x0))
	{
		#ifdef __pcdump__
			printf("0x%08x\n",pc);
		#endif
		x[zeroindex] = 0;
		if(pc%4 != 0){
			printf("Error PC is not 4 byte aligned: pc = %u\n", pc);
			exit(0);
		}
		if(pc > 65535){
			printf("Error PC is out of memory bound: pc = %u\n", pc);
			exit(0);
		}
		if((singlestep == 1) || ((bpset == 1) && (pcjump == pc)))
		{
			printf("Single Stepping entered: \n Enter 's' for single step, or 'c' for continue to last instruction or 'b' for setting breakpoint\n");
			scanf("%s",single);
			//printf("%s",single);
			if(single[0]  == 's')
			{
				singlestep = 1;
				bpset = 0;
				//printf("%d\n",singlestep);
			}
			else if(single[0] == 'c')
			{
				singlestep = 0;
				bpset = 0;
			}
			else if(single[0] == 'b')
			{
				printf("Enter PC value in hex\n");
				scanf("%x",&pcjump);
				singlestep = 0;
				bpset = 1;
			}
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
		
	}
	printf("\n\nAfter Final Instruction \n\n");
	DisplayRegs(x, pc);

	return(0);
}
