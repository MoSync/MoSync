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

#define Opcode(tok,method,opnum,opimm)	\
    case tok:				\
    {					\
        SkipWhiteSpace();		\
        method(ThisOpName,opnum,opimm);	\
        return;				\
    }

char ThisOpName[128];
char *AsmCharPtr = 0;

void Instructions()
{
	int t;

	if (*g_FilePtr == '.')
		return;

	// Clear all the special variables

	g_farop = g_imm = g_rd = g_rs = g_rt = g_op = 0;
	
	SkipWhiteSpace();

	AsmCharPtr = g_FilePtr;

	// Save IP file offset lookup

	ArraySet(&g_AsmCharIPArray, (int) (AsmCharPtr - g_FileTop) , g_CodeIP + 1);

	GetToken();	
	t = ScanAsmToken();

	strcpy(ThisOpName, g_Name);

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

	g_Name[64] = 0;

	Error(Error_Skip, "Illegal instruction '%s'",g_Name);
}

//****************************************
//		   Decode instructions
//****************************************

#else

#define Opcode(string,method,opnum,opimm)   \
    if (QToken(string))                     \
    {                                       \
        method(string,opnum,opimm);         \
        return;                             \
    }

void Instructions()
{
	SkipWhiteSpace();

	if (*g_FilePtr == '.')
		return;

	// Clear all the special variables

	g_farop = g_imm = g_rd = g_rs = g_rt = g_op = 0;

	AsmCharPtr = g_FilePtr;

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
	
	Error(Error_Skip, "Illegal instruction '%s'",g_Name);
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

	g_op = Opcode;
	WriteOpcode(AsmName, 0);
}

//****************************************
//
//****************************************

void Get_break(char *AsmName,int Opcode, int OpcodeImm)
{
	Opcode = 0;
	OpcodeImm = 0;
	AsmName = 0;
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
	
	OpcodeImm = 0;
	
	// Get the switch register
	
	case_reg = GetReg(1);

	// Skip the comma

	NeedToken(",");
	SkipWhiteSpace();

	// Get the start number

	NeedToken("#");
	HandleExpr();
	CheckBadCodeRef();		// Make sure no code ref in write

	case_startnum_imm = g_imm;

	// Skip the comma

	NeedToken(",");
	SkipWhiteSpace();

	// Get the length

	NeedToken("#");
	HandleExpr();
	CheckBadCodeRef();		// Make sure no code ref in write

	case_length_imm = g_imm;
	
	// Skip the comma

	NeedToken(",");
	SkipWhiteSpace();

	// get the switch table address

	NeedToken("#");

	GetAsmName();
	switch_table_imm = GetEnumValue(g_Name);

	if (g_Final_Pass)
		if (EnumIsFunction())
			Error(Error_Skip, "case targets a function");

	CheckDataAccess();

	// Skip the comma

	NeedToken(",");
	SkipWhiteSpace();

	// get the switch default address

	NeedToken("#");

	GetAsmName();
	switch_default = GetEnumValue(g_Name);

	if (g_Final_Pass)
		if (EnumIsFunction())
			Error(Error_Skip, "default in case targets a function");

	// Mark the default address in the call array

	if (GetLastSymbolRef())
		ArraySet(&g_CallArray, g_CodeIP, (int) GetLastSymbolRef());

	// Add a padding byte

//	PadDataSection(1);

	// make sure data is aligned

	AlignDataSection();

	// Get the data section address of the top of the case data
	
	case_dataip = g_DataIP >> 2;

	// Save the case data
	// Save the current section and select the data section
		
	oldsect = g_Section;
	g_Section = SECT_data;

	sprintf(g_Name, "_case_%x", g_DataIP);
	AddLabel(g_Name, 0);

	// Write the switch data

	WriteLong(case_startnum_imm);		// Write the start num
	WriteLong(case_length_imm);			// Write the case length
	WriteLong(switch_default);			// Address of the default case

	// ... Switch table follows

	// Put the old section back
	g_Section = oldsect;

	// write the opcode

	g_op = Opcode;
	g_rd = case_reg;
	g_imm = case_dataip;

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
		g_imm = isSysCall(g_Name);

		if (g_imm != -1)
		{
			g_op = _SYSCALL;
			WriteOpcode("SysCall", use_int8);		// was use_int
			return;
		}

		// Check for Thunks
		
		if (NextToken("%"))	
			GetAsmThunkName();

		// Evaluate the enum

		g_imm = GetEnumValue(g_Name);

		// Make sure this enum was a function

		//FIXME
		//if (Final_Pass)
			//if (!EnumIsFunction())
				//Warning("Warning: call targets a non-function\n");

		ArraySet(&g_CallArray, g_CodeIP, (int) GetLastSymbolRef());

		if (g_imm & 0xffff0000)	// Check for far jumps
		{
			g_farop = 1;
		}

		g_op = OpcodeImm;
		WriteOpcode(AsmName, use_addr);	
		return;
	}

	g_rd = GetReg(0);

	if (g_rd == -1)
	{
		// Not a reg must be a LC label

		g_rd = 0;

//!! Old
//		HandleExpr();

//!!added

		GetAsmName();

		// Normal symbol
		g_imm = GetEnumValue(g_Name);

		// Make sure this enum was a function

		//FIXME
		//if (Final_Pass)
			//if (!EnumIsFunction())
				//Warning("Warning: call targets a non-function\n");

		ArraySet(&g_CallArray, g_CodeIP, (int) GetLastSymbolRef());

//!!

		if (g_imm & 0xffff0000)	// Check for far jumps
		{
			g_farop = 1;
		}

		g_op = OpcodeImm;
		WriteOpcode(AsmName, use_addr);	
		return;
	}

	g_op = Opcode;
	WriteOpcode(AsmName, use_rd);
	return;
}

//****************************************
//		Decode call instructions
//****************************************

void Get_syscall(char *AsmName,int Opcode, int OpcodeImm)
{	
	AsmName = 0;
	Opcode = OpcodeImm = 0;
	
	g_imm = GetExpression();					// Get the syscall number
	g_op = _SYSCALL;
	WriteOpcode("SysCall", use_int8);		// was use_int
	return;
}

//****************************************
//	Make sure a branch ip is within
// 		the current function
//****************************************

void CheckLocalBounds(int branch_ip)
{
	SYMBOL *sym = g_CurrentFunction;
	
	if (g_AllowFarBranches)
		return;

	if (!sym)
		return;

	if (g_Final_Pass == 0)
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
	g_op = OpcodeImm;
	g_imm = 0;

	if (Token("&"))
	{
		// only allow labels

		GetAsmName();
		strcpy(JumpTargetName, g_Name);
		
		g_imm = GetEnumValue(g_Name);

		if (g_ArgVerifierOff == 0)
			if (g_Final_Pass)
				if (EnumIsFunction())
					Error(Error_Skip, "jmp targets a function");

		ArraySet(&g_CallArray, g_CodeIP, (int) GetLastSymbolRef());

		if (g_imm & 0xffff0000)	// Check for far jumps
		{
			g_farop = 1;
		}

		CheckLocalBounds(g_imm);

		WriteOpcode(AsmName, use_addr);
		return;
	}

	g_rd = GetReg(0);

	if (g_rd == -1)
	{
		// Not a reg must be a LC label
		g_rd = 0;

		NeedToken("#");

		// get name

		GetAsmName();
		strcpy(JumpTargetName, g_Name);

		g_imm = GetEnumValue(g_Name);

		if (g_ArgVerifierOff == 0)
			if (g_Final_Pass)
				if (EnumIsFunction())
					Error(Error_Skip, "jmp targets a function");

		ArraySet(&g_CallArray, g_CodeIP, (int) GetLastSymbolRef());
	
		if (g_imm & 0xffff0000)	// Check for far jumps
		{
			g_farop = 1;
		}

		CheckLocalBounds(g_imm);

		WriteOpcode(AsmName, use_addr);
		return;
	}

//	if (g_JavaPass)
//		Error(Error_Fatal, "Illegal obselete instruction, jmp register");

//	Error(Error_Skip, "Illegal instruction, jp register");

	g_op = Opcode;
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
	
	g_imm = 0;

	if (Token("gtu"))
		g_op = _JC_GTU;
	else if (Token("geu"))
		g_op = _JC_GEU;
	else if (Token("ltu"))
		g_op = _JC_LTU;
	else if (Token("leu"))
		g_op = _JC_LEU;
	else if (Token("eq"))
		g_op = _JC_EQ;
	else if (Token("ne"))
		g_op = _JC_NE;
	else if (Token("gt"))
		g_op = _JC_GT;
	else if (Token("ge"))
		g_op = _JC_GE;
	else if (Token("lt"))
		g_op = _JC_LT;
	else if (Token("le"))
		g_op = _JC_LE;
	else 
		Error(Error_Skip, "Unknown jump condition");

	NeedToken(",");

	g_rd = GetReg(1);

	NeedToken(",");

	g_rs = GetReg(1);

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
	
	strcpy(JumpTargetName, g_Name);

	g_imm = GetEnumValue(g_Name);

	if (g_Final_Pass)
		if (EnumIsFunction())
			Error(Error_Skip, "jc targets a function");

	ArraySet(&g_CallArray, g_CodeIP, (int) GetLastSymbolRef());

	// ** Check for far jumps

	if (g_imm & 0xffff0000)
	{
		g_farop = 1;
	}

	CheckLocalBounds(g_imm);

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

	ArraySet(&g_DataAccessArray, g_CodeIP, ref);
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

	ArraySet(&g_DataAccessArray, g_CodeIP, (int) ref);

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
		addr += g_MaxDataIP;

	WriteTypeAccessArray(addr, size);
	return;
}

//****************************************
//		Decode ld instructions
//****************************************

void Get_load_store(char *AsmName,int OpcodeLoad, int OpcodeStore)
{
	int const_subst;
	
	// Get the dest reg

	g_imm = FindVar(0);				// set imm as preset zero

	if (Token("["))					// ld [], rs
	{
		//----------------------------------------
		// Is a store instruction
		//----------------------------------------

		if (Token("&"))				// ld [&addr],rs
		{
			// Test for direct address

			g_rd = 0;

			HandleExprImm();
			CheckBadCodeRef();		// Make sure no code ref in write

			CheckDataAccess_LoadStore(OpcodeStore);

			NeedToken("]");

			NeedToken(",");

			g_rs = GetReg(1);

			g_op = OpcodeStore;
			WriteOpcode(AsmName, use_rd | use_rs | use_int);		// ld [rd, g_imm], rs
			return;	
		}

		// Is offset address

		g_rd = GetReg(1);

		// test for offset
		
		if (Token(","))
		{
			Token("&");				// There may be a '&' token !!

			// Get g_imm
			HandleExprImm();

			CheckDataAccess_LoadStore(OpcodeStore);
		}

		NeedToken("]");

		NeedToken(",");

		g_rs = GetReg(1);

		g_op = OpcodeStore;
		WriteOpcode(AsmName, use_rd | use_rs | use_int);		// ld [rd, imm], rs
		return;	

	}

	//----------------------------------------
	// Is a load instruction		ld rd,
	//----------------------------------------

	g_rd = GetReg(1);

	// Print a warning

	if (g_rd >= 32 || g_rd == 0)
		if (g_Pass == 2)
			printf("*Warning* Instruction overwrites constant register %s\n", GetRegName(g_rd));

	NeedToken(",");

	if (Token("#"))
	{
		// Is load immediate

		const_subst = HandleExprConst();

		if (const_subst)
		{
			CheckDataAccess_LoadStore(_LDR);

			if (g_LIST) printf("constant reg substitution (r%d = %x)\n", g_imm, g_ConstTable[g_imm & 0x7f]);

			g_op = _LDR;
			g_rs = g_imm;
			WriteOpcode(AsmName, use_rd | use_rs);		// ld rd, rs
			return;
		}

		// Must test for 0 in immediate

		CheckDataAccess_LoadStore(_LDI);

		g_op = _LDI;
		WriteOpcode(AsmName, use_rd | use_int);	// ld rd, #imm
		return;		
	}

	if (Token("&"))
	{
		// Is load immediate address

		const_subst = HandleExprConst();

		if (const_subst)
		{
			CheckDataAccess_LoadStore(_LDR);

			if (g_LIST) printf("constant reg substitution (r%d = %x)\n", g_imm, g_ConstTable[g_imm & 0x7f]);

			g_op = _LDR;
			g_rs = g_imm;
			WriteOpcode(AsmName, use_rd | use_rs);		// ld rd, rs
			return;
		}

		CheckDataAccess_LoadStore(_LDI);

		g_op = _LDI;
		WriteOpcode(AsmName, use_rd | use_int); // ld rd, &addr
		return;		
	}

	if (Token("["))
	{
		// Is a load offset instruction

		if (Token("&"))
		{
			// Test for direct address

			g_rs = 0;
			HandleExprImm();
			CheckBadCodeRef();		// Make sure no code ref in write

			CheckDataAccess_LoadStore(OpcodeLoad);

			NeedToken("]");

			g_op = OpcodeLoad;
			WriteOpcode(AsmName, use_rd | use_rs | use_int);	// ld rd, [rs, imm]
			return;		

		}

		g_rs = GetReg(1);

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

		g_op = OpcodeLoad;
		WriteOpcode(AsmName, use_rd | use_rs | use_int);	// ld rd, [rs, imm]
		return;			
	}

	// Must be a move

	// Note this is a getreg and test
	
	g_rs = GetReg(0);

	if (g_rs == -1)
	{
		// must be a LC label
		g_rs = 0;
		HandleExprImm();

//		CheckValidCodeRef();
		CheckDataAccess_LoadStore(_LDI);

		g_op = _LDI;
		WriteOpcode(AsmName, use_rd | use_int);	// ld rs, imm
		return;		
	}

	// Really is a move

	g_op = _LDR;
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

	g_rd = GetReg(1);
	NeedToken(",");
	g_rs = GetReg(1);

	g_imm = 0;
//	rt = 0;
	g_op = Opcode;

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

	g_rd = GetReg(1);

	NeedToken(",");

	g_rs = GetReg(1);

/*	if (Opcode == _PUSH)
	{
		printf("--- PUSH %s,%s\n", regsyms[g_rd], regsyms[g_rs] );
	}
*/

	if (g_rs < g_rd)
		Error(Error_Skip, "Push/Pop registers have wrong order");

	g_rs = g_rs - g_rd + 1;

	if (Opcode == _POP)
		g_rd = g_rd + g_rs - 1;		// Reverse regs
		
	g_imm = 0;
	g_op = Opcode;

	WriteOpcode(AsmName, use_rd | use_rs);
}

//****************************************
//		Decode arith instructions
//		  add,sub,xor,or,and etc
//****************************************

void Get_Arith2(char *AsmName,int Opcode,int OpcodeImm)
{	
	int const_subst;
	
	g_rd = GetReg(1);
	NeedToken(",");
		
	if (Token("#") || Token("&"))
	{
		// Test for constant registers
		
		const_subst = HandleExprConst();
		//CheckBadCodeRef();	//Fredrik

		CheckDataAccess();

		if (const_subst)
		{
			if (g_LIST) printf("constant reg substition (r%d = %x)\n", g_imm, g_ConstTable[g_imm & 0xff]);

			g_op = Opcode;
			g_rs = g_imm;
			WriteOpcode(AsmName, use_rd | use_rs);		// ld rd, rs
			return;
		}

		g_op = OpcodeImm;
		WriteOpcode(AsmName, use_rd | use_int);
		return;
	}

	g_rs = GetReg(1);
		
	g_op = Opcode;
	WriteOpcode(AsmName, use_rd | use_rs);
	return;
}

//****************************************
//		Decode shift instructions
//****************************************

void Get_Shift(char *AsmName,int Opcode,int OpcodeImm)
{
	g_rd = GetReg(1);
	NeedToken(",");
		
	if (Token("#"))
	{
		// Const substitution not needed here

		g_op = OpcodeImm;
		HandleExpr();
		CheckBadCodeRef();

		g_imm &= 31;

		g_op = OpcodeImm;
		WriteOpcode(AsmName, use_rd | use_int8);
		return;
	}

	g_rs = GetReg(1);
		
	g_op = Opcode;
	WriteOpcode(AsmName, use_rd | use_rs);
	return;
}

//****************************************
//		Parse basic expressions
//****************************************

void HandleExpr()
{
	short	Type;

	// MAHandle internal Lables
	
	g_imm = GetExpression();					// Evaluate the expression
	Type = (short)GetExpType();					// Get its type

	return;
}

//****************************************
//	   Parse expressions and create
//	  numeric constant pool reference
//****************************************

void HandleExprImm()
{
	short	Type;
	
	// MAHandle internal Lables
	
	g_imm = GetExpression();					// Evaluate the expression
	Type = (short)GetExpType();					// Get its type

	// If the expression contained a symbol that is unresolved
	// then don't create a constant for that address
	
	if (GetExpResolved() == -1)
	{
		g_imm = 0;
		return;
	}
		
	g_imm = FindVar(g_imm);
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
	short	Type;
	int		ConstReg;
	
	// MAHandle internal Lables
	
	g_imm = GetExpression();					// Evaluate the expression

	Type = (short) GetExpType();			// Get its type

	// test if this can be a register

	ConstReg = IsConst(g_imm);				// Check for const reg

	if (ConstReg != -1)
	{
		g_imm = ConstReg;						// Return the register in imm
		return 1;							// Say in reg
	}

	// If the expression contained a symbol that is unresolved
	// then don't create a constant for that address
	
	if (GetExpResolved() == -1)
	{
		g_imm = 0;
		return 0;
	}
	
	g_imm = FindVar(g_imm);
	return 0;
}

//****************************************
//		 Decode a register name
//****************************************
/*
   "zr", "sp",  "rt",  "fr", 				\
   "d0", "d1",  "d2",  "d3", "d4", "d5", "d6", "d7",	\
   "i0", "i1",  "i2",  "i3", "r0", "r1", "r2", "r3",	\
   "r4", "r5",  "r6",  "r7", "r8", "r9", "r10","r11",	\
  "r12","r13", "r14", "r15", "rap", "arg", "cc"		\
*/

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
		Error(Error_Skip, "Unknown register '%s'", g_Name);
	}
	
	return reg;
}

//****************************************
// Write an opcode to the code section
//	input  op, imm, rd , rs
//****************************************

int g_Last_Instruction = 0;

void WriteOpcode(char *AsmName, int field)
{
	int StartCodeIP = g_CodeIP;
		
	char *CodePtr = (char *) ArrayPtrBound(&g_CodeMemArray, g_CodeIP, g_CodeIP + 32);
	//char *StartCodePtr = CodePtr;

	g_Last_Instruction = g_CodeIP;

	if (AsmCharPtr)
		ArraySet(&g_AsmCharArray, g_CodeIP, (int) AsmCharPtr);
		
	if (!g_CurrentFunction)
		Error(Error_Fatal, "Opcodes can't be outside a function");
	
	if (g_Section != SECT_code)
	{
		Warning("Opcode '%s' : not in code section, switching to code section\n",AsmName,GetSectIP(),GetSectName((short)g_Section));
		g_Section = SECT_code;
	}

	if ((g_rd >= CONST_REG_SIZE) ||
		(g_rs >= CONST_REG_SIZE) ||
		(g_op >= CONST_REG_SIZE))
	{
		Error(Error_Fatal, "(WriteOpcode) internal overflow\n");
	}

	if (g_LIST)
	{
		printf("0x%-4x ",g_CodeIP);
		printf("%-8s : op = %-2d",AsmName,g_op);

	if (field & use_rd)
		printf(" : rd = %-3d",g_rd);

	if (field & use_rs)
		printf(" : rs = %-3d",g_rs);

	if (field & use_addr ||
		field & use_int8 ||
		field & use_int)
		{
			printf(" : imm = %-8x",g_imm);
		}
	}

	// Check index alignment rules

	CheckIndexAlign(g_op, g_imm, AsmName);

	// Check if a branch targets next instruction, if so eliminate it

	//if (BranchNextInst(AsmName))
		//return;

	// check stack pointer usage

	if (field & use_rd)
		if (g_rd == REG_sp)
			StackChecking(AsmName);

	// check return pointer usage

	CheckRT(field, AsmName);

	// this has no endian, all entrys are byte sized

	if (g_farop)
	{
		*CodePtr++ = _FAR;			// add a far opcode
		g_CodeIP++;
	}


	*CodePtr++ = (char)g_op;
	g_CodeIP++;

	if (field & use_rd)
	{
		*CodePtr++ = (char)g_rd;
		g_CodeIP++;
	}
	
	if (field & use_rs)
	{
		*CodePtr++ = (char)g_rs;
		g_CodeIP++;
	}

	if (field & use_addr)
	{
		if (g_farop)
		{
			*CodePtr++ = (char)(g_imm >> 16);
			g_CodeIP++;
		}

		*CodePtr++ = (char)(g_imm >> 8);
		g_CodeIP++;

		*CodePtr++ = (char)(g_imm & 0xff);
		g_CodeIP++;
	}
	else if (field & use_int8)
	{
		*CodePtr++ = (char)(g_imm & 0xff);
		g_CodeIP++;
	}
	else if (field & use_int)
	{

		if (g_SizeConstOpt)
		{
			// 8 and 16 bit ints
			
			if (g_imm < 128)
			{	
				*CodePtr++ = (char)g_imm;
				g_CodeIP++;
			}
			else
			{	
				*CodePtr++ = (char)(g_imm >> 8) | 0x80;
				g_CodeIP++;

				*CodePtr++ = (char)(g_imm & 0xff);
				g_CodeIP++;
			}

		}
		else
		{
			// Expand const fields, if g_SizeConstOpt==0
			// Fixed 16 bit ints

			*CodePtr++ = (char)(g_imm >> 8);
			g_CodeIP++;

			*CodePtr++ = (char)(g_imm & 0xff);
			g_CodeIP++;
		}
		
		
	}
	else if (field & use_int24)
	{		
		*CodePtr++ = (char)(g_imm >> 16);
		g_CodeIP++;

		*CodePtr++ = (char)(g_imm >> 8);
		g_CodeIP++;

		*CodePtr++ = (char)(g_imm & 0xff);
		g_CodeIP++;
	}
		
	// Debug list
		
	if (g_LIST)
	{
		int len = (g_CodeIP - StartCodeIP);
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
	int dest = g_CodeIP + 3;			// 3 is the normal length of a branch
	
	if (g_farop)
		dest++;

	switch(g_op)
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

			if (g_imm == dest)
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
//	if (!g_JavaPass)
		return;

	if (!g_Final_Pass)
		return;

	switch(g_op)
	{
		case _STB:
		case _STH:
		case _STW:
		return;

		case _ADD:
		case _SUB:

		if (g_rs < 32)			// Make sure its a constant register
			goto stack_err;
		return;

		case _ADDI:
		case _SUBI:
		return;

		case _PUSH:
		case _POP:
			if (g_rd > REG_sp)
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
	if (g_Pass != 0)
		return;

	if ((g_op == _PUSH) || (g_op == _POP))
		return;

	if (field & use_rd)
	{
		if (g_rd == REG_rt)
		{
			Error(Error_Warning, "verification failed : RT used in opcode '%s'", AsmName);
			return;
		}
	}	

	if (field & use_rs)
	{
		if (g_rs == REG_rt)
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
	if (!g_Final_Pass)
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
			immVal = g_VarPool[immVal];

			if (immVal & 0x1)
				Error(Error_Fatal, "opcode breaks index alignment rules '%s'", AsmName);
		}
		break;

		case _LDW:
		case _STW:
		{
			immVal = g_VarPool[immVal];

			if (immVal & 0x3)
				Error(Error_Warning, "opcode breaks index alignment rules '%s'", AsmName);
		}
		break;
	}
}

