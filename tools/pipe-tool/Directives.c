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

char *DataCharPtr;

//****************************************
//
//****************************************

short Directives()
{	
	int t;
	
	SkipWhiteSpace();

	if (*g_FilePtr != '.')
		return 0;

	DataCharPtr = g_FilePtr;

	if (QToken(".eof"))
	{
		// mark end of label array as end of bss
		
		strcpy(g_Name, "_________eof_________");
		g_Section = SECT_bss;
		DefineLabel(label_Local);

		SourceFileCheckEOF(LastTokenPtr() - g_FileTop);

		g_EndFile = 1;
		return 1;
	}


	GetToken();	
	t = ScanAsmToken();

	switch(t)
	{

//---------------------------------------------------------

	case 0:
		Error(Error_Skip, "Illegal directive '%s'",g_Name);


//---------------------------------------------------------

	case dir_sourcefile:
	{		
		int file;
		
		SkipWhiteSpace();
		GetLFileName();	

//		SetCSourceFile(Name);
		file = SetSLD_Name(g_Name);

		SetCSourceFile(g_Name, file);
		return 1;
	}

//---------------------------------------------------------


	case dir_model:
	{	
		int model=0;
	
		if (Token("full"))				// Get return type
			model = MODEL_full;
		else if (Token("half"))				// Get return type
			model = MODEL_half;
		else if (Token("compact"))			// Get return type
			model = MODEL_compact;
		else if (Token("mini"))				// Get return type
			model = MODEL_mini;
		else if (Token("tiny"))				// Get return type
			model = MODEL_tiny;
		else 
			Error(Error_Fatal, "invalid model type");

		// if model is not set then set it
			
		if (!g_CurrentModel)
			g_CurrentModel = model;
		else
		{
			if (model != g_CurrentModel)
				Error(Error_Fatal, "multiple architectual models defined");
		}

		return 1;
	}

//---------------------------------------------------------

	case dir_ctor:
	{
		int ref;

		g_imm = GetExpression();				// Get the number
		ref = (int) GetLastSymbolRef();

		if(g_pass_count > 1)
		{
			if (!ref)
				Error(Error_Fatal, "Constructor has bad reference");
		
			ArraySet(&g_CtorArray, g_CtorCount++, (int) ref);
		}
		return 1;		
	}

//---------------------------------------------------------

	case dir_dtor:
	{		
		int ref;

		g_imm = GetExpression();				// Get the number
		ref = (int) GetLastSymbolRef();

		if(g_pass_count > 1)
		{
			if (!ref)
				Error(Error_Fatal, "Destructor has bad reference");
		
			ArraySet(&g_DtorArray, g_DtorCount++, (int) ref);
		}
		return 1;		
	}


//---------------------------------------------------------

	case dir_sourcedir:
	{		
		SkipWhiteSpace();
		GetLFileName();	
		SetCSourceDir(g_Name);
		return 1;
	}

//---------------------------------------------------------

	case dir_line:
	{
		int v;
		
		SkipWhiteSpace();
		v = GetNum();	
		SetCSourceLine(v);
		SetSLD_Line(v);

		return 1;
	}

//---------------------------------------------------------

	case dir_dlab:
	{
		if (!g_ArgUseStabs)
		{
			SkipLine();			// Skip excess debug labels
			return 1;
		}

		// Define a label

		g_Section = SECT_code;

		GetAsmName();
		DefineLabel(label_Local);
		return 1;
	}

//---------------------------------------------------------

	case dir_library:
	{
		SkipLine();
		return 1;
	}

//---------------------------------------------------------

	case dir_exit:
	{
		Error(Error_Fatal, "Exiting with user error");
		return 1;
	}

//---------------------------------------------------------

	case dir_asm_on:
	{
		return 1;
	}

//---------------------------------------------------------

	case dir_asm_off:
	{
/*		if (QToken(".asm_off"))
		{
			return 1;
		}
*/
		return 1;
	}

//---------------------------------------------------------

	case dir_AllowFarBranches:
	{
		g_AllowFarBranches = 1;
		return 1;
	}

//---------------------------------------------------------

	case dir_bp:
	{
		return 1;
	}

//---------------------------------------------------------

	case dir_localscope:
	{
		SkipWhiteSpace();

		if (Token("+"))
		{
			g_LocalScope++;
			return 1;
		}

		if (Token("reset"))
		{
			g_LocalScope = 1;
			return 1;
		}

		Error(Error_Fatal, "Bad .localscope type");
		return 1;
	}

//---------------------------------------------------------

	case dir_func:
	{	
		g_Function_Return_Type = RET_void;

		GetName();

		g_Function_Param_Count = -1;

		if (Token(","))
		{
			g_Function_Param_Count = GetNum();

			if (Token(","))				// Get param count
			{				
				if (Token("int"))				// Get return type
					g_Function_Return_Type = RET_int;
				else if (Token("float"))
					g_Function_Return_Type = RET_float;
				else if (Token("double"))
					g_Function_Return_Type = RET_double;
				else if (Token("void"))
					g_Function_Return_Type = RET_void;
				else if (Token("?"))
					g_Function_Return_Type = RET_int;	//patch fix
			}
		}

		DefineLabel(label_Function);		
		return 1;
	}

//---------------------------------------------------------

	case dir_local:
	{
		GetName();
		return 1;
	}

//---------------------------------------------------------

	case dir_vec_elt:
	{
		GetName();
		return 1;
	}

//---------------------------------------------------------

	case dir_syscall:
	{
		char SysCallName[256];
		char istr[4096];
		char *iptr = 0;
		
		int v, p, r;
		
		p = 0;
		r = RET_void;
		
		GetName();
		strcpy(SysCallName,g_Name);
	
		SkipWhiteSpace();

		v = GetExpression();

		if (Token(","))				// Get param count
		{
			p = GetExpression();

			NeedToken(",");
			
			if (Token("int"))				// Get return type
				r = RET_int;
			else if (Token("float"))
				r = RET_float;
			else if (Token("double"))
				r = RET_double;
			else if (Token("void"))
				r = RET_void;

			if (Token(","))				// Get interface
			{
				GetStringName(2048);
				strcpy(istr,g_Name);
				iptr = istr;
			}

		}

		if (g_Pass == 1)
			MapSysCall(SysCallName, v, p, r, iptr);

		return 1;
	}

//---------------------------------------------------------

	case dir_lfile:
	{
		SkipWhiteSpace();
		GetLFileName();
		
		strcpy(g_CurrentFile, g_Name);
//		g_CurrentFileLine = GetLineNumber(g_FileTop,GetPrevFilePtr());
		
		AddFileSym(g_Name, g_LocalScope+1);
		
		GetRelPath(g_CurrentFile);
		return 1;
	}

//---------------------------------------------------------

	case dir_org:
	{
		int v = GetExpression();

		if (v >= MAX_CODE_MEM)
			Error(Error_Fatal, ".org has illegal address");

		g_CodeIP = v;
//!		CodePtr = &CodeMem[v];
		printf("*Warning* org %x\n", v);
		return 1;
	}


//---------------------------------------------------------

	case dir_set:
	{
		
		SkipWhiteSpace();

		// Catch Thunks
		
		if (Token("%"))
		{
			SYMBOL *ThunkSym;
			int v;

			v = GetDecNum(5);
			
			if (GetDecNumDigitCount() == 0)
				Error(Error_Fatal, "Missing Thunk Number");

			NeedToken("=");
			SkipWhiteSpace();

			GetName();			// Get the name of the thunk alias
			GetEnumValue(g_Name);	// Evaluate the symbol


			ThunkSym = GetLastSymbolRef();

			//N_FUN 141 4935 '_Z7XPClosev:F(0,6)'

			StabsEmit("N_THUNK %d %d '%s'\n", v, ThunkSym, g_Name);

			RegisterThunk(v, ThunkSym);
			return 1;
		}
		
		// Catch normal sets
		
		GetName();							// Get the new type Name	
		NeedToken("=");
		RedefENum(g_Name, GetExpression());
		return 1;
	}

//---------------------------------------------------------

	case dir_debug:
	{
		g_DEBUG = (short)GetExpression();
		return 1;
	}


//---------------------------------------------------------

	case dir_info:
	{
		g_INFO = (short)GetExpression();
		return 1;
	}

//---------------------------------------------------------

	case dir_list:
	{
		g_LIST = (short)GetExpression();
		return 1;
	}

//---------------------------------------------------------

	case dir_show:
	{
		int exp = GetExpression();
		
		if (g_Pass == 1)
			printf("%d",exp);

		if (!Token(";"))
			if (g_Pass == 1) printf("\n");
	
		return 1;
	}

//---------------------------------------------------------

	case dir_print:
	//if (QToken(".print") || QToken(".?"))
	{
		int exp;
		char v;
	
		if (Token("\""))
		{
			while(1)	
			{
				v = *g_FilePtr;
				
				if (v == 0 || v == '"')
					break;
				
				printf("%c",v);

				g_FilePtr++;
			}

			NeedToken("\"");

			if (!Token(";"))
				printf("\n");

			return 1;
		}

		exp = GetExpression();

		printf("%d",exp);

		if (!Token(";"))
			printf("\n");
	
		return 1;
	}

//---------------------------------------------------------

	case dir_ifdef:
	{
		GetName();							// Get the new type Name	
		SkipWhiteSpace();
		
		if (!SymbolExists(g_Name, section_Script, -1))
		{
			if (NextToken("{"))
			{
				SkipPair('{','}');
			}
		}
		return 1;
	}

//---------------------------------------------------------

	case dir_ifdefglobal:
	{
		GetName();							// Get the new type Name	
		SkipWhiteSpace();
		
		if (!SymbolExists(g_Name, section_Enum, 0))
		{
			if (NextToken("{"))
			{
				SkipPair('{','}');
			}
		}
		return 1;
	}

//---------------------------------------------------------

	case dir_ifndefglobal:
	{
		GetName();							// Get the new type Name	
		SkipWhiteSpace();
		
		if (SymbolExists(g_Name, section_Enum, 0))
		{
			if (NextToken("{"))
			{
				SkipPair('{','}');
			}
		}
		return 1;
	}

//---------------------------------------------------------

	case dir_ifndef:
	{
		GetName();							// Get the new type Name	
		SkipWhiteSpace();
		
		if (SymbolExists(g_Name, section_Script, -1))
		{
			if (NextToken("{"))
			{
				SkipPair('{','}');
			}
		}
		return 1;
	}

//---------------------------------------------------------

	case dir_if:
	{

		if (!GetExpression())
		{
			SkipWhiteSpace();

			if (NextToken("{"))
			{
				SkipPair('{','}');
			}
		}
		return 1;
	}

//---------------------------------------------------------

	case dir_while:
	{
	
		if (GetExpression())
		{
			g_FilePtr = SearchScope(g_FilePtr);
		}
		return 1;
	}

//---------------------------------------------------------

	case dir_enum:
	{
		AsmEnums();
		return 1;
	}


//---------------------------------------------------------

	case dir_breakpoint:
	{
		SkipWhiteSpace();
		return 1;
	}

//---------------------------------------------------------

	case dir_globals_in_code:
	{
		g_GlobalsInCode = 1;
		SkipWhiteSpace();
		return 1;
	}

//---------------------------------------------------------

	case dir_globl:
//	if (QToken(".globl") || QToken(".global") || QToken(".weak"))
	{
		unsigned int n;
		
		GetAsmName();

		if (g_GlobalsInCode && (g_Section == SECT_code))
		{
//!			*CodePtr++ = '*';

			ArraySet(&g_CodeMemArray, g_CodeIP, '*');
			g_CodeIP++;

			for (n=0;n<strlen(g_Name);n++)
			{
//!				*CodePtr++ = g_Name[n];

				ArraySet(&g_CodeMemArray, g_CodeIP, g_Name[n]);
				g_CodeIP++;
			}
	
//!			*CodePtr++ = '*';

			ArraySet(&g_CodeMemArray, g_CodeIP, '*');
			g_CodeIP++;
		}

		if (g_LIST)
			printf("Found global '%s'\n",g_Name);

		DefineLabelGlobal();

//#ifdef INCLUDE_JAVAGEN
//		if (g_JavaPass)
//			JavaGen_Global(g_Name);
//#endif
		
		SkipWhiteSpace();
		return 1;
	}

//---------------------------------------------------------

	case dir_extern:
//	if (QToken(".extern") || QToken(".extrn"))
	{
		GetAsmName();

		if (g_LIST)
			printf("Found external '%s'\n",g_Name);

		// SHOULD DO NOTHING 
		//ExternDecl(g_Name);

		SkipWhiteSpace();
		return 1;
	}

//---------------------------------------------------------

	case dir_byte:
	{
		SetAsmDataChar();

		do
		{
			g_imm = GetExpression();					// Get the number

			WriteDataTypeArray(dir_byte, 1);
			WriteByte(g_imm);
		}
		while (QToken(","));

		return 1;

	}

//---------------------------------------------------------

	case dir_half:
	{
		SetAsmDataChar();

		do
		{
			g_imm = GetExpression();					// Get the number

			WriteDataTypeArray(dir_half, 2);
			WriteWord(g_imm);
		}
		while (QToken(","));

		return 1;
	}

//---------------------------------------------------------

	case dir_word:
	{
		SYMBOL *ref;
		
		if (g_Section != SECT_data)
			Error(Error_Skip,"data word in not in data section");

		SetAsmDataChar();
		
		do
		{
			g_imm = GetExpression();				// Get the number

			WriteDataTypeArray(dir_word, 4);

			ref = GetLastSymbolRef();
			WriteLongRef(g_imm, ref);

		}
		while (QToken(","));
		return 1;
	}

//---------------------------------------------------------

	case dir_space:
	{
		SetAsmDataChar();

		g_imm = GetExpression();					// Get the number

		if (g_LIST)
			printf("space %d\n",g_imm);
	
		if (!g_imm)
			return 1;

		WriteDataTypeArray(dir_space, g_imm);
		
		do
		{
			WriteByteSpace(0);
		}
		while(--g_imm);

		return 1;
	}

//---------------------------------------------------------

	case dir_string:
	{
		SetAsmDataChar();

		WriteDataTypeArray(dir_string, 1);

		GetStrings();
		return 1;
	}

//---------------------------------------------------------

	case dir_comm:
	{
		int LastSect = g_Section;

		SetAsmDataChar();

		g_Section = SECT_bss;

		GetAsmName();
		
		//!! make this global too

		DefineLabelGlobal();
		DefineLabel(label_Local);
		
		NeedToken(",");
				
		g_imm = GetExpression();					// Get the number

		if (g_LIST)
			printf("comm %d to .bss\n",g_imm);
	
		if (g_imm == 0)
			return 1;

		WriteDataTypeArray(dir_comm, g_imm);
		
		g_BssIP += g_imm;

		g_Section = LastSect;

		return 1;
	}

//---------------------------------------------------------

	case dir_lcomm:
	{
		int LastSect = g_Section;

		SetAsmDataChar();

		g_Section = SECT_bss;

		GetAsmName();
		DefineLabel(label_Local);
		
		NeedToken(",");
				
		g_imm = GetExpression();					// Get the number

		if (g_LIST)
			printf("lcomm %d to .bss\n",g_imm);
	
		if (g_imm == 0)
			return 1;

		WriteDataTypeArray(dir_lcomm, g_imm);
		
		g_BssIP += g_imm;

		g_Section = LastSect;

		return 1;
	}

//---------------------------------------------------------

	case dir_align:
	{
		g_imm = GetExpression();					// Get the number

		if (g_Section == SECT_code)
			return 1;

		AddAlignRef(g_imm);
		return 1;
	}


//---------------------------------------------------------

	case dir_bss:
	{
		g_Section = SECT_bss;

		if (g_LIST)
			printf("** Section .bss IP = %x\n",g_BssIP);

		return 1;
	}

//---------------------------------------------------------

	case dir_data:
	{
		g_Section = SECT_data;

		if (g_LIST)
			printf("** Section .data IP = %x\n",g_DataIP);

		return 1;
	}

//---------------------------------------------------------

	case dir_code:
//	if (QToken(".code") || QToken(".text"))
	{
		g_Section = SECT_code;

		if (g_LIST)
			printf("** Section .code IP = %x\n",g_CodeIP);

		return 1;
	}

//---------------------------------------------------------

	case dir_ent:
	{
		SkipLine();
		return 1;
	}


//---------------------------------------------------------

	case dir_end:
		return 1;

//---------------------------------------------------------

	case dir_stabs:
	{
		Parse_stabs();
		//SkipLine();
		return 1;
	}

//---------------------------------------------------------

	case dir_stabn:
	{
		Parse_stabn();
		//SkipLine();
		return 1;
	}

//---------------------------------------------------------

	case dir_stabd:
	{
		SkipLine();
		return 1;
	}

//---------------------------------------------------------

	}		// End of switch

	return 0;
}

//****************************************
//		 
//****************************************

void AsmEnums()
{
	int Value=0;	//Fredrik: correct initializer in case first token is not "=" ?
		
	NeedToken("{");

	do
	{
		GetName();							// Get the new type Name	

		if (Token("="))
		{
			Value = GetExpression();
		}
		else
			Value++;

//		if (g_Pass == 1)
			RedefENum(g_Name, Value);

		//printf("- Enum %s' Value %ld\n",g_Name,Value);

		SkipWhiteSpace();
	}
	while(Token(","));
	
	NeedToken("}");
}

//****************************************
//			 Search Scope
//****************************************

char * SearchScope(char *StartPtr)
{
	short ScopeCount = 0;
	char *ptr = StartPtr;
	char v;
	
	while(1)
	{
		if (ptr == g_FileTop)
			Error(Error_Skip, "Scope mismatch");

		v = *ptr;

		if (v == '}')
			ScopeCount++;
			
		if (v == '{')
		{
			ScopeCount--;
			if (ScopeCount == 0)
				return ptr;
		}

		ptr--;
	}
}

//****************************************
//			 Get a identifier
//****************************************

void GetQuoteStr(short maxlen)
{
	unsigned int v = 0;
	short c=0;
	
	if (*g_FilePtr++ != '"')
		Error(Error_Skip, "String has missing '\"'");

	while (1)
	{
		v = *g_FilePtr++;

		if (v == 0 || v == 13)
			Error(Error_Skip, "string not terminated correctly");
			
		if (v == '\"')
			break;

		if (v == '\\')
		{
//			FilePtr++;			// skip the '\'
			v = GetEscCode();
		}

		WriteByte(v);

		if (c++ >= maxlen)
			Error(Error_Skip, "string exceeded maximum length");
	}

	return;
}

//****************************************
//			 Get Strings
//****************************************

void GetStrings()
{
	do
	{
		if (NextToken("\""))
		{
			GetQuoteStr(NAME_MAX);						// get a bounded string
		}
		else
		{
			g_imm = GetExpression();					// Get the number
			WriteByte(g_imm);
		}
	}
	while (QToken(","));
}


//****************************************
//		Define a global cdtor
//****************************************
/*
void DefineStructor(char *type, int count)
{
	int oldSect = g_Section;
	int oldData = g_DataIP;
	
	imm = GetExpression();				// Get the function value

	g_Section = SECT_data;
	g_DataIP = g_imm;

	sprintf(g_Name, "%s%d", type, count);

	DefineLabelGlobal();
	DefineLabel(label_Local);

	g_DataIP = oldData;
	g_Section = oldSect;
}
*/


//****************************************
//
//****************************************

int This_SLD_File = 0;

void SetSLD_Line(int line)
{
	if (g_Final_Pass)
	{
		ArraySet(&g_SLD_Line_Array, g_CodeIP, line);
		ArraySet(&g_SLD_File_Array, g_CodeIP, This_SLD_File);
	}
}

//****************************************
//
//****************************************

int SetSLD_Name(char *file)
{
	This_SLD_File = Get_SLD_FileNo(file);
	return This_SLD_File;
}

//****************************************
// 		Set Asm Char Data Array
//****************************************

void SetAsmDataChar()
{
	int v;

	if (g_Section == SECT_code)
		return;
	
	v = g_DataIP;
	
	if (g_Section == SECT_bss)
		v += g_MaxDataIP;
	
	ArraySet(&g_AsmCharDataArray, v, (int) DataCharPtr);
}


