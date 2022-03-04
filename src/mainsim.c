#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define opcodemask  0x0000007f
#define rdmask      0x00000f80
#define funct3mask  0x00003000
#define rs1mask     0x000f8000
#define rs2mask     0x01f00000
#define funct7mask  0xfe000f80
#define imm12Imask  0xfff00000
//#define imm12Smask
//#define imm13Bmask
//#define imm32Umask
//#define imm21Jmask
#define opralu 0x33
#define opialu 0x13
#define opload 0x03
#define opstore 0x23
#define opbranch 0x63
char regnames[32][5] = {"zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};
enum otherindexes{zeroindex, raindex, spindex};

typedef struct{
	unsigned int opcode;
	int rd;
	int funct3;
	int rs1;
	int rs2;
	int funct7;
}Rtype;
// all types to be created

typedef struct{
	unsigned int opcode;
	int rd;
	int funct3;
	int rs1;
	int rs2;
	int funct7;
	unsigned int imm;
}AllDecodeFields;



int DisplayRegs(unsigned int regs[32], unsigned int pc);
AllDecodeFields DecodeInst(unsigned int IR);
unsigned int Execute(AllDecodeFields df, unsigned int *umem, unsigned int regs[32], unsigned int pc);
unsigned int ReadMem(unsigned int *umem, unsigned int raddr);
int WriteMem(unsigned int *umem, unsigned int waddr, unsigned int writevalue);

void main(int argc,char *argv[] ){
	char source[] = "../tests/"; //hard coded sorce for mem
	char buffer[32];
	int address;
	unsigned int inst;
	char *token;
	unsigned int inst_mem[16384] = {0}; //64K Mem
	unsigned int pc = 0;
	unsigned int sp;
	unsigned int x[32] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,0,1,2,3,4,5,6,7,8,9,10};
	unsigned int i;
	//printf();
	if (argc == 7){
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
			pc = (int)strtol(argv[2], NULL, 16);
			sp = (int)strtol(argv[4], NULL, 10);
			}
		else if ((strcmp(argv[1],"-pc") == 0) && (strcmp(argv[3],"-mem") == 0)){
			strcat(source, argv[4]);
			pc = (int)strtol(argv[2], NULL, 16);
			}
		else if ((strcmp(argv[1],"-sp") == 0) && (strcmp(argv[3],"-pc") == 0)){
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
			}
		else if ((strcmp(argv[1],"-sp") == 0)){
			sp = (int)strtol(argv[2], NULL, 10);
			}
	}
	else {
		printf("No conditions passed Using Default value\n");
	}
	FILE *fp = fopen(source,"r");
	printf("PC Value(represented in hex):%x\n", pc);
	printf("Stack pointer Value(represented in decimal):%x\n",sp );
	printf("Number of arguments(represented in decimal):%d\n",(argc-1)); //not needed

	while ( fgets(buffer,32,fp) != NULL){
		//printf("%s",buffer);
		token  = strtok(buffer,":");
		//printf("Expecting address:%s\n",token);
		address = (int)strtol(token, NULL, 16);
		//printf ("from add : %x\n",address);
		
		token = strtok(NULL," ");
		//printf("Expecting Inst:%s",token);
		inst = (int)strtol(token, NULL, 16);
		inst_mem[address/4] = inst;
		//printf("From inst : %x\n\n",inst);
	}
	fclose(fp);
	for (i = 0; i<20; i++){
		printf("Element [%d] = %x\n",i,inst_mem[i]);
	}
	//unsigned int instruction[] = {10035,37878,65345};
	unsigned int IR = 0;

	//template
	x[0] = 664;
	x[spindex]	= sp;
	printf("%x\n",x[16]);
	AllDecodeFields decodedall;
	pc = 0;
	//DisplayRegs(x, pc);
	do
	{
		IR = inst_mem[pc/4];
		printf("IR  : 0x%08x\n", IR);
		pc = pc + 4;
		decodedall = DecodeInst(IR);
		pc = Execute(decodedall, inst_mem, x, pc);
		DisplayRegs(x, pc);
		//printf("%x\n",decodedall.opcode);
	}
	while(!(IR == 0x8067 && x[31]== 10));
	//WriteMem(instruction, 12, 65535);
	//printf("after writemem : %u\n", instruction[12/4]);
	//DisplayRegs(x, pc);
	//printf("SP = 0x%08u\n", x[spindex]);	// spindex is stack pointer index
}

int DisplayRegs(unsigned int regs[32], unsigned int pc){
	int i = 0;
	int j = 0;
	printf("pc  : 0x%08x\n", pc);
	
	for(i = 0; i < 8; i++)
	{
		for(j = 0; j < 4; j++)
			printf("\t%4s -> x%2d :   0x%08x", regnames[i*4 + j], i*4 + j, regs[i*4 + j]);
		printf("\n");
	}
	return 0;
}

AllDecodeFields DecodeInst(unsigned int IR){
	int opcode = 0;
	opcode = IR & opcodemask;
	AllDecodeFields df;
	printf("opcode = 0x%02x\n",opcode);
	switch(opcode)
	{
		case opralu://for Rtype
			df.opcode	= opcode;
			df.rd		= (IR & rdmask)		>> 7;
			df.funct3	= (IR & funct3mask)	>> 12;
			df.rs1		= (IR & rs1mask)	>> 15;
			df.rs2		= (IR & rs2mask)	>> 20;
			df.funct7	= (IR & funct7mask)	>> 25;
			break;
		case opload://for Itype
			df.opcode	= opcode;
			df.rd		= (IR & rdmask)		>> 7;
			df.funct3	= (IR & funct3mask)	>> 12;
			df.rs1		= (IR & rs1mask)	>> 15;
			df.imm		= (IR & imm12Imask)	>> 20;
			break;
		case opialu:
			df.opcode	= opcode;
			df.rd		= (IR & rdmask)		>> 7;
			df.funct3	= (IR & funct3mask)	>> 12;
			df.rs1		= (IR & rs1mask)	>> 15;
			df.imm		= (IR & imm12Imask)	>> 20;
			break;
		case 0x67:
			df.opcode	= opcode;
			df.rd		= (IR & rdmask)		>> 7;
			df.funct3	= (IR & funct3mask)	>> 12;
			df.rs1		= (IR & rs1mask)	>> 15;
			df.imm		= (IR & imm12Imask)	>> 20;
			break;
		case opstore:// make thus for Stype
			df.opcode	= opcode;
			df.funct3	= (IR & funct3mask)	>> 12;
			df.rs1		= (IR & rs1mask)	>> 15;
			df.rs2		= (IR & rs2mask)	>> 20;
			df.imm		= ((IR & rdmask)	>> 7)|((IR & funct7mask)	>> 18);
			break;
		case opbranch:
			printf("Branch decode\n");
			df.opcode	= opcode;
			df.funct3	= (IR & funct3mask)	>> 12;
			df.rs1		= (IR & rs1mask)	>> 15;
			df.rs2		= (IR & rs2mask)	>> 20;
			df.imm = 0;
			df.imm		= (((IR & 0x80000000)	>> 19) ? 0xfffff000 : 0x00000000)| ((IR & 0x7e000000)	>> 20)|((IR & 0x00000f00)	>> 7) | ((IR & 0x00000080)	<< 4);
			break;
		default:
			printf("Entered default in decode\n");
			break;
	}
	return df;
}

unsigned int Execute(AllDecodeFields df, unsigned int *umem, unsigned int regs[32], unsigned int pc){
printf("pc in Execute  : 0x%08x\n", pc);
	switch(df.opcode)
	{
		case opralu:
			switch(df.funct3)
			{
				case 0x0:
					if(df.funct7)
						regs[df.rd] = regs[df.rs1] - regs[df.rs2];
					else //check fields in specification
						regs[df.rd] = regs[df.rs1] + regs[df.rs2];
					break;
				case 0x4:
					regs[df.rd] = regs[df.rs1] ^ regs[df.rs2];
					break;
				case 0x6:
					regs[df.rd] = regs[df.rs1] | regs[df.rs2];
					break;
				case 0x7:
					regs[df.rd] = regs[df.rs1] & regs[df.rs2];
					break;
				case 0x1:
					regs[df.rd] = regs[df.rs1] << regs[df.rs2];
					break;
				case 0x5:
					if(df.funct7)
						regs[df.rd] = regs[df.rs1] >> regs[df.rs2];
					else //check fields in specification
						regs[df.rd] = regs[df.rs1] >> regs[df.rs2];  //////////////////////MSB EXTENDS
					break;
				case 0x2:
					regs[df.rd] = (regs[df.rs1] < regs[df.rs2]) ? 1:0;
					break;
				case 0x3:
					regs[df.rd] = (regs[df.rs1] < regs[df.rs2]) ? 1:0; /////////////////////zero extends
					break;

				default:
					break;
			}
			break;

		case opialu:
			switch(df.funct3)
			{
				case 0x0:
					regs[df.rd] = regs[df.rs1] + regs[df.imm];
					break;
				case 0x4:
					regs[df.rd] = regs[df.rs1] ^ regs[df.imm];
					break;
				case 0x6:
					regs[df.rd] = regs[df.rs1] | regs[df.imm];
					break;
				case 0x7:
					regs[df.rd] = regs[df.rs1] & regs[df.imm];
					break;
				case 0x1:
					if (!(df.imm & 0xfe0)) //!
						regs[df.rd] = regs[df.rs1] << regs[df.imm]&0x1f;////////////
					break;
				case 0x5:
					if (!(df.imm & 0xfe0))
						regs[df.rd] = regs[df.rs1] >> regs[df.imm]&0x1f;
					else //check fields in specification
						regs[df.rd] = regs[df.rs1] >> regs[df.imm]&0x1f;  //////////////////////MSB EXTENDS
					break;
				case 0x2:
					regs[df.rd] = (regs[df.rs1] < regs[df.imm]) ? 1:0;////////////
					break;
				case 0x3:
					regs[df.rd] = (regs[df.rs1] < regs[df.imm]) ? 1:0; /////////////////////zero extends
					break;

				default:
					break;
			}
			break;

		case opload:
			switch(df.funct3)
			{
				case 0x0:
					regs[df.rd] = *(umem+(df.rs1+df.imm)/4) & 0x80 ? ((*(umem+(df.rs1+df.imm)/4))&0xff)|0xffff00 : (*(umem+(df.rs1+df.imm)/4))&0xff;
					break;
				case 0x1:
					regs[df.rd] = *(umem+(df.rs1+df.imm)) & 0x8000 ? ((*(umem+(df.rs1+df.imm)))&0xffff)|0xff0000 : (*(umem+(df.rs1+df.imm)))&0xffff;
					break;
				case 0x2:
					regs[df.rd] = (*(umem+(df.rs1+df.imm)))&0xffffff;
					break;
				case 0x4:
					regs[df.rd] = (*(umem+(df.rs1+df.imm)))&0xff;  /////////////////////zero extends
					break;
				case 0x5:
					regs[df.rd] = (*(umem+(df.rs1+df.imm)))&0xffff; /////////////////////zero extends
					break;
				default:
					break;
			}
			break;

		case opstore:
			switch(df.funct3)
			{
				case 0x0:
					WriteMem(umem,df.rs1+df.imm,regs[df.rs2] & 0xff);
					//WriteMem(umem,df.rs1+df.imm,(*(umem+(df.rs1+df.imm)/4)&ffff00)|(regs[df.rs2] & 0xff))
					//*(umem+(df.rs1+df.imm)/4) & 0x80 ? ((*(umem+(df.rs1+df.imm)/4))&0xff)|0xffff00 : (*(umem+(df.rs1+df.imm)/4))&0xff = regs[df.rs2] & 0xff;
					break;
				case 0x1:
					WriteMem(umem,df.rs1+df.imm,regs[df.rs2] & 0xffff);
					//WriteMem(umem,df.rs1+df.imm,(*(umem+(df.rs1+df.imm)/4)&ff0000)|(regs[df.rs2] & 0xffff))
					//*(umem+(df.rs1+df.imm)) & 0x8000 ? ((*(umem+(df.rs1+df.imm)))&0xffff)|0xff0000 : (*(umem+(df.rs1+df.imm)))&0xffff = regs[df.rs2] & 0xffff;
					break;
				case 0x2:
					WriteMem(umem,df.rs1+df.imm,regs[df.rs2] & 0xffffff);
					//(*(umem+(df.rs1+df.imm)))&0xffffff = regs[df.rs2] & 0xffffff;
					break;

				default:
					break;
			}
			break;

		case opbranch://B  YET TO DO
			printf("Entered Branch execute funct3 = %d\n",df.funct3);
			switch(df.funct3)
			{
				case 0x0:
					if ((signed int)df.rs1 == (signed int)df.rs2)
						pc = pc + df.imm;
					break;
				case 0x1:
					if ((signed int)df.rs1 != (signed int)df.rs2)
						pc = pc + df.imm;
					break;
				case 0x4:
					if ((signed int)df.rs1 < (signed int)df.rs2)
						pc = pc + df.imm;
					break;
				case 0x5:
					if ((signed int)df.rs1 >= (signed int)df.rs2)
						pc = pc + df.imm;
					break;
				case 0x6:
					if (df.rs1 < df.rs2)
						pc = pc + df.imm;
					break;
				case 0x7:
					if (df.rs1 >= df.rs2)
						pc = pc + df.imm;
					break;

				default:
					printf("funct3 default in branch execute");
					break;
			}
			break;
		case 0x67:// change below conditions based on it
			switch(df.funct3)
			{
				case 0x01: //shamt and shift
				case 0x03:
					break;
				case 0x00:
					break;
				case 0x02:
					break;
				default:
					break;
			}
			break;
		default:
			printf("Entered default in Execute\n");
			break;
	}
	printf("Returning from Execute\n");
	return pc;
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