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
//				    PIP-e II Assembler Symbol Routines
// 						   Written by A.R.Hartley
//			This code is based on the GameTasker v2.0 Module System
//*********************************************************************************************

#include "compile.h"

#define USE_HASHING

//****************************************
//
//****************************************

char *Sect_Strings[] =
{
	"0",
	"DS",
	"CS",
	"BS",
	"RS",
	"CT",
	"DT",
	"EX",
	"1",
	"2",
	"3",
	"4"
};

//****************************************
//	Initialises Symbol table to null.
//****************************************

#define SYMBOL_HASH_SIZE 32749			// Prime

#ifdef USE_HASHING
BucketArray SymbolHash;
#endif

SYMBOL *NextFreeSym;
int NextSymbolCount;


int InitSymbolTable(void)
{
	register SYMBOL *Sym;
	register int n;

	SymTab = (SYMBOL *) NewPtr(sizeof(SYMBOL) * (SYMMAX + 10));

	if (!SymTab)
		return 0;
	
	Sym = SymTab;
	n = SYMMAX;

	do
	{
		Sym->Section	= 0;
		Sym->Name		= NULL;	
		Sym->LocalScope	= 0;
		Sym->LabelType	= 0;
		Sym->VirtualIndex = 0;
		Sym->EndIP		= 0;
		Sym->Flags		= 0;
		Sym->Params		= 0;
		Sym->RetType	= 0;
		Sym->LabelEnum	= 0;
		Sym->Interface	= 0;
		Sym++;
	}
	while(--n);

#ifdef USE_HASHING
	BucketArrayInit(&SymbolHash, SYMBOL_HASH_SIZE);
#endif

	NextFreeSym = SymTab;
	NextSymbolCount = 0;

	SymbolCount = 0;	
	return 1;
}

//****************************************
//	Initialises Symbol table to null.
//****************************************

void CloseSymbolTable(void)
{

#ifdef USE_HASHING
	BucketArrayDispose(&SymbolHash);
#endif

	DisposeSymbols();

	if (SymTab)
		DisposePtr((char *) SymTab);
		
	SymTab = 0;
}


//****************************************
//		 Find a free Symbol slot
//****************************************

void DisposeSymbols()
{
	int	n;
	SYMBOL *Sym = SymTab;

	n = SYMMAX;
	
	do
	{
		DelSymbol(Sym);
		Sym++;
	}
	while(--n);

	return;
}


//****************************************
//		 Find a free Symbol slot
//****************************************

SYMBOL * FreeSymbol(void)
{
	SYMBOL *Sym = NextFreeSym;

	NextFreeSym++;
	NextSymbolCount++;

	if (NextSymbolCount >= SYMMAX)
		Error(Error_Fatal, "Out of symbol memory!!");

	return Sym;

/*	int	n;
	SYMBOL *Sym = SymTab;

	n = SYMMAX;
	
	do
	{
		if (Sym->Section == 0 )
		{
			return Sym;
		}

		Sym++;
	}
	while(--n);

	Error(Error_Fatal, "Out of symbol memory!!");
	return NULL;
*/
}

//****************************************
//		 Find a free Symbol slot
//****************************************

void ClearSymbolFlags(int bits)
{
	SYMBOL *Sym = SymTab;
	int	n;
	int mask = ~bits;
	
	n = SYMMAX;
	
	do
	{
		Sym->Flags &= mask;
		Sym++;
	}
	while(--n);

	return;
}

//****************************************
//		  get a Symbol slot
//****************************************
/*
SYMBOL * GetSymbol(int num)
{
	SYMBOL *Sym;

	if (num > SYMMAX)
		return NULL;
	
	Sym = &SymTab[num];
	
	if (Sym->Section == 0 )
		return NULL;

	return Sym;
}
*/
//****************************************
//			  Hash Symbol
// Returns the entry point into hash table
//****************************************

//#define HASHFUNC(vv) ((vv * 1103515245) + 12345)

int HashSymbol(char *string, int scope, int section)
{
	uint v, n;
	uint len;
	
	len = strlen(string);
	
	v = 41;
	v += HASHFUNC(scope);
	v += HASHFUNC(section);
	v += HASHFUNC(len);

	for (n=0;n<len;n++)
		v += HASHFUNC(string[n]);

	return v % SYMBOL_HASH_SIZE;
}

//****************************************
//  *Symbol	FindSymbols (Ptr string)
//
//	Trys to find the Symbol at *string 
// (ASCZ terminated) returns a Ptr to a
//  Symbol containing the Symbol data.
// if NULL returned then the Symbol
// was not found.
//****************************************

#ifdef USE_HASHING

SYMBOL * FindSymbolsOld(char *string,int sectionStart,int sectionEnd)
{
	SYMBOL *Sym;
	int elem, entry;
	int	len;

	len = strlen(string);

	entry = HashSymbol(string, 0, sectionStart);
	elem = -1;

	while(1)
	{
		elem = BucketArraySearch(&SymbolHash, entry, elem, (uint*) &Sym);

		if (Sym == 0)
			return NULL;
			
		if (elem == -1)
			return NULL;

		if ( (Sym->Len == len) && 
			 (Sym->Section != 0)  &&
			 (Sym->Section >= sectionStart) &&
			 (Sym->Section <= sectionEnd) &&
			 (strcmp(string,Sym->Name) == 0)
			)
		{
//			LastSymCount = n;
			return Sym;
		}

		if (elem == 0)
			return NULL;
	}

	return NULL;
}

#else

// WORKING COPY
// Old version

SYMBOL * FindSymbolsOld(char *string,int sectionStart,int sectionEnd)
{
	int	n,len;
	SYMBOL *Sym = SymTab;

	len = strlen(string);

	n=SYMMAX;
	do
	{
		if ( (Sym->Len == len) && 
			 (Sym->Section != 0)  &&
			 (Sym->Section >= sectionStart) &&
			 (Sym->Section <= sectionEnd) &&
			 (strcmp(string,Sym->Name) == 0)
			)
		{
//			LastSymCount = n;
			return Sym;
		}

		Sym++;
	}
	while(--n);

	return NULL;
}

#endif

//****************************************
//  *Symbol	FindSymbols (Ptr string)
//
//	Trys to find the Symbol at *string 
// (ASCZ terminated) returns a Ptr to a
//  Symbol containing the Symbol data.
// if NULL returned then the Symbol
// was not found.
//****************************************

// With file scoping

#ifdef USE_HASHING

SYMBOL * FindSymbols(char *string,int sectionStart,int sectionEnd, int scope)
{
	SYMBOL *Sym;
	int elem, entry;
	int	len;

	len = strlen(string);

	entry = HashSymbol(string, scope, sectionStart);
	elem = -1;

	while(1)
	{
		elem = BucketArraySearch(&SymbolHash, entry, elem, (uint*) &Sym);

		if (Sym == 0)
			return NULL;
			
		if (elem == -1)
			return NULL;

		if ( (Sym->LocalScope == scope) &&
			 (Sym->Len == len) && 
			 (Sym->Section != 0)  &&
			 (Sym->Section >= sectionStart) &&
			 (Sym->Section <= sectionEnd) &&
			 (strcmp(string,Sym->Name) == 0)
			)
			{
//				LastSymCount = n;
				return Sym;
			}

		if (elem == 0)
			return NULL;
	}

	return NULL;
}

#else

// WORKING COPY
// With file scoping
// Old version

SYMBOL * FindSymbols(char *string,int sectionStart,int sectionEnd, int scope)
{
	int	n,len;
	SYMBOL *Sym = SymTab;

	len = strlen(string);

	n=SYMMAX;
	do
	{
		if ( (Sym->LocalScope == scope) &&
			 (Sym->Len == len) && 
			 (Sym->Section != 0)  &&
			 (Sym->Section >= sectionStart) &&
			 (Sym->Section <= sectionEnd) &&
			 (strcmp(string,Sym->Name) == 0)
			)
		{
//				LastSymCount = n;
				return Sym;
		}

		Sym++;
	}
	while(--n);

	return NULL;
}
#endif

//****************************************
//        Find Symbols by value
//****************************************

SYMBOL * ReverseFindSymbols(int v,int sectionStart,int sectionEnd)
{
	int	n;
	SYMBOL *Sym = SymTab;

	n=SYMMAX;
	do
	{
		if ( (Sym->Section != 0)  &&
			 (Sym->Section >= sectionStart) &&
			 (Sym->Section <= sectionEnd) &&
			 (Sym->Value == v)
			)
		{
//			LastSymCount = n;
			return Sym;
		}

		Sym++;
	}
	while(--n);

	return NULL;
}

//****************************************
//  *Symbol	FindNextSymbol (Ptr string)
//
//	Trys to find the next Symbol at *string 
// (ASCZ terminated) returns a Ptr to a
//  Symbol containing the Symbol data.
// if NULL returned then the Symbol
// was not found.
//****************************************
/*
SYMBOL * FindNextSymbol(SYMBOL *Sym,char *string,int sectionStart,int sectionEnd)
{
	int	n,len;
	len = strlen(string);

	n = LastSymCount - 1;
	Sym++;
	
	if (n <= 0)
		return NULL;

	do
	{
		if ( (Sym->Section != 0)  &&
			 (Sym->Len == len) &&
			 (Sym->Section >= sectionStart) &&
			 (Sym->Section <= sectionEnd) &&
			  (strcmp(string,Sym->Name) == 0)
			)
		{
			LastSymCount = n;
			return Sym;
		}

		Sym++;
	}
	while(--n);

	return NULL;
}
*/
//****************************************
//*Symbol	StoreSymbol (Symbol NewSymbol)
//
//	Stores a new symbol in the Symbol table,
//	returns NULL if unsuccessful.
//
//     This storage system is Dynamic
//****************************************

SYMBOL * StoreSymbol(SYMBOL *NewSym,char *string)
{
	SYMBOL *Sym = FreeSymbol();
	char *ThisName;					// Location of PTR to name
	int StringLen;
	int entry;

	StringLen = strlen(string) + 1;	// Get length of string
	
	// if there was not Symbol space quit

	if (Sym == NULL)
			return NULL;

	// if there was no name space quit

	ThisName = (char *) NewPtr((int) StringLen);

	// Copy the name to the NamesList

	memcpy(ThisName,string,StringLen);
	memcpy(Sym,NewSym,sizeof(SYMBOL));

	// Set the Symbol data ptr

	Sym->Name = ThisName;
	Sym->Len = StringLen-1;

	Sym->Flags = 0;

	// Add symbol to hash table

#ifdef USE_HASHING

	entry = HashSymbol(Sym->Name, Sym->LocalScope, Sym->Section);
	BucketArrayAdd(&SymbolHash, entry, (int) Sym);

#endif
	// Carry forward the names pointer

	return Sym;
}

//****************************************
//		   Delete a Symbol entry
//    int DelSymbol(SYMBOL *ThisSym)
//****************************************

int DelSymbol(SYMBOL *ThisSym)
{
	ThisSym->Section = 0;							// Make entry empty

	//OutEval("------ Undeclare '%s'\n",(char *) ThisSym->name);

	if (ThisSym->Name)
		DisposePtr((char *) ThisSym->Name);			// Give back memory

	return 1;										// Say o.k
}

//****************************************
//  Check a symbol not already present
//****************************************
/*
void CheckSym(char *ThisName,int ThisSec,int ThisStart,int ThisEnd,char *ErrorStr)
{
	if (ThisSec >= ThisStart && ThisSec <= ThisEnd)
	{
		if (FindSymbols(ThisName,ThisStart,ThisEnd))
		{
			Error("%s '%s' redeclare",ErrorStr,ThisName);
			ExitApp(1);
		}
	}
}
*/
//****************************************
//			Declare a type
//****************************************
/*
SYMBOL * DeclareEnum(char *ThisName,int Value,short Section)
{
	SYMBOL	sym;
	SYMBOL	*Sym;
		
	Sym = FindSymbols(ThisName,section_Enum,section_Enum);
	
	if (Sym)
	{
		Sym->Size	= Value;			// Revalue enum
		Sym->Type	= Section;
		return	Sym;
	}

	sym.Section		= section_Enum;
	sym.Type		= Section;
	sym.Size		= Value;

	return	StoreSymbol(&sym,ThisName);
}
*/

//****************************************
//			Declare a type
//****************************************
/*
SYMBOL * DefineGlobal(char *ThisName,int Value)
{
	SYMBOL	sym;
	SYMBOL	*theSym;
	
	theSym = FindSymbols(ThisName,section_Global,section_Global, 0);
	
	if (theSym)
	{
		// Allow redefinition
		
		theSym->Size = Value;
		theSym->Type = 0;
		return theSym;
	}

	sym.Section		= section_Global;
	sym.Type		= 0;
	sym.Value		= Value;

	return	StoreSymbol(&sym,ThisName);
}
*/

//****************************************
//			Declare a type
//****************************************
/*
SYMBOL * DefineExtern(char *ThisName)
{
	SYMBOL	sym;
	SYMBOL	*theSym;
	
	theSym = FindSymbols(ThisName,section_Extern,section_Extern, LocalScope);
	
	if (theSym)
		Error("External symbol '%s' has already been defined", ThisName);

	sym.Section		= section_Extern;
	sym.Type		= 0;
	sym.Value		= 0;
	sym.LocalScope	= LocalScope;

	return	StoreSymbol(&sym,ThisName);
}
*/

//****************************************
//			Declare a type
//****************************************

SYMBOL * RedefENum(char *ThisName,int Value)
{
	SYMBOL	sym;
	SYMBOL	*theSym;
	char	str[32];

	// Build string
	
	sprintf(str, "%i", Value);
	
	// see if the symbols exists
	
	theSym = FindSymbolsOld(ThisName,section_Script,section_Script);
	
	if (theSym)
	{
		// Allow redefinition
		
		theSym->Value = Value;
		theSym->Interface = CreateRedefString(str, theSym->Interface);
		return theSym;
	}

	sym.Section		= section_Script;
	sym.Type		= 0;
	sym.Value		= Value;
	sym.LocalScope	= 0;
	sym.Interface	= CreateRedefString(str, 0);

	return	StoreSymbol(&sym,ThisName);
}

//****************************************
//			Declare a type
//****************************************

SYMBOL * RedefENumString(char *ThisName, char *str)
{
	SYMBOL	sym;
	SYMBOL	*theSym;
	
	theSym = FindSymbolsOld(ThisName,section_Script,section_Script);
	
	if (theSym)
	{
		// Allow redefinition
		
		theSym->Interface = CreateRedefString(str, theSym->Interface);

		if (isdigit(*str))
			theSym->Value = atoi(str);

		return theSym;
	}
	
	// Create new string

	sym.Section		= section_Script;
	sym.Type		= 0;
	sym.Value		= 0;
	sym.LocalScope	= 0;
	sym.Interface	= CreateRedefString(str, 0);

	if (isdigit(*str))
		sym.Value = atoi(str);
	
	return	StoreSymbol(&sym,ThisName);
}

//****************************************
//	  Create and Redefine String
//****************************************

char * CreateRedefString(char *str, char *ref)
{
	char *newstr;

	int newlen = strlen(str) + 1;
	int reflen;
	
	// New strings don't have a previous reference, so create new

	if (!ref)
	{
		newstr = NewPtr(strlen(str) + 1);

		// if we could'nt allocate return the old ref

		if (!newstr)
		{
			Error(Error_Fatal, "Could'nt allocate string");
			return ref;
		}

		strcpy(newstr, str);		
		return newstr;
	}
	
	// Must be an existing ref
	
	reflen = strlen(ref) + 1;		// get the current allocated length

	// Do we have a large enough string already ?
	
	if (reflen >= newlen)
	{
		// We don't need to reallocate becuase the existing
		// space is big enough
		
		strcpy(ref, str);
		return ref;
	}

	// ok, we need to reallocate

	newstr = NewPtr(strlen(str) + 1);

	// if we could'nt allocate return the old ref

	if (!newstr)
	{
		Error(Error_Fatal, "Could'nt allocate string");
		return ref;
	}
	
	strcpy(newstr, str);

	// Delete the old ref

	DisposePtr(ref);
	
	return newstr;
}

//****************************************
//			Declare a type
//****************************************

int SymbolExists(char *ThisName, int sect, int scope)
{
	SYMBOL	*theSym;
	
	if (scope != -1)
		theSym = FindSymbols(ThisName, sect, sect, scope);
	else
		theSym = FindSymbolsOld(ThisName, sect, sect);
	
	if (theSym)
		return 1;

	return 0;
}

//****************************************
//			Declare a type
//****************************************

SYMBOL * AddSysCall(char *ThisName, int Value, int Param, int RetType, char *istr)
{
	SYMBOL	sym;
	SYMBOL	*theSym;
	char	*iptr = 0;
	int		len;
		
	theSym = FindSymbolsOld(ThisName,section_SysCall,section_SysCall);
	
	if (theSym)
		Error(Error_Fatal, "SysCall '%s' has already been defined", ThisName);

	if (istr)
	{
		len = strlen(istr);
		iptr = NewPtr(len + 2);
		
		if (!iptr)
			Error(Error_Fatal, "failed to allocate syscall interface");

		strcpy(iptr, istr);
	}

	sym.Section		= section_SysCall;
	sym.Type		= 0;
	sym.Value		= Value;
	sym.LocalScope	= 0;
	sym.Params		= Param;
	sym.RetType		= RetType;
	sym.Interface	= iptr;

	return	StoreSymbol(&sym,ThisName);
}

//****************************************
//			Declare a token
//****************************************

SYMBOL * AddToken(char *ThisName,int Value)
{
	SYMBOL	sym;
	SYMBOL	*theSym;
	
	theSym = FindSymbolsOld(ThisName,section_Token,section_Token);
	
	if (theSym)
		Error(Error_Fatal, "Token '%s' has already been defined", ThisName);

//	printf("Token '%s' = %d\n", ThisName, Value);

	sym.Section		= section_Token;
	sym.Type		= 0;
	sym.Value		= Value;
	sym.LocalScope	= 0;

	return	StoreSymbol(&sym,ThisName);
}

//****************************************
//			Declare a type
//****************************************

SYMBOL * AddFileSym(char *ThisName,int Value)
{
	SYMBOL	sym;
	
	if (Pass != 1)
		return 0;
		
	sym.Section		= section_File;
	sym.Type		= 0;
	sym.Value		= Value;
	sym.LocalScope	= 0;
//	sym.Extern		= 0;

	return	StoreSymbol(&sym,ThisName);
}

//****************************************
//		  Reset SLD File
//****************************************

int Current_SLD_FileNo = 0;

void Init_SLD_Files()
{
	Current_SLD_FileNo = 0;
}

//****************************************
//		  Declare SLD File
//****************************************

int Get_SLD_FileNo(char *ThisName)
{
	SYMBOL	sym;
	SYMBOL	*theSym;
	int n;
	int len;
	
	len = strlen(ThisName);
	
	for (n=0;n<len;n++)
	{
		if (ThisName[n] == '/')
			ThisName[n] = '\\';
	}
	
	theSym = FindSymbolsOld(ThisName,section_SLD_File,section_SLD_File);
	
	if (theSym)
		return theSym->Value;
		
	sym.Section		= section_SLD_File;
	sym.Type		= LocalScope;				// Was 0
	sym.Value		= Current_SLD_FileNo++;
	sym.LocalScope	= 0;

	StoreSymbol(&sym,ThisName);
	return Current_SLD_FileNo - 1;
}

//****************************************
//
//****************************************

SYMBOL *lastsymref = 0;

void ClearLastSymbolRef()
{
	lastsymref = 0;
}

//****************************************
//
//****************************************

void SetLastSymbolRef(SYMBOL *sym)
{
	// Record only the first symbol

	if (!lastsymref)
		lastsymref = sym;
}

//****************************************
//
//****************************************

SYMBOL * GetLastSymbolRef()
{
	return lastsymref;
}

//****************************************
//			Declare a type
//****************************************

int EnumIsFunction()
{
	if (!Final_Pass)
		return 0;

	if (!lastsymref)
		return -1;

	// Check if lable is current function top

	if (CurrentFunction)
	{
		// If it is then say its a lable

		if (CurrentFunction->Value == lastsymref->Value)
			return 0;
	}

	return lastsymref->LabelType;
}

//****************************************
//			Declare a type
//****************************************

int GetEnumValue(char *ThisName)
{
	SYMBOL	*theSym;
	int 	scope;

	ClearLastSymbolRef();

	// Test to see if this symbol is a thunk alias

	if (ThisName[0] == '%')
	{
		int v;
		
		PushTokenPtr(ThisName, 0);

		if (!Token("%"))
			Error(Error_Fatal, "Missing Thunk Delimiter");

		v = GetDecNum(5);

		if (GetDecNumDigitCount() == 0)
			Error(Error_Fatal, "Missing Thunk Number");

		PopTokenPtr();
	
		theSym = GetThunk(v);

		if (!theSym)
		{
			if (Pass == 1)
				return 0;

			printf("Bad thunk!!\n");
			doCannotEvaluate(ThisName+1);
			return 0;
		}
		
		SetLastSymbolRef(theSym);
		return theSym->Value;
	}

	// Now deal with all other symbols
	
	theSym = FindSymbols(ThisName,section_Enum,section_Enum, LocalScope);
		
	if (theSym)			// Variable was found in local scope
	{
		// Add one more reference to this symbol
		
		//theSym->Ref++;

		SetLastSymbolRef(theSym);

		// Save the symbols label type

		//EnumFunction = theSym->LabelType;
		return theSym->Value;
	}
	else
	{
		// Variable wasnt found is local scope, so check global scope
		
		theSym = FindSymbols(ThisName,section_Enum,section_Enum, 0);

		if (theSym)
		{
			// Found a global, so extract the scope of this global
			
			scope = theSym->Value;

			theSym = FindSymbols(ThisName,section_Enum,section_Enum, scope);
		
			if (theSym)
			{
				// Add one more reference to this symbol

				//theSym->Ref++;
				SetLastSymbolRef(theSym);

				// Save the symbols label type

				//EnumFunction = theSym->LabelType;
				return theSym->Value;
			}
		}
	}

	// Dont worry if a global doesnt exist in pass 1

	if (Pass == 1)
		return 0;

	// Do worry in other passes

	doCannotEvaluate(ThisName);
	return 0;
}

//****************************************
//			Get Global Value
//****************************************

int GetGlobalValue(char *ThisName)
{
	SYMBOL	*theSym;
	int 	scope;
	
	// Variable wasnt found is local scope, so check global scope
	
	theSym = FindSymbols(ThisName,section_Enum,section_Enum, 0);

	if (theSym)
	{
		// Found a global, so extract the scope of this global
		
		scope = theSym->Value;

		theSym = FindSymbols(ThisName,section_Enum,section_Enum, scope);
	
		if (theSym)
			return theSym->Value;
	}

	return 0;
}

//****************************************
//			Get Global Value
//****************************************

SYMBOL * GetGlobalSym(char *ThisName)
{
	SYMBOL	*theSym;
	int 	scope;
	
	// Variable wasnt found is local scope, so check global scope
	
	theSym = FindSymbols(ThisName,section_Enum,section_Enum, 0);

	if (theSym)
	{
		// Found a global, so extract the scope of this global
		
		scope = theSym->Value;

		theSym = FindSymbols(ThisName,section_Enum,section_Enum, scope);
	
		if (theSym)
			return theSym;
	}

	return 0;
}

//****************************************
//
//****************************************

void DumpEnumEntry(SYMBOL *Sym)
{
	SYMBOL	*theSym;

	if (!Sym)
	{
		printf("Null Symbol\n");
		return;
	}

#if 0
	if (Sym < SymTab)
		return;
		
	if ((int) Sym >= (int)((int) SymTab + sizeof(SYMBOL) * SYMMAX) )
		return;
#endif

	if ((Sym->Section == section_Enum) && (Sym->LocalScope != 0))
	{				
		if ((Sym->Flags & SymFlag_Ref) == 0)
			printf("*Unref* ");

		theSym = FindSymbols(Sym->Name,section_Enum,section_Enum, 0);

		if (theSym)
			printf("Global %2d: ",Sym->LocalScope);
		else
			printf("Local  %2d: ",Sym->LocalScope);

		printf("%2s ", Sect_Strings[Sym->Type]);

		printf("0x%s ",Hex32(Sym->Value));
		printf("- 0x%s ",Hex32(Sym->EndIP));
						
		printf("'%s'",Sym->Name);
		
		switch(Sym->LabelType)
		{
			case label_Local:
			printf(" (Local)");
			break;

			case label_Function:
			printf(" (Function)(%d)", Sym->Params);
			break;
			
			case label_Virtual:
			printf(" (Virtual Function 0x%x)(%d)", Sym->VirtualIndex, Sym->Params);
			break;
		}

		printf("\n");

	}
}


//****************************************
//			Dump Symbol Table
//****************************************

void DumpGlobalSymbol(char *name)
{
	SYMBOL * sym = GetGlobalSym(name);

	if (!sym)
		return;

	DumpEnumEntry(sym);
}

//****************************************
//			Dump Symbol Table
//****************************************

void DumpSymbols(int unref_only)
{
	SYMBOL	*Sym;

	int		n;

	printf("Symbol table\n\n");

	// Dump locals

	printf("Globals/Locals\n\n");

	Sym = SymTab;
	n = SYMMAX;

	do
	{
		if (unref_only == 1)
		{
			if ((Sym->Flags & SymFlag_Ref) == 0)
			{
				DumpEnumEntry(Sym);
			}
		}
		else
		{
			DumpEnumEntry(Sym);
		}
		
		Sym++;
	}
	while(--n);

	// Dump files
/*
	printf("\nFiles\n\n");

	Sym = SymTab;
	n = SYMMAX;
	do
	{
		if (Sym->Section == section_File)
		{				
				printf("Scope %2d ",Sym->Value);
				printf("File '%s' ",Sym->Name);
				printf("\n");
		}

		Sym++;
	}
	while(--n);
*/
	return;
}


//****************************************
//			Dump Symbol Table
//****************************************

int		*vtable;

void SortVirtuals()
{
	SYMBOL	*Sym;
	int		n, i, vCount;
	
	vCount = GetVirtualIndex();

	if (!vCount)
		return;

//	printf("\nVIRTUALS (%d)\n\n", vCount);
	
	vtable = (int *) NewPtrClear((vCount+2) * sizeof(int *));

	if (!vtable)
		return;

	Sym = SymTab;
	n = SYMMAX;

	do
	{
		if ((Sym->Section == section_Enum) && (Sym->LocalScope != 0))
		{				
			if (Sym->LabelType == label_Virtual)
			{
				i = Sym->VirtualIndex & 0xffff;
				vtable[i] = (int) Sym;
			}
		}

		Sym++;
	}
	while(--n);

/*	for (n=0;n<vCount;n++)
	{
		Sym = (SYMBOL *) vtable[n];

		if (print)
			DumpEnumEntry(Sym);
	}
*/
	return;
}

//****************************************
//
//****************************************

SYMBOL * FetchVirtual(int n)
{
	if (n < 0)
		return 0;

	if (n >= GetVirtualIndex())
		return 0;

	return (SYMBOL *) vtable[n];
}

//****************************************
//			Dump Symbol Table
//****************************************

int CountUsedSymbols()
{
	SYMBOL	*Sym;
	int		SymCount = 0;
	int		n;

	Sym = SymTab;
	n = SYMMAX;

	do
	{
		if (Sym->Section)
			SymCount++;
	
		Sym++;
	}
	while(--n);

	return SymCount;
}


//****************************************
//		Dump IP Translation table
//****************************************

char * GetFileNumString(int file)
{
	SYMBOL	*Sym;
	int		n;

	Sym = SymTab;
	n = SYMMAX;

	do
	{
		if (Sym->Section == section_SLD_File)
		{
			if (Sym->Value == file)
				return Sym->Name;
		}
	
		Sym++;
	}
	while(--n);

	return 0;
}

char * GetFileIdString(int id)
{
	SYMBOL	*Sym;
	int		n;

	Sym = SymTab;
	n = SYMMAX;

	do
	{
		if (Sym->Section == section_SLD_File)
		{
			if (Sym->Type == id)
				return Sym->Name;
		}
	
		Sym++;
	}
	while(--n);

	return 0;
}

//****************************************
//		Dump IP Translation table
//****************************************

void DumpIPTrans()
{
	SYMBOL	*Sym;
	SYMBOL  *LastSLDSym = NULL;
	FILE	*SldFile = 0;
	int		line, file;
	uint		n;
	int MustConvertPaths = 0;

	Sym = SymTab;
	n = SYMMAX;

	SldFile = fopen("Sld.tab", "w");

	if (!SldFile)
	{
		printf("Failed to create source line file'\n");
		return;
	}

	fprintf(SldFile, "Files\n");


	do
	{
		if (Sym->Section == section_SLD_File)
		{
			char *SymName = Sym->Name;
			size_t i, j = 0, len;
			char temp[256];

			if(!MustConvertPaths) {
				if(LastSLDSym && LastSLDSym->Type > Sym->Type)
				MustConvertPaths = 1;
				LastSLDSym = Sym;
			}
		
			if(MustConvertPaths) {
				char *ParentPath = GetFileIdString(Sym->Type);
				GetRelPath(ParentPath); 
				len = strlen(Sym->Name);
#define IS_SLASH(c) ((c)=='/' || (c)=='\\')
				if((len>2 && Sym->Name[1]==':' && IS_SLASH(Sym->Name[2])) || (len>0 && IS_SLASH(Sym->Name[0]))) {
					
				} else {
					SymName = AddRelPrefix(Sym->Name);
				}
			}

			len = strlen(SymName);
			// filter out bad slashes...
			for(i = 0; i < len; i++) {

				if(IS_SLASH(SymName[i])) {
					if(i+1<len-1 && IS_SLASH(SymName[i+1])) {
						i++;
					}
					temp[j++] = '/';
				} else 
					temp[j++] = SymName[i];
			}

			temp[j] = 0;

			fprintf(SldFile, "%d:%d:%s\n", Sym->Value, Sym->Type, temp);
		}
	
		Sym++;
	}
	while(--n);

	fprintf(SldFile, "SLD\n");
	
	if (!SLD_Line_Array.array)
		return;

	for (n=SLD_Line_Array.lo;n<SLD_Line_Array.hi+1;n++)
	{
	
		line = ArrayGet(&SLD_Line_Array, n);
		file= ArrayGet(&SLD_File_Array, n);

		if (line)
		{
			fprintf(SldFile, "%x:%d:%d\n", n, line, file);
		}
	}

	DumpFunctions(SldFile);
	DumpVariables(SldFile);
	//DumpAllSyms(SldFile);
	Dump_MasterIP_Trans(SldFile);

	fprintf(SldFile, "END\n");
	
	fclose(SldFile);
	return;
}


//****************************************
//		Dump Function Table
//****************************************

void DumpFunctions(FILE *out)
{
	SYMBOL	*Sym;

	int		n;

	fprintf(out, "FUNCTIONS\n");

	Sym = SymTab;
	n = SYMMAX;

	do
	{
		if (((Sym->LabelType == label_Function) || (Sym->LabelType == label_Virtual))
			//&& ((Sym->Section == section_Label) || (Sym->Section == section_Global)))
			//&& Sym->EndIP != 0)
			&& (Sym->Section == section_Enum))
		{
			fprintf(out, "%s ",Sym->Name);
			fprintf(out, "%s,",Hex32(Sym->Value));
			fprintf(out, "%s",Hex32(Sym->EndIP));
			fprintf(out, "\n");
		}

		Sym++;
	}
	while(--n);

	fprintf(out, "CDTOR ");
	fprintf(out, "%s,",   Hex32(CDtorStart));
	fprintf(out, "%s\n",  Hex32(CDtorEnd));
	return;
}

//****************************************
//		Dump Variable Table
//****************************************

void DumpVariables(FILE *out)
{
	SYMBOL *Sym;

	int n;

	fprintf(out, "VARIABLES\n");

	Sym = SymTab;
	n = SYMMAX;

	do
	{
		//if (Sym->Section == section_Global)
		if(Sym->LabelType == label_Local && Sym->Section == section_Enum &&
			(Sym->Type == SECT_data || Sym->Type == SECT_bss) &&
			strchr(Sym->Name, '.') == 0)	//HACK
		{
			int value = Sym->Value;

			if(Sym->Type == SECT_bss)
			{
				value += MaxDataIP;
			}

			fprintf(out, "%s %i ",Sym->Name, Sym->LocalScope);
			fprintf(out, "%s",Hex32(value));
			fprintf(out, "\n");
		}

		Sym++;
	}
	while(--n);

	return;
}

void DumpAllSyms(FILE *out)
{
	SYMBOL *Sym;

	int n;

	fprintf(out, "ALL_SYMS\n");

	Sym = SymTab;
	n = SYMMAX;

	do
	{
		{
			fprintf(out, "Ltype %i,sec %i,type %i", Sym->LabelType, Sym->Section, Sym->Type);
			fprintf(out, " %s ",Sym->Name);
			fprintf(out, "%s,",Hex32(Sym->Value));
			fprintf(out, "%s",Hex32(Sym->EndIP));
			fprintf(out, "\n");
		}

		Sym++;
	}
	while(--n);

	return;
}

//****************************************
//		Dump IP Translation table
//****************************************

#if 0

void Dump_MasterIP_Trans(FILE *out)
{
	char	*tptr;
	char	*ptr;
	uint	ip,line;

	tptr = FileTop;
	line = 0;

	fprintf(out, "\nLINEIP\n");
	
	if (!AsmCharArray.array)
		return;

	for (ip=AsmCharArray.lo;ip<AsmCharArray.hi+1;ip++)
	{
		// n = ip

		ptr = (char *) ArrayGet(&AsmCharArray, ip);

		if (ptr)
		{
			if (tptr != ptr)
				line = GetLineNumber(FileTop, ptr);

			fprintf(out, "%d:%x\n", line, ip);
			tptr = ptr;
		}
	}

	return;
}

#else


void Dump_MasterIP_Trans(FILE *out)
{
	char	c;
	uint	offset, line, ip;

	fprintf(out, "\nLINEIP\n");

	offset = 0;
	line = 1;

	if (!AsmCharArray.array)
		return;

	while(1)
	{
		c = FileTop[offset];
					
		if (c == 0x00)
			break;

		if (c == 0x0d)
			line++;

		ip = ArrayGet(&AsmCharIPArray, offset);

		if (ip)
			fprintf(out, "%d:%x\n", line, ip-1);

		offset++;
	}

	return;
}


#endif
//****************************************
//
//****************************************
/*
void DumpEnumEntry(SYMBOL *Sym)
{
	SYMBOL	*theSym;


	if ((Sym->Section == section_Enum) && (Sym->LocalScope != 0))
	{				
		if ((Sym->Flags & SymFlag_Ref) == 0)
			printf("*Unref* ");

		theSym = FindSymbols(Sym->Name,section_Enum,section_Enum, 0);

		if (theSym)
			printf("Global %2d: ",Sym->LocalScope);
		else
			printf("Local  %2d: ",Sym->LocalScope);

		printf("%2s ", Sect_Strings[Sym->Type]);

		printf("0x%s ",Hex32(Sym->Value));
		printf("- 0x%s ",Hex32(Sym->EndIP));
						
		printf("'%s'",Sym->Name);
		
		switch(Sym->LabelType)
		{
			case label_Local:
			printf(" (Local)");
			break;

			case label_Function:
			printf(" (Function)(%d)", Sym->Params);
			break;
			
			case label_Virtual:
			printf(" (Virtual Function 0x%x)(%d)", Sym->VirtualIndex, Sym->Params);
			break;
		}

		printf("\n");

	}
}
*/

