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

	Section = SECT_data;

	pass_count = 0;
//	JavaPass = 0;

	JavaPass = ArgJavaNative;

	DEBUG = 0;
	LIST = 0;

	MaxDataIP = 0;

	Rebuild_Mode = 0;

//-------------------------
//	 do multi passes
//-------------------------
	
	Final_Pass = 0;

	for (p=1;p<16;p++)
	{
		printf("pass %i. %i known symbols.\n", p, CountUsedSymbols());
		if (AsmPass(p))
			break;
	}

//	printf("final pass. %i known symbols.\n", CountUsedSymbols());

	Final_Pass = 1;

	AsmPass(p+1);

//	printf("Symbols used %d\n",CountUsedSymbols());

//-------------------------
//  Do dependency search
//-------------------------

#ifdef INCLUDE_CODE_REBUILD

	if (Do_Elimination)
	{
		SearchDep_Main();

		if (ArgJavaNative)
		{
			RebuildJava_Main();
			//Error(Error_Fatal, "Mode not available");
		}
#ifndef _CRT_SECURE_NO_DEPRECATE	//hack for msvc	//Fredrik
		else if (ArgBrewGen)
		{
			RebuildBrew_Main();
		}
#endif
		else
		{
			Rebuild_Main();
		}
	}

#endif

//-------------------------
//		Finish up
//-------------------------

	if (LIST)
		printf("LocalScope %d\n",LocalScope);

	if (!ArgJavaNative)
		WriteModule();

//	DumpVirtuals();

	if (Do_Dump_Symbols)
		DumpSymbols(0);

	if (Do_Dump_Unref_Symbols)
		DumpSymbols(1);

//	if (LIST)
//		DumpVarPool();

	if (ArgSLD)
		DumpIPTrans();

	// Dump meta data for recompiler
	if ( ArgWriteMeta )
	{
		FILE *fout = fopen( MetaFileName, "w" );
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
	
	Pass = 1;
	pass_count = thisPass;

//	if (!Final_Pass)
//		ResetVarPool();

	// Clear the access arrays

	if (thisPass > 1)
	{
		Pass = 2;

		// Save the previous ptrs for code/data/bss

		pCodeIP = CodeIP;
		pDataIP = DataIP;
		pBssIP = BssIP;

		// Clear access arrays

		ArrayClear(&CallArray);
		ArrayClear(&DataArray);
		ArrayClear(&DataAlignArray);
		ArrayClear(&DataTypeArray);
		ArrayClear(&LabelArray);
		ArrayClear(&CodeLabelArray);
		ArrayClear(&DataAccessArray);
		ArrayClear(&DataMixArray);
		ArrayClear(&PaddingArray);

		ArrayClear(&SLD_Line_Array);
		ArrayClear(&SLD_File_Array);
	
		// Sort constant pool

		SortVarPool();
		//printf("Pass %d VarPool Hash %x\n",thisPass,HashVarPool());
	}

	// Reset all the asm ptrs
	
	SetAsmPtrs();

	// Reset constructor/destructor ptrs

	SetAsmCDtors();

	if (INFO)
		printf("Pass %d\n", thisPass);

	// Set the pass number in script variable

	RedefENum("__pass",thisPass);

	// Assemble everything !!
	
	Assemble();

	// Write the constructor/destructor

	EmitCDtors();

	// Align the end of data, this is used as an offset for bss

	AlignDataSection();
	MaxDataIP = DataIP;

	if (INFO)
	{
		printf("Sections: Code=%x : Data=%d : BSS=%x (Final %d)\n",CodeIP, DataIP, BssIP, Final_Pass);
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

	if (CodeIP != pCodeIP)
		return 0;			// Build not ready yet
		
	if (DataIP != pDataIP)
		return 0;			// Build not ready yet

	if (BssIP  != pBssIP)
		return 0;			// Build not ready yet

	return 1;				// Build ready
}


//****************************************
//			 Assemble code
//****************************************

void Assemble()
{
//	int Line = 0;
	char *FPtr = FilePtr;				// Preserve file pointer

	FileLine = 0;
	EndFile = 0;

	ResetErrorCount();
	SetErrorReturn();

	while(1)
	{
		FileLine++;
	
		if (EndFile)
			break;

		if (*FilePtr == 0)
		{
			Error(Error_Fatal, "File terminates with null character\n");
			break;
		}
		
		if (*FilePtr == '{' || *FilePtr == '}')
		{
			FilePtr++;
			continue;	
		}

		if (Directives())
			continue;
	
/*		if (*FilePtr == 0)
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

	FilePtr = FPtr;
	
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
	LocalScope = 1;

//!	DataPtr = DataMem;
//!	CodePtr = CodeMem;

	CodeIP = 0;
	DataIP = 4;			// Avoid using location 0
	BssIP = 0;

	CurrentFunction = 0;

	CurrentFile[0] = 0;
//	CurrentFileLine = 0;
}

//****************************************
//
//****************************************

void SetAsmCDtors()
{
	CtorCount = 0;
	DtorCount = 0;
	
	ArrayClear(&CtorArray);
	ArrayClear(&DtorArray);
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
	regstr[0] = 0;

	if (reg & 0xffffff00)
		return regstr;

	if (reg < 32)
	{
		strcpy(regstr, regsyms[reg]);
		return regstr;
	}

	sprintf(regstr, "r%d(#%x)", reg, ConstTable[reg] );
	return regstr;
}

//****************************************
//			 Get a identifier
//****************************************

void GetAsmName()
{
	//unsigned int v = 0;
	char *NamePtr = Name;
	
	while (asmsym(*FilePtr))
		*NamePtr++ = *FilePtr++;

	*NamePtr++ = 0;

	return;
}

//****************************************
//			 Get a identifier
//****************************************

void GetAsmThunkName()
{
	//unsigned int v = 0;
	char *NamePtr = Name;
	
	if (*FilePtr != '%')
		Error(Error_Fatal, "invalid thunk name");

	// Copy Thunk delimiter
	
	*NamePtr++ = *FilePtr++;
	
	// Copy rest of symbol name
	
	while (asmsym(*FilePtr))
		*NamePtr++ = *FilePtr++;

	*NamePtr++ = 0;

	return;
}

//****************************************
//			 Get a identifier
//****************************************

short TestAsmLabel()
{
	unsigned short v = 0;
	char *NamePtr = Name;
	char *TempPtr;

	if (!asmsymf(*FilePtr))
		return 0;
	
	TempPtr = FilePtr;
		
	while (asmsym(*FilePtr))
		*NamePtr++ = *FilePtr++;

	*NamePtr++ = 0;

	if (NextToken(":"))
		v = 1;
	
	FilePtr = TempPtr;
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
		
		if (CurrentFunction)
		{
			// Save last IP of function
			
			CurrentFunction->EndIP = CodeIP - 1;

//#ifdef INCLUDE_JAVAGEN
//			if (JavaPass)
//				JavaGen_FunctionEnd(CurrentFunction->Name, CurrentFunction->Value);
//#endif

			// Reset function
			CurrentFunction = 0;
		}

		return;		
	}

//#ifdef INCLUDE_JAVAGEN
//	if (JavaPass)
//		JavaGen_FunctionBegin(Sym->Name, Sym->Value);
//#endif

	CurrentFunction = Sym;
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
	
	csym = (SYMBOL *) ArrayGet(&CodeLabelArray, CodeIP);

	// If there are none, just write the reference

	if (!csym)
	{
		ArraySet(&CodeLabelArray, CodeIP, (int) Sym);
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
	
	ArraySet(&CodeLabelArray, CodeIP, (uint) Sym);
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

	Sym = FindSymbols(Name,section_Enum,section_Enum, LocalScope);
	
	if (Sym)
	{
		if (Pass == 1)
			Error(Error_Exit, "Symbol '%s' is already defined",Name);

		Sym->Type		= Section;
		Sym->Value		= CodeIP;

		if (Section == SECT_data)
			Sym->Value = DataIP;

		if (Section == SECT_bss)
			Sym->Value = BssIP;

		if (isFunction)
			SetCurrentFunction(Sym);
			
		// Dont redefine scope

//#ifdef INCLUDE_JAVAGEN
//		if (!isFunction)
//			if (JavaPass)
//				JavaGen_Label(Name, Sym->Value, Section);
//#endif

		// Write ref to data label array

		if (Section == SECT_code)
			WriteCodeArray(Sym);
		
		if (Section == SECT_data)
			ArraySet(&LabelArray, DataIP, (int) Sym);

		if (Section == SECT_bss)
			ArraySet(&LabelArray, BssIP + MaxDataIP, (int) Sym);
 
		return;
	}

	sym.Section		= section_Enum;
	sym.Type		= Section;
	sym.LocalScope	= LocalScope;
	sym.LabelType	= isFunction;
	sym.EndIP		= 0;
	sym.Flags		= 0;
	sym.Value		= CodeIP;
	sym.Params		= Function_Param_Count;
	sym.RetType		= Function_Return_Type;

	if (Section == SECT_data)
		sym.Value = DataIP;

	if (Section == SECT_bss)
		sym.Value = BssIP;

	Sym = StoreSymbol(&sym,Name);
	
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

	if (Pass != 1)
		return;

	Sym = FindSymbols(Name,section_Enum,section_Enum, 0);
	
	if (Sym)
	{
		Error(Error_Skip, "Symbol '%s' is already defined",Name);
	}

	sym.Section		= section_Enum;
	sym.Value		= LocalScope;		// Save Local scope of variable
	sym.Type		= 0;
	sym.LocalScope	= 0;				// Store in scope 0 (Global Scope)

	StoreSymbol(&sym,Name);
}

//****************************************
//
//****************************************

void AddLabel(char *label, int global)
{
	strcpy(Name, label);
	
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
	int oldSect = Section;
	int n;

	CDtorStart = DataIP;

	Section = SECT_data;
	
	// Write CTORS Array

	AddAlignRef(4);
	AddLabel("__global_ctor_chain", 1);
		
	for (n=0;n<CtorCount;n++)
	{
		ref = (SYMBOL *) ArrayGet(&CtorArray, n);
		WriteLongRef(ref->Value, ref);
	}

	WriteLong(0);							// Terminate table

	AddLabel("__global_ctor_term", 1);		// Mark terminator
	WriteLong(0);							// Terminate table

	// Write DTORS Array

	AddAlignRef(4);
	AddLabel("__global_dtor_chain", 1);
		
	for (n=0;n<DtorCount;n++)
	{
		ref = (SYMBOL *) ArrayGet(&DtorArray, n);
		WriteLongRef(ref->Value, ref);
	}

	WriteLong(0);							// Terminate table

	AddLabel("__global_dtor_term", 1);		// Mark terminator
	WriteLong(0);							// Terminate table

	CDtorEnd = DataIP;

	Section = oldSect;
}

