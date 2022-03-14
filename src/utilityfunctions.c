#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commondefs.h"

int DisplayRegs(unsigned int regs[32], unsigned int pc){
	int i = 0;
	int j = 0;
	printf("pc  : 0x%08x\n", pc);
	#ifdef __verbose__
		printf("Verbose mode triggered\n");
	#endif
	for(i = 0; i < 8; i++)
	{
		for(j = 0; j < 4; j++)
			printf("\t%4s -> x%2d :   0x%08x", regnames[i*4 + j], i*4 + j, regs[i*4 + j]);
		printf("\n");
	}
	return 0;
}

unsigned int ReadMem(unsigned int *umem, unsigned int raddr){
	unsigned int readvalue;
	if(raddr <= 65535)
		//printf("In readmem function: %u\n",*(umem + raddr/4));
		readvalue = *(umem + raddr/4);
	else
		printf("!!!!!!!ReadAddress Error\n");
	return readvalue;
}

int WriteMem(unsigned int *umem, unsigned int waddr, unsigned int writevalue){
	//printf("Writemem: %u\n",waddr);
	if(waddr <= 65535)
		*(umem + waddr/4) = writevalue;
	else
		printf("!!!!!!!WriteAddress Error\n");
	return 0;
}