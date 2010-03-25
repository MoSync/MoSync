/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

//*********************************************************************************************
//				       			MoSync Code Tools API
//*********************************************************************************************

#if 1

#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <memory.h>
#include <string.h>

#include "helpers/TranslateSyscall.h"

#include "disas.h"

//****************************************
//		 Instruction descripters
//****************************************
enum
{
	_NOP = 0,
	_PUSH,
	_POP,
	_CALL,
	_CALLI,
	_LDB,
	_STB,
	_LDH,
	_STH,
	_LDW,
	_STW,
	_LDI,
	_LDR,
	_ADD,
	_ADDI,
	_MUL,
	_MULI,
	_SUB,
	_SUBI,
	_AND,
	_ANDI,
	_OR,
	_ORI,
	_XOR,
	_XORI,
	_DIVU,
	_DIVUI,
	_DIV,
	_DIVI,
	_SLL,
	_SLLI,
	_SRA,
	_SRAI,
	_SRL,
	_SRLI,
	_NOT,
	_NEG,
	_RET,	
	_JC_EQ,
	_JC_NE,
	_JC_GE,
	_JC_GEU,
	_JC_GT,
	_JC_GTU,
	_JC_LE,
	_JC_LEU,
	_JC_LT,
	_JC_LTU,
	_JPI,
	_JPR,
	_XB,
	_XH,
	_SYSCALL,
	_CASE,
	_FAR,
	_ENDOP
};

//****************************************
// 			Register enums
//****************************************
enum
{
	REG_zero = 0,
	REG_sp,
	REG_rt,
	REG_fr,
	REG_d0,
	REG_d1,
	REG_d2,
	REG_d3,
	REG_d4,
	REG_d5,
	REG_d6,
	REG_d7,
	REG_i0,
	REG_i1,
	REG_i2,
	REG_i3,

	REG_r0,
	REG_r1,
	REG_r2,
	REG_r3,
	REG_r4,
	REG_r5,
	REG_r6,
	REG_r7,
	REG_r8,
	REG_r9,
	REG_r10,
	REG_r11,
	REG_r12,
	REG_r13,
	REG_r14,
	REG_r15
};

char *Disas_CodeMemory;
int *Disas_ConstPool;
int Disas_RegConstTable[128];

//****************************************
//			Build Fetch Table
//****************************************

#undef INSTR_MACRO
#define INSTR_MACRO(op_far, op_num, op_str, op_jstr, op_fetch, op_extra)	op_fetch,

int DisasFetch[256] =
{
	#include "InstTable.h"
};

//****************************************
// 		Build Opcode Strings Table
//****************************************

#undef INSTR_MACRO
#define INSTR_MACRO(op_far, op_num, op_str, op_jstr ,op_fetch, op_extra)	op_str,

const char *DisasStrings[256] =
{
	#include "InstTable.h"
};



//***************************************
//
//***************************************

void DisasGenReg()
{
	int n, p;
	int mask;

	p = 32;
		
	memset(Disas_RegConstTable,0, 128 * sizeof(int));

	for (n=1;n<17;n++)
	{
		Disas_RegConstTable[p++] = n;
		Disas_RegConstTable[p++] = -n;
	}

	mask = 0x20;

	for (n=0;n<32-5;n++)
	{
		Disas_RegConstTable[p++] = mask-1;
		Disas_RegConstTable[p++] = mask;
		mask <<= 1;
	}

	mask = 0x10;

	for (n=0;n<10;n++)
	{
		Disas_RegConstTable[p++] = mask ^ 0xffffffff;
		mask <<= 1;
	}

	return;
}

//****************************************
//		   Decode an opcode
//****************************************

void DisasInit(char *codemem, int *constpool)
{
	Disas_CodeMemory = codemem;
	Disas_ConstPool = constpool;

	DisasGenReg();
}

//****************************************
//		   Decode an opcode
//****************************************

const uchar * DisasDecode(DisasInfo *thisOpcode, const uchar *code_ip)
{
	uchar thisOp;
	uchar thisRD=0,thisRS=0;
	uint thisIMM=0;
	uint rip = (int) code_ip - (int) Disas_CodeMemory;
	
	const uchar *start_code_ip = code_ip;		// Make a copy
	
	int farflag = 0;
	int flags;

	thisOp = *code_ip++;

	if (thisOp > _FAR)
		return 0;

	// Decode the op
	
	if (thisOp == _FAR)
	{
		thisOp = *code_ip++;
		farflag = 1;
	}

	if (thisOp >= _FAR)
		return 0;
	
	flags = DisasFetch[thisOp];
		
	// Fetch register info first
	
	if (flags & fetch_d)
		thisRD = *code_ip++;

	if (flags & fetch_s)
		thisRS = *code_ip++;

	// rest is either or

	if (flags & fetch_a)
	{
		if (farflag)
		{
			thisIMM = (*code_ip++ << 16);
			thisIMM += (*code_ip++ << 8);
			thisIMM += *code_ip++;
		}
		else
		{
			thisIMM = *code_ip++ << 8;
			thisIMM += *code_ip++;
		}
	}

	if (flags & fetch_c)
	{
		thisIMM = (*code_ip++ << 16);
		thisIMM += (*code_ip++ << 8);
		thisIMM += *code_ip++;
	}

	if (flags & (fetch_j | fetch_k))
		thisIMM = *code_ip++;

	if (flags & fetch_i)
	{
		thisIMM = *code_ip++;
		
		if(thisIMM > 127)
		{
			thisIMM = ((thisIMM & 127) << 8) + *code_ip++;
		}
	}

	if (flags & fetch_i)
		thisIMM = Disas_ConstPool[thisIMM];
	
	// ** Special case for SysCalls **
	// which marks that they use R14
	
	if (thisOp == _SYSCALL)
	{
		flags |= fetch_d;
		thisRD = REG_r14;
	}

	// Save info

	thisOpcode->flags	= flags;
	thisOpcode->farflag	= farflag;
	thisOpcode->op		= thisOp;
	thisOpcode->rd		= thisRD;
	thisOpcode->rs		= thisRS;
	thisOpcode->imm		= thisIMM;
	thisOpcode->rip		= rip;
	thisOpcode->str		= DisasStrings[thisOp];
	thisOpcode->len		= code_ip - start_code_ip;
	
	return code_ip;
}

//****************************************
//		   Decode an opcode
//****************************************

int DisasDecodeIP(DisasInfo *thisOpcode, int code_ip)
{
	const uchar *ip = (uchar *) &Disas_CodeMemory[code_ip];
	const uchar *sip = ip;

	ip = DisasDecode(thisOpcode, ip);
	
	code_ip += (int) (ip - sip);
	return code_ip;
}

//***************************************
//        print to disassembler
//***************************************

void DisasEmit(char *out, const char *Template, ...)
{
		char 	Str[1280];
		va_list args;

		va_start(args,Template);
		vsprintf(Str,Template,args);
		va_end(args);

		strcat(out,Str);
}

//****************************************
//			Get Reg Name
//****************************************

const char *disas_regs[] = {	"zr","sp","rt","fr","d0","d1","d2","d3",
						"d4","d5","d6","d7","i0","i1","i2","i3",
						"r0","r1","r2","r3","r4","r5","r6","r7",
						"r8","r9","r10","r11","r12","r13","r14","r15"
					 };

char regstr[256];

char * DisasRegName(int reg, int use_zero)
{
	regstr[0] = 0;

	if (reg & 0xffffff00)
		return regstr;

	if (use_zero && reg == 0)
	{
		sprintf(regstr, "#0");
		return regstr;
	}

	if (reg < 32)
	{
		strcpy(regstr, disas_regs[reg]);
		return regstr;
	}

	sprintf(regstr, "#0x%x", Disas_RegConstTable[reg] );
	return regstr;
}

//****************************************
//		Disassemble to string
//****************************************

void DisasString(DisasInfo *thisOpcode, char *out)
{
	short n,len;
	char c;

	out[0] = 0;

	len = strlen(thisOpcode->str);	
	
	for(n=0;n<len;n++)
	{
		c = thisOpcode->str[n];
	
		switch(c)
		{	
			case 'd': 		// Reg rd
			{
				DisasEmit(out,"%s",DisasRegName(thisOpcode->rd, 1));
			}
			break;

			case 's': 		// Reg rs
			{
				DisasEmit(out, "%s",DisasRegName(thisOpcode->rs, 1));
			}
			break;		

			case 'q': 		// Reg rs or DataAccess
			{
				DisasEmit(out, "%s",DisasRegName(thisOpcode->rs, 1));
			}
			break;		


			case 'i': 		// Immediate const
			case 'a': 		// Immediate address
			{
				DisasEmit(out,"#0x%x",thisOpcode->imm);
			}
			break;

			case 'c': 		// Immediate address				
				DisasEmit(out, "#0x%x", thisOpcode->imm);
			break;


			case 'j': 		// Immediate 8 bits
			{
				DisasEmit(out,"#0x%x",thisOpcode->imm);
			}
			break;

			case 'k': 		// Syscalls
			{
				const char* name = translateSyscall(thisOpcode->imm);
				if(name)
					DisasEmit(out,"%s",name);
				else
					DisasEmit(out,"%d",thisOpcode->imm);
			}		
			break;

			case 'm': 		// rs+imm	
			{
				DisasEmit(out,"%s,",DisasRegName(thisOpcode->rs, 0));
				DisasEmit(out,"%d", thisOpcode->imm);
			}		
			break;

			case 'n': 		// rd+imm	
			{
				DisasEmit(out,"%s,",DisasRegName(thisOpcode->rd, 0));
				DisasEmit(out,"%d", thisOpcode->imm);

			}		
			break;

			case 'x': 		// Push
			{
				DisasEmit(out,"%s,",DisasRegName(thisOpcode->rd, 0));
				DisasEmit(out,"%s",DisasRegName(thisOpcode->rd + thisOpcode->rs - 1, 0));
			}
			break;

			case 'y': 		// Pop
			{
				DisasEmit(out,"%s,", DisasRegName(thisOpcode->rd - thisOpcode->rs + 1, 0));
				DisasEmit(out,"%s",  DisasRegName(thisOpcode->rd, 0));
			}
			break;

			case  32: 		// Space
			case  9: 		// Space
			{
				DisasEmit(out," ");
			}
			break;

			default:
				if (isalpha(c))
					DisasEmit(out,"%c",tolower(c));
				else
					DisasEmit(out,"%c",c);

		}

	}
}

//****************************************
//
//****************************************

int DisasInst(int ip, char *out)
{
	DisasInfo thisOpcode;
	int new_ip;
	
	new_ip = DisasDecodeIP(&thisOpcode, ip);
	DisasString(&thisOpcode, out);
	return new_ip;
}

#endif
