#include <stdio.h>
#include <string.h>

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

enum othernames{zeroindex, raindex, spindex};

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
unsigned int Execute(AllDecodeFields df, unsigned int regs[32], unsigned int pc);
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
	unsigned int x[32] = {0};
	unsigned int i;
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
	for (i = 0; i<100; i++){
		printf("Element [%d] = %x\n",i,inst_mem[i]);
	}
	unsigned int instruction[] = {10035,37878,65345};
	unsigned int IR = 0;

	//template
	x[0] = 664;
	x[spindex]	= sp;
	unsigned int readvaluetemp;
	AllDecodeFields decodedall;
	pc = 0;
	do
	{
		IR = inst_mem[pc/4];
		readvaluetemp = ReadMem(inst_mem, pc);
		printf("after readmem function: %u\n",readvaluetemp);
		printf("IR  : 0x%08x\n", IR);
		pc = pc + 4;
		decodedall = DecodeInst(IR);
		pc = Execute(decodedall, x, pc);
		//printf("%x\n",decodedall.opcode);
	}
	while(IR != 0x8067);  // 0x8067 for the time being. checking inst_mem
	WriteMem(instruction, 12, 65535);
	printf("after writemem : %u\n", instruction[12/4]);
	//DisplayRegs(x, pc);
	//printf("SP = 0x%08u\n", x[spindex]);	// spindex is stack pointer index
}

int DisplayRegs(unsigned int regs[32], unsigned int pc){
	int i = 0;
	printf("pc  : 0x%08x\n", pc);
	for(i = 0; i < 32; i++)
	{
		printf("x%2d : 0x%08x\n", i, regs[i]);
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
		case 0x33://for Rtype
			df.opcode	= opcode;
			df.rd		= (IR & rdmask)		>> 7;
			df.funct3	= (IR & funct3mask)	>> 12;
			df.rs1		= (IR & rs1mask)	>> 15;
			df.rs2		= (IR & rs2mask)	>> 20;
			df.funct7	= (IR & funct7mask)	>> 25;
			break;
		case 0x03://for Itype
		case 0x13:
		case 0x67:
			df.opcode	= opcode;
			df.rd		= (IR & rdmask)		>> 7;
			df.funct3	= (IR & funct3mask)	>> 12;
			df.rs1		= (IR & rs1mask)	>> 15;
			df.imm		= (IR & imm12Imask)	>> 20;
			break;
		case 65:// make thus for Stype
			df.opcode	= opcode;
			df.funct3	= (IR & funct3mask)	>> 12;
			df.rs1		= (IR & rs1mask)	>> 15;
			df.rs2		= (IR & rs2mask)	>> 20;
			df.imm		= ((IR & rdmask)	>> 7)|((IR & funct7mask)	>> 18);
			break;
		default:
			printf("Entered default\n");
			break;
	}
	return df;
}

unsigned int Execute(AllDecodeFields df, unsigned int regs[32], unsigned int pc){
	//printf("pc in Execute  : 0x%08x\n", pc);
	switch(df.opcode)
	{
		case 0x33://make this for Rtype
			switch(df.funct3)
			{
				case 0x0:
					if(df.funct7)
						regs[df.rd] = regs[df.rs1] - regs[df.rs2];
					else //check fields in specification
						regs[df.rd] = regs[df.rs1] + regs[df.rs2];
					break;
				default:
					break;
			}
			break;
		case 0x03://for Itype
		case 0x13:
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
			printf("Entered default\n");
			break;
	}
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