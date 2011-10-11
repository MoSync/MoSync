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
//				       PIP-e II Assembler Routines
// 						   Written by A.R.Hartley
//			This code is based on the GameTasker v2.0 Module System
//*********************************************************************************************

#include "compile.h"


//****************************************
//		   Decode instructions
//****************************************

#if 1

#define Opcode(tok,method,opnum,opimm)		\
		case tok:							\
		{									\
			SkipWhiteSpace();				\
			method(ThisOpName,opnum,opimm);	\
			return;							\
		}

char ThisOpName[128];
char *AsmCharPtr = 0;

void Instructions()
{
	int t;

	if (*FilePtr == '.')
		return;

	if (*FilePtr == '}')
		return;

	if (*FilePtr == '{')
		return;

	// Clear all the special variables

	farop = imm = rd = rs = rt = op = 0;

	SkipWhiteSpace();

	AsmCharPtr = FilePtr;

	// Save IP file offset lookup

	ArraySet(&AsmCharIPArray, (int) (AsmCharPtr - FileTop) , CodeIP + 1);

	GetToken();
	t = ScanAsmToken();

	strcpy(ThisOpName, Name);

	switch(t)
	{
		Opcode(opc_syscall,Get_syscall,0,0);
		Opcode(opc_break,Get_break,0,0);
		Opcode(opc_case,Get_case,_CASE,0);
		Opcode(opc_push,Get_push_pop,_PUSH,0);
		Opcode(opc_call,Get_call,_CALL,_CALLI);
		Opcode(opc_divu,Get_Arith2,_DIVU,_DIVUI);
		Opcode(opc_ldb,Get_load_store,_LDB,_STB);
		Opcode(opc_ldh,Get_load_store,_LDH,_STH);
		Opcode(opc_div,Get_Arith2,_DIV,_DIVI);
		Opcode(opc_pop,Get_push_pop,_POP,0);
		Opcode(opc_and,Get_Arith2,_AND,_ANDI);
		Opcode(opc_xor,Get_Arith2,_XOR,_XORI);
		Opcode(opc_sll,Get_Shift,_SLL,_SLLI);
		Opcode(opc_sra,Get_Shift,_SRA,_SRAI);
		Opcode(opc_srl,Get_Shift,_SRL,_SRLI);
		Opcode(opc_add,Get_Arith2,_ADD,_ADDI);
		Opcode(opc_mul,Get_Arith2,_MUL,_MULI);
		Opcode(opc_sub,Get_Arith2,_SUB,_SUBI);
		Opcode(opc_ret,NullInst,_RET,0);
		Opcode(opc_nop,NullInst,_NOP,0);
		Opcode(opc_brk,NullInst,_ENDOP,0);
		Opcode(opc_not,Get_rd_rs,_NOT,0);
		Opcode(opc_neg,Get_rd_rs,_NEG,0);
		Opcode(opc_ld,Get_load_store,_LDW,_STW);
		Opcode(opc_jc,Get_jmp_cond,_CALL,_CALLI);
		Opcode(opc_jp,Get_jmp,_JPR,_JPI);
		Opcode(opc_or,Get_Arith2,_OR,_ORI);
		Opcode(opc_xb,Get_rd_rs,_XB,0);
		Opcode(opc_xh,Get_rd_rs,_XH,0);
	}

	Name[64] = 0;

	Error(Error_Skip, "Illegal instruction '%s'",Name);
}

//****************************************
//		   Decode instructions
//****************************************

#else

#define Opcode(string,method,opnum,opimm)		\
		if (QToken(string))						\
		{										\
			method(string,opnum,opimm);			\
			return;								\
		}

void Instructions()
{
	SkipWhiteSpace();

	if (*FilePtr == '.')
		return;

	// Clear all the special variables

	farop = imm = rd = rs = rt = op = 0;

	AsmCharPtr = FilePtr;

	Opcode("syscall",Get_syscall,0,0);
	Opcode("break",Get_break,0,0);
	Opcode("case",Get_case,_CASE,0);
	Opcode("push",Get_push_pop,_PUSH,0);
	Opcode("call",Get_call,_CALL,_CALLI);
	Opcode("divu",Get_Arith2,_DIVU,_DIVUI);
	Opcode("ld.b",Get_load_store,_LDB,_STB);
	Opcode("ld.h",Get_load_store,_LDH,_STH);
	Opcode("div",Get_Arith2,_DIV,_DIVI);
	Opcode("pop",Get_push_pop,_POP,0);
	Opcode("and",Get_Arith2,_AND,_ANDI);
	Opcode("xor",Get_Arith2,_XOR,_XORI);
	Opcode("sll",Get_Shift,_SLL,_SLLI);
	Opcode("sra",Get_Shift,_SRA,_SRAI);
	Opcode("srl",Get_Shift,_SRL,_SRLI);
	Opcode("add",Get_Arith2,_ADD,_ADDI);
	Opcode("div",Get_Arith2,_DIV,_DIVI);
	Opcode("mul",Get_Arith2,_MUL,_MULI);
	Opcode("sub",Get_Arith2,_SUB,_SUBI);
	Opcode("ret",NullInst,_RET,0);
	Opcode("brk",NullInst,_ENDOP,0);
	Opcode("nop",NullInst,_NOP,0);
	Opcode("not",Get_rd_rs,_NOT,0);
	Opcode("neg",Get_rd_rs,_NEG,0);
	Opcode("ld",Get_load_store,_LDW,_STW);
	Opcode("jc",Get_jmp_cond,_CALL,_CALLI);
	Opcode("jp",Get_jmp,_JPR,_JPI);
	Opcode("or",Get_Arith2,_OR,_ORI);
	Opcode("xb",Get_rd_rs,_XB,0);
	Opcode("xh",Get_rd_rs,_XH,0);

	// Special opcodes

	Error(Error_Skip, "Illegal instruction '%s'",Name);
}

#endif

//****************************************
//
//****************************************

void NullInst(char *AsmName,int Opcode, int OpcodeImm)
{
#if !defined(_MSC_VER) && !defined(__GNUC__)
#pragma unused(OpcodeImm)
#endif

	op = Opcode;
	WriteOpcode(AsmName, 0);
}

//****************************************
//
//****************************************

void Get_break(char *AsmName,int Opcode, int OpcodeImm)
{
#if 0
	Opcode = 0;
	OpcodeImm = 0;
	AsmName = 0;
#endif
}

//****************************************
//		Decode case instructions
//****************************************

// Case reg, startnum, length, switchtable, default

int case_reg;
int case_startnum_imm;
int case_length_imm;
int switch_table_imm;
int switch_default;

void Get_case(char *AsmName,int Opcode, int OpcodeImm)
{
	int case_dataip;
	int oldsect;

	//OpcodeImm = 0;

	// Get the switch register

	case_reg = GetReg(1);

	// Skip the comma

	NeedToken(",");
	SkipWhiteSpace();

	// Get the start number

	NeedToken("#");
	HandleExpr();
	CheckBadCodeRef();		// Make sure no code ref in write

	case_startnum_imm = imm;

	// Skip the comma

	NeedToken(",");
	SkipWhiteSpace();

	// Get the length

	NeedToken("#");
	HandleExpr();
	CheckBadCodeRef();		// Make sure no code ref in write

	case_length_imm = imm;

	// Skip the comma

	NeedToken(",");
	SkipWhiteSpace();

	// get the switch table address

	NeedToken("#");

	GetAsmName();
	switch_table_imm = GetEnumValue(Name);

	if (Final_Pass)
		if (EnumIsFunction())
			Error(Error_Skip, "case targets a function");

	CheckDataAccess();

	// Skip the comma

	NeedToken(",");
	SkipWhiteSpace();

	// get the switch default address

	NeedToken("#");

	GetAsmName();
	switch_default = GetEnumValue(Name);

	if (Final_Pass)
		if (EnumIsFunction())
			Error(Error_Skip, "default in case targets a function");

	// Mark the default address in the call array

	if (GetLastSymbolRef())
		ArraySet(&CallArray, CodeIP, (int) GetLastSymbolRef());

	// Add a padding byte

//	PadDataSection(1);

	// make sure data is aligned

	AlignDataSection();

	// Get the data section address of the top of the case data

	case_dataip = DataIP >> 2;

	// Save the case data
	// Save the current section and select the data section

	oldsect = Section;
	Section = SECT_data;

	sprintf(Name, "_case_%x", DataIP);
	AddLabel(Name, 0);

	// Write the switch data

	WriteLong(case_startnum_imm);		// Write the start num
	WriteLong(case_length_imm);			// Write the case length
	WriteLong(switch_default);			// Address of the default case

	// ... Switch table follows

	// Put the old section back
	Section = oldsect;

	// write the opcode

	op = Opcode;
	rd = case_reg;
	imm = case_dataip;

	WriteOpcode(AsmName, use_rd | use_int24);
}

//****************************************
//		Decode call instructions
//****************************************

void Get_call(char *AsmName,int Opcode, int OpcodeImm)
{
	//int entry_sym = 0;

	if (Token("&"))
	{
		// Check for a _SYSCALL

		GetAsmName();
		imm = isSysCall(Name);

		if (imm != -1)
		{
			op = _SYSCALL;
			WriteOpcode("SysCall", use_int8);		// was use_int
			return;
		}

		// Check for Thunks

		if (NextToken("%"))
			GetAsmThunkName();

		// Evaluate the enum

		imm = GetEnumValue(Name);

		// Make sure this enum was a function

		//FIXME
		//if (Final_Pass)
			//if (!EnumIsFunction())
				//Warning("Warning: call targets a non-function\n");

		ArraySet(&CallArray, CodeIP, (int) GetLastSymbolRef());

		if (imm & 0xffff0000)	// Check for far jumps
		{
			farop = 1;
		}

		op = OpcodeImm;
		WriteOpcode(AsmName, use_addr);
		return;
	}

	rd = GetReg(0);

	if (rd == -1)
	{
		// Not a reg must be a LC label

		rd = 0;

//!! Old
//		HandleExpr();

//!!added

		GetAsmName();

		// Normal symbol
		imm = GetEnumValue(Name);

		// Make sure this enum was a function

		//FIXME
		//if (Final_Pass)
			//if (!EnumIsFunction())
				//Warning("Warning: call targets a non-function\n");

		ArraySet(&CallArray, CodeIP, (int) GetLastSymbolRef());

//!!

		if (imm & 0xffff0000)	// Check for far jumps
		{
			farop = 1;
		}

		op = OpcodeImm;
		WriteOpcode(AsmName, use_addr);
		return;
	}

	op = Opcode;
	WriteOpcode(AsmName, use_rd);
	return;
}

//****************************************
//		Decode call instructions
//****************************************

void Get_syscall(char *AsmName,int Opcode, int OpcodeImm)
{
	//AsmName = 0;
	//Opcode = OpcodeImm = 0;

	imm = GetExpression();					// Get the syscall number
	op = _SYSCALL;
	WriteOpcode("SysCall", use_int8);		// was use_int
	return;
}

//****************************************
//	Make sure a branch ip is within
// 		the current function
//****************************************

void CheckLocalBounds(int branch_ip)
{
	SYMBOL *sym = CurrentFunction;

	if (AllowFarBranches)
		return;

	if (!sym)
		return;

	if (Final_Pass == 0)
		return;

	if (branch_ip < sym->Value)
	{
		Error(Error_Skip, "branch above function scope in '%s' (ip=%d)", sym->Name, branch_ip);
	}

	if (sym->EndIP == 0)
		return;

	if (branch_ip > sym->EndIP)
	{
		Error(Error_Skip, "branch below function scope in '%s' (ip=%d)", sym->Name, branch_ip);
	}
}

//****************************************
//		Decode jump instructions
//****************************************

char JumpTargetName[256];

//get imm

void Get_jmp(char *AsmName,int Opcode, int OpcodeImm)
{
	op = OpcodeImm;
	imm = 0;

	if (Token("&"))
	{
		// only allow labels

		GetAsmName();
		strcpy(JumpTargetName, Name);

		imm = GetEnumValue(Name);

		if (ArgVerifierOff == 0)
			if (Final_Pass)
				if (EnumIsFunction())
					Error(Error_Skip, "jmp targets a function");

		ArraySet(&CallArray, CodeIP, (int) GetLastSymbolRef());

		if (imm & 0xffff0000)	// Check for far jumps
		{
			farop = 1;
		}

		CheckLocalBounds(imm);

		WriteOpcode(AsmName, use_addr);
		return;
	}

	rd = GetReg(0);

	if (rd == -1)
	{
		// Not a reg must be a LC label
		rd = 0;

		NeedToken("#");

		// get name

		GetAsmName();
		strcpy(JumpTargetName, Name);

		imm = GetEnumValue(Name);

		if (ArgVerifierOff == 0)
			if (Final_Pass)
				if (EnumIsFunction())
					Error(Error_Skip, "jmp targets a function");

		ArraySet(&CallArray, CodeIP, (int) GetLastSymbolRef());

		if (imm & 0xffff0000)	// Check for far jumps
		{
			farop = 1;
		}

		CheckLocalBounds(imm);

		WriteOpcode(AsmName, use_addr);
		return;
	}

//	if (JavaPass)
//		Error(Error_Fatal, "Illegal obselete instruction, jmp register");

//	Error(Error_Skip, "Illegal instruction, jp register");

	op = Opcode;
	WriteOpcode(AsmName, use_rd);
	return;
}

//****************************************
//	Decode conditional jmp instructions
//****************************************

void Get_jmp_cond(char *AsmName,int Opcode, int OpcodeImm)
{
#if !defined(_MSC_VER) && !defined(__GNUC__)
#pragma unused(Opcode)
#pragma unused(OpcodeImm)
#endif

	//int cond = 0;

	imm = 0;

	if (Token("gtu"))
		op = _JC_GTU;
	else if (Token("geu"))
		op = _JC_GEU;
	else if (Token("ltu"))
		op = _JC_LTU;
	else if (Token("leu"))
		op = _JC_LEU;
	else if (Token("eq"))
		op = _JC_EQ;
	else if (Token("ne"))
		op = _JC_NE;
	else if (Token("gt"))
		op = _JC_GT;
	else if (Token("ge"))
		op = _JC_GE;
	else if (Token("lt"))
		op = _JC_LT;
	else if (Token("le"))
		op = _JC_LE;
	else
		Error(Error_Skip, "Unknown jump condition");

	NeedToken(",");

	rd = GetReg(1);

	NeedToken(",");

	rs = GetReg(1);

	NeedToken(",");

	// Resolve name

	if (Token("&"))
	{
		GetAsmName();
	}
	else
	{
		NeedToken("#");
		GetAsmName();
	}

	// get name

	strcpy(JumpTargetName, Name);

	imm = GetEnumValue(Name);

	if (Final_Pass)
		if (EnumIsFunction())
			Error(Error_Skip, "jc targets a function");

	ArraySet(&CallArray, CodeIP, (int) GetLastSymbolRef());

	// ** Check for far jumps

	if (imm & 0xffff0000)
	{
		farop = 1;
	}

	CheckLocalBounds(imm);

	WriteOpcode(AsmName, use_rd | use_rs | use_addr);
	return;
}


//****************************************
//Check if the last expression contained
//  a reference to code, if so error
//****************************************

void CheckBadCodeRef()
{
	char *ref = GetExpCodeRef();

	if (ref)
		Error(Error_Skip, "Illegal reference to code : '%s'", ref);
}

//****************************************
//	  Check if data section elements
//			 were accessed
//****************************************

void CheckDataAccess()
{
	int ref = (int) GetLastSymbolRef();

	if (!ref)
		return;

	ArraySet(&DataAccessArray, CodeIP, ref);
	return;
}

//****************************************
//	  Check if data section elements
//			 were accessed
//****************************************

void CheckDataAccess_LoadStore(int opcode)
{
	SYMBOL *ref = GetLastSymbolRef();
	int size = 0;
	int addr;

	if (!ref)
		return;

	ArraySet(&DataAccessArray, CodeIP, (int) ref);

	if (!((ref->Section == SECT_data) || (ref->Section == SECT_data)))
		return;

	switch(opcode)
	{
		case _LDB:
		case _STB:
			size = 1;
			break;

		case _LDH:
		case _STH:
			size = 2;
			break;

		case _LDI:
		case _LDR:
		case _LDW:
		case _STW:
			size = 4;
			break;

		default:
			Error(Error_Fatal, "unknown type in DataMixAccess %d", opcode);
	}

	addr = ref->Value;

	// deal with accessor

	if (ref->Section == SECT_bss)
		addr += MaxDataIP;

	WriteTypeAccessArray(addr, size);
	return;
}

//****************************************
//		Decode ld instructions
//****************************************

void Get_load_store(char *AsmName,int OpcodeLoad, int OpcodeStore)
{
	int const_subst;

	//if ((CodeIP == 0x52) && (pass_count == 30))
		//printf("");

	// Get the dest reg

	imm = FindVar(0);				// set imm as preset zero

	if (Token("["))					// ld [], rs
	{
		//----------------------------------------
		// Is a store instruction
		//----------------------------------------

		if (Token("&"))				// ld [&addr],rs
		{
			// Test for direct address

			rd = 0;

			HandleExprImm();
			CheckBadCodeRef();		// Make sure no code ref in write

			CheckDataAccess_LoadStore(OpcodeStore);

			NeedToken("]");

			NeedToken(",");

			rs = GetReg(1);

			op = OpcodeStore;
			WriteOpcode(AsmName, use_rd | use_rs | use_int);		// ld [rd, imm], rs
			return;
		}

		// Is offset address

		rd = GetReg(1);

		// test for offset

		if (Token(","))
		{
			Token("&");				// There may be a '&' token !!

			// Get imm
			HandleExprImm();

			CheckDataAccess_LoadStore(OpcodeStore);
		}

		NeedToken("]");

		NeedToken(",");

		rs = GetReg(1);

		op = OpcodeStore;
		WriteOpcode(AsmName, use_rd | use_rs | use_int);		// ld [rd, imm], rs
		return;

	}

	//----------------------------------------
	// Is a load instruction		ld rd,
	//----------------------------------------

	rd = GetReg(1);

	// Print a warning

	if (rd >= 32 || rd == 0)
		if (Pass == 2)
			printf("*Warning* Instruction overwrites constant register %s\n", GetRegName(rd));

	NeedToken(",");

	if (Token("#"))				// ld rd,#
	{
		// Is load immediate

		const_subst = HandleExprConst();

		if (const_subst)
		{
			CheckDataAccess_LoadStore(_LDR);

			if (LIST) printf("constant reg substitution (r%d = %x)\n", imm, ConstTable[imm & 0x7f]);

			op = _LDR;
			rs = imm;
			WriteOpcode(AsmName, use_rd | use_rs);		// ld rd, rs
			return;
		}

		// Must test for 0 in immediate

		CheckDataAccess_LoadStore(_LDI);

		// If the expression had a coderef then force int to fully expand

		if (GetExpFlags() & REF_code)
		{
			op = _LDI;
			WriteOpcode(AsmName, use_rd | use_int16);	// ld rd, #coderef
			return;
		}

		op = _LDI;
		WriteOpcode(AsmName, use_rd | use_int);	// ld rd, #imm
		return;
	}

	if (Token("&"))				// ld rd,&
	{
		// Is load immediate address

/* !! NOTE: Removed by *ARH* - Caused code not to settle, created a variant state in code section

		const_subst = HandleExprConst();

		if (const_subst)
		{
			CheckDataAccess_LoadStore(_LDR);

			if (LIST) printf("constant reg substitution (r%d = %x)\n", imm, ConstTable[imm & 0x7f]);

			op = _LDR;
			rs = imm;
			WriteOpcode(AsmName, use_rd | use_rs);		// ld rd, rs
			return;
		}
*/
		HandleExprImm();

		CheckDataAccess_LoadStore(_LDI);

		// If the expression had a coderef then force int to fully expand

		if (GetExpFlags() & REF_code)
		{
			op = _LDI;
			WriteOpcode(AsmName, use_rd | use_int16);	// ld rd, #coderef
			return;
		}

		op = _LDI;
		WriteOpcode(AsmName, use_rd | use_int); // ld rd, &addr
		return;
	}

	if (Token("["))
	{
		// Is a load offset instruction

		if (Token("&"))
		{
			// Test for direct address

			rs = 0;
			HandleExprImm();
			CheckBadCodeRef();		// Make sure no code ref in write

			CheckDataAccess_LoadStore(OpcodeLoad);

			NeedToken("]");

			op = OpcodeLoad;
			WriteOpcode(AsmName, use_rd | use_rs | use_int);	// ld rd, [rs, imm]
			return;

		}

		rs = GetReg(1);

		// test for offset, if no offset imm=0

		if (Token(","))
		{
			Token("&");				// There may be a '&' token !!

			// Get imm
			HandleExprImm();
			CheckBadCodeRef();		// Make sure no code ref in write

			CheckDataAccess_LoadStore(OpcodeLoad);
		}

		NeedToken("]");

		op = OpcodeLoad;
		WriteOpcode(AsmName, use_rd | use_rs | use_int);	// ld rd, [rs, imm]
		return;
	}

	// Must be a move

	// Note this is a getreg and test

	rs = GetReg(0);

	if (rs == -1)
	{
		// must be a LC label
		rs = 0;
		HandleExprImm();

//		CheckValidCodeRef();
		CheckDataAccess_LoadStore(_LDI);

		op = _LDI;
		WriteOpcode(AsmName, use_rd | use_int);	// ld rs, imm
		return;
	}

	// Really is a move

	op = _LDR;
	WriteOpcode(AsmName, use_rd | use_rs);		// ld rd, rs
	return;
}

//****************************************
//		Decode 2 reg instructions
// 		   not,neg, extent etc
//****************************************

void Get_rd_rs(char *AsmName,int Opcode, int OpcodeImm)
{
#if !defined(_MSC_VER) && !defined(__GNUC__)
#pragma unused(OpcodeImm)
#endif

	rd = GetReg(1);
	NeedToken(",");
	rs = GetReg(1);

	imm = 0;
	op = Opcode;

	WriteOpcode(AsmName, use_rd | use_rs);
}

//****************************************
//		Decode push/pop instructions
//****************************************

void Get_push_pop(char *AsmName,int Opcode, int OpcodeImm)
{
#if !defined(_MSC_VER) && !defined(__GNUC__)
#pragma unused(OpcodeImm)
#endif

	rd = GetReg(1);

	NeedToken(",");

	rs = GetReg(1);

	if (rs < rd)
		Error(Error_Skip, "Push/Pop registers have wrong order");

	rs = rs - rd + 1;

	if (Opcode == _POP)
		rd = rd + rs - 1;		// Reverse regs

	imm = 0;
	op = Opcode;

	WriteOpcode(AsmName, use_rd | use_rs);
}

//****************************************
//		Decode arith instructions
//		  add,sub,xor,or,and etc
//****************************************

void Get_Arith2(char *AsmName,int Opcode,int OpcodeImm)
{
	int const_subst;

	rd = GetReg(1);
	NeedToken(",");

	if (Token("#") || Token("&"))
	{
		// Test for constant registers

		const_subst = HandleExprConst();
		//CheckBadCodeRef();	//Fredrik

		CheckDataAccess();

		if (const_subst)
		{
			if (LIST) printf("constant reg substition (r%d = %x)\n", imm, ConstTable[imm & 0xff]);

			op = Opcode;
			rs = imm;
			WriteOpcode(AsmName, use_rd | use_rs);		// ld rd, rs
			return;
		}

		// If the expression had a coderef then force int to fully expand

		if (GetExpFlags() & REF_code)
		{
			op = OpcodeImm;
			WriteOpcode(AsmName, use_rd | use_int16);	// arith rd, #coderef
			return;
		}

		op = OpcodeImm;
		WriteOpcode(AsmName, use_rd | use_int);
		return;
	}

	rs = GetReg(1);

	op = Opcode;
	WriteOpcode(AsmName, use_rd | use_rs);
	return;
}

//****************************************
//		Decode shift instructions
//****************************************

void Get_Shift(char *AsmName,int Opcode,int OpcodeImm)
{
	rd = GetReg(1);
	NeedToken(",");

	if (Token("#"))
	{
		// Const substitution not needed here

		op = OpcodeImm;
		HandleExpr();
		CheckBadCodeRef();

		imm &= 31;

		op = OpcodeImm;
		WriteOpcode(AsmName, use_rd | use_int8);
		return;
	}

	rs = GetReg(1);

	op = Opcode;
	WriteOpcode(AsmName, use_rd | use_rs);
	return;
}

//****************************************
//		Parse basic expressions
//****************************************

void HandleExpr()
{
	//short	Type;

	// MAHandle internal Lables

	imm = GetExpression();					// Evaluate the expression
	/*Type = (short)*/GetExpType();					// Get its type

	return;
}

//****************************************
//	   Parse expressions and create
//	  numeric constant pool reference
//****************************************

void HandleExprImm()
{
	//short	flags;

	// MAHandle internal Lables

	imm = GetExpression();					// Evaluate the expression
	/*flags =*/ GetExpFlags();

	// If the expression contained a symbol that is unresolved
	// then don't create a constant for that address

	if (GetExpResolved() == -1)
	{
		imm = 0;
		return;
	}

	imm = FindVar(imm);
	return;
}
//****************************************
//	   	  Parse expressions and
// check if result value can be expressed
//   as a constant register, otherwise
// create numeric constant pool reference
//****************************************

int HandleExprConst()				// Const or Imm
{
	int		ConstReg;
	int		flags;

	// MAHandle internal Lables

	imm = GetExpression();					// Evaluate the expression

	// If the expression contains a code ref then don't
	// change it to a constant register

	flags = GetExpFlags();

	if (flags == REF_null)
	{
		ConstReg = IsConst(imm);				// Check for const reg

		if (ConstReg != -1)
		{
			imm = ConstReg;						// Return the register in imm
			return 1;							// Say in reg
		}

	}

	// If the expression contained a symbol that is unresolved
	// then don't create a constant for that address

	if (GetExpResolved() == -1)
	{
		imm = 0;
		return 0;
	}

	imm = FindVar(imm);
	return 0;
}

//****************************************
//		 Decode a register name
//****************************************

#define ifreg(str,val) if (QToken(str)) return val

int GetReg(int err)
{
	int reg = -1;

	ifreg("zr",REG_zero);
	ifreg("sp",REG_sp);
	ifreg("rt",REG_rt);
	ifreg("fr",REG_fr);

	ifreg("d0",4);
	ifreg("d1",5);
	ifreg("d2",6);
	ifreg("d3",7);
	ifreg("d4",8);
	ifreg("d5",9);
	ifreg("d6",10);
	ifreg("d7",11);

	ifreg("i0",12);
	ifreg("i1",13);
	ifreg("i2",14);
	ifreg("i3",15);

	ifreg("r10",26);
	ifreg("r11",27);
	ifreg("r12",28);
	ifreg("r13",29);
	ifreg("r14",30);
	ifreg("r15",31);

	ifreg("r0",16);
	ifreg("r1",17);
	ifreg("r2",18);
	ifreg("r3",19);
	ifreg("r4",20);
	ifreg("r5",21);
	ifreg("r6",22);
	ifreg("r7",23);
	ifreg("r8",24);
	ifreg("r9",25);

	if (Token("$r"))
	{
		reg = GetDecNum(3);

		if (reg > CONST_REG_SIZE)
		{
			Error(Error_Skip, "Illegal register r%d", reg);
			return 0;
		}

		return reg;
	}

	if (err)
	{
		GetName();
		Error(Error_Skip, "Unknown register '%s'", Name);
	}

	return reg;
}

//****************************************
// Write an opcode to the code section
//	input  op, imm, rd , rs
//****************************************

int Last_Instruction = 0;

void WriteOpcode(char *AsmName, int field)
{
	int StartCodeIP = CodeIP;

	char *CodePtr = (char *) ArrayPtrBound(&CodeMemArray, CodeIP, CodeIP + 32);
	//char *StartCodePtr = CodePtr;

	Last_Instruction = CodeIP;

	if (AsmCharPtr)
		ArraySet(&AsmCharArray, CodeIP, (int) AsmCharPtr);

	if (!CurrentFunction)
		Error(Error_Fatal, "Opcodes can't be outside a function");

	if (Section != SECT_code)
	{
		Warning("Opcode '%s' : not in code section, switching to code section\n",AsmName,GetSectIP(),GetSectName((short)Section));
		Section = SECT_code;
	}

	if ((rd >= CONST_REG_SIZE) ||
		(rs >= CONST_REG_SIZE) ||
		(op >= CONST_REG_SIZE))
	{
		Error(Error_Fatal, "(WriteOpcode) internal overflow\n");
	}

	if (LIST)
	{
		printf("0x%-4x ",CodeIP);
		printf("%-8s : op = %-2d",AsmName,op);

	if (field & use_rd)
		printf(" : rd = %-3d",rd);

	if (field & use_rs)
		printf(" : rs = %-3d",rs);

	if (field & use_addr ||
		field & use_int8 ||
		field & use_int)
		{
			printf(" : imm = %-8x",imm);
		}
	}

	// Check index alignment rules

	CheckIndexAlign(op, imm, AsmName);

	// Check if a branch targets next instruction, if so eliminate it

	//if (BranchNextInst(AsmName))
		//return;

	// check stack pointer usage

	if (field & use_rd)
		if (rd == REG_sp)
			StackChecking(AsmName);

	// check return pointer usage

	CheckRT(field, AsmName);

	// this has no endian, all entrys are byte sized

	if (farop)
	{
		*CodePtr++ = _FAR;			// add a far opcode
		CodeIP++;
	}

	// If size optimization is off force to 16 bit indices

	if (SizeConstOpt && (field & use_int))
	{
		field &= ~use_int;
		field |= use_int16;
	}


	*CodePtr++ = (char)op;
	CodeIP++;

	if (field & use_rd)
	{
		*CodePtr++ = (char)rd;
		CodeIP++;
	}

	if (field & use_rs)
	{
		*CodePtr++ = (char)rs;
		CodeIP++;
	}

	if (field & use_addr)
	{
		if (farop)
		{
			*CodePtr++ = (char)(imm >> 16);
			CodeIP++;
		}

		*CodePtr++ = (char)(imm >> 8);
		CodeIP++;

		*CodePtr++ = (char)(imm & 0xff);
		CodeIP++;
	}
	else if (field & use_int8)
	{
		*CodePtr++ = (char)(imm & 0xff);
		CodeIP++;
	}
	else if (field & use_int)
	{
		// 8 and 16 bit ints

		if (imm < 128)
		{
			*CodePtr++ = (char)imm;
			CodeIP++;
		}
		else
		{
			*CodePtr++ = (char)(imm >> 8) | 0x80;
			CodeIP++;

			*CodePtr++ = (char)(imm & 0xff);
			CodeIP++;
		}

	}
	else if (field & use_int16)
	{
		// Expand const fields, if SizeConstOpt==0
		// Fixed 16 bit ints

		*CodePtr++ = (char)(imm >> 8) | 0x80;
		CodeIP++;

		*CodePtr++ = (char)(imm & 0xff);
		CodeIP++;

	}
	else if (field & use_int24)
	{
		*CodePtr++ = (char)(imm >> 16);
		CodeIP++;

		*CodePtr++ = (char)(imm >> 8);
		CodeIP++;

		*CodePtr++ = (char)(imm & 0xff);
		CodeIP++;
	}

	// Debug list

	if (LIST)
	{
		int len = (CodeIP - StartCodeIP);
		printf(" : len = %d\n",len);
	}
}


//****************************************
//       Check if a branch targets
//         the next instruction
//		 If it does eliminate it.
//****************************************

int BranchNextInst(char *AsmName)
{
	int dest = CodeIP + 3;			// 3 is the normal length of a branch

	if (farop)
		dest++;

	switch(op)
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
			case _JPI:

			if (imm == dest)
			{
				Error(Error_Warning, "branch opcode targets next instruction '%s'", AsmName);
				return 1;
			}
	}

	return 0;
}

//****************************************
//	   Check instruction for illegal
//			  stack usage
//		 sp must be the target reg
//****************************************

void StackChecking(char *AsmName)
{
//	if (!JavaPass)
		return;

	if (!Final_Pass)
		return;

	switch(op)
	{
		case _STB:
		case _STH:
		case _STW:
		return;

		case _ADD:
		case _SUB:

		if (rs < 32)			// Make sure its a constant register
			goto stack_err;
		return;

		case _ADDI:
		case _SUBI:
		return;

		case _PUSH:
		case _POP:
			if (rd > REG_sp)
				return;

	}

stack_err:
	Error(Error_Warning, "Illegal stack usage in opcode '%s'", AsmName);
}

//****************************************
//	   Check instruction for illegal
//			  stack usage
//		 sp must be the target reg
//****************************************

void CheckRT(int field, char *AsmName)
{
	if (Pass != 0)
		return;

	if ((op == _PUSH) || (op == _POP))
		return;

	if (field & use_rd)
	{
		if (rd == REG_rt)
		{
			Error(Error_Warning, "verification failed : RT used in opcode '%s'", AsmName);
			return;
		}
	}

	if (field & use_rs)
	{
		if (rs == REG_rt)
		{
			Error(Error_Warning, "verification failed : RT used in opcode '%s'", AsmName);
			return;
		}
	}


}

//****************************************
//    Check if index alignment rules
//****************************************

void CheckIndexAlign(int opc, int immVal, char *AsmName)
{
	if (!Final_Pass)
		return;

	// this doesn't work (niklas) gcc seem to emit a misaligned
	// offsets, which is offset back to an aligned address with
	// this instruction.
	return;

	switch(opc)
	{
		case _LDH:
		case _STH:
		{
			immVal = VarPool[immVal];

			if (immVal & 0x1)
				Error(Error_Fatal, "opcode breaks index alignment rules '%s'", AsmName);
		}
		break;

		case _LDW:
		case _STW:
		{
			immVal = VarPool[immVal];

			if (immVal & 0x3)
				Error(Error_Warning, "opcode breaks index alignment rules '%s'", AsmName);
		}
		break;
	}
}

