#ifndef myheader
#define myheader

//#define __verbose__	//without single stepping
//#define __VerboseDebug__	//keep this also to make singlestepping and breakpoint active

#define opcodemask  0x0000007f
#define rdmask      0x00000f80
#define funct3mask  0x00007000
#define rs1mask     0x000f8000
#define rs2mask     0x01f00000
#define funct7mask  0xfe000f80
#define imm12Imask  0xfff00000

#define opralu 0x33
#define opialu 0x13
#define opload 0x03
#define opstore 0x23
#define opbranch 0x63
#define opj 0x6f
#define opji 0x67
#define oplui 0x37
#define opauipc 0x17

extern char regnames[32][5];

enum otherindexes{zeroindex, raindex, spindex, gpindex, tpindex, t0index, t1index, t2index, s0index, s1index,a0index, a1index, a2index, a3index, a4index, a5index, a6index, a7index, s2index, s3index, s4index, s5index, s6index, s7index, s8index, s9index, s10index, s11index, t3index, t4index, t5index, t6index};

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

#endif