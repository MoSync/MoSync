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
//					       			MoSync Stabs Parser
//*********************************************************************************************

#include "compile.h"

//int StabsEnabled = 0;

#define N_LSYM 0x80		//automatic var in the stack (also used for type desc.)

//****************************************
// 				Stuff
//****************************************

/*

.stabs "string",type,0,desc,value
.stabn          type,0,desc,value
.stabd          type,0,desc


Table A lists stab types sorted by type number. Stab type numbers are 32 and greater.
This is the full list of stab numbers, including stab types that are used in languages
other than C. 

The #define names for these stab types are defined in: devo/include/aout/stab.def 

type   type     #define   used to describe
dec    hex      name      source program feature
------------------------------------------------
32     0x20     N_GSYM    global symbol
34     0X22     N_FNAME   function name (for BSD Fortran)
36     0x24     N_FUN     function name or text segment variable for C
38     0x26     N_STSYM   static symbol (data segment w/internal linkage)
40     0x28     N_LCSYM   .lcomm symbol(BSS-seg variable w/internal linkage)
42     0x2a     N_MAIN    Name of main routine (not used in C)
48     0x30     N_PC      global symbol (for Pascal)
50     0x32     N_NSYMS   number of symbols (according to Ultrix V4.0)
52     0x34     N_NOMAP   no DST map for sym (according to Ultrix V4.0)
64     0x40     N_RSYM    register variable
66     0x42     N_M2C     Modula-2 compilation unit
68     0x44     N_SLINE   line number in text segment
70     0x46     N_DSLINE  line number in data segment

72     0x48     N_BSLINE  line number in bss segment
72     0x48     N_BROWS   Sun source code browser, path to .cb file

74     0x4a     N_DEFD    GNU Modula2 definition module dependency

80     0x50     N_EHDECL  GNU C++ exception variable
80     0x50     N_MOD2    Modula2 info "for imc" (according to Ultrix V4.0)

84     0x54     N_CATCH   GNU C++ "catch" clause
96     0x60     N_SSYM    structure of union element
100    0x64     N_SO      path and name of source file 
128    0x80     N_LSYM    automatic var in the stack (also used for type desc.)
130    0x82     N_BINCL   beginning of an include file (Sun only)
132    0x84     N_SOL     Name of sub-source (#include) file.
160    0xa0     N_PSYM    parameter variable
162    0xa2     N_EINCL   end of an include file
164    0xa4     N_ENTRY   alternate entry point
192    0xc0     N_LBRAC   beginning of a lexical block
194    0xc2     N_EXCL    place holder for a deleted include file
196    0xc4     N_SCOPE   modula2 scope information (Sun linker)
224    0xe0     N_RBRAC   end of a lexical block
226    0xe2     N_BCOMM   begin named common block
228    0xe4     N_ECOMM   end named common block
232    0xe8     N_ECOML   end common (local name)

       << used on Gould systems for non-base registers syms >>
240    0xf0     N_NBTEXT  ??
242    0xf2     N_NBDATA  ??
244    0xf4     N_NBBSS   ??
246    0xf6     N_NBSTS   ??
248    0xf8     N_NBLCS   ??


Line Numbers
------------

Directive: 
	.stabn 
Type: 
	N_SLINE 

The start of source lines is represented by the N_SLINE (68) stab type. 

.stabn N_SLINE, NIL, line, address

line is a source line number; address represents the code address for
the start of that source line. 

27 _main:
28 .stabn 68,0,4,LM1
29 LM1:
30      !#PROLOGUE# 0


Procedures
----------

Directive: 
	.stabs 
Type: 
	N_FUN 

Symbol Descriptors: 
f (local), F (global) 
Procedures are described by the N_FUN stab type. The symbol descriptor
for a procedure is `F' if the procedure is globally scoped and `f' if
the procedure is static (locally scoped). 

The N_FUN stab representing a procedure is located immediately following
the code of the procedure. The N_FUN stab is in turn directly followed by
a group of other stabs describing elements of the procedure. These other
stabs describe the procedure's parameters, its block local variables and
its block structure. 

48      ret
49      restore

The .stabs entry after this code fragment shows the name of the procedure
(main); the type descriptor desc (F, for a global procedure); a reference
to the predefined type int for the return type; and the starting address
of the procedure. 

Here is an exploded summary (with whitespace introduced for clarity),
followed by line 50 of our sample assembly output, which has this form: 

.stabs "name:
        desc  (global proc `F')
        return_type_ref  (int)
       ",N_FUN, NIL, NIL,
       address

50 .stabs "main:F1",36,0,0,_main
 
*/

//****************************************
//			 
//****************************************

typedef struct
{
	int type;
	char *name;
} StabSym;


StabSym StabNames[] =
{

	{ 32,  "N_GSYM"  },  // global symbol
	{ 34,  "N_FNAME" },  // function name (for BSD Fortran)
	{ 36,  "N_FUN"   },  // function name or text segment variable for C
	{ 38,  "N_STSYM" },  // static symbol (data segment w/internal linkage)
	{ 40,  "N_LCSYM" },  // .lcomm symbol(BSS-seg variable w/internal linkage)
	{ 42,  "N_MAIN"  },  // Name of main routine (not used in C)
	{ 48,  "N_PC"    },  // global symbol (for Pascal)
	{ 50,  "N_NSYMS" },  // number of symbols (according to Ultrix V4.0)
	{ 52,  "N_NOMAP" },  // no DST map for sym (according to Ultrix V4.0)
	{ 64,  "N_RSYM"  },  // register variable
	{ 66,  "N_M2C"   },  // Modula-2 compilation unit
	{ 68,  "N_SLINE" },  // line number in text segment
	{ 70,  "N_DSLINE"},  // line number in data segment
	{ 72,  "N_BSLINE"},  // line number in bss segment
	{ 72,  "N_BROWS" },  // Sun source code browser, path to .cb file
	{ 74,  "N_DEFD"  },  // GNU Modula2 definition module dependency
	{ 80,  "N_EHDECL"},  // GNU C++ exception variable
	{ 80,  "N_MOD2"  },  // Modula2 info "for imc" (according to Ultrix V4.0)
	{ 84,  "N_CATCH" },  // GNU C++ "catch" clause
	{ 96,  "N_SSYM"  },  // structure of union element
	{ 100, "N_SO"    },  // path and name of source file 
	{ 128, "N_LSYM"  },  // automatic var in the stack (also used for type desc.)
	{ 130, "N_BINCL" },  // beginning of an include file (Sun only)
	{ 132, "N_SOL"   },  // Name of sub-source (#include) file.
	{ 160, "N_PSYM"  },  // parameter variable
	{ 162, "N_EINCL" },  // end of an include file
	{ 164, "N_ENTRY" },  // alternate entry point
	{ 192, "N_LBRAC" },  // beginning of a lexical block
	{ 194, "N_EXCL"  },  // place holder for a deleted include file
	{ 196, "N_SCOPE" },  // modula2 scope information (Sun linker)
	{ 224, "N_RBRAC" },  // end of a lexical block
	{ 226, "N_BCOMM" },  // begin named common block
	{ 228, "N_ECOMM" },  // end named common block
	{ 232, "N_ECOML" },  // end common (local name)
	{ 0,   "END" }   	 // end common (local name)
};


//****************************************
//			 
//****************************************

void Stabs_Reset()
{
	StabFilePath[0] = 0;
	StabFileName[0] = 0;
	
	C_Source_Line = 0;	
	return;
}

//****************************************
//			 
//****************************************

//FILE *testdbg;

int Stabs_Init()
{
	//testdbg = fopen("testdbg.lst", "w");

	ArrayInit(&StabsArray, sizeof(char), 0);
	Stabs_Reset();
	return 1;
}

//****************************************
//			 
//****************************************

void Stabs_Dispose()
{
	//fclose(testdbg);
	Stabs_Write();

	ArrayDispose(&StabsArray);
}

//****************************************
//			 
//****************************************

void Stabs_Write()
{
	ArrayWrite(&StabsArray, "stabs.tab");
}

//***************************************
// 
//***************************************

void StabsEmit(char *Template, ...)
{
	char 	Str[NAME_MAX];
	va_list args;
	int n,len;

	if (!Final_Pass)
		return;
	
	va_start(args,Template);
	vsprintf(Str,Template,args);
	va_end(args);

	len = strlen(Str);
	
	for (n=0;n<len;n++)
		ArrayAppend(&StabsArray, Str[n]);
		
	//fwrite(Str, 1, len, testdbg);
}

//****************************************
//			 Get a identifier
//****************************************

void GetNameDubug()
{
	unsigned int v = 0;
	int c = 0;
	char *NamePtr = Name;
			
	while (1)
	{
		v = *FilePtr++;

		if (v == 0 || v == 13)
			break;

		if (v == ':')
		{
			FilePtr--;
			break;
		}
	
		*NamePtr++ = (char) v;

		if (c++ >= NAME_MAX)
			break;
	}
	
	*NamePtr++ = 0;

	return;
}

//****************************************
//
//****************************************

char *GetStabName(int type)
{

	StabSym *StabPtr = StabNames;
	
	while(1)
	{
		if (StabPtr->type == 0)
			break;

		if (StabPtr->type == type)
			return StabPtr->name;

		StabPtr++;
	}

	return "//";
}

//****************************************
//	  Parse .stabn type,0,desc,value
//****************************************
/*
	.stabn	192,0,0,LBB3-__ZN8BlooMainC2Ev
	.stabn	224,0,0,LBE3-__ZN8BlooMainC2Ev
*/
int Parse_stabn()
{
	int Type,Desc,Value;

	if (!Final_Pass)
	{
		SkipLine();
		return 1;
	}	

	if (!ArgUseStabs)
	{
		SkipLine();
		return 1;
	}	
	
	// Get type
	
	Type = GetExpPure();

	// Skip comma

	NeedToken(",");
	SkipWhiteSpace();

	// Get zero

	NeedToken("0");
	SkipWhiteSpace();

	// Skip comma

	NeedToken(",");
	SkipWhiteSpace();

	// Get Desc
	
	Desc = GetExpPure();
	SkipWhiteSpace();

	NeedToken(",");
	SkipWhiteSpace();

	// Get value
	
	Value = GetExpPure();

	StabsEmit("#%s %d %d\n", GetStabName(Type), Desc, Value);
	return 1;
}

//****************************************
//Parse .stabs "string",type,0,desc,value
//****************************************

int Parse_stabs()
{
	int Type,Desc,Value;
	char str[32*1024];

	if (!Final_Pass)
	{
		SkipLine();
		return 1;
	}

	if (!ArgUseStabs)
	{
		SkipLine();
		return 1;
	}	
	
	// Get string
	
	GetStringName(32*1024-3);
	strcpy(str, Name);

	// Skip comma

	NeedToken(",");
	SkipWhiteSpace();

	// Get type
	
	Type = GetExpPure();

	// Skip comma

	NeedToken(",");
	SkipWhiteSpace();

	// Get zero

	NeedToken("0");

	// Skip comma

	NeedToken(",");
	SkipWhiteSpace();

	// Get Desc
	
	Desc = GetExpPure();

	NeedToken(",");
	SkipWhiteSpace();

	// Get value
	
	Value = GetExpPure();

	// Push the file ptr

	PushTokenPtr(str, 0);
	
	// Set sld name for header files.
	if(Type == 132) {
		SetSLD_Name(Name);
	}

	StabsEmit("%s %d %d '%s'\n", GetStabName(Type), Desc, Value, str);

	PopTokenPtr();
	return 1;
}

//****************************************
//		Get C Source Line Info
//****************************************

void SetCSourceLine(int v)
{
	C_Source_Line = v;
}

//****************************************
//		Get C Source File Info
//****************************************

void SetCSourceFile(char *name, int file)
{
	strcpy(StabFileName, name);

	if (Final_Pass)
		StabsEmit("N_FILE %d 0 '%s'\n", file, StabFileName);
}

//****************************************
//		Get C Source Dir Info
//****************************************

void SetCSourceDir(char *dir)
{
	strcpy(StabFilePath, dir);
}






