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
//				  			  	  Java Rebuilder
// 						   		Written by A.R.Hartley
//*********************************************************************************************

#include "compile.h"

#define DYN_TYPE "final "
#define CLASS_SEGMENT_SIZE 16384		// Power of 2 please
#define GET_CODE_SEGMENT(v) (v/CLASS_SEGMENT_SIZE)

#ifdef INCLUDE_JAVA_REBUILD

//#define JRDEBUG
//#define NO_ELIM

#define REGBIT(v)		(1 << v)
#define REGUSED(v, reg) (reg & REGBIT(v))

static int ThisFunctionRegs;			// Register usage for current function
static int ThisFunctionRetType;
static int ThisFunctionExit;			// True on last instruction
static int ReturnCount;

#define STATIC_CODE_DOT "StaticCode."
#define SP_STR STATIC_CODE_DOT "sp"
#define DBL_HIGH STATIC_CODE_DOT "__dbl_high"
#define MEM_DS STATIC_CODE_DOT "mem_ds"


#define SYSCALLDOT "StaticCode.syscall."

static char *java_reg[] = {"zr",SP_STR,"rt","fr","d0","d1","d2","d3",
					"d4","d5","d6","d7","i0","i1","i2","i3",
					"r0","r1","r2","r3","r4","r5","r6","r7",
					"r8","r9","r10","r11","r12","r13","r14","r15"
				   };

static char JavaSyscallUsed[1024];

int function_registers_used;		// reg bits used by function
int register_initialized;			// if a register has been initialized

//****************************************
//			 
//****************************************

int RebuildJavaInst(OpcodeInfo *theOp)
{
	int ip = theOp->rip;
	char str[256];

#ifdef JRDEBUG
	str[0] = 0;
	DisassembleFromSource(ip, str);
	RebuildEmit("\t\t\t\t\t\t//%s\n", str);
#endif

	switch (theOp->op)
	{
		case _PUSH:
			RebuildEmit("	//push %s,%d\n",java_reg[theOp->rd], theOp->rs);

			if (function_registers_used & (1 << REG_sp))
			{
				RebuildEmit("	" SP_STR " -= %d;\n",theOp->rs*4);
			}
		return 1;
			
		case _POP:
			RebuildEmit("	//pop  %s,%d\n",java_reg[theOp->rd], theOp->rs);

			if (function_registers_used & (1 << REG_sp))
			{
				RebuildEmit("	" SP_STR " += %d;\n",theOp->rs*4);
			}

		return 1;

		case _CASE:
			JavaDecodeSwitch(theOp);
		break;


		case _CALLI:
			JavaDecodeCall(theOp);
		break;

		case _SYSCALL:
			JavaDecodeSysCall(theOp);
		break;

		case _CALL:
			JavaDecodeCallReg(theOp);
		break;

		case _LDI:
			RebuildEmit("	%s = 0x%x;", java_reg[theOp->rd], theOp->imm);			
			SetRegInit(theOp->rd);
		break;

		case _LDR:
		{
			SetRegInit(theOp->rd);

			if (IsRegConst(theOp->rs))
				RebuildEmit("	%s = 0x%x;", java_reg[theOp->rd], ConstRegValue(theOp->rs));
			else
				RebuildEmit("	%s = %s;", java_reg[theOp->rd], java_reg[theOp->rs]);		
		}
		break;

		// Arithmatic
		
		case _ADD:
			JavaEmitArith(theOp,"+", 0);
		break;

		case _ADDI:
			JavaEmitArith(theOp,"+", 1);
		break;

		case _MUL:
			JavaEmitArith(theOp,"*", 0);
		break;

		case _MULI:
			JavaEmitArith(theOp,"*", 1);
		break;

		case _SUB:
			JavaEmitArith(theOp,"-", 0);
		break;

		case _SUBI:
			JavaEmitArith(theOp,"-", 1);
		break;

		case _AND:
			JavaEmitArith(theOp,"&", 0);
		break;

		case _ANDI:
			JavaEmitArith(theOp,"&", 1);
		break;

		case _OR:
			JavaEmitArith(theOp,"|", 0);
		break;

		case _ORI:
			JavaEmitArith(theOp,"|", 1);
		break;

		case _XOR:
			JavaEmitArith(theOp,"^", 0);
		break;

		case _XORI:
			JavaEmitArith(theOp,"^", 1);
		break;

		case _DIVU:
			JavaEmitDivu(theOp, 0);
		break;

		case _DIVUI:
			JavaEmitDivu(theOp, 1);
		break;

		case _DIV:
			JavaEmitArith(theOp,"/", 0);
		break;

		case _DIVI:
			JavaEmitArith(theOp,"/", 1);
		break;

		// Shifts

		case _SLL:
			JavaEmitArith(theOp,"<<", 0);
		break;

		case _SLLI:
			JavaEmitArith(theOp,"<<", 1);
		break;

		case _SRA:
			JavaEmitArith(theOp,">>", 0);
		break;

		case _SRAI:
			JavaEmitArith(theOp,">>", 1);
		break;

		case _SRL:
			JavaEmitArith(theOp,">>>", 0);
		break;

		case _SRLI:
			JavaEmitArith(theOp,">>>", 1);
		break;

		case _NOT:
			RebuildEmit("	%s = ~%s;", java_reg[theOp->rd], java_reg[theOp->rs]);		
			SetRegInit(theOp->rd);
		break;

		case _NEG:
			RebuildEmit("	%s = -%s;", java_reg[theOp->rd], java_reg[theOp->rs]);
			SetRegInit(theOp->rd);
		break;

		case _RET:
		{
			if (ThisFunctionExit == 0)	// Don't output a return jump on last instruction
			{
				RebuildEmit("	ms.goto_0();	// return");
				ReturnCount++;
			}
		}
		break;

		// Conditional jumps

		case _JC_EQ:
			JavaEmitJumpCond(theOp, "==", 0);
		break;

		case _JC_NE:
			JavaEmitJumpCond(theOp, "!=", 0);
		break;

		case _JC_GE:
			JavaEmitJumpCond(theOp, ">=", 0);
		break;

		case _JC_GEU:
			JavaEmitJumpCond(theOp, ">=", 1);
		break;

		case _JC_GT:
			JavaEmitJumpCond(theOp, ">", 0);
		break;

		case _JC_GTU:
			JavaEmitJumpCond(theOp, ">", 1);
		break;

		case _JC_LE:
			JavaEmitJumpCond(theOp, "<=", 0);
		break;

		case _JC_LEU:
			JavaEmitJumpCond(theOp, "<=", 1);
		break;

		case _JC_LT:
			JavaEmitJumpCond(theOp, "<", 0);
		break;

		case _JC_LTU:
			JavaEmitJumpCond(theOp, "<", 1);
		break;

		case _JPI:
			JavaDecodeLabel(theOp, "	ms.goto_%d();");
		break;

#ifdef NO_ELIM
		case _JPR:
//			JavaDecodeLabel(theOp, "	ms.goto_%d();");
		break;
#endif
		// Memory instructions

		case _LDW:
			Java_LoadMem(theOp, "RINT");
		break;

		case _LDH:
			Java_LoadMem(theOp, "RSHORT");
		break;

		case _LDB:
			Java_LoadMem(theOp, "RBYTE");
		break;

		case _STW:
			Java_StoreMem(theOp, "WINT");
		break;

		case _STH:
			Java_StoreMem(theOp, "WSHORT");
		break;

		case _STB:
			Java_StoreMem(theOp, "WBYTE");
		break;

		case _XB:
			RebuildEmit("	%s = (int)((byte) %s);", java_reg[theOp->rd], java_reg[theOp->rs]);		
		break;

		case _XH:
			RebuildEmit("	%s = (int)((short) %s);", java_reg[theOp->rd], java_reg[theOp->rs]);		
		break;

		default:
			str[0] = 0;
			DisassembleFromSource(ip, str);
			ErrorOnIP(Error_Fatal, ip, "Missing instruction in Java rebuilder '%s'\n", str);
	}

	RebuildEmit("\n");
	return 1;
}


//****************************************
//			 
//****************************************

void JavaDecodeReturn()
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
		RebuildEmit("	" DBL_HIGH " = r15;\n");
		RebuildEmit("	return r14;");
		break;
	}
}

//****************************************
//			 
//****************************************

void JavaDecodeSysCall(OpcodeInfo *theOp)
{
	int param_count, need_comma, n;

	SYMBOL *theSysCall =  FindSysCall(theOp->imm);

	if (!theSysCall)
	{
		Error(Error_System, "Could'nt locate syscall\n");
		return;
	}

	JavaSyscallUsed[theOp->imm]++;

	param_count = theSysCall->Params;

	JavaEmitReturnType(theSysCall->RetType);

	if (theSysCall->Interface == 0)
		RebuildEmit( SYSCALLDOT "%s(", theSysCall->Name + 1);
	else
		RebuildEmit("%s(", theSysCall->Name);

	if (param_count > 4)
		param_count = 4;

	need_comma = 0;

	for (n=0;n<param_count;n++)
	{
		if (need_comma)
			RebuildEmit(", ");				
	
		RebuildEmit("%s", java_reg[REG_i0 + n]);
		need_comma = 1;
	}

	RebuildEmit(");");

	if (theSysCall->RetType == RET_double)
	{
		RebuildEmit("\n	r15 = " DBL_HIGH ";");
		SetRegInit(REG_r15);
	}
}

//****************************************
//			 
//****************************************

void JavaForceSysCallUsed(char *name)
{
	SYMBOL	*sym;
		
	sym = FindSymbolsOld(name,section_SysCall,section_SysCall);
	
	if (!sym)
		return;

	JavaSyscallUsed[sym->Value]++;
}

//****************************************
//			 
//****************************************

void Java_LoadMem(OpcodeInfo *theOp, char *str)
{
	SetRegInit(theOp->rd);

	if (strcmp(str, "RINT") == 0)
	{
		// Optimization for int

		if (theOp->rs == 0)
		{
			RebuildEmit("	%s = " MEM_DS "[0x%x];", java_reg[theOp->rd], theOp->imm >> 2);
			return;
		}

		if (theOp->imm == 0)
		{
			RebuildEmit("	%s = " MEM_DS "[%s >> 2];", java_reg[theOp->rd], java_reg[theOp->rs]);
			return;
		}
			
		RebuildEmit("	%s = " MEM_DS "[(%s+0x%x) >> 2];", java_reg[theOp->rd], java_reg[theOp->rs], theOp->imm);
		return;
	}
	
	
	if (theOp->rs == 0)
	{
		RebuildEmit("	%s = " STATIC_CODE_DOT "%s(0x%x);", java_reg[theOp->rd], str, theOp->imm);
		return;
	}

	if (theOp->imm == 0)
	{
		RebuildEmit("	%s = " STATIC_CODE_DOT "%s(%s);", java_reg[theOp->rd], str, java_reg[theOp->rs]);
		return;
	}
		
	RebuildEmit("	%s = " STATIC_CODE_DOT "%s(%s+0x%x);", java_reg[theOp->rd], str, java_reg[theOp->rs], theOp->imm);
}


//****************************************
//			 
//****************************************

void Java_StoreMem(OpcodeInfo *theOp, char *str)
{
	// optimized ints

	SetRegInit(theOp->rd);
	
	if (strcmp(str, "WINT") == 0)
	{
		if (theOp->rd == 0)
		{
			RebuildEmit("	" MEM_DS "[0x%x] = %s;", theOp->imm >> 2, java_reg[theOp->rs]);
			return;
		}

		if (theOp->imm == 0)
		{
			RebuildEmit("	" MEM_DS "[%s >> 2] = %s;", java_reg[theOp->rd], java_reg[theOp->rs]);
			return;
		}

		RebuildEmit("	" MEM_DS "[(%s+0x%x) >> 2] = %s;", java_reg[theOp->rd], theOp->imm, java_reg[theOp->rs]);
		return;
	}

// others	

	if (theOp->rd == 0)
	{
		RebuildEmit("	" STATIC_CODE_DOT "%s(0x%x, %s);", str, theOp->imm, java_reg[theOp->rs]);
		return;
	}

	if (theOp->imm == 0)
	{
		RebuildEmit("	" STATIC_CODE_DOT "%s(%s, %s);", str, java_reg[theOp->rd], java_reg[theOp->rs]);
		return;
	}

	RebuildEmit("	" STATIC_CODE_DOT "%s(%s+0x%x, %s);", str, java_reg[theOp->rd], theOp->imm, java_reg[theOp->rs]);
}


//****************************************
//			 
//****************************************

int JavaDecodeLabel(OpcodeInfo *theOp, char *str)
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

int JavaDecodeCall(OpcodeInfo *theOp)
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

	return JavaCallFunction(ref);
}

//****************************************
//			 
//****************************************

void JavaDecodeCallReg(OpcodeInfo *theOp)
{
	int i0 = ThisFunctionRegs & REGBIT(REG_i0);
	int i1 = ThisFunctionRegs & REGBIT(REG_i1);
	int i2 = ThisFunctionRegs & REGBIT(REG_i2);
	int i3 = ThisFunctionRegs & REGBIT(REG_i3);

	RebuildEmit("	CallReg(%s", java_reg[theOp->rd]);

	if (i0 && IsRegInit(REG_i0))
		RebuildEmit(", i0");
	else
		RebuildEmit(", 0");
	
	if (i1 && IsRegInit(REG_i1))
		RebuildEmit(", i1");
	else
		RebuildEmit(", 0");
	
	if (i2 && IsRegInit(REG_i2))
		RebuildEmit(", i2");
	else
		RebuildEmit(", 0");
	
	if (i3 && IsRegInit(REG_i3))
		RebuildEmit(", i3");
	else
		RebuildEmit(", 0");

	RebuildEmit(");\n");
}

//****************************************
//			 
//****************************************

int JavaDecodeCase(int ip)
{
	SYMBOL *ref;
	
	ref = (SYMBOL *) ArrayGet(&CodeLabelArray, ip);

	if (!ref)
		return 0;
	
	RebuildEmit("ms.goto_%d();", ref->LabelEnum);
	return 1;
}

//****************************************
//			 
//****************************************

int JavaDecodeSwitch(OpcodeInfo *theOp)
{
	int start, len, data_ip, def_ip, i;
	int lab_ip;
	
	data_ip = theOp->imm;

	start	= GetDataMemLong(data_ip++);
	len		= GetDataMemLong(data_ip++);
	def_ip	= GetDataMemLong(data_ip++);	// default case


	RebuildEmit("	switch(%s)\n",  java_reg[theOp->rd]);
	RebuildEmit("	{\n");

	for (i=0;i<len+1;i++)
	{
		lab_ip = GetDataMemLong(data_ip++);

		RebuildEmit("		case 0x%x: ", start);
		JavaDecodeCase(lab_ip);
		RebuildEmit("\n");
		start++;
	}

	RebuildEmit("		default: ", start);
	JavaDecodeCase(def_ip);
	RebuildEmit("\n");


	RebuildEmit("	}\n");


	return 1;
}

//****************************************
//			 
//****************************************

char FunctionClassString[2048];

char * GetFunctionClass(SYMBOL *ref)
{
	int theClassSegment;

	FunctionClassString[0] = 0;
	
	theClassSegment = GET_CODE_SEGMENT(ref->Value);

	sprintf(FunctionClassString, "Code%d.", theClassSegment);

	return FunctionClassString;
}

//****************************************
//			 
//****************************************

int JavaCallFunction(SYMBOL *ref)
{
	int param_count, need_comma, n;
	int rettype = ref->RetType;

	JavaEmitReturnType(rettype);

	
	RebuildEmit("%s%s_%d(", GetFunctionClass(ref), ref->Name, ref->LocalScope);
	
	param_count = ref->Params;
	
	if (param_count > 4)
		param_count = 4;

	need_comma = 0;

	for (n=0;n<param_count;n++)
	{
		if (need_comma)
			RebuildEmit(", ");				
	
		RebuildEmit("%s", java_reg[REG_i0 + n]);
		need_comma = 1;
	}

	RebuildEmit(");");
	
	if (rettype == RET_double)
	{
		RebuildEmit("\n	r15 = " DBL_HIGH ";");
		SetRegInit(REG_r15);
	}
	return 1;
}

//****************************************
//			 
//****************************************

void JavaEmitArith(OpcodeInfo *theOp, char *str, int hasImm)
{
	SetRegInit(theOp->rd);

	if (hasImm)
	{
		RebuildEmit("	%s %s= 0x%x;", java_reg[theOp->rd], str, theOp->imm);
		return;
	}
	
	RebuildEmit("	%s %s= %s;", java_reg[theOp->rd], str, java_reg[theOp->rs]);
	return;
}

//****************************************
//			 
//****************************************

void JavaEmitDiv(OpcodeInfo *theOp, int hasImm)
{
	SetRegInit(theOp->rd);

	if (hasImm)
	{
		RebuildEmit("	if (%s == 0) MoSync.BFE();\n", theOp->imm);		// Fails here
		RebuildEmit("	%s /= %d;", java_reg[theOp->rd], theOp->imm);
	}
	else
	{
		RebuildEmit("	if (%s == 0) MoSync.BFE();\n", java_reg[theOp->rs]);
		RebuildEmit("	%s /= %s;", java_reg[theOp->rd], java_reg[theOp->rs]);
	}
}

//****************************************
//			 
//****************************************

void JavaEmitDivu(OpcodeInfo *theOp, int hasImm)
{
	SetRegInit(theOp->rd);

	if (hasImm)
	{
		//Division by zero in java will cause an ArithmeticException, so we don't need this check. /Fredrik
		//RebuildEmit("	if(%s == 0) MoSync.BFE();\n", java_reg[theOp->rs]); 		// Fails here
		RebuildEmit("	%s = (int) (((long)(%s) & 0x0ffffffffL) / ((long)(%d) & 0x0ffffffffL));", java_reg[theOp->rd], java_reg[theOp->rd], theOp->imm);
	}
	else
	{
		//RebuildEmit("	if(%s == 0) MoSync.BFE();\n", java_reg[theOp->rs]);
		RebuildEmit("	%s = (int) (((long)(%s) & 0x0ffffffffL) / ((long)(%s) & 0x0ffffffffL));", java_reg[theOp->rd], java_reg[theOp->rd], java_reg[theOp->rs]);
	}
}

//****************************************
//			 
//****************************************

void JavaEmitJumpCond(OpcodeInfo *theOp, char *str, int unsign)
{
	RebuildEmit("	if (%s %s %s) ",java_reg[theOp->rd], str, java_reg[theOp->rs]);
	JavaDecodeLabel(theOp, "ms.goto_%d();");

	unsign = 0;
}

//****************************************
//
//****************************************

void JavaEmitReturnType(int rettype)
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
		SetRegInit(REG_r14);
		break;

		case RET_double:
		RebuildEmit("	r14 = ");
		SetRegInit(REG_r14);
		break;
	}
}

//****************************************
//
//****************************************

void JavaEmitReturnDecl(int rettype)
{
	RebuildEmit("public " DYN_TYPE);

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
//		Set Register Initialized
//****************************************

void SetRegInit(int reg)
{
	register_initialized |= (1 << reg);
}

int IsRegInit(int reg)
{
	if (register_initialized & (1 << reg))
		return 1;

	return 0;
}

//****************************************
//		Disassemble Function
//****************************************

void RebuildJavaProlog(SYMBOL *sym)
{
	int reg_alloc;
	int reg_used;
	int param_count;
	int need_comma;
	int n;
		
	// Find registers used in function

	reg_used = FunctionRegUsage(sym);
	reg_alloc = 0;
	
	// Output helpful header

	RebuildEmit("\n//****************************************\n");
	RebuildEmit("// Function: %s\n", sym->Name);
	RebuildEmit("//****************************************\n\n");

	// Output function decl

	RebuildEmit("public static " DYN_TYPE);

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
	
		RebuildEmit("int %s", java_reg[REG_i0 + n]);
		need_comma = 1;

		reg_alloc |=  1 << (REG_i0 + n);

	}

	RebuildEmit(") throws Exception\n{\n");

	// Write local decl

	function_registers_used = reg_used;

	// Remove regs that are already declared in func decl
	
	reg_used &= ~reg_alloc;
	
	// remove sp from locals
	
	reg_used &= ~(1 << REG_sp);
	reg_used &= ~(1 << REG_zero);

//	if (ThisFunctionRetType == RET_double)
	{
		reg_used |= (1 << REG_r15);
		reg_used |= (1 << REG_r14);
	}
		
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

				RebuildEmit("%s", java_reg[n]);

				// **FIXME** Cheat by preinitializing registers
				RebuildEmit(" = 0");

				need_comma = 1;
			}
		}

		RebuildEmit(";\n\n");
	}



	// set registers that are initialized to default, SP

	register_initialized = (1 << REG_sp);

}

//****************************************
//		Disassemble Function
//****************************************

void RebuildJavaEpilog(SYMBOL *sym)
{
//	ThisFunctionExit

	if (ReturnCount > 0)
		RebuildEmit("ms.label_0();\n");

	JavaDecodeReturn();
	RebuildEmit("\n");

	RebuildEmit("} // %s\n", sym->Name);
}

//****************************************
//		Disassemble Function
//****************************************

void RebuildJavaFunc(SYMBOL *sym)
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

	// Enumerate this functions labels
	
	EnumerateFunctionLabels(sym);

	ThisFunctionRegs = FunctionRegUsage(sym);
	ThisFunctionRetType = sym->RetType;

	if (ThisFunctionRegs == -1)
		return;

	RebuildJavaProlog(sym);

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
#ifdef JRDEBUG
				RebuildEmit("// %s_%d:\n", ref->Name, ref->LocalScope);
#endif
				RebuildEmit("ms.label_%d();\n", ref->LabelEnum);
			}
		}

#ifndef NO_ELIM
	
		if (ArrayGet(&CodeTouchArray, real_ip) == 0)
			RebuildEmit("// ");
#endif

		CaseRef = 0;

		ip = DecodeOpcode(&thisOp, ip);

		ThisFunctionExit = 0;
	
		if (ip > ip_end)
			ThisFunctionExit = 1;	

		RebuildJavaInst(&thisOp);

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

	RebuildJavaEpilog(sym);
}


//****************************************
//
//****************************************

int FindLastCodeSegment()
{
	SYMBOL *sym;
	int n;
	int last = -1;
	
	for (n=0;n<CodeIP;n++)
	{
		// Check to see if we are in a new segment, ie. new class

		sym = (SYMBOL *) ArrayGet(&CodeLabelArray, n);

		if (sym)
		{

#ifndef NO_ELIM
			if (sym->Flags & SymFlag_Ref)
#endif
			if (sym->LabelType >= label_Function)
			{
				last = GET_CODE_SEGMENT(n);
			}
		}
	}

	if (last == -1)
		Error(Error_Fatal,"Cant find last code segment");

	return last;
}

//****************************************
//
//****************************************

int CurrentCodeSegment;

void RebuildJava_Code()
{
	SYMBOL *sym;
	int n;
	int c = 0;

	int LastSeg = FindLastCodeSegment();

	CurrentCodeSegment = GET_CODE_SEGMENT(0);

	RebuildJava_BeginCodeSegment(CurrentCodeSegment);

//	for (n=0;n<CodeIP+1;n++)
	for (n=0;n<CodeIP;n++)
	{

		if (CurrentCodeSegment != LastSeg)
		if (GET_CODE_SEGMENT(n) != CurrentCodeSegment)
		{
			RebuildJava_EndCodeSegment(CurrentCodeSegment);

			CurrentCodeSegment = GET_CODE_SEGMENT(n);
			RebuildJava_BeginCodeSegment(CurrentCodeSegment);
		}

		// Check to see if we are in a new segment, ie. new class

		sym = (SYMBOL *) ArrayGet(&CodeLabelArray, n);

		if (sym)
		{

//			if (strcmp(sym->Name,"__Znam") == 0)
//				printf("");

#ifndef NO_ELIM
			if (sym->Flags & SymFlag_Ref)
#endif
			if (sym->LabelType >= label_Function)
			{
				RebuildJavaFunc(sym);
				c++;
			}
		}
	}

	RebuildJava_EndCodeSegment(CurrentCodeSegment);
}

//****************************************
//
//****************************************

void RebuildJava_CallReg()
{
	SYMBOL *sym;
	int n, count;

	SortVirtuals();
	count = GetVirtualIndex();

	if (!count)
	{
		RebuildEmit("\n// No virtuals\n\n");
		return;
	}
	
	RebuildEmit("\n");
	RebuildEmit("//****************************************\n");
	RebuildEmit("//             CallReg Sink\n");
	RebuildEmit("//****************************************\n\n");


	RebuildEmit("public " DYN_TYPE);
	RebuildEmit("int CallReg(int s, int i0, int i1, int i2, int i3) throws Exception\n");

	RebuildEmit("{\n");

	RebuildEmit("	int r14,r15;\n\n");

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
				JavaCallFunction(sym);
				RebuildEmit("\n");
	//			RebuildEmit("		return;\n\n");	

				ThisFunctionRetType = sym->RetType;

				if(ThisFunctionRetType == RET_void)
				{
					RebuildEmit("			return 0;\n");
				}
				else
				{
					RebuildEmit("\t\t");
					JavaDecodeReturn();
					RebuildEmit("\n");
				}
			}
		}

		RebuildEmit("		default:\n");
		RebuildEmit("			break;\n");

		RebuildEmit("	}\n");
	}

	RebuildEmit("	return 0;\n");
	RebuildEmit("}\n");
}

//****************************************
//
//****************************************

#ifdef JAVA_EMIT_DS


void RebuildJava_EmitDS()
{
	//int need_comma;
	//int count;
	//int n;

	RebuildEmit("\n");
	RebuildEmit("//****************************************\n");
	RebuildEmit("//             Data Section\n");
	RebuildEmit("//****************************************\n\n");

//	GetDataMemLong(data_ip++);

	RebuildEmit("int mem_ds[] = new int[%i];\n\n", Default_DataSize >> 2);

#if 0
	RebuildEmit("static final int data_section[] = {\n");

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

#endif	//0

	RebuildEmit("};\n");
#endif	//0

	RebuildEmit("int ds_len = %d;\n", DataIP >> 2);
	RebuildEmit("int bs_len = %d;\n", BssIP >> 2);
	RebuildEmit("int all_len = %d;\n", (BssIP + DataIP) >> 2);

}

#endif

//****************************************
// 
//****************************************

void RebuildJava_FlowClass()
{
	int n;
	
	RebuildEmit("//****************************************\n");
	RebuildEmit("//          Flow Control Class\n");
	RebuildEmit("//****************************************\n");


	RebuildEmit("class ms\n");
	RebuildEmit("{\n");

	for (n=0;n<MaxEnumLabel+1;n++)
	{		
		RebuildEmit("	public static void label_%d() {}\n", n);
		RebuildEmit("	public static void goto_%d() {}\n", n);
	}

	RebuildEmit("} // end ms class\n");
}

//****************************************
// 
//****************************************

void RebuildJava_StartUp()
{
	SYMBOL *ep;
	FILE* out;

	RebuildEmit("\n");
	RebuildEmit("//****************************************\n");
	RebuildEmit("//          	 Startup\n");
	RebuildEmit("//****************************************\n");
	RebuildEmit("\n");
	RebuildEmit("public " DYN_TYPE "void run(Syscall s) throws Exception\n");
	RebuildEmit("{\n");
	RebuildEmit("	syscall = s;\n");

	RebuildEmit("	int i0,i1,i2;\n");
	RebuildEmit("\n");
	//RebuildEmit("	i0 = i1 = i2 = i3 = r14 = r15 = 0;\n");
	RebuildEmit("	i0 = %i;	//mem size\n", Default_DataSize);
	RebuildEmit("	i1 = %i;	//stack size\n", Default_StackSize);
	RebuildEmit("	i2 = %i;	//heap size\n", Default_HeapSize);
	RebuildEmit("	" SP_STR " = %i-16; //Init stack\n", Default_DataSize);
	RebuildEmit("\n\n");

	// init data array
	//RebuildEmit("	System.arraycopy(data_section, 0, mem_ds, 0, ds_len);\n");
	RebuildEmit("	InputStream is = getClass().getResourceAsStream(\"data_section.bin\");\n");
	RebuildEmit("	DataInputStream dis = new DataInputStream(is);\n");
	RebuildEmit("	for(int i=0; i<ds_len; i++) {\n");
	RebuildEmit("		 mem_ds[i] = dis.readInt();\n");
	RebuildEmit("	}\n");
	RebuildEmit("	dis.close();\n");

	// emit the bin file
	out = fopen("data_section.bin", "wb");
	ArrayWriteFP(&DataMemArray, out, MaxDataIP);
	fclose(out);

	ep	= GetGlobalSym(Code_EntryPoint);

	if (ep)
	{
		JavaCallFunction(ep);
	}
	
	RebuildEmit("\n}\n");
//	RebuildEmit("\n");

	/*
	RebuildEmit("public static void main(String[] args) throws Exception\n");
	RebuildEmit("{\n");

	RebuildEmit("	StaticCode sc = new StaticCode();\n");

	RebuildEmit("	sc.run(new Syscall());\n");
	RebuildEmit("}\n");
	RebuildEmit("\n");
*/
}

//****************************************
// 
//****************************************

#ifdef JAVA_EMIT_INTERFACE_FUNC

void RebuildJava_EmitInterfaceFunc(SYMBOL *sym)
{
	int param_count, need_comma, n;

	if (!sym->Interface)
		return;	

	RebuildEmit("\n");
	RebuildEmit("//****************************************\n");
	RebuildEmit("//  Syscall interface '%s'\n", sym->Name);
	RebuildEmit("//****************************************\n");
	RebuildEmit("\n");

	param_count = sym->Params;

	JavaEmitReturnDecl(sym->RetType);

	RebuildEmit("%s(", sym->Name);

	if (param_count > 4)
		param_count = 4;

	need_comma = 0;

	for (n=0;n<param_count;n++)
	{
		if (need_comma)
			RebuildEmit(", ");				
	
		RebuildEmit("int %s", java_reg[REG_i0 + n]);
		need_comma = 1;
	}

	RebuildEmit(")");

	RebuildEmit("\n{\n");
	RebuildEmit("	%s", sym->Interface);
	RebuildEmit("\n}\n");
}

#endif

//****************************************
// 
//****************************************

#ifdef JAVA_EMIT_INTERFACE_FUNC

void RebuildJava_EmitInterfaces()
{
	SYMBOL *sym;
	int len = sizeof(JavaSyscallUsed);
	int n;
	
	for (n=0;n<len;n++)
	{
		if (JavaSyscallUsed[n])
		{
			sym = FindSysCall(n);

			if (sym)
				RebuildJava_EmitInterfaceFunc(sym);	
		}
	}
}

#endif

//****************************************
// 
//****************************************

#ifdef JAVA_EMIT_SYSCALLS

void RebuildJava_EmitSyscallFunc(SYMBOL *sym)
{
	int param_count, need_comma, n;

	RebuildEmit("\n");
	RebuildEmit("//****************************************\n");
	RebuildEmit("//  Syscall interface '%s'\n", sym->Name);
	RebuildEmit("//****************************************\n");
	RebuildEmit("\n");

	param_count = sym->Params;

	JavaEmitReturnDecl(sym->RetType);

	RebuildEmit("%s(", sym->Name);

	if (param_count > 4)
		param_count = 4;

	need_comma = 0;

	for (n=0;n<param_count;n++)
	{
		if (need_comma)
			RebuildEmit(", ");				
	
		RebuildEmit("int %s", java_reg[REG_i0 + n]);
		need_comma = 1;
	}

	RebuildEmit(")\n");


	RebuildEmit("{\n");

	RebuildEmit("	return");

	if (sym->RetType != RET_void)
		RebuildEmit(" 0");

	RebuildEmit(";\n");
	RebuildEmit("}\n");
}

#endif

//****************************************
// 
//****************************************

#ifdef JAVA_EMIT_SYSCALLS

void RebuildJava_EmitSyscalls()
{
	SYMBOL *sym;
	int len = sizeof(JavaSyscallUsed);
	int n;

	RebuildEmit("\n");
	RebuildEmit("//****************************************\n");
	RebuildEmit("//             Syscall Class \n");
	RebuildEmit("//****************************************\n\n");


	RebuildEmit("final class Syscall\n");

	RebuildEmit("{\n");

	
	for (n=0;n<len;n++)
	{
		if (JavaSyscallUsed[n])
		{
			sym = FindSysCall(n);

			if (!sym->Interface)
				if (sym)
					RebuildJava_EmitSyscallFunc(sym);	
		}
	}

	RebuildEmit("//********** End Syscall Class ***********\n");

	RebuildEmit("}\n");

}

#endif

//****************************************
//
//****************************************

void RebuildJava_BeginCodeSegment(int SegNum)
{
	RebuildEmit("\n");
	RebuildEmit("//----------------------------------------\n");
	RebuildEmit("//            Code Segment %d\n", SegNum);
	RebuildEmit("//----------------------------------------\n");
	RebuildEmit("\n");

	RebuildEmit("public static final class Code%d\n", SegNum);
	RebuildEmit("{\n");
}

//****************************************
//
//****************************************

void RebuildJava_EndCodeSegment(int SegNum)
{
	RebuildEmit("\n} // End Segment %d\n", SegNum);
}

//****************************************
//
//****************************************

void RebuildJava_WriteMemAccessFuncs()
{
	RebuildEmit("\n");
	RebuildEmit("public static " DYN_TYPE "int RBYTE(int addr)\n");
	RebuildEmit("{\n");
	RebuildEmit("	int c = ((" MEM_DS "[addr>>2] >> ((addr & 3) << 3)) & 0x0ff);\n");
	RebuildEmit("	//System.out.println(\"RBYTE(\"+addr+\")=\"+(char)c+\"(\"+c+\")\");\n");
	RebuildEmit("	return c;\n");
	RebuildEmit("}\n\n");

	RebuildEmit("public static " DYN_TYPE "void WBYTE(int addr, int b)\n");
	RebuildEmit("{\n");
	RebuildEmit("	//System.out.println(\"RBYTE(\"+addr+\")=\"+(char)b+\"(\"+b+\")\");\n");
	RebuildEmit("	int shift = (addr & 3) << 3;\n");
	RebuildEmit("	" MEM_DS "[addr >> 2] = (" MEM_DS "[addr >> 2] & ~(0x000000ff << shift)) | ((b & 0x0ff) << shift);\n");
	RebuildEmit("}\n\n");

	RebuildEmit("public static " DYN_TYPE "int RSHORT(int addr)\n");
	RebuildEmit("{\n");
	RebuildEmit("	return ((" MEM_DS "[addr >> 2] >> ((addr & 3) << 3)) & (0x0ffff));\n");
	RebuildEmit("}\n\n");

	RebuildEmit("public static " DYN_TYPE "void WSHORT(int addr, int s)\n");
	RebuildEmit("{\n");
	RebuildEmit("	int shift = (addr & 3) << 3;\n");
	RebuildEmit("	" MEM_DS "[addr >> 2] = (" MEM_DS "[addr >> 2] & ~(0x0ffff << shift)) | ((s & 0x0ffff) << shift);\n");
	RebuildEmit("}\n\n");
}

//****************************************
//
//****************************************

void RebuildJava_Main()
{
	//int res;
	
	if (ArgConstOpt != 0)
		Error(Error_System, "(RebuildJava_Main) ArgConstOpt must be switched off");

	ArrayInit(&RebuildArray, sizeof(char), 0);
	ArrayInit(&LabelDone, sizeof(char), 0);

	memset(JavaSyscallUsed, 0, sizeof(JavaSyscallUsed));

	JavaForceSysCallUsed("RBYTE");
	JavaForceSysCallUsed("WBYTE");

	Rebuild_Mode = 1;

	RebuildEmit("//****************************************\n");
	RebuildEmit("//          Generated java code\n");
	RebuildEmit("//****************************************\n");
	RebuildEmit("\n");
	RebuildEmit("import java.io.*;\n");
	RebuildEmit("\n");

	RebuildEmit("final class StaticCode\n");
	RebuildEmit("{\n");

	RebuildEmit("\n");
	RebuildEmit("int sp;\n");
	RebuildEmit("int __dbl_high;\n");
	RebuildEmit("Syscall syscall;\n");
	RebuildEmit("static int mem_ds[] = new int[%i];\n", Default_DataSize >> 2);

	RebuildEmit("\n");

#ifdef JAVA_EMIT_DS
	RebuildJava_EmitDS();
	RebuildEmit("\n");
#endif

	RebuildJava_WriteMemAccessFuncs();
	RebuildJava_StartUp();

	MaxEnumLabel = 0;

//	RebuildJava_Code();

#ifdef JAVA_EMIT_INTERFACE_FUNC
	RebuildJava_EmitInterfaces();
#endif

	RebuildJava_CallReg();

	RebuildEmit("} // End of StaticCode class\n");

	RebuildJava_Code();

	RebuildEmit("// MaxEnumLabel=%d\n", MaxEnumLabel);
	RebuildEmit("\n");

	RebuildJava_FlowClass();

#ifdef JAVA_EMIT_SYSCALLS
	RebuildJava_EmitSyscalls();
	Rebuild_Variables();
#endif

	ArrayWrite(&RebuildArray, "rebuild.java");

	// Build class file

#if 0	
	res = system("copy rebuild.java StaticCode.java");

	if(res != 0)
		Error(Error_Fatal, "copy fail");

	{
		//const char* cmd = "gcj -O9 -C --CLASSPATH=libgcj.jar StaticCode.java";
		char cmd[2048];

		sprintf(cmd, "gcj -O9 -C %s StaticCode.java", GcjFlags);
		printf("%s\n", cmd);

		res = system(cmd);

		if(res != 0)
			Error(Error_Fatal, "gcj fail");
	}
	
//	system("gcj -O9 -S --CLASSPATH=libgcj.jar StaticCode.java");
//	system("jcf-dump -c StaticCode.class > javadis_before.s");
//	system("del ms.class");
//	system("del rebuild.java");

	PatchClass("StaticCode.class", "StaticCode.class");
//	system("jcf-dump -c StaticCode.class > javadis_after.s");
#endif

//jcf-dump -c StaticCode.class
//gcj -C --CLASSPATH=libgcj.jar rebuild.java
} 



//****************************************
//
//****************************************

void Rebuild_Variable(SYMBOL *sym)
{
//	SYMBOL *thisSym;

	//uint ta = 0;
	
	int ip;
	int len;
//	int n;
//	int c;
	int left;
	int align;

	// decode the field
	
	ip = sym->Value;

	if (sym->Type == SECT_bss)
		ip += MaxDataIP;

	len = sym->EndIP;
	left = len;

	// Has this been done already

	if (ArrayGet(&LabelDone, ip))
		return;

	// Mark as done

	ArraySet(&LabelDone, ip, 1);
	
	if (!len)
	{
		RebuildEmit("// empty %s_%d,%d\n", sym->Name, sym->LocalScope, len);
		return;
	}

	align = ArrayGet(&DataAlignArray, ip);
	
	if (sym->Type == SECT_bss)
	{
		RebuildEmit("// .comm %s_%d,%d\n", sym->Name, sym->LocalScope, len);
		return;
	}
	
	if (sym->Type != SECT_data)
		Error(Error_System, "(Rebuild_Data) Illegal section in data output");

	// write a data section field

	RebuildEmit("// Var %s %d %d %d", sym->Name, sym->LocalScope, sym->Value, sym->EndIP );


	RebuildEmit("\n");
	return;
}

//****************************************
//
//****************************************

void Rebuild_Variables()
{
	SYMBOL *sym;
	int n;

	for (n=0;n<MaxDataIP + BssIP;n++)
	{
		sym = (SYMBOL *) ArrayGet(&LabelArray, n);

		if (sym)
		{
			if (sym->Flags & SymFlag_Ref)
			{
				//printf("%d: Found '%s' size %d\n", n, sym->Name, sym->EndIP);

				Rebuild_Variable(sym);
			}
		}
	}
}

//****************************************
//			  End Of File
//****************************************

#endif
