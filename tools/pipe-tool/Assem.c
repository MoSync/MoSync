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

#ifdef WIN32
#include <windows.h>
#else
#include "linux.h"
#endif

#include "compile.h"

//****************************************
//
//****************************************

void AsmMain()
{
	int p;
	int t = GetTickCount();

//	PerfBegin();

	SetError_Mode(ErrorMode_Full);

	InitAsmTokens();

	InitVirtualIndex();
	InitVarPool();
	GenRegConst();					// Generate the constant table
	AsmAllocMem();

	g_Section = SECT_data;

	g_pass_count = 0;
//	g_JavaPass = 0;

	g_JavaPass = g_ArgJavaNative;

	g_DEBUG = 0;
	g_LIST = 0;

	g_MaxDataIP = 0;

	g_Rebuild_Mode = 0;

//-------------------------
//	 do multi passes
//-------------------------
	
	g_Final_Pass = 0;

	for (p=1;p<16;p++)
	{
		printf("pass %i. %i known symbols.\n", p, CountUsedSymbols());
		if (AsmPass(p))
			break;
	}

//	printf("final pass. %i known symbols.\n", CountUsedSymbols());

	g_Final_Pass = 1;

	AsmPass(p+1);

//	printf("Symbols used %d\n",CountUsedSymbols());

//-------------------------
//  Do dependency search
//-------------------------

#ifdef INCLUDE_CODE_REBUILD

	if (g_Do_Elimination)
	{
		SearchDep_Main();

		if (g_ArgJavaNative)
		{
			RebuildJava_Main();
			//Error(Error_Fatal, "Mode not available");
		}
#ifndef _CRT_SECURE_NO_DEPRECATE	//hack for msvc	//Fredrik
		else if (g_ArgBrewGen)
		{
			RebuildBrew_Main();
		}
#endif
		else if (g_ArgCppGen)
		{
			RebuildCpp_Main();
		}
		else
		{
			Rebuild_Main();
		}
	}

#endif

//-------------------------
//		Finish up
//-------------------------

	if (g_LIST)
		printf("LocalScope %d\n",g_LocalScope);

	if (!g_ArgJavaNative)
		WriteModule();

//	DumpVirtuals();

	if (g_Do_Dump_Symbols)
		DumpSymbols(0);

	if (g_Do_Dump_Unref_Symbols)
		DumpSymbols(1);

//	if (g_LIST)
//		DumpVarPool();

	if (g_ArgSLD)
		DumpIPTrans();

	// Dump meta data for recompiler
	if ( g_ArgWriteMeta )
	{
		FILE *fout = fopen( g_MetaFileName, "w" );
		if ( fout == NULL )
			printf( "Failed to meta data file\n" );		
		else
		{
			DumpMetaData( fout );
			fclose( fout );
		}
	}


//	PerfEnd();

	t = GetTickCount() - t;

	printf("Symbols used %d\n",CountUsedSymbols());
	printf("Done... (%d ms)\n", t);
}

//****************************************
//		Do one pass of assembly
//****************************************

int AsmPass(int thisPass)
{
	// Set the pass number
	
	g_Pass = 1;
	g_pass_count = thisPass;

//	if (!Final_Pass)
//		ResetVarPool();

	// Clear the access arrays

	if (thisPass > 1)
	{
		g_Pass = 2;

		// Save the previous ptrs for code/data/bss

		g_pCodeIP = g_CodeIP;
		g_pDataIP = g_DataIP;
		g_pBssIP = g_BssIP;

		// Clear access arrays

		ArrayClear(&g_CallArray);
		ArrayClear(&g_DataArray);
		ArrayClear(&g_DataAlignArray);
		ArrayClear(&g_DataTypeArray);
		ArrayClear(&g_LabelArray);
		ArrayClear(&g_CodeLabelArray);
		ArrayClear(&g_DataAccessArray);
		ArrayClear(&g_DataMixArray);
		ArrayClear(&g_PaddingArray);

		ArrayClear(&g_SLD_Line_Array);
		ArrayClear(&g_SLD_File_Array);
	
		// Sort constant pool

		SortVarPool();
		//printf("Pass %d VarPool Hash %x\n",thisPass,HashVarPool());
	}

	// Reset all the asm ptrs
	
	SetAsmPtrs();

	// Reset constructor/destructor ptrs

	SetAsmCDtors();

	if (g_INFO)
		printf("Pass %d\n", thisPass);

	// Set the pass number in script variable

	RedefENum("__pass",thisPass);

	// Assemble everything !!
	
	Assemble();

	// Write the constructor/destructor

	EmitCDtors();

	// Align the end of data, this is used as an offset for bss

	AlignDataSection();
	g_MaxDataIP = g_DataIP;

	if (g_INFO)
	{
		printf("Sections: Code=%x : Data=%d : BSS=%x (Final %d)\n",g_CodeIP, g_DataIP, g_BssIP, g_Final_Pass);
	}

	// Sort constant pool

//	if (!Final_Pass)
//		SortElimVarPool();

//	printf("Pass %d VarPool Hash %x\n",thisPass,HashVarPool());


	// Check if the code is ready

	if (thisPass == 1)
	{
		return 0;			// Say not ready on pass 1	
	}

	if (g_CodeIP != g_pCodeIP)
		return 0;			// Build not ready yet
		
	if (g_DataIP != g_pDataIP)
		return 0;			// Build not ready yet

	if (g_BssIP  != g_pBssIP)
		return 0;			// Build not ready yet

	return 1;				// Build ready
}


//****************************************
//			 Assemble code
//****************************************

void Assemble()
{
//	int Line = 0;
	char *FPtr = g_FilePtr;				// Preserve file pointer

	g_FileLine = 0;
	g_EndFile = 0;

	ResetErrorCount();
	SetErrorReturn();

	while(1)
	{
		g_FileLine++;
	
		if (g_EndFile)
			break;

		if (*g_FilePtr == 0)
		{
			Error(Error_Fatal, "File terminates with null character\n");
			break;
		}
		
		if (*g_FilePtr == '{' || *g_FilePtr == '}')
		{
			g_FilePtr++;
			continue;	
		}

		if (Directives())
			continue;
	
/*		if (*g_FilePtr == 0)
		{
			Error(Error_Fatal, "File terminates with null character\n");
			break;
		}
*/	
		if (TestAsmLabel())
		{
			GetAsmName();
			NeedToken(":");

			DefineLabel(label_Local);
			continue;
		}
		else
		{
			do
			{
				Instructions();
			}
			while(QToken(";"));
		}
	}

	// End last function
	SetCurrentFunction(0);

	AsmCheckBounds();

	g_FilePtr = FPtr;
	
	ExitErrorCount();
	
	SetErrorReset();
}

//****************************************
//		   Allocate Output Memory
//****************************************

void AsmAllocMem()
{

//!	ConstMem = NewPtrClear( (int) MAX_CONST_MEM);
//!	DataMem  = NewPtrClear( (int) MAX_DATA_MEM);
//!	CodeMem  = NewPtrClear( (int) MAX_CODE_MEM);
//!	ResMem   = NewPtrClear( (int) MAX_RES_MEM);
	
//!	if (ConstMem == 0)	Error(Error_Fatal, "can't allocate ConstMem");
//!	if (DataMem == 0)	Error(Error_Fatal, "can't allocate DataMem");
//!	if (CodeMem == 0)	Error(Error_Fatal, "can't allocate CodeMem");
//!	if (ResMem == 0)	Error(Error_Fatal, "can't allocate ResMem");
	
}

//****************************************
//
//****************************************

void AsmCheckBounds()
{
//!	if (DataPtr > (DataMem + (MAX_DATA_MEM - MEM_BOUNDS_WARN)))
//!		Error(Error_Fatal, "DataMem is low %d",DataPtr - DataMem);
	
//!	if (CodePtr > (CodeMem + (MAX_CODE_MEM - MEM_BOUNDS_WARN)))
//!		Error(Error_Fatal, "CodeMem is low %d",CodePtr - CodeMem);

//!	if (ResPtr > (ResMem + (MAX_RES_MEM - MEM_BOUNDS_WARN)))
//!		Error(Error_Fatal, "ResMem is  low %d",ResPtr - ResMem);

	return;

}

//****************************************
//		   Allocate Output Memory
//****************************************

void SetAsmPtrs()
{
	g_LocalScope = 1;

//!	DataPtr = DataMem;
//!	CodePtr = CodeMem;

	g_CodeIP = 0;
	g_DataIP = 4;			// Avoid using location 0
	g_BssIP = 0;

	g_CurrentFunction = 0;

	g_CurrentFile[0] = 0;
//	g_CurrentFileLine = 0;
}

//****************************************
//
//****************************************

void SetAsmCDtors()
{
	g_CtorCount = 0;
	g_DtorCount = 0;
	
	ArrayClear(&g_CtorArray);
	ArrayClear(&g_DtorArray);
}

//****************************************
//		   Allocate Output Memory
//****************************************

void AsmDisposeMem()
{
//!	if (ConstMem)
//!		DisposePtr(ConstMem);

//!	if (DataMem)
//!		DisposePtr(DataMem);

//!	if (CodeMem)
//!		DisposePtr(CodeMem);

//!	if (ResMem)
//!		DisposePtr(ResMem);
}

//****************************************
//			Get Reg Name
//****************************************

char * GetRegName(int reg)
{
	g_regstr[0] = 0;

	if (reg & 0xffffff00)
		return g_regstr;

	if (reg < 32)
	{
		strcpy(g_regstr, regsyms[reg]);
		return g_regstr;
	}

	sprintf(g_regstr, "r%d(#%x)", reg, g_ConstTable[reg] );
	return g_regstr;
}

//****************************************
//			 Get a identifier
//****************************************

void GetAsmName()
{
	//unsigned int v = 0;
	char *NamePtr = g_Name;
	
	while (asmsym(*g_FilePtr))
		*NamePtr++ = *g_FilePtr++;

	*NamePtr++ = 0;

	return;
}

//****************************************
//			 Get a identifier
//****************************************

void GetAsmThunkName()
{
	//unsigned int v = 0;
	char *NamePtr = g_Name;
	
	if (*g_FilePtr != '%')
		Error(Error_Fatal, "invalid thunk name");

	// Copy Thunk delimiter
	
	*NamePtr++ = *g_FilePtr++;
	
	// Copy rest of symbol name
	
	while (asmsym(*g_FilePtr))
		*NamePtr++ = *g_FilePtr++;

	*NamePtr++ = 0;

	return;
}

//****************************************
//			 Get a identifier
//****************************************

short TestAsmLabel()
{
	unsigned short v = 0;
	char *NamePtr = g_Name;
	char *TempPtr;

	if (!asmsymf(*g_FilePtr))
		return 0;
	
	TempPtr = g_FilePtr;
		
	while (asmsym(*g_FilePtr))
		*NamePtr++ = *g_FilePtr++;

	*NamePtr++ = 0;

	if (NextToken(":"))
		v = 1;
	
	g_FilePtr = TempPtr;
	return v;
}

//************************************************************************************************
//								Declare Globals/externs
//************************************************************************************************

//****************************************
//
//****************************************

void SetCurrentFunction(SYMBOL * Sym)
{
	// Terminate function
	
	if (Sym == 0)
	{
		// Terminate previous function, if it exists
		
		if (g_CurrentFunction)
		{
			// Save last IP of function
			
			g_CurrentFunction->EndIP = g_CodeIP - 1;

//#ifdef INCLUDE_JAVAGEN
//			if (g_JavaPass)
//				JavaGen_FunctionEnd(g_CurrentFunction->Name, g_CurrentFunction->Value);
//#endif

			// Reset function
			g_CurrentFunction = 0;
		}

		return;		
	}

//#ifdef INCLUDE_JAVAGEN
//	if (g_JavaPass)
//		JavaGen_FunctionBegin(Sym->Name, Sym->Value);
//#endif

	g_CurrentFunction = Sym;
	return;
}

//****************************************
// Write a symref to the code lable array
//		and obey function priority
//****************************************

void WriteCodeArray(SYMBOL *Sym)
{
	SYMBOL *csym;

	// Check if there are any entries in the code lable array
	
	csym = (SYMBOL *) ArrayGet(&g_CodeLabelArray, g_CodeIP);

	// If there are none, just write the reference

	if (!csym)
	{
		ArraySet(&g_CodeLabelArray, g_CodeIP, (int) Sym);
		return;
	}
	
	// There must already be a ref, if its a function then return
	
	if (csym->LabelType >= label_Function)
		return;
	
	// So the ref must be a label, check if the new sym
	// is a label, if it is then don't bother, since we already have a ref
	
	if (Sym->LabelType == label_Local)
		return;
	
	// The new sym must be a function of some sort, so
	// write it into the array
	
	ArraySet(&g_CodeLabelArray, g_CodeIP, (uint) Sym);
}

//****************************************
//
//****************************************

void DefineLabel(int isFunction)
{
	SYMBOL	*Sym;
	SYMBOL	sym;

	if (isFunction)
		SetCurrentFunction(0);

	Sym = FindSymbols(g_Name,section_Enum,section_Enum, g_LocalScope);
	
	if (Sym)
	{
		if (g_Pass == 1)
			Error(Error_Exit, "Symbol '%s' is already defined",g_Name);

		Sym->Type		= g_Section;
		Sym->Value		= g_CodeIP;

		if (g_Section == SECT_data)
			Sym->Value = g_DataIP;

		if (g_Section == SECT_bss)
			Sym->Value = g_BssIP;

		if (isFunction)
			SetCurrentFunction(Sym);
			
		// Dont redefine scope

//#ifdef INCLUDE_JAVAGEN
//		if (!isFunction)
//			if (g_JavaPass)
//				JavaGen_Label(g_Name, Sym->Value, Section);
//#endif

		// Write ref to data label array

		if (g_Section == SECT_code)
			WriteCodeArray(Sym);
		
		if (g_Section == SECT_data)
			ArraySet(&g_LabelArray, g_DataIP, (int) Sym);

		if (g_Section == SECT_bss)
			ArraySet(&g_LabelArray, g_BssIP + g_MaxDataIP, (int) Sym);
 
		return;
	}

	sym.Section	= section_Enum;
	sym.Type	= g_Section;
	sym.LocalScope	= g_LocalScope;
	sym.LabelType	= isFunction;
	sym.EndIP	= 0;
	sym.Flags	= 0;
	sym.Value	= g_CodeIP;
	sym.Params	= g_Function_Param_Count;
	sym.RetType	= g_Function_Return_Type;

	if (g_Section == SECT_data)
		sym.Value = g_DataIP;

	if (g_Section == SECT_bss)
		sym.Value = g_BssIP;

	Sym = StoreSymbol(&sym,g_Name);
	
	if (isFunction)
		SetCurrentFunction(Sym);			
}

//****************************************
//
//****************************************

void DefineLabelGlobal()
{
	SYMBOL	*Sym;
	SYMBOL	sym;

	if (g_Pass != 1)
		return;

	Sym = FindSymbols(g_Name,section_Enum,section_Enum, 0);
	
	if (Sym)
	{
		Error(Error_Skip, "Symbol '%s' is already defined",g_Name);
	}

	sym.Section		= section_Enum;
	sym.Value		= g_LocalScope;		// Save Local scope of variable
	sym.Type		= 0;
	sym.LocalScope	= 0;				// Store in scope 0 (Global Scope)

	StoreSymbol(&sym,g_Name);
}

//****************************************
//
//****************************************

void AddLabel(char *label, int global)
{
	strcpy(g_Name, label);
	
	if (global)
		DefineLabelGlobal();

	DefineLabel(label_Local);
}

//****************************************
//
//****************************************

void EmitCDtors()
{
	SYMBOL *ref;
	int oldSect = g_Section;
	int n;

	g_CDtorStart = g_DataIP;

	g_Section = SECT_data;
	
	// Write CTORS Array

	AddAlignRef(4);
	AddLabel("__global_ctor_chain", 1);
		
	for (n=0;n<g_CtorCount;n++)
	{
		ref = (SYMBOL *) ArrayGet(&g_CtorArray, n);
		WriteLongRef(ref->Value, ref);
	}

	WriteLong(0);							// Terminate table

	AddLabel("__global_ctor_term", 1);		// Mark terminator
	WriteLong(0);							// Terminate table

	// Write DTORS Array

	AddAlignRef(4);
	AddLabel("__global_dtor_chain", 1);
		
	for (n=0;n<g_DtorCount;n++)
	{
		ref = (SYMBOL *) ArrayGet(&g_DtorArray, n);
		WriteLongRef(ref->Value, ref);
	}

	WriteLong(0);							// Terminate table

	AddLabel("__global_dtor_term", 1);		// Mark terminator
	WriteLong(0);							// Terminate table

	g_CDtorEnd = g_DataIP;

	g_Section = oldSect;
}

