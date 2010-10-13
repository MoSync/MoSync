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

	if (*FilePtr != '.')
		return 0;

	DataCharPtr = FilePtr;

	if (QToken(".eof"))
	{
		// mark end of label array as end of bss
		
		strcpy(Name, "_________eof_________");
		Section = SECT_bss;
		DefineLabel(label_Local);

		SourceFileCheckEOF(LastTokenPtr() - FileTop);

		EndFile = 1;
		return 1;
	}


	GetToken();	
	t = ScanAsmToken();

	switch(t)
	{

//---------------------------------------------------------

	case 0:
		Error(Error_Skip, "Illegal directive '%s'",Name);


//---------------------------------------------------------

	case dir_sourcefile:
	{		
		int file;
		
		SkipWhiteSpace();
		GetLFileName();	

//		SetCSourceFile(Name);
		file = SetSLD_Name(Name);

		SetCSourceFile(Name, file);
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
			
		if (!CurrentModel)
			CurrentModel = model;
		else
		{
			if (model != CurrentModel)
				Error(Error_Fatal, "multiple architectual models defined");
		}

		return 1;
	}

//---------------------------------------------------------

	case dir_ctor:
	{
		SYMBOL* ref;
		imm = GetExpression();				// Get the number
		ref = GetLastSymbolRef();

		if(pass_count > 1)
		{
			if (!ref)
				Error(Error_Fatal, "Constructor has bad reference");

			ArraySet(&CtorArrayImm, CtorCount, imm);
			ArraySet(&CtorArray, CtorCount++, (int) ref);
		}
		return 1;		
	}

//---------------------------------------------------------

	case dir_dtor:
	{		
		SYMBOL* ref;
		imm = GetExpression();				// Get the number
		ref = GetLastSymbolRef();

		if(pass_count > 1)
		{
			if (!ref)
				Error(Error_Fatal, "Destructor has bad reference");

			ArraySet(&DtorArrayImm, DtorCount, imm);
			ArraySet(&DtorArray, DtorCount++, (int) ref);
		}
		return 1;		
	}


//---------------------------------------------------------

	case dir_sourcedir:
	{		
		SkipWhiteSpace();
		GetLFileName();	
		SetCSourceDir(Name);
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
		if (!ArgUseStabs)
		{
			SkipLine();			// Skip excess debug labels
			return 1;
		}

		// Define a label

		Section = SECT_code;

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
		if (NextToken("\""))
		{
			GetStringName(1024);

			Error(Error_Fatal, "User: %s", Name);
			return 1;
		}
	
	
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
		AllowFarBranches = 1;
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
			LocalScope++;
			return 1;
		}

		if (Token("reset"))
		{
			LocalScope = 1;
			return 1;
		}

		Error(Error_Fatal, "Bad .localscope type");
		return 1;
	}

//---------------------------------------------------------

	case dir_func:
	{	
		Function_Return_Type = RET_void;

		GetName();

		ReplaceChar(Name, '.', '_');

		Function_Param_Count = -1;

		if (Token(","))
		{
			Function_Param_Count = GetNum();

			if (Token(","))				// Get param count
			{				
				if (Token("int"))				// Get return type
					Function_Return_Type = RET_int;
				else if (Token("float"))
					Function_Return_Type = RET_float;
				else if (Token("double"))
					Function_Return_Type = RET_double;
				else if (Token("void"))
					Function_Return_Type = RET_void;
				else if (Token("?"))
					Function_Return_Type = RET_double;	//patch fix
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
		strcpy(SysCallName,Name);
	
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
				strcpy(istr,Name);
				iptr = istr;
			}

		}

		if (Pass == 1)
			MapSysCall(SysCallName, v, p, r, iptr);

		return 1;
	}

//---------------------------------------------------------

	case dir_lfile:
	{
		SkipWhiteSpace();
		GetLFileName();
		
		strcpy(CurrentFile, Name);
//		CurrentFileLine = GetLineNumber(FileTop,GetPrevFilePtr());
		
		AddFileSym(Name, LocalScope+1);
		
		GetRelPath(CurrentFile);
		return 1;
	}

//---------------------------------------------------------

	case dir_org:
	{
		int v = GetExpression();

		if (v >= MAX_CODE_MEM)
			Error(Error_Fatal, ".org has illegal address");

		CodeIP = v;
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
			GetEnumValue(Name);	// Evaluate the symbol


			ThunkSym = GetLastSymbolRef();

			//N_FUN 141 4935 '_Z7XPClosev:F(0,6)'

			StabsEmit("N_THUNK %d %d '%s'\n", v, ThunkSym, Name);

			RegisterThunk(v, ThunkSym);
			return 1;
		}
		
		// Catch normal sets
		
		GetName();							// Get the new type Name	
		NeedToken("=");
		RedefENum(Name, GetExpression());	
		return 1;
	}

//---------------------------------------------------------

	case dir_debug:
	{
		DEBUG = (short)GetExpression();	
		return 1;
	}


//---------------------------------------------------------

	case dir_info:
	{
		INFO = (short)GetExpression();	
		return 1;
	}

//---------------------------------------------------------

	case dir_list:
	{
		LIST = (short)GetExpression();	
		return 1;
	}

//---------------------------------------------------------

	case dir_show:
	{
		int exp = GetExpression();
		
		if (Pass == 1)
			printf("%d",exp);

		if (!Token(";"))
			if (Pass == 1) printf("\n");
	
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
				v = *FilePtr;
				
				if (v == 0 || v == '"')
					break;
				
				printf("%c",v);

				FilePtr++;
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
		
		if (!SymbolExists(Name, section_Script, -1))
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
		
		if (!SymbolExists(Name, section_Enum, 0))
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
		
		if (SymbolExists(Name, section_Enum, 0))
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
		
		if (SymbolExists(Name, section_Script, -1))
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

	case dir_do:
	{

		// If we have a '{' just ignore it an keep going
		
		if (Token("{"))
			return 1;
	
		// Must be an expression then

		// if false then skip to while

		if (!GetExpression())
		{
			SearchScopeForward();
			NeedToken(".enddo");
		}
			
		return 1;
	}

//---------------------------------------------------------

	case dir_enddo:
	{
		FilePtr = SearchScope(FilePtr);			
		return 1;
	}


//---------------------------------------------------------

	case dir_while:
	{
		if (GetExpression())
		{
			FilePtr = SearchScope(FilePtr);
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
		GlobalsInCode = 1;
		SkipWhiteSpace();
		return 1;
	}

//---------------------------------------------------------

	case dir_globl:
//	if (QToken(".globl") || QToken(".global") || QToken(".weak"))
	{
		unsigned int n;
		
		GetAsmName();

		if (GlobalsInCode && (Section == SECT_code))
		{
//!			*CodePtr++ = '*';

			ArraySet(&CodeMemArray, CodeIP, '*');
			CodeIP++;

			for (n=0;n<strlen(Name);n++)
			{
//!				*CodePtr++ = Name[n];

				ArraySet(&CodeMemArray, CodeIP, Name[n]);
				CodeIP++;
			}
	
//!			*CodePtr++ = '*';

			ArraySet(&CodeMemArray, CodeIP, '*');
			CodeIP++;
		}

		if (LIST)
			printf("Found global '%s'\n",Name);

		DefineLabelGlobal();

//#ifdef INCLUDE_JAVAGEN
//		if (JavaPass)
//			JavaGen_Global(Name);
//#endif
		
		SkipWhiteSpace();
		return 1;
	}

//---------------------------------------------------------

	case dir_extern:
//	if (QToken(".extern") || QToken(".extrn"))
	{
		GetAsmName();

		if (LIST)
			printf("Found external '%s'\n",Name);

		// SHOULD DO NOTHING 
		//ExternDecl(Name);

		SkipWhiteSpace();
		return 1;
	}

//---------------------------------------------------------

	case dir_byte:
	{
		SetAsmDataChar();

		do
		{
			imm = GetExpression();					// Get the number

			WriteDataTypeArray(dir_byte, 1);
			WriteByte(imm);
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
			imm = GetExpression();					// Get the number

			WriteDataTypeArray(dir_half, 2);
			WriteWord(imm);
		}
		while (QToken(","));

		return 1;
	}

//---------------------------------------------------------

	case dir_word:
	{
		SYMBOL *ref;
		
		if (Section != SECT_data)
			Error(Error_Skip,"data word in not in data section");

		SetAsmDataChar();
		
		do
		{
			imm = GetExpression();				// Get the number

			WriteDataTypeArray(dir_word, 4);

			ref = GetLastSymbolRef();
			WriteLongRef(imm, ref);

		}
		while (QToken(","));
		return 1;
	}

//---------------------------------------------------------

	case dir_space:
	{
		SetAsmDataChar();

		imm = GetExpression();					// Get the number

		if (LIST)
			printf("space %d\n",imm);
	
		if (!imm)
			return 1;

		WriteDataTypeArray(dir_space, imm);
		
		do
		{
			WriteByteSpace(0);
		}
		while(--imm);

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
		int LastSect = Section;

		SetAsmDataChar();

		Section = SECT_bss;

		GetAsmName();
		
		//!! make this global too

		DefineLabelGlobal();
		DefineLabel(label_Local);
		
		NeedToken(",");
				
		imm = GetExpression();					// Get the number

		if (LIST)
			printf("comm %d to .bss\n",imm);
	
		if (imm == 0)
			return 1;

		WriteDataTypeArray(dir_comm, imm);
		
		BssIP += imm;

		Section = LastSect;

		return 1;
	}

//---------------------------------------------------------

	case dir_lcomm:
	{
		int LastSect = Section;

		SetAsmDataChar();

		Section = SECT_bss;

		GetAsmName();
		DefineLabel(label_Local);
		
		NeedToken(",");
				
		imm = GetExpression();					// Get the number

		if (LIST)
			printf("lcomm %d to .bss\n",imm);
	
		if (imm == 0)
			return 1;

		WriteDataTypeArray(dir_lcomm, imm);
		
		BssIP += imm;

		Section = LastSect;

		return 1;
	}

//---------------------------------------------------------

	case dir_align:
	{
		imm = GetExpression();					// Get the number

		if (Section == SECT_code)
			return 1;

		AddAlignRef(imm);
		return 1;
	}


//---------------------------------------------------------

	case dir_bss:
	{
		Section = SECT_bss;

		if (LIST)
			printf("** Section .bss IP = %x\n",BssIP);

		return 1;
	}

//---------------------------------------------------------

	case dir_data:
	{
		Section = SECT_data;

		if (LIST)
			printf("** Section .data IP = %x\n",DataIP);

		return 1;
	}

//---------------------------------------------------------

	case dir_code:
//	if (QToken(".code") || QToken(".text"))
	{
		Section = SECT_code;

		if (LIST)
			printf("** Section .code IP = %x\n",CodeIP);

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

//		if (Pass == 1)
			RedefENum(Name, Value);

		//printf("- Enum %s' Value %ld\n",Name,Value);

		SkipWhiteSpace();
	}
	while(Token(","));
	
	NeedToken("}");
}

//****************************************
//			 Search Scope (Backward)
//****************************************

char * SearchScope(char *StartPtr)
{
	short ScopeCount = 0;
	char *ptr = StartPtr;
	char v;
	
	while(1)
	{
		if (ptr == FileTop)
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
//		 Search Scope (Forward)
//****************************************

void SearchScopeForward()
{
	if (NextToken("{"))
	{
		SkipPair('{','}');
	}
}

//****************************************
//			 Get a identifier
//****************************************

void GetQuoteStr(short maxlen)
{
	unsigned int v = 0;
	short c=0;
	
	if (*FilePtr++ != '"')
		Error(Error_Skip, "String has missing '\"'");

	while (1)
	{
		v = *FilePtr++;

		if (v == 0 || v == 13)
			Error(Error_Skip, "string not terminated correctly");
			
		if (v == '\"')
			break;

		if (v == '\\')
		{
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
			imm = GetExpression();					// Get the number
			WriteByte(imm);
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
	int oldSect = Section;
	int oldData = DataIP;
	
	imm = GetExpression();				// Get the function value

	Section = SECT_data;
	DataIP = imm;

	sprintf(Name, "%s%d", type, count);

	DefineLabelGlobal();
	DefineLabel(label_Local);

	DataIP = oldData;
	Section = oldSect;
}
*/


//****************************************
//
//****************************************

int This_SLD_File = 0;

void SetSLD_Line(int line)
{
	if (Final_Pass)
	{
		ArraySet(&SLD_Line_Array, CodeIP, line);
		ArraySet(&SLD_File_Array, CodeIP, This_SLD_File);
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

	if (Section == SECT_code)
		return;
	
	v = DataIP;
	
	if (Section == SECT_bss)
		v += MaxDataIP;	
	
	ArraySet(&AsmCharDataArray, v, (int) DataCharPtr);
}


