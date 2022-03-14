#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commondefs.h"

AllDecodeFields DecodeInst(unsigned int IR){
	int opcode = 0;
	opcode = IR & opcodemask;
	AllDecodeFields df;
	#ifdef __verbose__
		printf("opcode in Decode = 0x%02x\n",opcode);
	#endif
	switch(opcode)
	{
		case opralu:
			df.opcode	= opcode;
			df.rd		= (IR & rdmask)		>> 7;
			df.funct3	= (IR & funct3mask)	>> 12;
			df.rs1		= (IR & rs1mask)	>> 15;
			df.rs2		= (IR & rs2mask)	>> 20;
			df.funct7	= (IR & funct7mask)	>> 25;
			df.imm = 0;
			break;
		case opload:
			df.opcode	= opcode;
			df.rd		= (IR & rdmask)		>> 7;
			df.funct3	= (IR & funct3mask)	>> 12;
			df.rs1		= (IR & rs1mask)	>> 15;
			df.imm = 0;
			df.imm		= ((IR & 0x80000000) ? 0xfffff000 : 0x00000000) | ((IR & imm12Imask)	>> 20);
			break;
		case opialu:
			df.opcode	= opcode;
			df.rd		= (IR & rdmask)		>> 7;
			df.funct3	= (IR & funct3mask)	>> 12;
			df.rs1		= (IR & rs1mask)	>> 15;
			df.imm = 0;
			df.imm		= ((IR & 0x80000000) ? 0xfffff000 : 0x00000000)	| ((IR & imm12Imask)	>> 20);
			break;
		case opji:
			df.opcode	= opcode;
			df.rd		= (IR & rdmask)		>> 7;
			df.funct3	= (IR & funct3mask)	>> 12;
			df.rs1		= (IR & rs1mask)	>> 15;
			df.imm = 0;
			df.imm		= ((IR & 0x80000000) ? 0xfffff000 : 0x00000000)	| ((IR & imm12Imask)	>> 20);
			break;
		case opstore:
			df.opcode	= opcode;
			df.funct3	= (IR & funct3mask)	>> 12;
			df.rs1		= (IR & rs1mask)	>> 15;
			df.rs2		= (IR & rs2mask)	>> 20;
			df.imm = 0;
			df.imm		= ((IR & 0x80000000)	? 0xfffff000 : 0x00000000)|((IR & rdmask)	>> 7)|((IR & funct7mask)	>> 20);
			break;
		case opbranch:
			df.opcode	= opcode;
			df.funct3	= (IR & funct3mask)	>> 12;
			df.rs1		= (IR & rs1mask)	>> 15;
			df.rs2		= (IR & rs2mask)	>> 20;
			df.imm = 0;
			df.imm		= (((IR & 0x80000000)	>> 19) ? 0xfffff000 : 0x00000000)| ((IR & 0x7e000000)	>> 20)|((IR & 0x00000f00)	>> 7) | ((IR & 0x00000080)	<< 4);
			break;
		case opj:
			df.opcode	= opcode;
			df.rd		= (IR & rdmask)		>> 7;
			df.imm = 0;
			df.imm		= ((IR & 0x80000000) ? 0xfff00000 : 0x00000000)| (IR & 0x000ff000) | ((IR  & 0x00100000) >> 9) | ((IR & 0x7fe00000) >> 20);
			break;
		case oplui:
			df.opcode	= opcode;
			df.rd		= (IR & rdmask)		>> 7;
			df.imm		= (IR & 0xfffff000);
			break;
		case opauipc:
			df.opcode	= opcode;
			df.rd		= (IR & rdmask)		>> 7;
			df.imm		= (IR & 0xfffff000);
			break;
		default:
			printf("Entered default in decode\n");
			exit(0);
			break;
	}
	return df;
}