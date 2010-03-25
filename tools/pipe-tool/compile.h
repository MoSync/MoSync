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
//				          VMGP (TM) Main Header File
// 					  Written by A.R.Hartley & A.Norlander
//			This code is based on the GameTasker v2.0 Module System
//*********************************************************************************************

//#define USE_ZLIB

//***************************************
//			  Includes
//***************************************

//#ifdef _STD_INCLUDES_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <setjmp.h>

#ifdef USE_ZLIB
#include <zlib.h>
#else
#include "FreeImage.h"
#endif

#ifndef _MSC_VER
#include <unistd.h>
#endif	//_MSC_VER

#ifdef _MSC_VER
#define inline
#endif

//#endif

//***************************************
//		  Extern C Declarations
//***************************************

#ifdef __cplusplus
#error this file must be compiled as C, not C++.
extern "C" {
#undef extern_c
#define extern_c extern "C"
#endif

//***************************************
// 		 Global header file
//***************************************

#ifdef __DEFGLOBALS__
	#define dec(a) a;
	#define decset(a,b) a = b;
#else
	#define dec(a) extern a;
	#define decset(a,b) extern a;
#endif

//****************************************
//			Extra Types
//****************************************

typedef unsigned char	uchar;
typedef unsigned short	ushort;
typedef unsigned int	uint;

//****************************************
//		Code Modules in development
// !! Please comment out in release !!
//****************************************

#define CODE_ANALYSIS
#define INCLUDE_DEPEND_SEARCH
#define INCLUDE_CODE_REBUILD
#define INCLUDE_JAVA_REBUILD
#define INCLUDE_BREW_REBUILD

//#define INCLUDE_JAVAGEN
#define INCLUDE_INSTRUCTION_ELIM


//****************************************
//	  Code Modules in use (Leave in)
//****************************************

#define INCLUDE_ARRAY_CLASS

//****************************************
//			Error Types
//****************************************

enum
{
	Error_Exit	= 0,
	Error_Fatal,
	Error_Continue,
	Error_Skip,
	Error_Warning,
	Error_System
};

enum
{
	ErrorMode_null = 0, 
	ErrorMode_Full, 
	ErrorMode_Basic
};

//****************************************
//			Linker flag types
//****************************************

enum
{
	LINK_null = 0,			// End of the constand pool marker
	LINK_internal,			// An internal label
	LINK_unresolved,		// An unresolved label
	LINK_global,			// A global label
	LINK_jumpdata,			// A int word that contains an label
	LINK_word,				// One or more shorts 
	LINK_long,				// One or more int words
	LINK_const,				// A constant
	LINK_import				// Import file
};

//****************************************
//			Section types
//****************************************

enum
{
	SECT_null = 0,
	SECT_data,
	SECT_code,
	SECT_bss,
	SECT_res,
	SECT_ext
};

enum
{
	EXP_numeric = 0,
	EXP_data,
	EXP_code,
	EXP_bss,
	EXP_ext
};

enum
{
	RET_null = 0,
	RET_void,
	RET_int,
	RET_float,
	RET_double
};

//****************************************
// 			Register enums
//****************************************

enum
{
	REG_zero = 0,
	REG_sp,
	REG_rt,
	REG_fr,
	REG_d0,
	REG_d1,
	REG_d2,
	REG_d3,
	REG_d4,
	REG_d5,
	REG_d6,
	REG_d7,
	REG_i0,
	REG_i1,
	REG_i2,
	REG_i3,

	REG_r0,
	REG_r1,
	REG_r2,
	REG_r3,
	REG_r4,
	REG_r5,
	REG_r6,
	REG_r7,
	REG_r8,
	REG_r9,
	REG_r10,
	REG_r11,
	REG_r12,
	REG_r13,
	REG_r14,
	REG_r15
};

//****************************************
//		 Instruction descripters
//****************************************
enum
{
	_NOP = 0,
	_PUSH,
	_POP,
	_CALL,
	_CALLI,
	_LDB,
	_STB,
	_LDH,
	_STH,
	_LDW,
	_STW,
	_LDI,
	_LDR,
	_ADD,
	_ADDI,
	_MUL,
	_MULI,
	_SUB,
	_SUBI,
	_AND,
	_ANDI,
	_OR,
	_ORI,
	_XOR,
	_XORI,
	_DIVU,
	_DIVUI,
	_DIV,
	_DIVI,
	_SLL,
	_SLLI,
	_SRA,
	_SRAI,
	_SRL,
	_SRLI,
	_NOT,
	_NEG,
	_RET,	
	_JC_EQ,
	_JC_NE,
	_JC_GE,
	_JC_GEU,
	_JC_GT,
	_JC_GTU,
	_JC_LE,
	_JC_LEU,
	_JC_LT,
	_JC_LTU,
	_JPI,
	_JPR,
	_XB,
	_XH,
	_SYSCALL,
	_CASE,
	_FAR,
	_ENDOP
};

//****************************************
//		  Module Header Structure
//****************************************

typedef struct _MA_HEAD
{
	int	Magic;				// Header magic
	int	CodeLen;			// Length of code section
	int	DataLen;			// Length of data section

	int	DataSize;			// Prefurred data memory size	
	int	StackSize;		// Stack size
	int	HeapSize;			// Heap size

	int	AppCode;			// App Code (App MIME)
	int AppID;				// App ID

	int	EntryPoint;		// Program entry point
	int	IntLen;
} MA_HEAD;

//****************************************
// 			Opcode Constants
//****************************************

#define fetch_d 1		// rd
#define fetch_a	2		// address
#define fetch_s 4		// rs
#define fetch_i 8		// const imm
#define fetch_j 16		// imm 8
#define fetch_k 32		// syscall imm
#define fetch_f	64		// far address
#define fetch_c	128		// Case

#define fetch_ds	(fetch_d | fetch_s)
#define fetch_dsa	(fetch_d | fetch_s | fetch_a)
#define fetch_dsf	(fetch_d | fetch_s | fetch_f)
#define fetch_dsi	(fetch_d | fetch_s | fetch_i)
#define fetch_di	(fetch_d | fetch_i)
#define fetch_dj	(fetch_d | fetch_j)
#define fetch_dc	(fetch_d | fetch_c)

//****************************************
//			Array Structure
//****************************************

typedef struct
{
	int		type;
	void	*array;
	uint	size;
	uint	lo,hi,ps;
} ArrayStore;


//****************************************
//			Opcode Info
//****************************************

typedef struct
{
	char *str;
	int flags;
	int farflag;
	int op;
	int rd;
	int rs;
	int imm;
	int rip;
	int len;
} OpcodeInfo;

//****************************************
//		  Analyser structure
//****************************************

typedef struct
{
	int regcount[32];
	int regvalue[32];
	int ip;
} AnalyserInfo;

//****************************************
//
//****************************************

typedef struct
{
	char FileName[256];
	int Line;
	int MasterLine;
	char *MasterPtr;
	int IP;
} LineInfo;

//****************************************************************************************
//								Header file for Symbols
//****************************************************************************************
// TODO: maxlen for string sizes seems to be short
#define NAME_MAX 32767

#define SYMMAX_MIN 1024				// Maximum symbols in table
#define SYMMAX (16*64*1024)			// Maximum symbols in table


//#define LABMAX 32					// Maximum symbol length
//#define NAMESPACE 64000			// Maximum memory for name storage

//****************************************
//    Section types in symbol table
//****************************************

enum
{
	label_Local = 0,
	label_Function,
	label_Virtual,
};

//****************************************
//    Section types in symbol table
//****************************************

enum
{
	section_Empty		= 0,				// Empty section
	section_Enum,
	section_SysCall,
	section_Label,
	section_Global,
	section_Resource,
	section_Script,
	section_File,
	section_SLD_File,
	section_Token
};

//****************************************
//
//****************************************

enum
{
	typebits_null		= 0,
	typebits_byte		= 1,
	typebits_half		= 2,
	typebits_word		= 4,
	typebits_space		= 8,
	typebits_string		= 16,
	typebits_comm		= 32,
	typebits_lcomm		= 64
};



//****************************************
//		 	 Symbol Flags
//****************************************

enum
{
	SymFlag_null	= 0,
	SymFlag_Ref		= 1
};

//****************************************
//		  Symbol table structure
//****************************************

typedef struct SYMBOL
{
	char	*Name;			// Symbol name in Symbol buffer
	char	*Interface;		// Syscall interface string (used in script as a string ptr)
	int		Type;			// Symbol type
	int		Value;
	int		EndIP;
	int		Flags;

	short	VirtualIndex;
	short	LocalScope;		// Local Scope of variable
	short	LabelEnum;

	short	Len;			// Length of actual name
	char	Section;		// Symbol table section
	char	LabelType;
	char	Params;			// Parameter count
	char	RetType;		// Return type

//	int		Size;			// size of anything
//	char	*SymLink;		// Ptr to anything
//	int		Extern;
} SYMBOL;

//****************************************
//		 constant table structure
//****************************************

typedef struct _CPE
{
	char	*Name;			// Symbol name in Symbol buffer
	char	Len;			// Length of actual name
	int		Type;			// Symbol type 
	int	Size;			// Size of a value
	int	Section;		// Allocated size including arrays and stuff
	int	Value;			// IP for this label
} CPE;

//****************************************
//          Evaluator Structure
//****************************************

typedef struct
{
	SYMBOL *Sym;			// Pointer to original symbol

	int	Value;			// Constant value or address variable string
	int		Type;			// copy of SYMBOL.flags (used in type conversion)
} EVAL;

//****************************************
//
//****************************************

typedef struct _ModEntry
{
	char	*Name;				// Name of the module

	char	*Data;				// Data area
	int	DataLen;			// Length of data

	char	*Code;				// Code area

	char	*Stack;				// Stack area ( Registers stored here too )
	int	StackLen;			// Length of stack

	char	*Pool;				// Constant pool
	char	**PoolList;			// Constant pool index table

} ModEntry;


//****************************************
//			The Heap Structure
//****************************************

typedef struct
{
	char *addr;
	int size;
	short next;
	short flag;

} MemLink;

typedef struct
{
	MemLink * LinkBase; 
	short	* LinkStack;
	short	LinkBasePos;
	short	LinkMax;
	int	ProtectStart;
	int	ProtectEnd;
} Heap;

typedef struct
{
	char reg;
	char type;
	char mapped;
	char name[7];
} CpuRegEntry;

//***************************************
//
//***************************************

#define HASHFUNC(vv) ((vv * 1103515245) + 12345)

typedef struct
{
	int		data;
	int		next;
} BucketEntry;

typedef struct
{
	int 	first;
	int		last;
} BucketListEntry;


typedef struct
{
	ArrayStore BucketList;
	ArrayStore Buckets;
	uint listSize;
	uint free;
	uint entry;
} BucketArray;


typedef struct
{
	void *entry;	 
	
} TreeEntry;

typedef struct
{
	TreeEntry RootTreeList;
	TreeEntry *up;
	TreeEntry *current;
} TreeArray;

//****************************************
//		  Some useful defines
//****************************************

//#define bypass()			tptr += strlen((char *)tptr) + 1

#ifdef _MSC_VER
#undef iscsymf
#undef iscsym
#undef iswhite
#endif
#define iswhite(c)			(c == ' ' || c == '\t' || c == 0x0d || c == 0x0a)
#define iscsymf(c)			(isalpha(c) || c == '_' || c == '@')
#define iscsym(c)			(isalnum(c) || c == '_' || c == ':' || c == '.' || c == '@')

#define asmsym(c)		(isalnum(c) || c == '_' || c == '@' || c == '.')
#define asmsymf(c)		(isalpha(c) || c == '_' || c == '@')

//****************************************
//
//****************************************

enum
{
	MODEL_null = 0,
	MODEL_full,
	MODEL_half,
	MODEL_compact,
	MODEL_mini,
	MODEL_tiny
};

//****************************************
//
//****************************************

enum
{
	use_rd			= 0x01,
	use_rs			= 0x02,
	use_addr		= 0x04,
	use_int			= 0x08,
	use_int8		= 0x10,
	use_int24		= 0x20
};

//****************************************
//
//****************************************

enum
{
	ResType_null = 0,
	ResType_PlaceHolder = 1,
	ResType_Image = 2,
	ResType_Sprite = 3,
	ResType_Binary = 4,
	ResType_UBinary = 5,
	ResType_Skip = 6,
	ResType_TileSet = 7,
	ResType_TileMap = 8,
	ResType_Label = 9,
//	ResType_Media = 10,
//	ResType_UMedia = 11
};

//****************************************
//
//****************************************

#define CONST_REG_SIZE 128					// 128 or 256
#define CONST_REG_MASK 127

#define ALLOW_DATA_IN_CODE 0
#define ALLOW_DATA_GLOBALS 1


// Absolute maximums

#define MAX_CODE_MEM		(2048 * 1024)
#define MAX_DATA_MEM		(2048 * 1024)

//****************************************
//		 Global Variables
//****************************************

dec(char g_relPath[1024])
dec(char g_BasePath[1024])
decset(char g_Code_EntryPoint[128], "crt0_startup")

decset(int g_CurrentModel, 0)

decset(int g_ArgMasterDump, 0)
decset(int g_ArgVerifierOff, 0)
decset(int g_ArgDebug, 0)
decset(int g_ArgInfo, 0)
decset(int g_ArgList, 0)
decset(int g_ArgLink, 0)
decset(int g_ArgBuild, 0)
decset(int g_ArgDumpFile, 0)
decset(int g_ArgConstOpt, 1)
decset(int g_ArgCustomConfig, 0)
decset(int g_SizeConstOpt, 1)
decset(int g_FarAddressing, 1)
decset(int g_ArgRes, 0)
decset(int g_ArgJavaNative, 0)
decset(int g_ArgBrewGen, 0)
decset(int g_ArgFilePaths, 0)
decset(int g_ArgOptimize, 1)
decset(int g_Do_Elimination, 0)
decset(int g_ArgDebugRebuild, 0)
decset(int g_ArgSkipElim, 0)
decset(int g_ArgSLD, 0)
decset(int g_ArgUseStabs, 0)

dec(char g_SldName[256])
dec(char g_StabsName[256])

decset(int g_ArgUseMasterDump, 0)

decset(short g_AsmMsg, 1)
decset(short g_DebugActive, 0)
decset(short g_AssemActive, 0)

dec(char g_Name[NAME_MAX])

dec(int g_Rebuild_Mode)


dec(char *g_FilePtr)
dec(char *g_FileTop)

dec(int g_RealFileLineTop)

decset(char *g_mosync_dir, 0)

decset(short g_EJmp, 0)
dec(jmp_buf g_ErrorJump)

dec(int g_Default_DataSize)
dec(int g_Default_StackSize)
dec(int g_Default_HeapSize)

dec(int g_Default_AppCode)
dec(int g_Default_AppID)

dec(int g_Do_Dump_Symbols)
dec(int g_Do_Dump_Unref_Symbols)

dec(char g_ProfilePath[256])

dec(int	g_theExitCode)
dec(char *g_appname)
dec(int g_argno)
decset(int g_xerr, 0)
dec(char g_DisasFunc[128])
dec(char g_GcjFlags[1024])

dec(char g_DependName[256])
decset(int g_Do_Export_Dependencies, 0)
decset(FILE *g_DependFile, NULL)

dec(FILE *g_InFile)
dec(FILE *g_CodeFile)
dec(FILE *g_HeaderFile)

dec(short g_DEBUG)
dec(short g_LIST)
dec(short g_INFO)

dec(short g_EndFile)
decset(short g_AllowFarBranches, 0)

decset(short g_Pass, 1)
decset(short g_JavaPass, 0)
decset(short g_Final_Pass, 0)


dec(int g_LocalScope)
dec(int g_MaxEnumLabel)


dec(int	g_rs)
dec(int g_rt)
dec(int g_rd)
dec(int g_imm)
dec(int g_op)
dec(int g_farop)

decset(int g_GlobalsInCode, 0)

decset(int g_Section, SECT_data)

dec(int g_ConstTable[CONST_REG_SIZE])				// Constant table

dec(int	g_pCodeIP)
dec(int	g_pDataIP)
dec(int	g_pBssIP)

decset(int g_pass_count, 0)

decset(SYMBOL *g_CurrentFunction, 0)

dec(char g_CurrentFile[256])
decset(int g_CurrentFileLine, 0)

dec(int g_Function_Param_Count)
dec(int g_Function_Return_Type)

dec(SYMBOL *g_CaseRef)

// VarTables

dec(int	g_VarCount)
decset(int	*g_VarPool, 0)
decset(int	*g_VarFreq, 0)
decset(int	*g_VarTouch, 0)
decset(int	*g_ThisVar, 0)


// Resourcer

dec(int g_CurrentResource)
dec(char g_ResName[512])
decset(int g_ResType, 0)
decset(int g_ResDispose, 0)
dec(int g_IndexTable[32768])
dec(short g_IndexCount)
dec(int g_IndexWidth)

// Eval

dec(char g_xName[NAME_MAX])
dec(int	g_ExpType)
dec(int	g_ExpSection)
dec(int	g_ExpResolved)

//Output

dec(jmp_buf g_ErrorRet)
decset(int g_ErrRetSet, 0)
decset(int g_ErrorCount, 0)
decset(int g_ErrorMode, 0)

// Symbols

dec(SYMBOL *g_SymTab)			// Ptr to Symbol data
dec(int	g_SymbolCount)		// Amount of Symbols
dec(int	g_LastSymCount)

// SysCall

dec(char g_SysCallMap[4096])
decset(int g_LastSysCall, 0)

// Disas

// Demangle

decset(const char *g_last_mangled_name, 0)

// Stabs

dec(char g_StabFilePath[512])
dec(char g_StabFileName[128])
dec(int g_C_Source_Line)


dec(char g_regstr[128])

dec(int g_FileLine)

//****************************************
//			Dynamic Arrays
//****************************************

dec(ArrayStore g_AsmCharArray)
dec(ArrayStore g_AsmCharDataArray)
dec(ArrayStore g_AsmCharIPArray)


dec(ArrayStore g_CallArray)
dec(ArrayStore g_DataArray)
dec(ArrayStore g_DataAccessArray)
dec(ArrayStore g_DataAlignArray)
dec(ArrayStore g_LabelArray)
dec(ArrayStore g_CodeLabelArray)

dec(ArrayStore g_CodeTouchArray)

dec(ArrayStore g_DataTypeArray)
dec(ArrayStore g_DataMixArray)

dec(ArrayStore g_PaddingArray)
dec(ArrayStore g_LabelDone)

dec(ArrayStore g_CtorArray)
dec(ArrayStore g_DtorArray)
dec(int g_CtorCount)
dec(int g_DtorCount)

dec(ArrayStore g_SLD_Line_Array)
dec(ArrayStore g_SLD_File_Array)

dec(ArrayStore g_RebuildArray)

dec(ArrayStore g_CodeMemArray)
dec(int	g_CodeIP)

dec(ArrayStore g_DataMemArray)
dec(int	g_DataIP)
dec(int	g_BssIP)
dec(int	g_MaxDataIP)
decset(int g_CDtorStart, 0)
decset(int g_CDtorEnd, 0)

dec(ArrayStore g_ResMemArray)
dec(int	g_ResIP)

dec(ArrayStore g_StabsArray)
decset(int g_ExpIsPure, 0)

//****************************************
//			Special globals
//****************************************

#ifdef __DEFGLOBALS__
char *regsyms[] = {	"zr","sp","rt","fr","d0","d1","d2","d3",
					"d4","d5","d6","d7","i0","i1","i2","i3",
					"r0","r1","r2","r3","r4","r5","r6","r7",
					"r8","r9","r10","r11","r12","r13","r14","r15"
				  };

#else
extern char *regsyms[];
#endif

//********************************************************************************
//							Java Class Loader Stuff
//********************************************************************************

//****************************************
// 
//****************************************

enum
{
	C_Utf8 = 1,
	C_Unicode,
	C_Integer,
	C_Float,
	C_Long,
	C_Double,
	C_Class,
	C_String,
	C_Fieldref,
	C_Methodref,
	C_InterfaceMethodref,
	C_NameAndType
};

//****************************************
//				Structures
//****************************************

// The contents of my tables

typedef uchar *ConstantData;

typedef struct
{
	ushort	*fieldInfoP;
	int		offset;				// in bytes
	int		size;				// in bytes
	uchar	*type;				// ptr to sig constant
} FieldData;

typedef struct
{
	ushort	*methodInfoP;
	ushort	*codeAttrP;
	uchar	*codeP;
	uint	paramCount;		// in bytes
} MethodData;

//****************************************
//
//****************************************

typedef struct
{
	uchar	*ClassFilePtr;

	short	Major_version;
	short	Minor_version;

	long	*PoolPtr;
	short	PoolCount;
	
	short	AccessFlags;
	short	ThisClass;
	short	SuperClass;

	short	InterfaceCount;

	unsigned char *FieldsFPos;
	short	FieldsCount;

	unsigned char *MethodsFPos;
	short	MethodsCount;
	
} ClassEntry;

//****************************************
//
//****************************************

typedef struct
{
	uchar		Type;
	short		Value;
	short		Index;
	float		fValue;
	double		dValue;
	long long	llValue;
	char		String[512];
} PoolInfo;

//****************************************
//			Class Constants
//****************************************

#define	JAVAMAGIC					0xCafeBabe
#define	MAJOR_VERSION				45
#define	MINOR_VERSION				3

#define	CONSTANT_Utf8				1
#define	CONSTANT_Unicode			2
#define	CONSTANT_Integer			3
#define	CONSTANT_Float				4
#define	CONSTANT_Long				5
#define	CONSTANT_Double				6
#define	CONSTANT_Class				7
#define	CONSTANT_String				8
#define	CONSTANT_Fieldref			9
#define	CONSTANT_Methodref			10
#define	CONSTANT_InterfaceMethodref	11
#define	CONSTANT_NameAndType		12

#define	CONSTANT_LongC				129
#define	CONSTANT_DoubleC			130

#define	CONSTANT_Unknown			0
#define CONSTANT_ResolvedString		(16+CONSTANT_String)
#define CONSTANT_ResolvedClass		(16+CONSTANT_Class)

//****************************************
//
//****************************************

#define memL(m)		(((m)[0] << 24) | ((m)[1] << 16)| ((m)[2] << 8) | (m)[3])
#define memW(m)		(((m)[0] << 8) | (m)[1])
#define memB(m)		((m)[0])

#define memLI(m)	(((m)[0] << 24) | ((m)[1] << 16)| ((m)[2] << 8) | (m)[3]);m+=4
#define memWI(m)	(((m)[0] << 8) | (m)[1]);m+=2
#define memBI(m)	((m)[0]);m+=1

//#define g_CodePtr g_FilePtr

#define CODE_OFFSET() ((long) CodePtr - (long) CodeStart)

dec(uchar * ClassFilePtr)
dec(uchar *ClassPtr)
dec(ClassEntry mainClass)

//****************************************
//				Enums
//****************************************

#define INSTRDEF(str, sym, flag, len, perm) sym,

enum 
{
#include "Bytecode.h"
};

#undef INSTRDEF

//********************************************************************************

//****************************************
//				Prototypes
//****************************************

#include "PBProto.h"

//****************************************
// 		 Build token enum table
//****************************************

#undef TOKENTAB
#undef TOKENEQL

#define TOKENTAB(str, num, equal)	num,
#define TOKENEQL(str, num, equal)	num = equal,

enum
{
	tok_null = 0,
#include "tokentable.h"
	tok_end
};	   

//****************************************
// 		 Build token string table
//****************************************

#ifdef __DEFGLOBALS__

#undef TOKENTAB
#undef TOKENEQL

#define TOKENTAB(str, num, equal)	str,
#define TOKENEQL(str, num, equal)

char *TokenTableStrings[] =
{
	"null",
#include "tokentable.h"
};	   

#else
extern char *TokenTableStrings[];
#endif
