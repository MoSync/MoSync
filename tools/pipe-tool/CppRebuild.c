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
//				  			  	  Cpp Rebuilder
// 						   		Written by A.R.Hartley
//*********************************************************************************************

#include "compile.h"

// fix >>>
// put in prototypes
// remove static

#define INCLUDE_CPP_REBUILD

#ifdef INCLUDE_CPP_REBUILD

//#define CPP_DEBUG

#define REGBIT(v)		(1 << v)
#define REGUSED(v, reg) (reg & REGBIT(v))

static int ThisFunctionRegs;			// Register usage for current function
static int ThisFunctionRetType;
static int ThisFunctionExit;			// True on last instruction
static int ReturnCount;

static int CppUsedCallReg;

static char *Cpp_reg[] = {"zr","sp","rt","fr","d0","d1","d2","d3",
					"d4","d5","d6","d7","i0","i1","i2","i3",
					"r0","r1","r2","r3","r4","r5","r6","r7",
					"r8","r9","r10","r11","r12","r13","r14","r15"
				   };

static char CppSyscallUsed[1024];

static FuncProp funcprop;

//****************************************
//			 
//****************************************

int RebuildCppInst(OpcodeInfo *theOp)
{
	int ip = theOp->rip;
	char str[256];

#ifdef CPP_DEBUG
	str[0] = 0;
	DisassembleFromSource(ip, str);
	RebuildEmit("\t\t\t\t\t\t//%s\n", str);
#endif

	switch (theOp->op)
	{
		case _PUSH:
			RebuildEmit("	//push %s,%d\n",Cpp_reg[theOp->rd], theOp->rs);

			if (funcprop.reg_used & (1 << REG_sp))
			{
				RebuildEmit("	sp -= %d;\n",theOp->rs*4);
			}
		return 1;
			
		case _POP:
			RebuildEmit("	//pop  %s,%d\n",Cpp_reg[theOp->rd], theOp->rs);

			if (funcprop.reg_used & (1 << REG_sp))
			{
				RebuildEmit("	sp += %d;\n",theOp->rs*4);
			}

		return 1;

		case _CASE:
			CppDecodeSwitch(theOp);
		break;


		case _CALLI:
			CppDecodeCall(theOp);
		break;

		case _SYSCALL:
			CppDecodeSysCall(theOp);
		break;

		case _CALL:
			CppDecodeCallReg(theOp);
		break;

		case _LDI:
			RebuildEmit("	%s = 0x%x;", Cpp_reg[theOp->rd], theOp->imm);			
		break;

		case _LDR:
		{
			if (IsRegConst(theOp->rs))
				RebuildEmit("	%s = 0x%x;", Cpp_reg[theOp->rd], ConstRegValue(theOp->rs));
			else
				RebuildEmit("	%s = %s;", Cpp_reg[theOp->rd], Cpp_reg[theOp->rs]);		
		}
		break;

		// Arithmatic
		
		case _ADD:
			CppEmitArith(theOp,"+", 0);
		break;

		case _ADDI:
			CppEmitArith(theOp,"+", 1);
		break;

		case _MUL:
			CppEmitArith(theOp,"*", 0);
		break;

		case _MULI:
			CppEmitArith(theOp,"*", 1);
		break;

		case _SUB:
			CppEmitArith(theOp,"-", 0);
		break;

		case _SUBI:
			CppEmitArith(theOp,"-", 1);
		break;

		case _AND:
			CppEmitArith(theOp,"&", 0);
		break;

		case _ANDI:
			CppEmitArith(theOp,"&", 1);
		break;

		case _OR:
			CppEmitArith(theOp,"|", 0);
		break;

		case _ORI:
			CppEmitArith(theOp,"|", 1);
		break;

		case _XOR:
			CppEmitArith(theOp,"^", 0);
		break;

		case _XORI:
			CppEmitArith(theOp,"^", 1);
		break;

		case _DIVU:
			CppEmitDivu(theOp, 0);
		break;

		case _DIVUI:
			CppEmitDivu(theOp, 1);
		break;

		case _DIV:
			CppEmitArith(theOp,"/", 0);
		break;

		case _DIVI:
			CppEmitArith(theOp,"/", 1);
		break;

		// Shifts

		case _SLL:
			CppEmitShift(theOp,"<<", 0, 0);
		break;

		case _SLLI:
			CppEmitShift(theOp,"<<", 1, 0);
		break;

		case _SRA:
			CppEmitShift(theOp,">>", 0, 0);
		break;

		case _SRAI:
			CppEmitShift(theOp,">>", 1, 0);
		break;

		case _SRL:
			CppEmitShift(theOp,">>", 0, 1);		// Unsigned
		break;

		case _SRLI:
			CppEmitShift(theOp,">>", 1, 1);		// Unsigned
		break;

		case _NOT:
			RebuildEmit("	%s = ~%s;", Cpp_reg[theOp->rd], Cpp_reg[theOp->rs]);		
		break;

		case _NEG:
			RebuildEmit("	%s = -%s;", Cpp_reg[theOp->rd], Cpp_reg[theOp->rs]);
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
			CppEmitJumpCond(theOp, "==", 0);
		break;

		case _JC_NE:
			CppEmitJumpCond(theOp, "!=", 0);
		break;

		case _JC_GE:
			CppEmitJumpCond(theOp, ">=", 0);
		break;

		case _JC_GEU:
			CppEmitJumpCond(theOp, ">=", 1);
		break;

		case _JC_GT:
			CppEmitJumpCond(theOp, ">", 0);
		break;

		case _JC_GTU:
			CppEmitJumpCond(theOp, ">", 1);
		break;

		case _JC_LE:
			CppEmitJumpCond(theOp, "<=", 0);
		break;

		case _JC_LEU:
			CppEmitJumpCond(theOp, "<=", 1);
		break;

		case _JC_LT:
			CppEmitJumpCond(theOp, "<", 0);
		break;

		case _JC_LTU:
			CppEmitJumpCond(theOp, "<", 1);
		break;

		case _JPI:
			CppDecodeLabel(theOp, "	goto label_%d;");
		break;

		// Memory instructions

		case _LDW:
			Cpp_LoadMem(theOp, "RINT");
		break;

		case _LDH:
			Cpp_LoadMem(theOp, "RSHORT");
		break;

		case _LDB:
			Cpp_LoadMem(theOp, "RBYTE");
		break;

		case _STW:
			Cpp_StoreMem(theOp, "WINT");
		break;

		case _STH:
			Cpp_StoreMem(theOp, "WSHORT");
		break;

		case _STB:
			Cpp_StoreMem(theOp, "WBYTE");
		break;

		case _XB:
			RebuildEmit("	%s = (int)((char) %s);", Cpp_reg[theOp->rd], Cpp_reg[theOp->rs]);		
		break;

		case _XH:
			RebuildEmit("	%s = (int)((short) %s);", Cpp_reg[theOp->rd], Cpp_reg[theOp->rs]);		
		break;

		default:
			str[0] = 0;
			DisassembleFromSource(ip, str);
			ErrorOnIP(Error_Fatal, ip, "Missing instruction in Cpp rebuilder '%s'\n", str);
	}

	RebuildEmit("\n");
	return 1;
}


//****************************************
//			 
//****************************************

void CppDecodeReturn()
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

void CppDecodeReturnNull()
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

void CppDecodeSysCall(OpcodeInfo *theOp)
{
	int param_count, need_comma, n;

	SYMBOL *syscall =  FindSysCall(theOp->imm);

	if (!syscall)
	{
		Error(Error_System, "Could'nt locate syscall\n");
		return;
	}

	CppSyscallUsed[theOp->imm]++;

	param_count = syscall->Params;

	CppEmitReturnType(syscall->RetType);


	RebuildEmit("SYSCALL(");
	RebuildEmit("%s", &syscall->Name[1]);
	RebuildEmit(")(");

	if (param_count > 4)
		param_count = 4;

	need_comma = 0;

	for (n=0;n<param_count;n++)
	{
		if (need_comma)
			RebuildEmit(", ");				
	
		RebuildEmit("%s", Cpp_reg[REG_i0 + n]);
		need_comma = 1;
	}

	RebuildEmit(");");

	if (syscall->RetType == RET_double)
		RebuildEmit("\n	r15 = __dbl_high;");
}

//****************************************
//			 
//****************************************

void CppForceSysCallUsed(char *name)
{
	SYMBOL	*sym;
		
	sym = FindSymbolsOld(name,section_SysCall,section_SysCall);
	
	if (!sym)
		return;

	CppSyscallUsed[sym->Value]++;
}

//****************************************
//			 
//****************************************

void Cpp_LoadMem(OpcodeInfo *theOp, char *str)
{
/*	if (strcmp(str, "RINT") == 0)
	{
		// Optimization for int

		if (theOp->rs == 0)
		{
			RebuildEmit("	%s = mem_ds[0x%x];", Cpp_reg[theOp->rd], theOp->imm >> 2);
			return;
		}

		if (theOp->imm == 0)
		{
			RebuildEmit("	%s = mem_ds[%s >> 2];", Cpp_reg[theOp->rd], Cpp_reg[theOp->rs]);
			return;
		}
			
		RebuildEmit("	%s = mem_ds[(%s+0x%x) >> 2];", Cpp_reg[theOp->rd], Cpp_reg[theOp->rs], theOp->imm);
		return;
	}
*/	
	
	if (theOp->rs == 0)
	{
		RebuildEmit("	%s = %s(0x%x);", Cpp_reg[theOp->rd], str, theOp->imm);
		return;
	}

	if (theOp->imm == 0)
	{
		RebuildEmit("	%s = %s(%s);", Cpp_reg[theOp->rd], str, Cpp_reg[theOp->rs]);
		return;
	}
		
	RebuildEmit("	%s = %s(%s+0x%x);", Cpp_reg[theOp->rd], str, Cpp_reg[theOp->rs], theOp->imm);
}


//****************************************
//			 
//****************************************

void Cpp_StoreMem(OpcodeInfo *theOp, char *str)
{
	// optimized ints
	
/*	if (strcmp(str, "WINT") == 0)
	{
		if (theOp->rd == 0)
		{
			RebuildEmit("	mem_ds[0x%x] = %s;", theOp->imm >> 2, Cpp_reg[theOp->rs]);
			return;
		}

		if (theOp->imm == 0)
		{
			RebuildEmit("	mem_ds[%s >> 2] = %s;", Cpp_reg[theOp->rd], Cpp_reg[theOp->rs]);
			return;
		}

		RebuildEmit("	mem_ds[(%s+0x%x) >> 2] = %s;", Cpp_reg[theOp->rd], theOp->imm, Cpp_reg[theOp->rs]);
		return;
	}
*/
// others	

	if (theOp->rd == 0)
	{
		RebuildEmit("	%s(0x%x, %s);", str, theOp->imm, Cpp_reg[theOp->rs]);
		return;
	}

	if (theOp->imm == 0)
	{
		RebuildEmit("	%s(%s, %s);", str, Cpp_reg[theOp->rd], Cpp_reg[theOp->rs]);
		return;
	}

	RebuildEmit("	%s(%s+0x%x, %s);", str, Cpp_reg[theOp->rd], theOp->imm, Cpp_reg[theOp->rs]);
}


//****************************************
//			 
//****************************************

int CppDecodeLabel(OpcodeInfo *theOp, char *str)
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

int CppDecodeCall(OpcodeInfo *theOp)
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

	return CppCallFunction(ref, 1);
}

//****************************************
//			 
//****************************************

void CppDecodeCallReg(OpcodeInfo *theOp)
{
	int i0 = ThisFunctionRegs & REGBIT(REG_i0);
	int i1 = ThisFunctionRegs & REGBIT(REG_i1);
	int i2 = ThisFunctionRegs & REGBIT(REG_i2);
	int i3 = ThisFunctionRegs & REGBIT(REG_i3);

	RebuildEmit("	r14 = CallReg(%s", Cpp_reg[theOp->rd]);

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

	CppUsedCallReg = 1;

	RebuildEmit(");\n");
}

//****************************************
//			 
//****************************************

int CppDecodeCase(int ip)
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

int CppDecodeSwitch(OpcodeInfo *theOp)
{
	int start, len, data_ip, def_ip, i;
	int lab_ip;
	
	data_ip = theOp->imm;

	start	= GetDataMemLong(data_ip++);
	len		= GetDataMemLong(data_ip++);
	def_ip	= GetDataMemLong(data_ip++);	// default case


	RebuildEmit("	switch(%s)\n",  Cpp_reg[theOp->rd]);
	RebuildEmit("	{\n");

	for (i=0;i<len+1;i++)
	{
		lab_ip = GetDataMemLong(data_ip++);

		RebuildEmit("		case 0x%x: ", start);
		CppDecodeCase(lab_ip);
		RebuildEmit("\n");
		start++;
	}

	RebuildEmit("		default: ", start);
	CppDecodeCase(def_ip);
	RebuildEmit("\n");


	RebuildEmit("	}\n");


	return 1;
}

//****************************************
//			 
//****************************************
#if 0
int xCppCallFunction(SYMBOL *ref)
{
	int param_count, need_comma, n;
	int rettype = ref->RetType;

	CppEmitReturnType(rettype);

	RebuildEmit("%s_%d(", ref->Name, ref->LocalScope);
	
	param_count = ref->Params;
	
	if (param_count > 4)
		param_count = 4;

	need_comma = 0;

	for (n=0;n<param_count;n++)
	{
		if (need_comma)
			RebuildEmit(", ");				
	
		RebuildEmit("%s", Cpp_reg[REG_i0 + n]);
		need_comma = 1;
	}

	RebuildEmit(");");
	
	if (rettype == RET_double)
		RebuildEmit("\n	r15 = __dbl_high;");
	
	return 1;
}
#else
int CppCallFunction(SYMBOL *ref, int emit_r15)
{
	int param_count, need_comma, n;
	int rettype = ref->RetType;
	int regs;
	
	CppEmitReturnType(rettype);

	RebuildEmit("%s_%d(", ref->Name, ref->LocalScope);
	
	param_count = ref->Params;
	
	if (param_count > 4)
		param_count = 4;

	need_comma = 0;

	regs = funcprop.reg_used;

	for (n=0;n<param_count;n++)
	{
		if (need_comma)
			RebuildEmit(", ");				

/*		if (REGUSED(REG_i0 + n, regs))
		{
			printf("Rebuilder: Function '%s' parameter %d was not initialized\n",ref->Name, n);
			//Error(Error_Fatal,"Function parameter %d was not initialized", n);
			RebuildEmit("<Error>");
		}
*/		
		RebuildEmit("%s", Cpp_reg[REG_i0 + n]);
		need_comma = 1;

		// Make sure

	}

	RebuildEmit(");");
	
	if (rettype == RET_double && emit_r15)
	{
		RebuildEmit("\n	r15 = __dbl_high;");
		SetRegInit(REG_r15);
	}
	return 1;
}
#endif
//****************************************
//			 
//****************************************

void CppEmitArith(OpcodeInfo *theOp, char *str, int imm)
{
	if (imm)
	{
		RebuildEmit("	%s %s= 0x%x;", Cpp_reg[theOp->rd], str, theOp->imm);
		return;
	}
	
	RebuildEmit("	%s %s= %s;", Cpp_reg[theOp->rd], str, Cpp_reg[theOp->rs]);
	return;
}

//****************************************
//			 
//****************************************

void CppEmitShift(OpcodeInfo *theOp, char *oper, int imm, int issigned)
{
	char *us = "";
	
	if (issigned)
		us = "(unsigned int)";

	// Dest

	RebuildEmit("	%s = ", Cpp_reg[theOp->rd]);

	// Typecast and Src

	RebuildEmit("%s %s", us, Cpp_reg[theOp->rd]);

	// Operator
	
	RebuildEmit(" %s ", oper);

	if (imm)
	{
		RebuildEmit("0x%x;", theOp->imm);
		return;
	}
	else
		RebuildEmit("%s;", Cpp_reg[theOp->rs]);
	return;
}


//****************************************
//			 
//****************************************

void CppEmitDiv(OpcodeInfo *theOp, int imm)
{

	if ((theOp->rs == 0) || (theOp->imm == 0))
		printf("");
	
	if (imm)
	{
//		RebuildEmit("	if (%d == 0) MoSyncDiv0();\n", theOp->imm);		// Fails here

		if (theOp->imm == 0)
			Warning("Division by zero in recompiler");

		RebuildEmit("	%s /= %d;", Cpp_reg[theOp->rd], theOp->imm);
	}
	else
	{
		RebuildEmit("	if (%s == 0) MoSyncDiv0();\n", Cpp_reg[theOp->rs]);
		RebuildEmit("	%s /= %s;", Cpp_reg[theOp->rd], Cpp_reg[theOp->rs]);
	}
}

//****************************************
//			 
//****************************************

void CppEmitDivu(OpcodeInfo *theOp, int imm)
{
//	if ((theOp->rs == 0) || (theOp->imm == 0))
//		printf("");

	if (imm)
	{
//		RebuildEmit("	if(%d == 0) MoSyncDiv0();\n", theOp->imm); 		// Fails here

		if (theOp->imm == 0)
			Warning("Division by zero in recompiler");

		RebuildEmit("	%s = (int) ((unsigned long)(%s)) / ((unsigned long)(%d));", Cpp_reg[theOp->rd], Cpp_reg[theOp->rd], theOp->imm);
	}
	else
	{
		RebuildEmit("	if(%s == 0) MoSyncDiv0();\n", Cpp_reg[theOp->rs]);
		RebuildEmit("	%s = (int) ((unsigned long)(%s)) / ((unsigned long)(%s));", Cpp_reg[theOp->rd], Cpp_reg[theOp->rd], Cpp_reg[theOp->rs]);
	}
}

//****************************************
//			 
//****************************************

void CppEmitJumpCond(OpcodeInfo *theOp, char *str, int unsign)
{
	RebuildEmit("	if (%s %s %s) ",Cpp_reg[theOp->rd], str, Cpp_reg[theOp->rs]);
	CppDecodeLabel(theOp, "goto label_%d;");

	unsign = 0;
}

//****************************************
//
//****************************************

void CppEmitReturnType(int rettype)
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

void CppEmitReturnDecl(int rettype)
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

void RebuildCppProlog(SYMBOL *sym, int isproto)
{
	int reg_used;
	int reg_alloc;

	int param_count;
	int need_comma;
	int n;
		
	// Find registers used in function

//	reg_used = FunctionRegUsage(sym);
	
	reg_used = FunctionRegAnalyse(sym, &funcprop);
	
	reg_alloc = 0;
	
	// Output helpful header

	if (isproto == 0)
	{
		RebuildEmit("\n//****************************************\n");
		RebuildEmit("// Function: %s\n", sym->Name);
		RebuildEmit("//****************************************\n\n");
	}
	

	if (!isproto)
	{
		RebuildEmit("//             rrrrrrrrrrrrrrrriiiiddddddddfrsz\n");
		RebuildEmit("//             fedcba9876543210321076543210rtpr\n");

		RebuildEmit("//src_reg    = %s\n", Bin32(funcprop.src_reg));
		RebuildEmit("//dst_reg    = %s\n", Bin32(funcprop.dst_reg));
		RebuildEmit("//assign_reg = %s\n", Bin32(funcprop.assign_reg));
		RebuildEmit("//uninit_reg = %s\n", Bin32(funcprop.uninit_reg));
		RebuildEmit("//used_reg   = %s\n", Bin32(funcprop.reg_used));
		RebuildEmit("\n");
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
	
		RebuildEmit("int %s", Cpp_reg[REG_i0 + n]);
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

				RebuildEmit("%s", Cpp_reg[n]);
				need_comma = 1;
			}
		}

		RebuildEmit(";\n\n");
	}

}

//****************************************
//		Disassemble Function
//****************************************

void RebuildCppEpilog(SYMBOL *sym)
{
//	ThisFunctionExit

	if (ReturnCount > 0)
		RebuildEmit("label_0:;\n");

	CppDecodeReturn();
	RebuildEmit("\n");

	RebuildEmit("} // %s\n", sym->Name);
}

//****************************************
//		Disassemble Function
//****************************************

void RebuildCppFunc(SYMBOL *sym, int isproto)
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

	RebuildCppProlog(sym, isproto);

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
#ifdef Cpp_DEBUG
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

		RebuildCppInst(&thisOp);

//		DecodeAsmString(&thisOp, str);
//		RebuildEmit("\t%s", str);

#ifdef CPP_DEBUG
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

	RebuildCppEpilog(sym);
}

//****************************************
//
//****************************************

void RebuildCpp_Code()
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
				RebuildCppFunc(sym, 0);
				c++;
			}
		}
	}
}

//****************************************
//
//****************************************

void RebuildCpp_EmitProtos()
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
				RebuildCppFunc(sym, 1);
			}
		}
	}
}


//****************************************
//
//****************************************

void RebuildCpp_CallReg()
{
	SYMBOL *sym;
	int n, count;

	SortVirtuals();
	count = GetVirtualIndex();

//	if (!count)
	if (!CppUsedCallReg)
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

	RebuildEmit("	int r14,r15;\n");
	RebuildEmit("	r14 = r15 = 0;\n\n");

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
				CppCallFunction(sym, 0);
				RebuildEmit("\n\t");

				ThisFunctionRetType = sym->RetType;

				if(ThisFunctionRetType == RET_void)
				{
					RebuildEmit("	return 0;\n\n");
				}
				else
				{
					CppDecodeReturn(0);			// was 1
					RebuildEmit("\n\n");
				}

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
/*
void RebuildCpp_EmitDS()
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
	RebuildEmit("#define MaxDataIP %d\n\n", MaxDataIP >> 2);

	RebuildEmit("int *mem_ds\n");
}
*/
//****************************************
// 
//****************************************

void RebuildCpp_StartUp()
{
	SYMBOL *ep;
	FILE *out;
	
	RebuildEmit("\n");
	RebuildEmit("//****************************************\n");
	RebuildEmit("//          	 Startup\n");
	RebuildEmit("//****************************************\n");
	RebuildEmit("\n");


	RebuildEmit("#define ds_len  %d\n", DataIP);
	RebuildEmit("#define bs_len  %d\n", BssIP);
	RebuildEmit("#define all_len %d\n", (BssIP + DataIP));
	RebuildEmit("#define max_data %d\n\n", MaxDataIP);

//	RebuildEmit("int *mem_ds;\n");

	RebuildEmit("void cpp_main()\n");
	RebuildEmit("{\n");

	RebuildEmit("	int i0,i1,i2;\n");
	RebuildEmit("\n");

	RebuildEmit("	i0 = %i;	//mem size\n", Default_DataSize);
	RebuildEmit("	i1 = %i;		//stack size\n", Default_StackSize);
	RebuildEmit("	i2 = %i;		//heap size\n", Default_HeapSize);
	RebuildEmit("	sp = %i-16; //Init stack\n", Default_DataSize);
	RebuildEmit("\n");

	RebuildEmit("	mem_ds = CppInitReadData(\"data_section.bin\", max_data, i0);\n");

	// emit the bin file

	out = fopen("data_section.bin", "wb");
	ArrayWriteFP(&DataMemArray, out, MaxDataIP);
	fclose(out);

	ep	= GetGlobalSym(Code_EntryPoint);

	if (ep)
		CppCallFunction(ep, 1);
	
	RebuildEmit("\n}\n");

}

//****************************************
// 
//****************************************

void RebuildCpp_EmitSyscallFunc(SYMBOL *sym, int proto, int stub)
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

	CppEmitReturnDecl(sym->RetType);

	RebuildEmit("%s(", sym->Name);

	if (param_count > 4)
		param_count = 4;

	need_comma = 0;

	for (n=0;n<param_count;n++)
	{
		if (need_comma)
			RebuildEmit(", ");				
	
		RebuildEmit("int %s", Cpp_reg[REG_i0 + n]);
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
		CppDecodeReturnNull();
	}
	
	RebuildEmit("\n}\n");
}

//****************************************
// 
//****************************************

void RebuildCpp_EmitExtensions(int stub)
{
	SYMBOL *sym;
	int len = sizeof(CppSyscallUsed);
	int n;
	
	for (n=0;n<len;n++)
	{
		if (CppSyscallUsed[n])
		{
			sym = FindSysCall(n);

			if (sym)
				RebuildCpp_EmitSyscallFunc(sym, 0, stub);	
		}
	}
}

//****************************************
// 
//****************************************

void RebuildCpp_EmitExtensionsProto()
{
	SYMBOL *sym;
	int len = sizeof(CppSyscallUsed);
	int n;
	
	for (n=0;n<len;n++)
	{
			sym = FindSysCall(n);

			if (sym)
				RebuildCpp_EmitSyscallFunc(sym, 1, 1);	
	}
}

//****************************************
//
//****************************************

void RebuildCpp_Main()
{
	if (ArgConstOpt != 0)
		Error(Error_System, "(RebuildCpp_Main) ArgConstOpt must be switched off");

	ArrayInit(&RebuildArray, sizeof(char), 0);
	ArrayInit(&LabelDone, sizeof(char), 0);

	memset(CppSyscallUsed, 0, sizeof(CppSyscallUsed));

	CppForceSysCallUsed("RBYTE");
	CppForceSysCallUsed("WBYTE");

	Rebuild_Mode = 1;
	CppUsedCallReg = 0;

	RebuildEmit("//****************************************\n");
	RebuildEmit("//          Generated Cpp code\n");
	RebuildEmit("//****************************************\n");

	RebuildEmit("\n");
	RebuildEmit("#include \"mstypeinfo.h\"\n");
	RebuildEmit("\n");

#if 0			//My Testing only
	RebuildCpp_EmitExtensionsProto();
#endif

	RebuildCpp_EmitProtos();

//	RebuildCpp_EmitDS();

//	RebuildEmit("class MoSyncCode\n");
//	RebuildEmit("{\n");

//	RebuildEmit("\n");					// Out in .h file
//	RebuildEmit("int sp;\n");
//	RebuildEmit("int __dbl_high;\n");

	RebuildEmit("\n");

//	RebuildCpp_EmitDS();
	RebuildCpp_StartUp();

	MaxEnumLabel = 0;

	RebuildCpp_Code();
	//RebuildCpp_EmitExtensions(1);
	RebuildCpp_CallReg();

//	RebuildEmit("}; // End of MosyncCode class\n");
//	RebuildEmit("// MaxEnumLabel=%d\n", MaxEnumLabel);

//	RebuildCpp_FlowClass();

	ArrayWrite(&RebuildArray, "rebuild.build.cpp");
} 

//****************************************
//			  End Of File
//****************************************

#endif