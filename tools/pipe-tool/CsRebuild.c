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
//				  			  	  Csharp Rebuilder
// 						   		Written by A.R.Hartley
//								Modified by N. Nummelin (from CppRebuild.c)
//*********************************************************************************************

#include "compile.h"

#define INCLUDE_CS_REBUILD

#ifdef INCLUDE_CS_REBUILD

//#define CS_SHOW_LINES
//#define LOG_REGISTER_STATE_CHANGES
//#define CS_DEBUG

#define REGBIT(reg)		(1 << (reg))
#define REGUSED(v, reg) (v & REGBIT(reg))

static int ThisFunctionRegs;			// Register usage for current function
static int ThisFunctionRetType;
static int ThisFunctionExit;			// True on last instruction
static int ReturnCount;

static int CsUsedCallReg;

static char *Cs_reg[] = {"zr","sp","rt","fr","d0","d1","d2","d3",
					"d4","d5","d6","d7","i0","i1","i2","i3",
					"r0","r1","r2","r3","r4","r5","r6","r7",
					"r8","r9","r10","r11","r12","r13","r14","r15"
				   };

static char CsSyscallUsed[1024];

static FuncProp funcprop;


//****************************************
//
//****************************************
#ifdef CS_SHOW_LINES

typedef struct FileInfo_t {
	int numLines;
	char** lines;
	char* fileData;
} FileInfo;

static FileInfo fileInfo[1024];

void InitFiles() {
	size_t n;

	for(n = 0; n < 1024; n++) {
		fileInfo[n].numLines = -1;
		fileInfo[n].fileData = NULL;
	}

	for (n=SLD_File_Array.lo;n<SLD_File_Array.hi+1;n++) {
		int file = ArrayGet(&SLD_File_Array, n);
		FILE *f;
		const char *fileName;
		if(fileInfo[file].numLines!=-1) continue;
		fileName = GetFileNumString(file);
		f = fopen(fileName, "rb");
		if(f)
		{
			size_t i, dataSize, line, res;
			fseek(f, 0, SEEK_END);
			dataSize = ftell(f);

			fileInfo[file].fileData = (char*) malloc(dataSize+1);

			fileInfo[file].fileData[dataSize] = 0;
			fseek(f, 0, SEEK_SET);
			res = fread(fileInfo[file].fileData, 1, dataSize, f);
			if(res != dataSize) {
				printf("Error reading file '%s'\n", fileName);
				exit(1);
			}
			line = 0;
			for(i = 0; i < dataSize; i++) {
				if(fileInfo[file].fileData[i] == '\n') line++;
			}
			fileInfo[file].numLines = line;

			fileInfo[file].lines = (char**) malloc(sizeof(char*)*(line+1));

			line = 0;
			fileInfo[file].lines[0] = fileInfo[file].fileData;
			for(i = 0; i < dataSize; i++) {
				if(fileInfo[file].fileData[i] == '\n') {
					line++;
					fileInfo[file].fileData[i] = 0;
					fileInfo[file].lines[line] = &fileInfo[file].fileData[i+1];
				}
			}

			fclose(f);
		}
	}
}

void FreeFiles() {
	size_t i;
	for(i = 0; i < 1024; i++) {
		if(fileInfo[i].fileData) {
			free(fileInfo[i].fileData);
			free(fileInfo[i].lines);
		}
	}
}

const char* GetFileLine(int file, int line) {
	line-=1;
	if(line<0 || line>=fileInfo[file].numLines) return "?";
	return fileInfo[file].lines[line];
}
#endif

int RebuildCsInst(OpcodeInfo *theOp)
{
	int ip = theOp->rip;
	char str[256];

#ifdef CPP_DEBUG
	str[0] = 0;
	DisassembleFromSource(ip, str);
	RebuildEmit("\t\t\t\t\t\t//%s\n", str);
#endif


#ifdef CS_SHOW_LINES
	{
		int line = ArrayGet(&SLD_Line_Array, ip);
		int file = ArrayGet(&SLD_File_Array, ip);

		if(line!=0) {
			RebuildEmit("\n	// %s:%d\n", GetFileNumString(file), line);
			RebuildEmit("	// %s\n", GetFileLine(file, line));
		}
	}
#endif

	switch (theOp->op)
	{
		case _PUSH:
			RebuildEmit("	//push %s,%d\n",Cs_reg[theOp->rd], theOp->rs);

			if (REGUSED(funcprop.reg_used, REG_sp))
			{
				RebuildEmit("	sp -= %d;\n",theOp->rs*4);
			}
		return 1;

		case _POP:
			RebuildEmit("	//pop  %s,%d\n",Cs_reg[theOp->rd], theOp->rs);

			if (REGUSED(funcprop.reg_used, REG_sp))
			{
				RebuildEmit("	sp += %d;\n",theOp->rs*4);
			}

		return 1;

		case _CASE:
			CsDecodeSwitch(theOp);
		break;


		case _CALLI:
			CsDecodeCall(theOp);
		break;

		case _SYSCALL:
			CsDecodeSysCall(theOp);
		break;

		case _CALL:
			CsDecodeCallReg(theOp);
		break;

		case _LDI:
			RebuildEmit("	%s = %d;", Cs_reg[theOp->rd], theOp->imm);
		break;

		case _LDR:
		{
			if (IsRegConst(theOp->rs))
				RebuildEmit("	%s = %d;", Cs_reg[theOp->rd], ConstRegValue(theOp->rs));
			else
				RebuildEmit("	%s = %s;", Cs_reg[theOp->rd], Cs_reg[theOp->rs]);
		}
		break;

		// Arithmatic

		case _ADD:
			CsEmitArith(theOp,"+", 0);
		break;

		case _ADDI:
			CsEmitArith(theOp,"+", 1);
		break;

		case _MUL:
			CsEmitArith(theOp,"*", 0);
		break;

		case _MULI:
			CsEmitArith(theOp,"*", 1);
		break;

		case _SUB:
			CsEmitArith(theOp,"-", 0);
		break;

		case _SUBI:
			CsEmitArith(theOp,"-", 1);
		break;

		case _AND:
			CsEmitArith(theOp,"&", 0);
		break;

		case _ANDI:
			CsEmitArith(theOp,"&", 1);
		break;

		case _OR:
			CsEmitArith(theOp,"|", 0);
		break;

		case _ORI:
			CsEmitArith(theOp,"|", 1);
		break;

		case _XOR:
			CsEmitArith(theOp,"^", 0);
		break;

		case _XORI:
			CsEmitArith(theOp,"^", 1);
		break;

		case _DIVU:
			CsEmitDivu(theOp, 0);
		break;

		case _DIVUI:
			CsEmitDivu(theOp, 1);
		break;

		case _DIV:
			CsEmitArith(theOp,"/", 0);
		break;

		case _DIVI:
			CsEmitArith(theOp,"/", 1);
		break;

		// Shifts

		case _SLL:
			CsEmitShift(theOp,"<<", 0, 0);
		break;

		case _SLLI:
			CsEmitShift(theOp,"<<", 1, 0);
		break;

		case _SRA:
			CsEmitShift(theOp,">>", 0, 0);
		break;

		case _SRAI:
			CsEmitShift(theOp,">>", 1, 0);
		break;

		case _SRL:
			CsEmitShift(theOp,">>", 0, 1);		// Unsigned
		break;

		case _SRLI:
			CsEmitShift(theOp,">>", 1, 1);		// Unsigned
		break;

		case _NOT:
			RebuildEmit("	%s = ~%s;", Cs_reg[theOp->rd], Cs_reg[theOp->rs]);
		break;

		case _NEG:
			RebuildEmit("	%s = -%s;", Cs_reg[theOp->rd], Cs_reg[theOp->rs]);
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
			CsEmitJumpCond(theOp, "==", 0);
		break;

		case _JC_NE:
			CsEmitJumpCond(theOp, "!=", 0);
		break;

		case _JC_GE:
			CsEmitJumpCond(theOp, ">=", 0);
		break;

		case _JC_GEU:
			CsEmitJumpCond(theOp, ">=", 1);
		break;

		case _JC_GT:
			CsEmitJumpCond(theOp, ">", 0);
		break;

		case _JC_GTU:
			CsEmitJumpCond(theOp, ">", 1);
		break;

		case _JC_LE:
			CsEmitJumpCond(theOp, "<=", 0);
		break;

		case _JC_LEU:
			CsEmitJumpCond(theOp, "<=", 1);
		break;

		case _JC_LT:
			CsEmitJumpCond(theOp, "<", 0);
		break;

		case _JC_LTU:
			CsEmitJumpCond(theOp, "<", 1);
		break;

		case _JPI:
			CsDecodeLabel(theOp, "	goto label_%d;");
		break;

		// Memory instructions

		case _LDW:
			Cs_LoadMem(theOp, "mDataMemory.ReadInt32");
		break;

		case _LDH:
			Cs_LoadMem(theOp, "mDataMemory.ReadInt16");
		break;

		case _LDB:
			Cs_LoadMem(theOp, "mDataMemory.ReadInt8");
		break;

		case _STW:
			Cs_StoreMem(theOp, "mDataMemory.WriteInt32", "");
		break;

		case _STH:
			Cs_StoreMem(theOp, "mDataMemory.WriteUInt16", "(ushort)");
		break;

		case _STB:
			Cs_StoreMem(theOp, "mDataMemory.WriteUInt8", "(byte)");
		break;

		case _XB:
			RebuildEmit("	%s = (int)((sbyte) %s);", Cs_reg[theOp->rd], Cs_reg[theOp->rs]);
		break;

		case _XH:
			RebuildEmit("	%s = (int)((short) %s);", Cs_reg[theOp->rd], Cs_reg[theOp->rs]);
		break;

		default:
			str[0] = 0;
			DisassembleFromSource(ip, str);
			ErrorOnIP(Error_Fatal, ip, "Missing instruction in C# rebuilder '%s'\n", str);
	}

	//	ArraySet(&SLD_Line_Array, CodeIP, line);
	//	ArraySet(&SLD_File_Array, CodeIP, This_SLD_File);

//	RebuildEmit("\n");

	RebuildEmit("\n");

#ifdef LOG_REGISTER_STATE_CHANGES
	if (funcprop.reg_used)
	{
		/*
		int n;
		for (n=0;n<32;n++)
		{
			if (reg_used & (1 << n))
			{
				RebuildEmit("if(last_%s != %s) { LOG_REGISTER(%s); last_%s = %s;}\n", Cpp_reg[n], Cpp_reg[n], Cpp_reg[n], Cpp_reg[n], Cpp_reg[n]);
			}
		}
		RebuildEmit(";\n\n");
		*/
		RebuildEmit("\tLOG_REGISTER_STATE_CHANGES(0x%x)\n", funcprop.reg_used);
	}
#endif


	return 1;
}


//****************************************
//
//****************************************

void CsDecodeReturn(int shouldPassR15)
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
		if(shouldPassR15)
			RebuildEmit("	__dbl_high = r15;\n");
		RebuildEmit("	return r14;");
		break;
	}
}

//****************************************
//
//****************************************

void CsDecodeReturnNull()
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

void CsDecodeSysCall(OpcodeInfo *theOp)
{
	int param_count, need_comma, n;

	SYMBOL *syscall =  FindSysCall(theOp->imm);

	if (!syscall)
	{
		Error(Error_System, "Couldn't locate syscall\n");
		return;
	}

	CsSyscallUsed[theOp->imm]++;

	param_count = syscall->Params;

	CsEmitReturnType(syscall->RetType);

	RebuildEmit("mSyscallInvoker.%s(", &syscall->Name[1]);

	if (param_count > 4)
		param_count = 4;

	need_comma = 0;

	for (n=0;n<param_count;n++)
	{
		if (need_comma)
			RebuildEmit(", ");

		RebuildEmit("%s", Cs_reg[REG_i0 + n]);
		need_comma = 1;
	}

	RebuildEmit(");");

	if (syscall->RetType == RET_double)
		RebuildEmit("\n	r15 = __dbl_high;");
}

//****************************************
//
//****************************************

void CsForceSysCallUsed(char *name)
{
	SYMBOL	*sym;

	sym = FindSymbolsOld(name,section_SysCall,section_SysCall);

	if (!sym)
		return;

	CsSyscallUsed[sym->Value]++;
}

//****************************************
//
//****************************************

void Cs_LoadMem(OpcodeInfo *theOp, char *str)
{

	if (theOp->rs == 0)
	{
		RebuildEmit("	%s = %s(%d);", Cs_reg[theOp->rd], str, theOp->imm);
		return;
	}

	if (theOp->imm == 0)
	{
		RebuildEmit("	%s = %s(%s);", Cs_reg[theOp->rd], str, Cs_reg[theOp->rs]);
		return;
	}

	RebuildEmit("	%s = %s(%s+%d);", Cs_reg[theOp->rd], str, Cs_reg[theOp->rs], theOp->imm);
}


//****************************************
//
//****************************************

void Cs_StoreMem(OpcodeInfo *theOp, char *str, const char* cast)
{

	if (theOp->rd == 0)
	{
		RebuildEmit("	%s(%d, %s%s);", str, theOp->imm, cast, Cs_reg[theOp->rs]);
		return;
	}

	if (theOp->imm == 0)
	{
		RebuildEmit("	%s(%s, %s%s);", str, Cs_reg[theOp->rd], cast, Cs_reg[theOp->rs]);
		return;
	}

	RebuildEmit("	%s(%s+%d, %s%s);", str, Cs_reg[theOp->rd], theOp->imm, cast, Cs_reg[theOp->rs]);
}


//****************************************
//
//****************************************

int CsDecodeLabel(OpcodeInfo *theOp, char *str)
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

	RebuildEmit(str, ref->LabelEnum);
	return 1;
}


//****************************************
//
//****************************************

int CsDecodeCall(OpcodeInfo *theOp)
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

	return CsCallFunction(ref, 1);
}

//****************************************
//
//****************************************

void CsDecodeCallReg(OpcodeInfo *theOp)
{
	// changed ThisFunctionRegs to funcprop.reg_used (didn't take the parameters to the function into account).
	int i0 = funcprop.reg_used & REGBIT(REG_i0);
	int i1 = funcprop.reg_used & REGBIT(REG_i1);
	int i2 = funcprop.reg_used & REGBIT(REG_i2);
	int i3 = funcprop.reg_used & REGBIT(REG_i3);

	RebuildEmit("	r14 = CallReg(%s", Cs_reg[theOp->rd]);

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

	CsUsedCallReg = 1;

	RebuildEmit(");\n");

	// r14 and r15 are always scratch registers after a function call (they may have changed. So we can safely overwrite the content here).
	if(funcprop.reg_used & REGBIT(REG_r15))
	{
		RebuildEmit("	r15 = __dbl_high;\n");
	}
}

//****************************************
//
//****************************************

int CsDecodeCase(int ip)
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

int CsDecodeSwitch(OpcodeInfo *theOp)
{
	int start, len, data_ip, def_ip, i;
	int lab_ip;

	data_ip = theOp->imm;

	start	= GetDataMemLong(data_ip++);
	len		= GetDataMemLong(data_ip++);
	def_ip	= GetDataMemLong(data_ip++);	// default case


	RebuildEmit("	switch(%s)\n",  Cs_reg[theOp->rd]);
	RebuildEmit("	{\n");

	for (i=0;i<len+1;i++)
	{
		lab_ip = GetDataMemLong(data_ip++);

		RebuildEmit("		case %d: ", start);
		CsDecodeCase(lab_ip);
		RebuildEmit("\n");
		start++;
	}

	RebuildEmit("		default: ", start);
	CsDecodeCase(def_ip);
	RebuildEmit("\n");


	RebuildEmit("	}\n");


	return 1;
}

//****************************************
//
//****************************************

int CsCallFunction(SYMBOL *ref, int emit_r15)
{
	int param_count, need_comma, n;
	int rettype = ref->RetType;
	int regs;

	CsEmitReturnType(rettype);

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

		RebuildEmit("%s", Cs_reg[REG_i0 + n]);
		need_comma = 1;
	}

	RebuildEmit(");");

	if (rettype == RET_double && emit_r15 && (regs & REGBIT(REG_r15)))
	{
		RebuildEmit("\n	r15 = __dbl_high;");
		SetRegInit(REG_r15);
	}
	return 1;
}

//****************************************
//
//****************************************

void CsEmitArith(OpcodeInfo *theOp, char *str, int imm)
{
	if (imm)
	{
		RebuildEmit("	%s %s= %d;", Cs_reg[theOp->rd], str, theOp->imm);
		return;
	}

	RebuildEmit("	%s %s= %s;", Cs_reg[theOp->rd], str, Cs_reg[theOp->rs]);
	return;
}

//****************************************
//
//****************************************

void CsEmitShift(OpcodeInfo *theOp, char *oper, int imm, int issigned)
{
	char *us = "";

	if (issigned)
		us = "(uint)";

	// Dest

	RebuildEmit("	%s = ", Cs_reg[theOp->rd]);

	// Typecast and Src

	RebuildEmit("(int)(%s %s", us, Cs_reg[theOp->rd]);

	// Operator

	RebuildEmit(" %s ", oper);

	if (imm)
	{
		RebuildEmit("%d);", theOp->imm);
		return;
	}
	else
		RebuildEmit("%s);", Cs_reg[theOp->rs]);
	return;
}

//****************************************
//
//****************************************

void CsEmitDiv(OpcodeInfo *theOp, int imm)
{
	if ((theOp->rs == 0) || (theOp->imm == 0))
		printf(" ");

	if (imm)
	{
		if (theOp->imm == 0)
			Warning("Division by zero in recompiler");

		RebuildEmit("	%s /= %d;", Cs_reg[theOp->rd], theOp->imm);
	}
	else
	{
		RebuildEmit("	if (%s == 0) MoSyncDiv0();\n", Cs_reg[theOp->rs]);
		RebuildEmit("	%s /= %s;", Cs_reg[theOp->rd], Cs_reg[theOp->rs]);
	}
}

//****************************************
//
//****************************************

void CsEmitDivu(OpcodeInfo *theOp, int imm)
{
	if (imm)
	{
		if (theOp->imm == 0)
			Warning("Division by zero in recompiler");

		RebuildEmit("	%s = (int) ((uint)(%s) / (uint)(%d));", Cs_reg[theOp->rd], Cs_reg[theOp->rd], theOp->imm);
	}
	else
	{
		RebuildEmit("	if(%s == 0) MoSyncDiv0();\n", Cs_reg[theOp->rs]);
		RebuildEmit("	%s = (int) ((uint)(%s) / (uint)(%s));", Cs_reg[theOp->rd], Cs_reg[theOp->rd], Cs_reg[theOp->rs]);
	}
}

//****************************************
//
//****************************************

void CsEmitJumpCond(OpcodeInfo *theOp, char *str, int unsign)
{
	if(unsign)
		RebuildEmit("	if ((uint)%s %s (uint)%s) ",Cs_reg[theOp->rd], str, Cs_reg[theOp->rs]);
	else
		RebuildEmit("	if (%s %s %s) ",Cs_reg[theOp->rd], str, Cs_reg[theOp->rs]);
	CsDecodeLabel(theOp, "goto label_%d;");

	unsign = 0;
}

//****************************************
//
//****************************************

void CsEmitReturnType(int rettype)
{
	switch(rettype)
	{
		case RET_null:
		RebuildEmit("? // Error report to MoSync");
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

void CsEmitReturnDecl(int rettype)
{
	switch(rettype)
	{
		case RET_null:
		RebuildEmit("? // Error report to MoSync");
		break;

		case RET_void:
		RebuildEmit("private void ");
		break;

		case RET_float:
		case RET_int:
		RebuildEmit("private int ");
		break;

		case RET_double:
		RebuildEmit("private int ");
		break;
	}
}

//****************************************
//		Disassemble Function
//****************************************

void RebuildCsProlog(SYMBOL *sym, int isproto)
{
	int reg_used;
	int reg_alloc;

	int param_count;
	int need_comma;
	int n;

	// Find registers used in function

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
		RebuildEmit("//tfr        = %s\n", Bin32(ThisFunctionRegs));
		RebuildEmit("\n");
	}
	else RebuildEmit("static ");

	// Output function decl
	switch(ThisFunctionRetType)
	{
		case RET_null:
		RebuildEmit(" ?; // Error report to MobileSorcery\n");
		break;

		case RET_void:
		RebuildEmit("private void ");
		break;

		case RET_float:
		case RET_int:
		case RET_double:
		RebuildEmit("private int ");
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

		RebuildEmit("int %s", Cs_reg[REG_i0 + n]);
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

	// ok this has sort of been reverse engineered by looking at the bitmasks in the rebuilt code... ;)
	// this is added to remove warnings about unusued variables.
	reg_used &= ThisFunctionRegs | (1 << REG_r14) | (1 << REG_r15) | (1 << REG_i0) | (1 << REG_i1) | (1 << REG_i2) | (1 << REG_i3);

	reg_used &= (~reg_alloc);

	// remove sp from locals

	reg_used &= ~(1 << REG_sp);
	reg_used &= ~(1 << REG_zero);

	if (ThisFunctionRetType != RET_null && ThisFunctionRetType != RET_void)
		reg_used |= (1 << REG_r14);
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

				RebuildEmit("%s", Cs_reg[n]);

				//if (funcprop.uninit_reg & (1<<n))
					RebuildEmit("=0");

				need_comma = 1;
			}
		}

		RebuildEmit(";\n\n");
	}

}

//****************************************
//		Disassemble Function
//****************************************

void RebuildCsEpilog(SYMBOL *sym)
{
//	ThisFunctionExit

	if (ReturnCount > 0)
		RebuildEmit("label_0:;\n");

	CsDecodeReturn(1);
	RebuildEmit("\n");

	RebuildEmit("} // %s\n", sym->Name);
}

//****************************************
//		Disassemble Function
//****************************************

void RebuildCsFunc(SYMBOL *sym, int isproto)
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

	RebuildCsProlog(sym, isproto);

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
#ifdef CS_DEBUG
				RebuildEmit("// %s_%d:\n", ref->Name, ref->LocalScope);
#endif
				RebuildEmit("label_%d:\n", ref->LabelEnum);
			}
		}

		if (ArrayGet(&CodeTouchArray, real_ip) == 0)
			RebuildEmit("// ");

		CaseRef = 0;

		ip = DecodeOpcode(&thisOp, ip);

		ThisFunctionExit = 0;

		if (ip > ip_end)
			ThisFunctionExit = 1;

		RebuildCsInst(&thisOp);

//		DecodeAsmString(&thisOp, str);
//		RebuildEmit("\t%s", str);

#ifdef CS_DEBUG
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

	RebuildCsEpilog(sym);
}

//****************************************
//
//****************************************

void RebuildCs_Code()
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
				RebuildCsFunc(sym, 0);
				c++;
			}
		}
	}
}

//****************************************
//
//****************************************
#if 0
void RebuildCs_EmitProtos()
{
	SYMBOL *sym;
	int n;

	RebuildEmit("\n// Prototypes\n\n");

	RebuildEmit("static int CallReg(int s, int i0, int i1, int i2, int i3);\n");

	for (n=0;n<CodeIP+1;n++)
	{
		sym = (SYMBOL *) ArrayGet(&CodeLabelArray, n);

		if (sym)
		{
			if (sym->Flags & SymFlag_Ref)
			if (sym->LabelType >= label_Function)
			{
				RebuildCsFunc(sym, 1);
			}
		}
	}
}
#endif

//****************************************
//
//****************************************

void RebuildCs_CallReg()
{
	SYMBOL *sym;
	int n, count;

	SortVirtuals();
	count = GetVirtualIndex();

//	if (!count)
	if (!CsUsedCallReg)
	{
		RebuildEmit("\n// No virtuals\n\n");
		return;
	}

	RebuildEmit("\n");
	RebuildEmit("//****************************************\n");
	RebuildEmit("//             CallReg Sink\n");
	RebuildEmit("//****************************************\n\n");


	RebuildEmit("private int CallReg(int s, int i0, int i1, int i2, int i3)\n");
	RebuildEmit("{\n");

//	RebuildEmit("	int r14,r15;\n");
//	RebuildEmit("	r14 = r15 = 0;\n\n");
	RebuildEmit("	int r14 = 0;\n\n");

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
				CsCallFunction(sym, 0);
				RebuildEmit("\n\t");

				ThisFunctionRetType = sym->RetType;

				if(ThisFunctionRetType == RET_void)
				{
					RebuildEmit("	return 0;\n\n");
				}
				else
				{
					CsDecodeReturn(0);			// was 1
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

void RebuildCs_StartUp()
{
	SYMBOL *ep;
	FILE *out;

	RebuildEmit("\n");
	RebuildEmit("//****************************************\n");
	RebuildEmit("//          	 Startup\n");
	RebuildEmit("//****************************************\n");
	RebuildEmit("\n");

#ifdef LOG_REGISTER_STATE_CHANGES
	{
	int n;
	for(n = 0; n < 32; n++) {
		RebuildEmit("static int last_%s = 0;\n", Cpp_reg[n]);
	}


	RebuildEmit("#define LOG_REGISTER_STATE_CHANGES(reg_used)\\\n");
	for(n = 0; n < 32; n++) {
		RebuildEmit("if((regused&(1<<%d)) && (last_%s != %s)) { last_%s = %s; LOG_REGISTER(%s); }\\\n", n, Cpp_reg[n], Cpp_reg[n], Cpp_reg[n], Cpp_reg[n], Cpp_reg[n], Cpp_reg[n]);
	}
	RebuildEmit("\n");
	}
#endif


	RebuildEmit("public class CoreNativeProgram : MoSync.CoreNative\n");
	RebuildEmit("{\n\n");

	RebuildEmit("protected const int ds_len = %d;\n", DataIP);
	RebuildEmit("protected const int bs_len = %d;\n", BssIP);
	RebuildEmit("protected const int all_len = %d;\n", (BssIP + DataIP));
	RebuildEmit("protected const int max_data = %d;\n\n", MaxDataIP);

	RebuildEmit("public override void Main()\n");
	RebuildEmit("{\n");

	RebuildEmit("	int i0,i1,i2;\n");
	RebuildEmit("\n");

	RebuildEmit("	i0 = %i;	//mem size\n", Default_DataSize);
	RebuildEmit("	i1 = %i;		//stack size\n", Default_StackSize);
	RebuildEmit("	i2 = %i;		//heap size\n", Default_HeapSize);
	RebuildEmit("	sp = %i-16; //Init stack\n", Default_DataSize);
	RebuildEmit("\n");

	RebuildEmit("	InitData(\"data_section.bin\", max_data, i0);\n");

	// emit the bin file
	out = fopen("data_section.bin", "wb");
	ArrayWriteFP(&DataMemArray, out, MaxDataIP);
	fclose(out);

	ep	= GetGlobalSym(Code_EntryPoint);

	if (ep)
		CsCallFunction(ep, 1);

	RebuildEmit("\n}\n");
}

//****************************************
//
//****************************************

void RebuildCs_EmitSyscallFunc(SYMBOL *sym, int proto, int stub)
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

	CsEmitReturnDecl(sym->RetType);

	RebuildEmit("%s(", sym->Name);

	if (param_count > 4)
		param_count = 4;

	need_comma = 0;

	for (n=0;n<param_count;n++)
	{
		if (need_comma)
			RebuildEmit(", ");

		RebuildEmit("int %s", Cs_reg[REG_i0 + n]);
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
		CsDecodeReturnNull();
	}

	RebuildEmit("\n}\n");
}

//****************************************
//
//****************************************

void RebuildCs_EmitExtensions(int stub)
{
	SYMBOL *sym;
	int len = sizeof(CsSyscallUsed);
	int n;

	for (n=0;n<len;n++)
	{
		if (CsSyscallUsed[n])
		{
			sym = FindSysCall(n);

			if (sym)
				RebuildCs_EmitSyscallFunc(sym, 0, stub);
		}
	}
}

//****************************************
//
//****************************************

void RebuildCs_EmitExtensionsProto()
{
	SYMBOL *sym;
	int len = sizeof(CsSyscallUsed);
	int n;

	for (n=0;n<len;n++)
	{
			sym = FindSysCall(n);

			if (sym)
				RebuildCs_EmitSyscallFunc(sym, 1, 1);
	}
}

//****************************************
//
//****************************************

void RebuildCs_Main()
{
	if (ArgConstOpt != 0)
		Error(Error_System, "(RebuildCs_Main) ArgConstOpt must be switched off");

	#ifdef CPP_SHOW_LINES
	InitFiles();
	#endif

	ArrayInit(&RebuildArray, sizeof(char), 0);
	ArrayInit(&LabelDone, sizeof(char), 0);

	memset(CsSyscallUsed, 0, sizeof(CsSyscallUsed));

	CsForceSysCallUsed("RBYTE");
	CsForceSysCallUsed("WBYTE");

	Rebuild_Mode = 1;
	CsUsedCallReg = 0;

	RebuildEmit("//****************************************\n");
	RebuildEmit("//          Generated Cs code\n");
	RebuildEmit("//****************************************\n");
	RebuildEmit("\n");

	RebuildEmit("#if REBUILD\n\n");

	RebuildEmit("// disable unreferenced label warnings\n");
	RebuildEmit("#pragma warning disable 0164\n");
	//RebuildEmit("#include \"mstypeinfo.h\"\n");
	//RebuildEmit("\n");

#if 0			//My Testing only
	RebuildCs_EmitExtensionsProto();
#endif

#if 0
	RebuildCs_EmitProtos();
#endif

//	RebuildEmit("class MoSyncCode\n");
//	RebuildEmit("{\n");

//	RebuildEmit("\n");					// Out in .h file
//	RebuildEmit("int sp;\n");
//	RebuildEmit("int __dbl_high;\n");

	RebuildEmit("\n");

	RebuildCs_StartUp();

	MaxEnumLabel = 0;

	RebuildCs_Code();
	//RebuildCs_EmitExtensions(1);
	RebuildCs_CallReg();

	RebuildEmit("\n} // CoreNative\n");
	RebuildEmit("#endif // REBUILD\n");
//	RebuildEmit("}; // End of MosyncCode class\n");
//	RebuildEmit("// MaxEnumLabel=%d\n", MaxEnumLabel);

//	RebuildCs_FlowClass();

	#ifdef CS_SHOW_LINES
	FreeFiles();
	#endif

	ArrayWrite(&RebuildArray, "rebuild.build.cs");
}

//****************************************
//			  End Of File
//****************************************

#endif
