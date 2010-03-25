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
//					       			MoSync Code Analyser
//*********************************************************************************************

#include "compile.h"

#if 0 /* OLD VERSION */

#define MAX_JUMP_TARGETS	100

int HitRet = 0;
int LogJmp = 0;

AnalyserInfo *AnalyseInfo;
AnalyserInfo AnalyseStack[MAX_JUMP_TARGETS];

int AnalyseSP = 0;
int AnalyseEntry = 0;

//****************************************
//		  Analyse Instruction
//****************************************

void OldAnalyseInit(AnalyserInfo *thisInfo, int ip)
{
	// Clear the master tracer info
	
	memset(thisInfo, 0, sizeof(AnalyserInfo));

	// reset the stacks
	
	AnalyseSP = 0;
	AnalyseEntry = 0;
	
	// Clear the tracer stack
	
	memset(AnalyseStack, 0, sizeof(AnalyseStack));

	thisInfo->ip = ip;
}

//****************************************
//
//****************************************

void OldAnalyserAddTarget(int ip)
{	
	int n;
	
	if (AnalyseEntry >= MAX_JUMP_TARGETS)
	{
		printf("out of analyser space\n");
		return;
	}
	
	for (n=0;n<AnalyseEntry;n++)
	{
		// Already in stack ? if so exit
		
		if (AnalyseStack[n].ip == ip)
			return;
	}
	
	memcpy(AnalyseStack[AnalyseEntry].regvalue, AnalyseInfo->regvalue, sizeof(int) * 32);
	AnalyseStack[AnalyseEntry].ip = ip;

	AnalyseEntry++;
}

//****************************************
//		  Select Analyse Info
//****************************************

void OldAnalyseSelect(AnalyserInfo *thisInfo)
{
	AnalyseInfo = thisInfo;
}

//****************************************
// 	 Mark a register has been used
//****************************************

void OldAnalyseMarkReg(int reg)
{
	if (reg >= 32)
		return;

	AnalyseInfo->regcount[reg]++;
}

//****************************************
//	  Find out if a reg is used/active
//****************************************

int OldAnalyseRegCount(int reg)
{
	if (reg >= 32)
		return 1;

	return AnalyseInfo->regcount[reg];
}

//****************************************
//		  Analyse Instruction
//****************************************

int OldAnalyseInstruction(OpcodeInfo *thisOpcode)
{
	int flags = thisOpcode->flags;
	//int imm = thisOpcode->imm;
	int g_rd,g_rs;
	int v;
		
	// Get dest reg statistics	

	if (flags & fetch_d)
	{	
		g_rd = thisOpcode->rd;

		// Check that destinatiom reg is not a contant
		
		if (g_rd >= 32)
		{
			printf("Verfier: Constant register cannot be a destination\n");
			return 0;
		}

		// Increment usage of register		

		AnalyseMarkReg(g_rd);

	}
	// Get source reg statistics	

	if (flags & fetch_s)
	{
		g_rs = thisOpcode->rs;

		// Check if register is defined, otherwise error

		if (!AnalyseRegCount(g_rs))
		{
			printf("Verfier: Source register used is in an undefined state\n");
			return 0;
		}
		
		// Increment usage of register		

		AnalyseMarkReg(g_rs);
	}

	v = AnalyseTrace(thisOpcode);
	return v;
}

//****************************************
//
//****************************************

extern int g_ConstTable[];

int OldAnalyseGetReg(int reg)
{
	if (reg >= 128)
	{
		printf("Verfier: Illegal register\n");
		return 0;
	}

	if (reg >= 32)
		return g_ConstTable[reg];

	return AnalyseInfo->regcount[reg];
}

//****************************************
//
//****************************************

void OldAnalyseSetReg(int reg, int value)
{
	if (reg >= 32)
	{
		printf("Verfier: Illegal set register\n");
		return;
	}

	AnalyseInfo->regcount[reg] = value;
}

//****************************************
//		  Analyse Trace
//****************************************

#define AOP(opcode)		case opcode:
#define AEND			break;
#define REGJUMP			AnalyserAddTarget(g_imm);

#define DU ((unsigned int) d)
#define SU ((unsigned int) s)
#define IU ((unsigned int) g_imm)

int OldAnalyseTrace(OpcodeInfo *thisOpcode)
{
	int flags = thisOpcode->flags;
	int g_imm = thisOpcode->imm;

	int s,d,sd;

	s = d = 0;

	if (flags & fetch_d)
		d = AnalyseGetReg(thisOpcode->rd);

	if (flags & fetch_s)
		s = AnalyseGetReg(thisOpcode->rs);

	sd = d;

	HitRet = -1;
	LogJmp = -1;
	
	switch (thisOpcode->op)
	{
		AOP(_ADD)	d += s;			AEND;
		AOP(_ADDI)	d += g_imm;		AEND;
		AOP(_SUB)	d -= s;			AEND;
		AOP(_SUBI)	d -= g_imm;		AEND;
		AOP(_MUL)	d *= s;			AEND;
		AOP(_MULI)	d *= g_imm;		AEND;
		AOP(_AND)	d &= s;			AEND;
		AOP(_ANDI)	d &= g_imm;		AEND;
		AOP(_OR)	d |= s;			AEND;
		AOP(_ORI)	d |= g_imm;		AEND;
		AOP(_XOR)	d ^= s;			AEND;
		AOP(_XORI)	d ^= g_imm;		AEND;
		AOP(_DIVU)	d = DU / SU;	AEND;
		AOP(_DIVUI)	d = DU / IU;	AEND;
		AOP(_DIV)	d /= s;			AEND;
		AOP(_DIVI)	d /= g_imm;		AEND;
		AOP(_SLL)	d = DU << SU;	AEND;
		AOP(_SLLI)	d = d << IU;	AEND;
		AOP(_SRA)	d >>= s;		AEND;
		AOP(_SRAI)	d >>= g_imm;		AEND;
		AOP(_SRL)	d = DU >> SU;	AEND;
		AOP(_SRLI)	d = DU >> IU;	AEND;
		AOP(_NOT)	d = ~s;			AEND;
		AOP(_NEG)	d = -s;			AEND;

		AOP(_PUSH)	
			AnalyseSetReg(REG_sp, AnalyseGetReg(REG_sp) - 4 * s);
		AEND;
		
		AOP(_POP)
			AnalyseSetReg(REG_sp, AnalyseGetReg(REG_sp) + 4 * s);
		AEND;

		AOP(_CALL)					AEND;
		AOP(_CALLI)					AEND;

		AOP(_LDB)	d = 0xffffffff;	AEND;
		AOP(_LDH)	d = 0xffffffff;	AEND;
		AOP(_LDW)	d = 0xffffffff;	AEND;
		AOP(_STB)					AEND;
		AOP(_STH)					AEND;
		AOP(_STW)					AEND;

		AOP(_LDI)	d = g_imm;		AEND;
		AOP(_LDR)	d = s;			AEND;

		AOP(_RET)	HitRet=1;		AEND;

		AOP(_JC_EQ)	 REGJUMP		AEND;
		AOP(_JC_NE)	 REGJUMP 		AEND;
		AOP(_JC_GE)	 REGJUMP 		AEND;
		AOP(_JC_GT)	 REGJUMP 		AEND;
		AOP(_JC_LE)	 REGJUMP 		AEND;
		AOP(_JC_LT)	 REGJUMP 		AEND;

		AOP(_JC_LTU) REGJUMP 		AEND;
		AOP(_JC_GEU) REGJUMP 		AEND;
		AOP(_JC_GTU) REGJUMP 		AEND;
		AOP(_JC_LEU) REGJUMP 		AEND;

		AOP(_JPI)	 REGJUMP 		AEND;
		AOP(_JPR)					AEND;

		AOP(_XB) d = (int)((char) s);	AEND;
		AOP(_XH) d = (int)((short) s);	AEND;

		AOP(_SYSCALL)				AEND;
		AOP(_CASE)					AEND;

	default:
		printf("Bad instruction in analyser %d\n",thisOpcode->op);
		return 0;

	}

	if (flags & fetch_d)
		if (sd != d)
			AnalyseSetReg(thisOpcode->rd, d);

	return 1;
}

#endif

//*********************************************************************************************
//	New Version
//*********************************************************************************************

#undef CODE_ANALYSIS
#ifdef CODE_ANALYSIS

//****************************************
//
//****************************************

#define DEBUG_CODEFLOW

//****************************************
//
//****************************************

#define MAX_LOOP_LIST 1024

SYMBOL *g_BaseSym = 0;
int g_BaseIP = 0;
int g_BaseEIP = 0;

int g_LoopIP[MAX_LOOP_LIST];
int g_PrevLoopIP[MAX_LOOP_LIST];
int g_LoopCountIP[MAX_LOOP_LIST];

int g_LoopCount;

//****************************************
//
//****************************************

void AnalyseSetFunctionBase(SYMBOL *sym)
{
#ifdef DEBUG_CODEFLOW
	printf("Tracing CodeFlow: %s\n", sym->Name);
#endif

	g_BaseIP  = sym->Value;
	g_BaseEIP = sym->EndIP;

	if (sym->Type != SECT_code)
		ErrorOnIP(Error_Fatal, g_BaseIP, "SetFunctionBase: symbol not in code");

	g_BaseSym = sym;
	
	memset(g_LoopCountIP, 0, sizeof(g_LoopCountIP));
	g_LoopCount = 0;
}

//****************************************
//
//****************************************

int AnalyseCheckLoopPoints(int ip, int prev_ip)
{
	int n;
	
	// Search for loop matches

	for (n=0;n<g_LoopCount;n++)
	{
		if ((g_LoopIP[n] == ip) && (g_PrevLoopIP[n] == prev_ip))
		{
			if (g_LoopCountIP[n]++ > 10)
				return 1;

			return 0;
		}
	}

	// Add a loop to the list

	g_LoopIP[n] = ip;
	g_PrevLoopIP[n] = prev_ip;
	g_LoopCountIP[n] = 0;

	if (g_LoopCount == MAX_LOOP_LIST)
		ErrorOnIP(Error_Fatal, ip, "CheckLoopPoints: reached MAX_LOOP_LIST");

	g_LoopCount++;
	return 0;
}

//****************************************
//
//****************************************

char DisStr[1024];

void AnalyseComputePath(int ip, int prev_ip, int prev_link, int depth, int *in_regs)
{
	OpcodeInfo thisOp;
	int regvalue[32];
	int n;
	
	// Sanity checking

	if (depth > 64)
		ErrorOnIP(Error_System, ip, "ComputePath: depth exceeded");

	// Copy the incoming register set to the local register set

	memcpy(regvalue, in_regs, sizeof(regvalue));

	// Since prev_ip is the source and ip is the destination,
	// check if the we have been encountered them before.

	if (prev_ip)
		if (AnalyseCheckLoopPoints(ip, prev_ip))
			return;

	// Ok this path has not been encountered before
	
	while(1)
	{		
		if (ip > g_BaseEIP)
			break;
	
		prev_ip = ip;
		ip = DecodeOpcodeIP(&thisOp, ip);

#ifdef DEBUG_CODEFLOW
		DecodeAsmLabel(prev_ip, DisStr);

		if (DisStr[0])
			printf("%s:\n", DisStr);

		for (n=0;n<depth+16;n++)
			printf(" ");
	
		DecodeAsmString(&thisOp, DisStr);
		printf("%d: %s\n", prev_ip, DisStr);
#endif
		
		switch (thisOp.op)
		{
			case _JC_EQ:
			case _JC_NE:
			case _JC_GE:
			case _JC_GEU:
			case _JC_GT:
			case _JC_GTU:
			case _JC_LE:
			case _JC_LEU:
			case _JC_LT:
			case _JC_LTU:
			{
				int v = 0;
				int g_rd = AnalyseGetReg(regvalue, thisOp.rd);
				int g_rs = AnalyseGetReg(regvalue, thisOp.rs);

				if ((thisOp.op == _JC_EQ) && (g_rd == g_rs)) v = 1;
				if ((thisOp.op == _JC_NE) && (g_rd != g_rs)) v = 1;
				if ((thisOp.op == _JC_GT) && (g_rd >  g_rs)) v = 1;
				if ((thisOp.op == _JC_GE) && (g_rd >= g_rs)) v = 1;
				if ((thisOp.op == _JC_LT) && (g_rd <  g_rs)) v = 1;
				if ((thisOp.op == _JC_LE) && (g_rd <= g_rs)) v = 1;

				if ((thisOp.op == _JC_GTU) && ((uint) g_rd >  (uint) g_rs)) v = 1;
				if ((thisOp.op == _JC_GEU) && ((uint) g_rd >= (uint) g_rs)) v = 1;
				if ((thisOp.op == _JC_LTU) && ((uint) g_rd <  (uint) g_rs)) v = 1;
				if ((thisOp.op == _JC_LEU) && ((uint) g_rd <= (uint) g_rs)) v = 1;

				// Compute the alternative path from the jump
				if (v)
					AnalyseComputePath(thisOp.imm, prev_ip, 0, depth+1, regvalue);		

				break;
			}

			case _JPI:
			{
				// Follow the jump ip

				ip = thisOp.imm;

				// Make sure this is not infinite
				
				if (AnalyseCheckLoopPoints(ip, prev_ip))
					return;

				break;
			}

			case _CASE:
			{
				int start, len, data_ip, def_ip, i;
				int lab_ip;
				
				data_ip = thisOp.imm;

				start	= GetDataMemLong(data_ip++);
				len		= GetDataMemLong(data_ip++);
				def_ip	= GetDataMemLong(data_ip++);	// default case

				for (i=0;i<len+1;i++)
				{
					lab_ip = GetDataMemLong(data_ip++);

					// Follow this case
					AnalyseComputePath(lab_ip, prev_ip, 0, depth+1, regvalue);
				}

				// Follow the default ip

				ip = def_ip;
				break;
			}

			case _RET:
				return;			// Return we are done

			default:
				AnalyseTrace(&thisOp, regvalue);
				break;
		}
	}
	return;
}

//****************************************
//
//****************************************

extern int g_ConstTable[];

int AnalyseGetReg(int *reglist, int reg)
{
	int v;
	
	if (reg >= 128)
	{
		printf("AnalyseGetReg: Illegal register\n");
		return 0;
	}

	if (reg >= 32)
		v = g_ConstTable[reg];
	else
		v = reglist[reg];
	
	printf("get %s = %d\n", DecodeRegName(reg, 0), v);
	return v;
}

//****************************************
//
//****************************************

void AnalyseSetReg(int *reglist, int reg, int value)
{
	if (reg >= 32)
	{
		printf("AnalyseSetReg: Illegal set register\n");
		return;
	}

	reglist[reg] = value;

#ifdef DEBUG_CODEFLOW
	printf("set %s = %d\n", DecodeRegName(reg, 0), value);
#endif
}

//****************************************
//		      Trace
//****************************************

#define AOP(opcode)		case opcode:
#define AEND			break;
#define REGJUMP			AnalyserAddTarget(g_imm);

#define DU ((unsigned int) d)
#define SU ((unsigned int) s)
#define IU ((unsigned int) g_imm)

int AnalyseTrace(OpcodeInfo *thisOp, int *reglist)
{
	int flags = thisOp->flags;
	int g_imm = thisOp->imm;

	int s,d,sd,sp;

	s = d = 0;

	sp = reglist[REG_sp];

	if (flags & fetch_d)
		d = AnalyseGetReg(reglist, thisOp->rd);

	if (flags & fetch_s)
		s = AnalyseGetReg(reglist, thisOp->rs);

	sd = d;
	
	switch (thisOp->op)
	{
		AOP(_ADD)	d += s;						AEND;
		AOP(_ADDI)	d += g_imm;					AEND;
		AOP(_SUB)	d -= s;						AEND;
		AOP(_SUBI)	d -= g_imm;					AEND;
		AOP(_MUL)	d *= s;						AEND;
		AOP(_MULI)	d *= g_imm;					AEND;
		AOP(_AND)	d &= s;						AEND;
		AOP(_ANDI)	d &= g_imm;					AEND;
		AOP(_OR)	d |= s;						AEND;
		AOP(_ORI)	d |= g_imm;					AEND;
		AOP(_XOR)	d ^= s;						AEND;
		AOP(_XORI)	d ^= g_imm;					AEND;
		AOP(_DIVU)	d = DU / SU;				AEND;
		AOP(_DIVUI)	d = DU / IU;				AEND;
		AOP(_DIV)	d /= s;						AEND;
		AOP(_DIVI)	d /= g_imm;					AEND;
		AOP(_SLL)	d = DU << SU;				AEND;
		AOP(_SLLI)	d = d << IU;				AEND;
		AOP(_SRA)	d >>= s;					AEND;
		AOP(_SRAI)	d >>= g_imm;					AEND;
		AOP(_SRL)	d = DU >> SU;				AEND;
		AOP(_SRLI)	d = DU >> IU;				AEND;
		AOP(_NOT)	d = ~s;						AEND;
		AOP(_NEG)	d = -s;						AEND;
		AOP(_PUSH)	sp -= 4 * s;				AEND;
		AOP(_POP)	sp += 4 * s;				AEND;
		AOP(_CALL)								AEND;
		AOP(_CALLI)								AEND;
		AOP(_LDB)	d = 0xffffffff;				AEND;
		AOP(_LDH)	d = 0xffffffff;				AEND;
		AOP(_LDW)	d = 0xffffffff;				AEND;
		AOP(_STB)								AEND;
		AOP(_STH)								AEND;
		AOP(_STW)								AEND;
		AOP(_LDI)	d = g_imm;					AEND;
		AOP(_LDR)	d = s;						AEND;
		AOP(_XB)		d = (int)((char) s);	AEND;
		AOP(_XH)		d = (int)((short) s);	AEND;
		AOP(_SYSCALL)	d = 0xffffffff;			AEND;
		//AOP(_CASE)							AEND;
		//AOP(_JPR)								AEND;

	default:
		printf("Bad instruction in analyser %d\n",thisOp->op);
		return 0;
	}

	if (flags & fetch_d)
		AnalyseSetReg(reglist, thisOp->rd, d);

	reglist[REG_sp] = sp;
	return 1;
}

//****************************************
//
//****************************************

void TestAnalyse(SYMBOL *func)
{
	int regvalue[32];

//	SYMBOL * func;
//	func	= GetGlobalSym("_x");

	memset(regvalue, 0, sizeof(regvalue));

	if (func)
	{
		AnalyseSetFunctionBase(func);
		AnalyseComputePath(g_BaseIP, 0, 0, 0, regvalue);
	}
//	Error(Error_System,"Finished Analysis\n");
}

//****************************************

#endif //CODE_ANALYSIS
