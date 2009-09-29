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
//				  			  	  Brew Rebuilder
// 						   		Written by A.R.Hartley
//*********************************************************************************************

#include "compile.h"

// fix >>>
// put in prototypes
// remove static


#ifdef INCLUDE_BREW_REBUILD

#define BREW_DEBUG

#define REGBIT(v)		(1 << v)
#define REGUSED(v, reg) (reg & REGBIT(v))

static int ThisFunctionRegs;			// Register usage for current function
static int ThisFunctionRetType;
static int ThisFunctionExit;			// True on last instruction
static int ReturnCount;

static int BrewUsedCallReg;

static char *brew_reg[] = {"zr","sp","rt","fr","d0","d1","d2","d3",
					"d4","d5","d6","d7","i0","i1","i2","i3",
					"r0","r1","r2","r3","r4","r5","r6","r7",
					"r8","r9","r10","r11","r12","r13","r14","r15"
				   };

static char BrewSyscallUsed[1024];

//****************************************
//			 
//****************************************

int RebuildBrewInst(OpcodeInfo *theOp)
{
	int ip = theOp->rip;
	char str[256];

#ifdef BREW_DEBUG
	str[0] = 0;
	DisassembleFromSource(ip, str);
	RebuildEmit("\t\t\t\t\t\t//%s\n", str);
#endif

	switch (theOp->op)
	{
		case _PUSH:
//			RebuildEmit("	//push");
		return 1;
			
		case _POP:
//			RebuildEmit("	//pop");
		return 1;

		case _CASE:
			BrewDecodeSwitch(theOp);
		break;


		case _CALLI:
			BrewDecodeCall(theOp);
		break;

		case _SYSCALL:
			BrewDecodeSysCall(theOp);
		break;

		case _CALL:
			BrewDecodeCallReg(theOp);
		break;

		case _LDI:
			RebuildEmit("	%s = 0x%x;", brew_reg[theOp->rd], theOp->imm);			
		break;

		case _LDR:
		{
			if (IsRegConst(theOp->rs))
				RebuildEmit("	%s = 0x%x;", brew_reg[theOp->rd], ConstRegValue(theOp->rs));
			else
				RebuildEmit("	%s = %s;", brew_reg[theOp->rd], brew_reg[theOp->rs]);		
		}
		break;

		// Arithmatic
		
		case _ADD:
			BrewEmitArith(theOp,"+", 0);
		break;

		case _ADDI:
			BrewEmitArith(theOp,"+", 1);
		break;

		case _MUL:
			BrewEmitArith(theOp,"*", 0);
		break;

		case _MULI:
			BrewEmitArith(theOp,"*", 1);
		break;

		case _SUB:
			BrewEmitArith(theOp,"-", 0);
		break;

		case _SUBI:
			BrewEmitArith(theOp,"-", 1);
		break;

		case _AND:
			BrewEmitArith(theOp,"&", 0);
		break;

		case _ANDI:
			BrewEmitArith(theOp,"&", 1);
		break;

		case _OR:
			BrewEmitArith(theOp,"|", 0);
		break;

		case _ORI:
			BrewEmitArith(theOp,"|", 1);
		break;

		case _XOR:
			BrewEmitArith(theOp,"^", 0);
		break;

		case _XORI:
			BrewEmitArith(theOp,"^", 1);
		break;

		case _DIVU:
			BrewEmitDivu(theOp, 0);
		break;

		case _DIVUI:
			BrewEmitDivu(theOp, 1);
		break;

		case _DIV:
			BrewEmitArith(theOp,"/", 0);
		break;

		case _DIVI:
			BrewEmitArith(theOp,"/", 1);
		break;

		// Shifts

		case _SLL:
			BrewEmitArith(theOp,"<<", 0);
		break;

		case _SLLI:
			BrewEmitArith(theOp,"<<", 1);
		break;

		case _SRA:
			BrewEmitArith(theOp,">>", 0);
		break;

		case _SRAI:
			BrewEmitArith(theOp,">>", 1);
		break;

		case _SRL:
			BrewEmitArith(theOp,">>>", 0);
		break;

		case _SRLI:
			BrewEmitArith(theOp,">>>", 1);
		break;

		case _NOT:
			RebuildEmit("	%s = ~%s;", brew_reg[theOp->rd], brew_reg[theOp->rs]);		
		break;

		case _NEG:
			RebuildEmit("	%s = -%s;", brew_reg[theOp->rd], brew_reg[theOp->rs]);
		break;

		case _RET:
		{
			if (ThisFunctionExit == 0)	// Don't output a return jump on last instruction
			{
				RebuildEmit("	goto label_0;	// return");
				ReturnCount++;
			}
		}
		break;

		// Conditional jumps

		case _JC_EQ:
			BrewEmitJumpCond(theOp, "==", 0);
		break;

		case _JC_NE:
			BrewEmitJumpCond(theOp, "!=", 0);
		break;

		case _JC_GE:
			BrewEmitJumpCond(theOp, ">=", 0);
		break;

		case _JC_GEU:
			BrewEmitJumpCond(theOp, ">=", 1);
		break;

		case _JC_GT:
			BrewEmitJumpCond(theOp, ">", 0);
		break;

		case _JC_GTU:
			BrewEmitJumpCond(theOp, ">", 1);
		break;

		case _JC_LE:
			BrewEmitJumpCond(theOp, "<=", 0);
		break;

		case _JC_LEU:
			BrewEmitJumpCond(theOp, "<=", 1);
		break;

		case _JC_LT:
			BrewEmitJumpCond(theOp, "<", 0);
		break;

		case _JC_LTU:
			BrewEmitJumpCond(theOp, "<", 1);
		break;

		case _JPI:
			BrewDecodeLabel(theOp, "	goto label_%d;");
		break;

		// Memory instructions

		case _LDW:
			Brew_LoadMem(theOp, "RINT");
		break;

		case _LDH:
			Brew_LoadMem(theOp, "RSHORT");
		break;

		case _LDB:
			Brew_LoadMem(theOp, "RBYTE");
		break;

		case _STW:
			Brew_StoreMem(theOp, "WINT");
		break;

		case _STH:
			Brew_StoreMem(theOp, "WSHORT");
		break;

		case _STB:
			Brew_StoreMem(theOp, "WBYTE");
		break;

		case _XB:
			RebuildEmit("	%s = (int)((byte) %s);", brew_reg[theOp->rd], brew_reg[theOp->rs]);		
		break;

		case _XH:
			RebuildEmit("	%s = (int)((short) %s);", brew_reg[theOp->rd], brew_reg[theOp->rs]);		
		break;

		default:
			str[0] = 0;
			DisassembleFromSource(ip, str);
			ErrorOnIP(Error_Fatal, ip, "Missing instruction in Brew rebuilder '%s'\n", str);
	}

	RebuildEmit("\n");
	return 1;
}


//****************************************
//			 
//****************************************

void BrewDecodeReturn()
{
	switch(ThisFunctionRetType)
	{
		case RET_null:
		RebuildEmit("	return ?; // Error report to MobileSorcery");
		break;

		case RET_void:
		RebuildEmit("	return;");
		break;

		case RET_float:
		case RET_int:
		RebuildEmit("	return r14;");
		break;

		case RET_double:
		RebuildEmit("	__dbl_high = r15;\n");
		RebuildEmit("	return r14;");
		break;
	}
}

//****************************************
//			 
//****************************************

void BrewDecodeReturnNull()
{
	switch(ThisFunctionRetType)
	{
		case RET_null:
		RebuildEmit("	return ?; // Error report to MobileSorcery");
		break;

		case RET_void:
		RebuildEmit("	return;");
		break;

		case RET_float:
		case RET_int:
		RebuildEmit("	return 0;");
		break;

		case RET_double:
		RebuildEmit("	__dbl_high = 0;\n");
		RebuildEmit("	return 0;");
		break;
	}
}

//****************************************
//			 
//****************************************

void BrewDecodeSysCall(OpcodeInfo *theOp)
{
	int param_count, need_comma, n;

	SYMBOL *syscall =  FindSysCall(theOp->imm);

	if (!syscall)
	{
		Error(Error_System, "Could'nt locate syscall\n");
		return;
	}

	BrewSyscallUsed[theOp->imm]++;

	param_count = syscall->Params;

	BrewEmitReturnType(syscall->RetType);

	if (syscall->Interface == 0)
//		RebuildEmit("MoSync.%s(", syscall->Name);
		RebuildEmit("%s(", syscall->Name);
	else
		RebuildEmit("%s(", syscall->Name);

	if (param_count > 4)
		param_count = 4;

	need_comma = 0;

	for (n=0;n<param_count;n++)
	{
		if (need_comma)
			RebuildEmit(", ");				
	
		RebuildEmit("%s", brew_reg[REG_i0 + n]);
		need_comma = 1;
	}

	RebuildEmit(");");

	if (syscall->RetType == RET_double)
		RebuildEmit("\n	r15 = __dbl_high;");
}

//****************************************
//			 
//****************************************

void BrewForceSysCallUsed(char *name)
{
	SYMBOL	*sym;
		
	sym = FindSymbolsOld(name,section_SysCall,section_SysCall);
	
	if (!sym)
		return;

	BrewSyscallUsed[sym->Value]++;
}

//****************************************
//			 
//****************************************

void Brew_LoadMem(OpcodeInfo *theOp, char *str)
{
/*	if (strcmp(str, "RINT") == 0)
	{
		// Optimization for int

		if (theOp->rs == 0)
		{
			RebuildEmit("	%s = mem_ds[0x%x];", brew_reg[theOp->rd], theOp->imm >> 2);
			return;
		}

		if (theOp->imm == 0)
		{
			RebuildEmit("	%s = mem_ds[%s >> 2];", brew_reg[theOp->rd], brew_reg[theOp->rs]);
			return;
		}
			
		RebuildEmit("	%s = mem_ds[(%s+0x%x) >> 2];", brew_reg[theOp->rd], brew_reg[theOp->rs], theOp->imm);
		return;
	}
*/	
	
	if (theOp->rs == 0)
	{
		RebuildEmit("	%s = %s(0x%x);", brew_reg[theOp->rd], str, theOp->imm);
		return;
	}

	if (theOp->imm == 0)
	{
		RebuildEmit("	%s = %s(%s);", brew_reg[theOp->rd], str, brew_reg[theOp->rs]);
		return;
	}
		
	RebuildEmit("	%s = %s(%s+0x%x);", brew_reg[theOp->rd], str, brew_reg[theOp->rs], theOp->imm);
}


//****************************************
//			 
//****************************************

void Brew_StoreMem(OpcodeInfo *theOp, char *str)
{
	// optimized ints
	
/*	if (strcmp(str, "WINT") == 0)
	{
		if (theOp->rd == 0)
		{
			RebuildEmit("	mem_ds[0x%x] = %s;", theOp->imm >> 2, brew_reg[theOp->rs]);
			return;
		}

		if (theOp->imm == 0)
		{
			RebuildEmit("	mem_ds[%s >> 2] = %s;", brew_reg[theOp->rd], brew_reg[theOp->rs]);
			return;
		}

		RebuildEmit("	mem_ds[(%s+0x%x) >> 2] = %s;", brew_reg[theOp->rd], theOp->imm, brew_reg[theOp->rs]);
		return;
	}
*/
// others	

	if (theOp->rd == 0)
	{
		RebuildEmit("	%s(0x%x, %s);", str, theOp->imm, brew_reg[theOp->rs]);
		return;
	}

	if (theOp->imm == 0)
	{
		RebuildEmit("	%s(%s, %s);", str, brew_reg[theOp->rd], brew_reg[theOp->rs]);
		return;
	}

	RebuildEmit("	%s(%s+0x%x, %s);", str, brew_reg[theOp->rd], theOp->imm, brew_reg[theOp->rs]);
}


//****************************************
//			 
//****************************************

int BrewDecodeLabel(OpcodeInfo *theOp, char *str)
{

	SYMBOL *ref, *labref;
	int addr;
	
	ref = (SYMBOL *) ArrayGet(&CallArray, theOp->rip);
	
	// !! Check if what it points to in lablearry and use that !!
			
	if (!ref)
		return 0;

	addr = ref->Value;
	
	labref = (SYMBOL *) ArrayGet(&CodeLabelArray, addr);

	if (!labref)
		return 0;
	
	ref = labref;

//	RebuildEmit("	jp &%s_%d", ref->Name, ref->LocalScope);
	RebuildEmit(str, ref->LabelEnum);
	return 1;
}


//****************************************
//			 
//****************************************

int BrewDecodeCall(OpcodeInfo *theOp)
{
	SYMBOL *ref, *labref;
	int addr;
		
	ref = (SYMBOL *) ArrayGet(&CallArray, theOp->rip);
	
	// !! Check if what it points to in lablearry and use that !!
			
	if (!ref)
		return 0;

	addr = ref->Value;
	
	labref = (SYMBOL *) ArrayGet(&CodeLabelArray, addr);

	if (!labref)
		return 0;
	
	ref = labref;

	return BrewCallFunction(ref);
}

//****************************************
//			 
//****************************************

void BrewDecodeCallReg(OpcodeInfo *theOp)
{
	int i0 = ThisFunctionRegs & REGBIT(REG_i0);
	int i1 = ThisFunctionRegs & REGBIT(REG_i1);
	int i2 = ThisFunctionRegs & REGBIT(REG_i2);
	int i3 = ThisFunctionRegs & REGBIT(REG_i3);

	RebuildEmit("	r14 = CallReg(%s", brew_reg[theOp->rd]);

	if (i0)
		RebuildEmit(", i0");
	else
		RebuildEmit(", 0");
	
	if (i1)
		RebuildEmit(", i1");
	else
		RebuildEmit(", 0");
	
	if (i2)
		RebuildEmit(", i2");
	else
		RebuildEmit(", 0");
	
	if (i3)
		RebuildEmit(", i3");
	else
		RebuildEmit(", 0");

	BrewUsedCallReg = 1;

	RebuildEmit(");\n");
}

//****************************************
//			 
//****************************************

int BrewDecodeCase(int ip)
{
	SYMBOL *ref;
	
	ref = (SYMBOL *) ArrayGet(&CodeLabelArray, ip);

	if (!ref)
		return 0;
	
	RebuildEmit("goto label_%d;", ref->LabelEnum);
	return 1;
}

//****************************************
//			 
//****************************************

int BrewDecodeSwitch(OpcodeInfo *theOp)
{
	int start, len, data_ip, def_ip, i;
	int lab_ip;
	
	data_ip = theOp->imm;

	start	= GetDataMemLong(data_ip++);
	len		= GetDataMemLong(data_ip++);
	def_ip	= GetDataMemLong(data_ip++);	// default case


	RebuildEmit("	switch(%s)\n",  brew_reg[theOp->rd]);
	RebuildEmit("	{\n");

	for (i=0;i<len+1;i++)
	{
		lab_ip = GetDataMemLong(data_ip++);

		RebuildEmit("		case 0x%x: ", start);
		BrewDecodeCase(lab_ip);
		RebuildEmit("\n");
		start++;
	}

	RebuildEmit("		default: ", start);
	BrewDecodeCase(def_ip);
	RebuildEmit("\n");


	RebuildEmit("	}\n");


	return 1;
}

//****************************************
//			 
//****************************************

int BrewCallFunction(SYMBOL *ref)
{
	int param_count, need_comma, n;
	int rettype = ref->RetType;

	BrewEmitReturnType(rettype);

	RebuildEmit("%s_%d(", ref->Name, ref->LocalScope);
	
	param_count = ref->Params;
	
	if (param_count > 4)
		param_count = 4;

	need_comma = 0;

	for (n=0;n<param_count;n++)
	{
		if (need_comma)
			RebuildEmit(", ");				
	
		RebuildEmit("%s", brew_reg[REG_i0 + n]);
		need_comma = 1;
	}

	RebuildEmit(");");
	
	if (rettype == RET_double)
		RebuildEmit("\n	r15 = __dbl_high;");
	
	return 1;
}

//****************************************
//			 
//****************************************

void BrewEmitArith(OpcodeInfo *theOp, char *str, int imm)
{
	if (imm)
	{
		RebuildEmit("	%s %s= 0x%x;", brew_reg[theOp->rd], str, theOp->imm);
		return;
	}
	
	RebuildEmit("	%s %s= %s;", brew_reg[theOp->rd], str, brew_reg[theOp->rs]);
	return;
}

//****************************************
//			 
//****************************************

void BrewEmitDiv(OpcodeInfo *theOp, int imm)
{
	if (imm)
	{
		RebuildEmit("	if (%s == 0) MoSync.BFE();\n", theOp->imm);		// Fails here
		RebuildEmit("	%s /= %d;", brew_reg[theOp->rd], theOp->imm);
	}
	else
	{
		RebuildEmit("	if (%s == 0) MoSync.BFE();\n", brew_reg[theOp->rs]);
		RebuildEmit("	%s /= %s;", brew_reg[theOp->rd], brew_reg[theOp->rs]);
	}
}

//****************************************
//			 
//****************************************

void BrewEmitDivu(OpcodeInfo *theOp, int imm)
{
	if (imm)
	{
		RebuildEmit("	if(%s == 0) MoSync.BFE();\n", brew_reg[theOp->rs]); 		// Fails here
		RebuildEmit("	%s = (int) ((long)(%s) & 0x0ffffffffL) / ((long)(%d) & 0x0ffffffffL);", brew_reg[theOp->rd], brew_reg[theOp->rd], theOp->imm);
	}
	else
	{
		RebuildEmit("	if(%s == 0) MoSync.BFE();\n", brew_reg[theOp->rs]);
		RebuildEmit("	%s = (int) ((long)(%s) & 0x0ffffffffL) / ((long)(%s) & 0x0ffffffffL);", brew_reg[theOp->rd], brew_reg[theOp->rd], brew_reg[theOp->rs]);
	}
}

//****************************************
//			 
//****************************************

void BrewEmitJumpCond(OpcodeInfo *theOp, char *str, int unsign)
{
	RebuildEmit("	if (%s %s %s) ",brew_reg[theOp->rd], str, brew_reg[theOp->rs]);
	BrewDecodeLabel(theOp, "goto label_%d;");

	unsign = 0;
}

//****************************************
//
//****************************************

void BrewEmitReturnType(int rettype)
{
	switch(rettype)
	{
		case RET_null:
		RebuildEmit("? // Error report to MobileSorcery");
		break;

		case RET_void:
		RebuildEmit("	");
		break;

		case RET_float:
		case RET_int:
		RebuildEmit("	r14 = ");
		break;

		case RET_double:
		RebuildEmit("	r14 = ");
		break;
	}
}

//****************************************
//
//****************************************

void BrewEmitReturnDecl(int rettype)
{
//	RebuildEmit("static ");

	switch(rettype)
	{
		case RET_null:
		RebuildEmit("? // Error report to MobileSorcery");
		break;

		case RET_void:
		RebuildEmit("void ");
		break;

		case RET_float:
		case RET_int:
		RebuildEmit("int ");
		break;

		case RET_double:
		RebuildEmit("int ");
		break;
	}
}

//****************************************
//		Disassemble Function
//****************************************

void RebuildBrewProlog(SYMBOL *sym, int isproto)
{
	int reg_used;
	int reg_alloc;

	int param_count;
	int need_comma;
	int n;
		
	// Find registers used in function

	reg_used = FunctionRegUsage(sym);
	reg_alloc = 0;
	
	// Output helpful header

	if (isproto == 0)
	{
		RebuildEmit("\n//****************************************\n");
		RebuildEmit("// Function: %s\n", sym->Name);
		RebuildEmit("//****************************************\n\n");
	}
	
	// Output function decl

//	RebuildEmit("static ");

	switch(ThisFunctionRetType)
	{
		case RET_null:
		RebuildEmit(" ?; // Error report to MobileSorcery\n");
		break;

		case RET_void:
		RebuildEmit("void ");
		break;

		case RET_float:
		case RET_int:
		case RET_double:
		RebuildEmit("int ");
		break;
	}

	RebuildEmit("%s_%d(", sym->Name, sym->LocalScope);

	param_count = sym->Params;
	
	if (param_count > 4)
		param_count = 4;

	need_comma = 0;

	for (n=0;n<param_count;n++)
	{
		if (need_comma)
			RebuildEmit(", ");				
	
		RebuildEmit("int %s", brew_reg[REG_i0 + n]);
		need_comma = 1;

		reg_alloc |=  1 << (REG_i0 + n);

	}

	if (isproto)
	{
		RebuildEmit(");\n");
		return;
	}

	RebuildEmit(")\n{\n");

	// Write local decl

	// Remove regs that are already declared in func decl
	
	reg_used &= ~reg_alloc;
	
	// remove sp from locals
	
	reg_used &= ~(1 << REG_sp);
	reg_used &= ~(1 << REG_zero);

	if (ThisFunctionRetType == RET_double)
		reg_used |= (1 << REG_r15);
	
	if (reg_used)
	{
		RebuildEmit("\tint ");

		need_comma = 0;

		for (n=0;n<32;n++)
		{		
			if (reg_used & (1 << n))
			{
				if (need_comma)
					RebuildEmit(", ");				

				RebuildEmit("%s", brew_reg[n]);
				need_comma = 1;
			}
		}

		RebuildEmit(";\n\n");
	}

}

//****************************************
//		Disassemble Function
//****************************************

void RebuildBrewEpilog(SYMBOL *sym)
{
//	ThisFunctionExit

	if (ReturnCount > 0)
		RebuildEmit("label_0:;\n");

	BrewDecodeReturn();
	RebuildEmit("\n");

	RebuildEmit("} // %s\n", sym->Name);
}

//****************************************
//		Disassemble Function
//****************************************

void RebuildBrewFunc(SYMBOL *sym, int isproto)
{
	OpcodeInfo thisOp;
	SYMBOL *ref;
	
	uchar *ip, *ip_end, *ip_last;
	
	int real_ip;
//	char str[256];

	if (!sym)
		return;

	// Say no returns yet

	ReturnCount = 0;

	// Enumerate this functions labels, unless we're generating a proto

	if (isproto	== 0)
		EnumerateFunctionLabels(sym);

	ThisFunctionRegs = FunctionRegUsage(sym);
	ThisFunctionRetType = sym->RetType;

	if (ThisFunctionRegs == -1)
		return;

	RebuildBrewProlog(sym, isproto);

	// if we're generating a proto return

	if (isproto)
		return;

	ip_end = (uchar *) ArrayPtr(&CodeMemArray, sym->EndIP);
	ip = (uchar *) ArrayPtr(&CodeMemArray, sym->Value);
	real_ip	= sym->Value;

	while(1)
	{
		ip_last = ip;
		
		if (ip > ip_end)
			break;

		// Print labels
		
		ref = (SYMBOL *) ArrayGet(&CodeLabelArray, real_ip);

		if (ref)
		{
			if (ref->LabelType == label_Local)
			{
#ifdef BREW_DEBUG
				RebuildEmit("// %s_%d:\n", ref->Name, ref->LocalScope);
#endif
				RebuildEmit("label_%d:;\n", ref->LabelEnum);
			}
		}
	
		if (ArrayGet(&CodeTouchArray, real_ip) == 0)
			RebuildEmit("// ");

		CaseRef = 0;

		ip = DecodeOpcode(&thisOp, ip);

		ThisFunctionExit = 0;
	
		if (ip > ip_end)
			ThisFunctionExit = 1;	

		RebuildBrewInst(&thisOp);

//		DecodeAsmString(&thisOp, str);
//		RebuildEmit("\t%s", str);

#ifdef DEBUG_REBUILD
		{
			int len = 4 + strlen(str);		
			str[0] = 0;
			
			while(len < 40)
			{
				RebuildEmit(" ", str);
				len++;
			}
			
			DisassembleFromSource(real_ip, str);
			RebuildEmit(";%s", str);
		}
#endif

//		RebuildEmit("\n");

		// Check for case statement, which need case data after them
		
/*		if (CaseRef)
		{
			RebuildEmit(".data\n");
			Rebuild_Data(CaseRef);
			RebuildEmit(".code\n");
		}
*/
		real_ip += (ip - ip_last);	
	}

	RebuildBrewEpilog(sym);
}

//****************************************
//
//****************************************

void RebuildBrew_Code()
{
	SYMBOL *sym;
	int n;
	int c = 0;

	for (n=0;n<CodeIP+1;n++)
	{
		sym = (SYMBOL *) ArrayGet(&CodeLabelArray, n);

		if (sym)
		{
			if (sym->Flags & SymFlag_Ref)
			if (sym->LabelType >= label_Function)
			{
				RebuildBrewFunc(sym, 0);
				c++;
			}
		}
	}
}

//****************************************
//
//****************************************

void RebuildBrew_EmitProtos()
{
	SYMBOL *sym;
	int n;

	RebuildEmit("\n// Prototypes\n\n");

	RebuildEmit("int CallReg(int s, int i0, int i1, int i2, int i3);\n");

	for (n=0;n<CodeIP+1;n++)
	{
		sym = (SYMBOL *) ArrayGet(&CodeLabelArray, n);

		if (sym)
		{
			if (sym->Flags & SymFlag_Ref)
			if (sym->LabelType >= label_Function)
			{
				RebuildBrewFunc(sym, 1);
			}
		}
	}
}


//****************************************
//
//****************************************

void RebuildBrew_CallReg()
{
	SYMBOL *sym;
	int n, count;

	SortVirtuals();
	count = GetVirtualIndex();

//	if (!count)
	if (!BrewUsedCallReg)
	{
		RebuildEmit("\n// No virtuals\n\n");
		return;
	}
	
	RebuildEmit("\n");
	RebuildEmit("//****************************************\n");
	RebuildEmit("//             CallReg Sink\n");
	RebuildEmit("//****************************************\n\n");


	RebuildEmit("int CallReg(int s, int i0, int i1, int i2, int i3)\n");
	RebuildEmit("{\n");

	if (count)
	{
		RebuildEmit("	switch(s & 0xffffff)\n");
		RebuildEmit("	{\n");
		
		for (n=0;n<count;n++)
		{
			sym = FetchVirtual(n);

			if (sym->Flags & SymFlag_Ref)
			{
				RebuildEmit("		case 0x%x:\n", sym->VirtualIndex);
				RebuildEmit("		");
				BrewCallFunction(sym);
				RebuildEmit("\n");
//				RebuildEmit("		return;\n\n");	

				ThisFunctionRetType = sym->RetType;
				BrewDecodeReturn();

			}

		}

		RebuildEmit("		default:\n");
		RebuildEmit("		break;\n");

		RebuildEmit("	}\n");
	}

	RebuildEmit("	return 0;\n");
	RebuildEmit("}\n");

}

//****************************************
//
//****************************************

void RebuildBrew_EmitDS()
{
	int need_comma;
	int count;
	int n;
	
	RebuildEmit("\n");
	RebuildEmit("//****************************************\n");
	RebuildEmit("//             Data Section\n");
	RebuildEmit("//****************************************\n\n");

	RebuildEmit("#define ds_len %d\n", DataIP >> 2);
	RebuildEmit("#define bs_len %d\n", BssIP >> 2);
	RebuildEmit("#define all_len %d\n\n", (BssIP + DataIP) >> 2);


//	GetDataMemLong(data_ip++);

//	RebuildEmit("static int mem_ds[] = new int[16384];\n\n");


	RebuildEmit("int mem_ds[16384] = \n{\n");

	need_comma = 0;
	count = 0;

#if 0

	for (n=0;n<DataIP >> 2;n++)
	{
		if (need_comma)
		{
			RebuildEmit(", ");
			need_comma = 1;

		
			if ((count & 0x7) == 0)
				RebuildEmit("\n");
		}
		
		RebuildEmit("0x%x", GetDataMemLong(n));
		need_comma = 1;

		count++;

	}

	if (need_comma)
	{
		RebuildEmit(", ");
	}

	RebuildEmit("\n// Bss\n");

	need_comma = 0;
	count = 0;

	for (n=0;n<BssIP >> 2;n++)
	{
		if (need_comma)
		{
			RebuildEmit(", ");
			need_comma = 1;

		
			if ((count & 0x7) == 0)
				RebuildEmit("\n");
		}
		
		RebuildEmit("0");
		need_comma = 1;

		count++;

	}
#else

	for (n=0;n<(DataIP) >> 2;n++)
	{		
		RebuildEmit("	0x%x,	//%d\n", GetDataMemLong(n), n);
	}

	RebuildEmit("	0\n");

#endif

	RebuildEmit("};\n");
}

//****************************************
// 
//****************************************

void RebuildBrew_StartUp()
{
	SYMBOL *ep;
	
	RebuildEmit("\n");
	RebuildEmit("//****************************************\n");
	RebuildEmit("//          	 Startup\n");
	RebuildEmit("//****************************************\n");
	RebuildEmit("\n");


	RebuildEmit("void main()\n");
	RebuildEmit("{\n");

	RebuildEmit("	int i0,i1,i2,i3,r14,r15;\n");
	RebuildEmit("\n");

	RebuildEmit("	i0 = 0;\n");
	RebuildEmit("	i1 = 0;\n");
	RebuildEmit("	i2 = 0;\n");
	RebuildEmit("	i3 = 0;\n");
	RebuildEmit("	r14 = 0;\n");
	RebuildEmit("	r15 = 0;\n");
	RebuildEmit("\n");

	// init data array

//	RebuildEmit("	System.arraycopy(data_section, 0, mem_ds, 0, ds_len);\n"); 
	RebuildEmit("	sp = 16384 - 16;\n");

	RebuildEmit("\n");

	ep	= GetGlobalSym(Code_EntryPoint);

	if (ep)
	{
		BrewCallFunction(ep);
	}
	
	RebuildEmit("\n}\n");
	

	RebuildEmit("\n");

// test function

/*	RebuildEmit("public static void sys_print(int i0)\n");
	RebuildEmit("{\n");

	RebuildEmit("	System.out.print(\" \" + (int) i0 );\n");
	RebuildEmit("	return;\n");	
	RebuildEmit("\n}\n");	
*/
}

//****************************************
// 
//****************************************

void RebuildBrew_EmitSyscallFunc(SYMBOL *sym, int proto, int stub)
{
	int param_count, need_comma, n;

	if (!stub)
		if (!sym->Interface)
			return;	

	if (!proto)
	{
		RebuildEmit("\n");
		RebuildEmit("//****************************************\n");
		RebuildEmit("//  Syscall interface '%s'\n", sym->Name);
		RebuildEmit("//****************************************\n");
		RebuildEmit("\n");
	}
	
	param_count = sym->Params;

	BrewEmitReturnDecl(sym->RetType);

	RebuildEmit("%s(", sym->Name);

	if (param_count > 4)
		param_count = 4;

	need_comma = 0;

	for (n=0;n<param_count;n++)
	{
		if (need_comma)
			RebuildEmit(", ");				
	
		RebuildEmit("int %s", brew_reg[REG_i0 + n]);
		need_comma = 1;
	}

	RebuildEmit(")");

	if (proto)
	{
		RebuildEmit(";\n");
		return;
	}

	RebuildEmit("\n{\n");

	if (!stub)
		RebuildEmit("	%s", sym->Interface);
	else
	{
		ThisFunctionRetType = sym->RetType;
		BrewDecodeReturnNull();
	}
	
	RebuildEmit("\n}\n");
}

//****************************************
// 
//****************************************

void RebuildBrew_EmitExtensions(int stub)
{
	SYMBOL *sym;
	int len = sizeof(BrewSyscallUsed);
	int n;
	
	for (n=0;n<len;n++)
	{
		if (BrewSyscallUsed[n])
		{
			sym = FindSysCall(n);

			if (sym)
				RebuildBrew_EmitSyscallFunc(sym, 0, stub);	
		}
	}
}

//****************************************
// 
//****************************************

void RebuildBrew_EmitExtensionsProto()
{
	SYMBOL *sym;
	int len = sizeof(BrewSyscallUsed);
	int n;
	
	for (n=0;n<len;n++)
	{
			sym = FindSysCall(n);

			if (sym)
				RebuildBrew_EmitSyscallFunc(sym, 1, 1);	
	}
}

//****************************************
//
//****************************************

void RebuildBrew_Main()
{
	if (ArgConstOpt != 0)
		Error(Error_System, "(RebuildBrew_Main) ArgConstOpt must be switched off");

	ArrayInit(&RebuildArray, sizeof(char), 0);
	ArrayInit(&LabelDone, sizeof(char), 0);

	memset(BrewSyscallUsed, 0, sizeof(BrewSyscallUsed));

	BrewForceSysCallUsed("RBYTE");
	BrewForceSysCallUsed("WBYTE");

	Rebuild_Mode = 1;
	BrewUsedCallReg = 0;

	RebuildEmit("//****************************************\n");
	RebuildEmit("//          Generated brew code\n");
	RebuildEmit("//****************************************\n");

	RebuildEmit("\n");
	RebuildEmit("#include \"mstypeinfo.h\"\n");
	RebuildEmit("\n");

	RebuildBrew_EmitExtensionsProto();
	RebuildBrew_EmitProtos();

	RebuildBrew_EmitDS();

//	RebuildEmit("class MoSyncCode\n");
//	RebuildEmit("{\n");

	RebuildEmit("\n");
	RebuildEmit("int sp;\n");
	RebuildEmit("int __dbl_high;\n");

	RebuildEmit("\n");

//	RebuildBrew_EmitDS();
	RebuildBrew_StartUp();

	MaxEnumLabel = 0;

	RebuildBrew_Code();
	RebuildBrew_EmitExtensions(1);
	RebuildBrew_CallReg();

//	RebuildEmit("}; // End of MosyncCode class\n");
//	RebuildEmit("// MaxEnumLabel=%d\n", MaxEnumLabel);

//	RebuildBrew_FlowClass();

	ArrayWrite(&RebuildArray, "rebuild.brew.cpp");
} 

//****************************************
//			  End Of File
//****************************************

#endif