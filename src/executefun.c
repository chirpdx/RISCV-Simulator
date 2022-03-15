#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commondefs.h"

unsigned int Execute(AllDecodeFields df, unsigned int *umem, unsigned int regs[32], unsigned int pc){
#ifdef __verbose__
	printf("\npc in Execute  : 0x%08x\n", pc);
#endif
unsigned int tempadd = 0;
unsigned int tempmask = 0xffffffff;
unsigned int tempsignbit = 0x80000000;
unsigned int tempread = 0;
unsigned int tempstore = 0;
unsigned int signa = 0;
unsigned int signb = 0;
unsigned long long int tempmulresult = 0;
unsigned int a = 0;
unsigned int b = 0;
unsigned int bzero = 0;
unsigned int tempdivresult = 0;

	switch(df.opcode)
	{
		case opralu:
			#ifdef __VerboseDebug__
				printf("OP_ALU_R Opcode: 0x%02x funct3 = 0x%01x funct7 = 0x%02x   rs1 = x%2d rs2 = x%2d rd = x%2d\n",df.opcode,df.funct3,df.funct7,df.rs1,df.rs2,df.rd);
			#endif
			if(df.funct7 == 1)
			{
				signa = (regs[df.rs1] & 0x80000000) ? 1 : 0;
				signb = (regs[df.rs2] & 0x80000000) ? 1 : 0;
				bzero = (regs[df.rs2] == 0) ? 1 : 0;
				switch(df.funct3)
				{
					case 0x0://MUL
						regs[df.rd] = regs[df.rs1] * regs[df.rs2];
						break;
					case 0x1://MULH
						a = regs[df.rs1];
						b = regs[df.rs2];
						if(signa == 1)
						{
							a = ~a;
							a = a + 1;
						}
						if(signb == 1)
						{
							b = ~b;
							b = b + 1;
						}
						tempmulresult = (unsigned long long int)a * (unsigned long long int)b;
						if((signa+signb)%2 == 1)
						{
							tempmulresult = ~tempmulresult;
							tempmulresult = tempmulresult + 1;
						}
						regs[df.rd] = tempmulresult >> 32;
						break;
					case 0x2://MULHSU
						a = regs[df.rs1];
						b = regs[df.rs2];
						if(signa == 1)
						{
							a = ~a;
							a = a + 1;
						}
						tempmulresult = (unsigned long long int)a * (unsigned long long int)b;
						if(signa == 1)
						{
							tempmulresult = ~tempmulresult;
							tempmulresult = tempmulresult + 1;
						}
						regs[df.rd] = tempmulresult >> 32;
						break;
					case 0x3://MULHU
						tempmulresult = (unsigned long long int)regs[df.rs1] * (unsigned long long int)regs[df.rs2];
						regs[df.rd] = tempmulresult >> 32;
						break;
					case 0x4://DIV
						a = regs[df.rs1];
						b = regs[df.rs2];
						if(signa == 1)
						{
							a = ~a;
							a = a + 1;
						}
						if(signb == 1)
						{
							b = ~b;
							b = b + 1;
						}
						if(bzero == 1)
						{
							tempdivresult = 0xffffffff;
						}
						else if((regs[df.rs1] == 0x80000000) && (regs[df.rs2] == 0xffffffff))
						{
							tempdivresult = 0x80000000;
						}
						else
						{
							tempdivresult = a/b;
							if((signa+signb)%2 == 1)
							{
								tempdivresult = ~tempdivresult;
								tempdivresult = tempdivresult + 1;
							}
						}
						regs[df.rd] = tempdivresult;
						break;
					case 0x5://DIVU
						if(bzero == 1)
						{
							tempdivresult = 0xffffffff;
						}
						else
						{
							tempdivresult = regs[df.rs1]/regs[df.rs2];
						}
						regs[df.rd] = tempdivresult;
						break;
					case 0x6://REM
						a = regs[df.rs1];
						b = regs[df.rs2];
						if(signa == 1)
						{
							a = ~a;
							a = a + 1;
						}
						if(signb == 1)
						{
							b = ~b;
							b = b + 1;
						}
						if(bzero == 1)
						{
							tempdivresult = regs[df.rs1];
						}
						else if((regs[df.rs1] == 0x80000000) && (regs[df.rs2] == 0xffffffff))
						{
							tempdivresult = 0;
						}
						else
						{
							tempdivresult = a%b;
							if(signa == 1)
							{
								tempdivresult = ~tempdivresult;
								tempdivresult = tempdivresult + 1;
							}
						}
						regs[df.rd] = tempdivresult;
						break;
					case 0x7://REMU
						if(bzero == 1)
						{
							tempdivresult = regs[df.rs1];
						}
						else
						{
							tempdivresult = a%b;
						}
						regs[df.rd] = tempdivresult;
						break;
					default:
						printf("Entered default in alu register M extension instruction in funct3 when funct7 is 0x1\n");
						exit(0);
						break; 
				}
			}
			else
			{
			switch(df.funct3)
			{
				case 0x0:
					switch(df.funct7)
					{
						case 0x0:
							regs[df.rd] = regs[df.rs1] + regs[df.rs2];
							break;
						case 0x20:
							regs[df.rd] = regs[df.rs1] - regs[df.rs2];
							break;
						default:
							printf("Entered default in alu register instruction add sub funct7\n");
							exit(0);
							break;
					}
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
					regs[df.rd] = regs[df.rs1] << (regs[df.rs2] & 0x0000001f);
					break;
				case 0x5:
					switch(df.funct7)
					{
						case 0x0:
							regs[df.rd] = regs[df.rs1] >> (regs[df.rs2] & 0x0000001f);
							break;
						case 0x20:
							regs[df.rd] = (signed int)regs[df.rs1] >> (regs[df.rs2] & 0x0000001f);
							break;
						default:
							printf("Entered default in alu register instruction shift right funct7\n");
							exit(0);
							break;
					}
					break;
				case 0x2:
					regs[df.rd] = ((signed int)regs[df.rs1] < (signed int)regs[df.rs2]) ? 1 : 0;
					break;
				case 0x3:
					regs[df.rd] = (regs[df.rs1] < regs[df.rs2]) ? 1 : 0;
					break;

				default:
					printf("Entered default in alu register instruction funct3\n");
					exit(0);
					break;
				}
			}
			break;

		case opialu:
			#ifdef __VerboseDebug__
				printf("OP_ALU_I Opcode: 0x%02x funct3 = 0x%01x imm(hex) = 0x%08x imm(decimal) = %d  rs1 = x%2d  rd = x%2d\n",df.opcode,df.funct3,df.imm,df.imm,df.rs1,df.rd);
			#endif
			switch(df.funct3)
			{
				case 0x0:
					regs[df.rd] = regs[df.rs1] + df.imm;
					break;
				case 0x4:
					regs[df.rd] = regs[df.rs1] ^ df.imm;
					break;
				case 0x6:
					regs[df.rd] = regs[df.rs1] | df.imm;
					break;
				case 0x7:
					regs[df.rd] = regs[df.rs1] & df.imm;
					break;
				case 0x1:
					if ((df.imm & 0x00000fe0) == 0)
						regs[df.rd] = regs[df.rs1] << (df.imm & 0x0000001f);
					break;
				case 0x5:
					if ((df.imm & 0x00000fe0) == 0)
						regs[df.rd] = regs[df.rs1] >> (df.imm & 0x0000001f);
					else if ((df.imm & 0x00000fe0) == 0x400)
						regs[df.rd] = (regs[df.rs1] & 0x80000000) ? ((signed int)regs[df.rs1] >> (df.imm & 0x0000001f)) : (regs[df.rs1] >> (df.imm & 0x0000001f));
					else
					{
						printf("Entered default in alu immediate instruction shift right funct7\n");
						exit(0);
					}
					break;
				case 0x2:
					regs[df.rd] = ((signed int)regs[df.rs1] < (signed int)df.imm) ? 1:0;
					break;
				case 0x3:
					df.imm = df.imm & 0x00000fff;
					regs[df.rd] = (regs[df.rs1] < df.imm) ? 1:0;
					break;

				default:
					printf("Entered default in alu immediate instruction funct3\n");
					exit(0);
					break;
			}
			break;

		case opload:
			#ifdef __VerboseDebug__
				printf("OP_LOADS Opcode: 0x%02x funct3 = 0x%01x imm(hex) = 0x%x imm(decimal) = %d  rs1 = x%2d  rd = x%2d\n",df.opcode,df.funct3,df.imm,df.imm,df.rs1,df.rd);
			#endif
			tempadd = regs[df.rs1]+df.imm;
			tempread = ReadMem(umem,tempadd);
			switch(df.funct3)
			{
				case 0x0:
					switch(tempadd%4)
					{
						case 0: tempmask = 0x000000ff;
								tempsignbit = 0x00000080;
								regs[df.rd] = (tempread & tempsignbit) ? ((tempread & tempmask) | 0xffffff00) : (tempread & tempmask);
							break;
						case 1: tempmask = 0x0000ff00;
								tempsignbit = 0x00008000;
								regs[df.rd] = (tempread & tempsignbit) ? (((tempread & tempmask) >> 8)| 0xffffff00) : ((tempread & tempmask) >> 8);
							break;
						case 2: tempmask = 0x00ff0000;
								tempsignbit = 0x00800000;
								regs[df.rd] = (tempread & tempsignbit) ? (((tempread & tempmask) >> 16)| 0xffffff00) : ((tempread & tempmask) >> 16);
							break;
						case 3: tempmask = 0xff000000;
								tempsignbit = 0x80000000;
								regs[df.rd] = (tempread & tempsignbit) ? (((tempread & tempmask) >> 24)| 0xffffff00) : ((tempread & tempmask) >> 24);
							break;
						default:
							break;
					}
					break;
				case 0x1:
					switch(tempadd%4)
					{
						case 0: tempmask = 0x0000ffff;
								tempsignbit = 0x00008000;
								regs[df.rd] = (tempread & tempsignbit) ? ((tempread & tempmask) | 0xffff0000) : (tempread & tempmask);
							break;
						case 2: tempmask = 0xffff0000;
								tempsignbit = 0x80000000;
								regs[df.rd] = (tempread & tempsignbit) ? (((tempread & tempmask) >> 16)| 0xffff0000) : ((tempread & tempmask) >> 16);
							break;
						default:
							printf("Error : Non aligned address 0x%08x for 2 byte data loading\n", tempadd);
							break;
					}
					break;
				case 0x2:
					if(tempadd%4 == 0)
						regs[df.rd] = tempread;
					else
						printf("Error : Non aligned address 0x%08x for 4 byte data loading\n", tempadd);
					break;
				case 0x4:
					switch(tempadd%4)
					{
						case 0: tempmask = 0x000000ff;
								regs[df.rd] = (tempread & tempmask);
							break;
						case 1: tempmask = 0x0000ff00;
								regs[df.rd] = ((tempread & tempmask) >> 8);
							break;
						case 2: tempmask = 0x00ff0000;
								regs[df.rd] = ((tempread & tempmask) >> 16);
							break;
						case 3: tempmask = 0xff000000;
								regs[df.rd] = ((tempread & tempmask) >> 24);
							break;
						default:
							break;
					}
					break;
				case 0x5:
					switch(tempadd%4)
					{
						case 0: tempmask = 0x0000ffff;
								regs[df.rd] = (tempread & tempmask);
							break;
						case 2: tempmask = 0xffff0000;
								regs[df.rd] = ((tempread & tempmask) >> 16);
							break;
						default:
							printf("Error : Non aligned address 0x%08x for 2 byte data loading\n", tempadd);
							break;
					}
					break;
				default:
					printf("Entered default in load instruction funct3\n");
					exit(0);
					break;
			}
			break;

		case opstore:
			#ifdef __VerboseDebug__
				printf("OP_STORESS Opcode: 0x%02x funct3 = 0x%01x imm(hex) = 0x%x imm(decimal) = %d  rs1 = x%2d rs2 = x%2d\n",df.opcode,df.funct3,df.imm,df.imm,df.rs1,df.rs2);
			#endif
			tempstore = 0;
			tempadd = 0;
			tempadd = regs[df.rs1]+df.imm;
			switch(df.funct3)
			{
				case 0x0:
					switch(tempadd%4)
					{
						case 0: tempstore = (regs[df.rs2] & 0x000000ff);
							break;
						case 1: tempstore = (regs[df.rs2] & 0x000000ff) << 8;
							break;
						case 2: tempstore = (regs[df.rs2] & 0x000000ff) << 16;
							break;
						case 3: tempstore = (regs[df.rs2] & 0x000000ff) << 24;
							break;
						default:
							break;
					}
					WriteMem(umem,tempadd,tempstore);
					//WriteMem(umem,df.rs1+df.imm,(*(umem+(df.rs1+df.imm)/4)&ffff00)|(regs[df.rs2] & 0xff))
					//*(umem+(df.rs1+df.imm)/4) & 0x80 ? ((*(umem+(df.rs1+df.imm)/4))&0xff)|0xffff00 : (*(umem+(df.rs1+df.imm)/4))&0xff = regs[df.rs2] & 0xff;
					break;
				case 0x1:
					switch(tempadd%4)
					{
						case 0: tempstore = (regs[df.rs2] & 0x0000ffff);
							break;
						case 2: tempstore = (regs[df.rs2] & 0x0000ffff) << 16;
							break;
						default:
							printf("Error : Non aligned address 0x%08x for 2 byte data storing\n", tempadd);
							break;
					}
					WriteMem(umem,tempadd,tempstore);
					//WriteMem(umem,df.rs1+df.imm,(*(umem+(df.rs1+df.imm)/4)&ff0000)|(regs[df.rs2] & 0xffff))
					//*(umem+(df.rs1+df.imm)) & 0x8000 ? ((*(umem+(df.rs1+df.imm)))&0xffff)|0xff0000 : (*(umem+(df.rs1+df.imm)))&0xffff = regs[df.rs2] & 0xffff;
					break;
				case 0x2:
					if(tempadd%4 == 0)
						WriteMem(umem,tempadd,regs[df.rs2]);
					else
						printf("Error : Non aligned address 0x%08x for 4 byte data storing\n", tempadd);
					//(*(umem+(df.rs1+df.imm)))&0xffffff = regs[df.rs2] & 0xffffff;
					break;

				default:
					printf("Entered default in store instruction funct3\n");
					exit(0);
					break;
			}
			break;

		case opbranch:
			#ifdef __VerboseDebug__
				printf("OP_BRANCHS Opcode: 0x%02x funct3 = 0x%01x imm(hex) = 0x%x imm(decimal) = %d  rs1 = x%2d  rs2 = x%2d \n",df.opcode,df.funct3,df.imm,df.imm,df.rs1,df.rs2);
			#endif
			switch(df.funct3)
			{
				case 0x0:
					if ((signed int)regs[df.rs1] == (signed int)regs[df.rs2])
						pc = pc + df.imm - 4;
					break;
				case 0x1:
					if ((signed int)regs[df.rs1] != (signed int)regs[df.rs2])
						pc = pc + df.imm - 4;
					break;
				case 0x4:
					if ((signed int)regs[df.rs1] < (signed int)regs[df.rs2])
						pc = pc + df.imm - 4;
					break;
				case 0x5:
					if ((signed int)regs[df.rs1] >= (signed int)regs[df.rs2])
						pc = pc + df.imm - 4;
					break;
				case 0x6:
					if (regs[df.rs1] < regs[df.rs2])
						pc = pc + df.imm - 4;
					break;
				case 0x7:
					if (regs[df.rs1] >= regs[df.rs2])
						pc = pc + df.imm - 4;
					break;

				default:
					printf("Entered default in branch instruction funct3\n");
					exit(0);
					break;
			}
			break;
		case opj:
			#ifdef __VerboseDebug__
				printf("OP_JUMP Opcode: 0x%02x imm(hex) = 0x%x imm(decimal) = %d  rd = x%2d \n",df.opcode,df.imm,df.imm,df.rd);
			#endif
			regs[df.rd] = pc + 4;
			pc = pc + df.imm - 4;
			break;
		case opji:
			#ifdef __VerboseDebug__
				printf("OP_JUMPI Opcode: 0x%02x imm(hex) = 0x%x imm(decimal) = %d  rd = x%2d rs1 = x%2d \n",df.opcode,df.imm,df.imm,df.rd, df.rs1);
			#endif
			switch(df.funct3)
			{
				case 0x0:
					regs[df.rd] = pc + 4;
					pc = regs[df.rs1] + df.imm - 4;
					break;
				default:
					printf("Entered default in jump instruction funct3\n");
					exit(0);
					break;
			}
			break;
		case oplui:
			#ifdef __VerboseDebug__
				printf("LUI Opcode: 0x%02x imm(hex) = 0x%x imm(decimal) = %d  rd = x%2d \n",df.opcode,df.imm,df.imm,df.rd);
			#endif
			regs[df.rd] = df.imm;
			break;
		case opauipc:
			#ifdef __VerboseDebug__
				printf("AUIPC Opcode: 0x%02x imm(hex) = 0x%x imm(decimal) = %d  rd = x%2d \n",df.opcode,df.imm,df.imm,df.rd);
			#endif
			regs[df.rd] = pc +  df.imm;
			break;
		default:
			printf("Entered default in Execute\n");
			exit(0);
			break;
	}
	//printf("Returning from Execute p = %x\n",pc);
	return pc;
}