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
//			PIP-e II Assembler Expression Evaluation Routines
// 						   Written by A.R.Hartley
//			This code is based on the GameTasker v2.0 Module System
//*********************************************************************************************

#include "compile.h"
#include "demangle.h"
#include "PBProto.h"

//****************************************
//			 Get a Hex Digit
//****************************************

int HexDig(char digit)
{
	digit = tolower(digit);
	
	if (digit >= 'a')
		return (digit - 'a') + 10;

	return (digit - '0');
}


//****************************************
//			 Get a number
//****************************************
/*
char * GetSym()
{
//	unsigned int v = 0;
	char *lab = xName;
		
	while (iscsym(*FilePtr))
		*lab++ = *FilePtr++;
		
	*lab++ = 0;

	return xName;
}
*/

char * GetSym()
{
	unsigned int v = 0;
	char *NamePtr = xName;
	
	while (iscsym(*FilePtr))
	{
		*NamePtr++ = *FilePtr++;

		if (v++ >= NAME_MAX)
			Error(Error_Fatal, "Symbol too int");
	}
	
	*NamePtr++ = 0;

	return xName;
}

//****************************************
//			 Get a string
//****************************************

char * GetStr()
{
	char *lab = xName;
	
	*lab++ = *FilePtr++;
	
	while (*FilePtr != '\"')
		*lab++ = *FilePtr++;

	*lab++ = *FilePtr++;
	*lab++ = 0;

	return xName;
}

//****************************************
//			 Get Hex Number
//****************************************

int GetHexNum(register int digits)
{
	register unsigned int v = 0;
	register char c;

	while (isxdigit(*FilePtr))
	{
		if (!digits)
		{
			Error(Error_Skip, "Hex value too int !");
			ExitApp(1);
		}
		
		c = tolower(*FilePtr++);		
		v <<= 4;
	
		if (isdigit(c))
			v += c - (unsigned char) '0';
		else 
			v += c - (unsigned char) 'a' + 10;

		digits--;

	}

	return (int) v;
}

//****************************************
//			 Get Hex Number
//****************************************

int GetHexByte()
{
	register unsigned int v = 0;
	register char c;

	if (!isxdigit(*FilePtr))
		Error(Error_Skip, "Missing Hex digit !");

	c = tolower(*FilePtr++);		
	v <<= 4;
	
	if (isdigit(c))
		v += c - (unsigned char) '0';
	else 
		v += c - (unsigned char) 'a' + 10;

	if (!isxdigit(*FilePtr))
		Error(Error_Skip, "Missing Hex digit !");

	c = tolower(*FilePtr++);		
	v <<= 4;
	
	if (isdigit(c))
		v += c - (unsigned char) '0';
	else 
		v += c - (unsigned char) 'a' + 10;

	return (int) v;
}

//****************************************
//		    Get Decimal Number
//****************************************

int DigitCount;

int GetDecNum(int digits)
{
	register unsigned int v = 0;

	DigitCount = 0;

	while (isdigit(*FilePtr))
	{
		if (!digits)
		{
			Error(Error_Skip, "Decimal value too int !");
			ExitApp(1);
		}
		
		v = (v * 10) + (*FilePtr++ - 0x30);
		digits--;
		DigitCount++;
	}
	
	return (int) v;
}

//****************************************
//
//****************************************

int GetDecNumDigitCount()
{
	return DigitCount;
}

//****************************************
//		    Get Decimal Number
//****************************************

int GetDecByte()
{
	register unsigned int v = 0;

	if (!isdigit(*FilePtr))
		Error(Error_Skip, "Missing Decimal digit !");

	v = (v * 10) + (*FilePtr++ - 0x30);

	if (!isdigit(*FilePtr))
		Error(Error_Skip, "Missing Decimal digit !");

	v = (v * 10) + (*FilePtr++ - 0x30);

	if (!isdigit(*FilePtr))
		Error(Error_Skip, "Missing Decimal digit !");

	v = (v * 10) + (*FilePtr++ - 0x30);
	
	return (int) v;
}

//****************************************
//		    Get Decimal Number
//****************************************

#define octodigit(x) ((x) >= '0' && (x) <= '7')

int GetOctoByte()
{
	register unsigned int v = 0;

	if (!octodigit(*FilePtr))
		Error(Error_Skip, "Missing Octal digit !");

	while(1)
	{
		v += (*FilePtr++ - '0');

		if (!octodigit(*FilePtr))
			break;

		v <<= 3;		
	}
	
	if (v > 255)
		Error(Error_Skip, "Octal Overflow!");
	
	return (int) v;
}

//****************************************
//		    Get Char Value
//****************************************

int GetCharNum()
{
	unsigned int v = 0;
	int n;
	char c;

	for (n=0;n<4;n++)
	{
		c = *FilePtr;
		
		if (c == '\'')
			return v;

		if ((c == 0x0d) || (c == 0x0a) || (c == 0x00))
			return v;

		v = (v << 8) + c;

		FilePtr++;
	}
	
	return (int) v;
}

//****************************************
//		    Get escape code
//****************************************

int GetEscCode()
{
	if (octodigit(*FilePtr) && *FilePtr != '0')
		return GetOctoByte();

	switch(*FilePtr++)
	{
		case 'n' : return 0x0a;		// 0x0A (LF)
		case 't' : return 0x09;		// 0x09 (TAB)
		case 'f' : return 0x0c;		// 0x0C (FF)
		case 'a' : return 0x07;		// 0x07 (BEL)
		case 'b' : return 0x08;		// 0x08 (BS)
		case 'r' : return 0x0d;		// 0x0D (CR)
		case 'v' : return 0x0b;		// 0x0B	(VT)
		case '0' : return 0x00;		// 0x00 (NUL)

		case '\\' :
			return '\\';

		case '\"' :
			return '\"';

		case '\'' :
			return '\'';

		case '%' :
			return '%';

		case 'x' :
		case 'X' :
			return GetHexByte();

		case 'd' :
		case 'D' :
			return GetDecByte();

		default:
			FilePtr--;
//			Error(Error_Warning, "Bad escape sequence");
			return '\\';
	}

	return 0;
}

//****************************************
//			 Get a number
//****************************************

int GetNum()
{
	register unsigned int v = 0;

	if (Token("'"))			// found «
	{		
		if (Token("\\"))		// Test "\"
		{
			v = GetEscCode();
			NeedToken("\'");
			return v;
		}

		v = GetCharNum();
		NeedToken("\'");
		return v;
	}

	if (*FilePtr == '0' && (*(FilePtr+1) =='x' || *(FilePtr+1) == 'X'))
	{
		FilePtr+=2;
		
		return GetHexNum(16);
	}

	return GetDecNum(16);
}

//****************************************
// 		   Test for an operator
//  if a valid operator was found then
//		   store it in 'ThisOp' (Slow)
//****************************************

#define TestOp(str,len) if(strncmp(FilePtr,str,len) == 0){ ThisOp = str; Oplen = len ;return; }	

#define TestOpChar(chr,str) if (*FilePtr == chr) { ThisOp = str; Oplen = 1; return;}


#define IsOper(str)		!strcmp(ThisOp,str)

#define SkipOper()		FilePtr+=Oplen

char *ThisOp;
int   Oplen;

#if 1

void GetOper()
{
	ThisOp = "";
	Oplen  = 0;

	// Fast char variant

	TestOpChar('+', "+");
	TestOpChar('-', "-");
	TestOpChar('/', "/");
	TestOpChar('*', "*");
	TestOpChar('%', "%");
	TestOpChar('^', "^");
	TestOpChar('!', "!");
	TestOpChar('~', "~");
	TestOpChar('?', "?");

	// slower checks

	TestOp(">>>",3)
	TestOp(">>",2)
	TestOp(">=",2)
	TestOp(">",1)

	TestOp("<<",2)
	TestOp("<=",2)
	TestOp("<",1)

	TestOp("==",2)
	TestOp("=",1)

	TestOp("!=",2)

	TestOp("&&",2)
	TestOp("&",1)

	TestOp("||",2)
	TestOp("|",1)
	return;
}


#else


void GetOper()
{	
	ThisOp = "";
	Oplen  = 0;

//	TestOp("sizeof",6)

	TestOp(">>>",3)

	TestOp(">>",2)
	TestOp("<<",2)

//	TestOp("++",2)
//	TestOp("--",2)

//	TestOp("->",2)

	TestOp("!=",2)
	TestOp("==",2)
	TestOp(">=",2)
	TestOp("<=",2)

	TestOp("&&",2)
	TestOp("||",2)

/*	TestOp("+=",2)
	TestOp("-=",2)
	TestOp("*=",2)
	TestOp("/=",2)
	TestOp("%=",2)
	TestOp("|=",2)
	TestOp("&=",2)
	TestOp("^=",2)
*/
	TestOp("+",1)
	TestOp("-",1)
	TestOp("/",1)
	TestOp("*",1)
	TestOp("%",1)
	TestOp(">",1)
	TestOp("<",1)
	TestOp("=",1)
	
	TestOp("&",1)
	TestOp("|",1)
	TestOp("^",1)
	TestOp("!",1)
	TestOp("~",1)

	TestOp("?",1)
	
	return;
}
#endif
//****************************************
//	  Initialise expression handeler
//****************************************

void InitExpMain()
{
	//lab = Labels;
}

//****************************************
//			Clear out a Part
//****************************************

void ClearPart(EVAL *Part)
{
	Part->Sym = NULL;
	Part->Value = 0;
	Part->Type = 0;				
}

//****************************************
//	   Get numeric expressions only
//****************************************

int CodeRef = 0;
int CodeValue;
char CodeRefName[256];

//****************************************
//	   Get numeric expressions only
//****************************************

// Entry here

int GetExpression()
{
	EVAL Part1;

	ExpType	= EXP_numeric;
	ExpSection = 0;
	ExpResolved = 0;

	CodeRef = 0;
	CodeValue = 0;

	ExpIsPure = 0;

	ClearLastSymbolRef();

	ClearPart(&Part1);
	assign(&Part1);
	SkipWhiteSpace();
	
	if (CodeRef)
		if (CodeValue != Part1.Value)
			Error(Error_Skip, "Illegal function pointer manipulation");
		
	return Part1.Value;
}

//****************************************
//	 Get pure numeric expressions
//	  No function error checking
//****************************************

int GetExpPure()
{
	EVAL Part1;

	ExpType	= EXP_numeric;
	ExpSection = 0;
	ExpResolved = 0;

	CodeRef = 0;
	CodeValue = 0;

	ExpIsPure = 1;

	ClearLastSymbolRef();

	ClearPart(&Part1);
	assign(&Part1);
	SkipWhiteSpace();

	ExpIsPure = 0;
		
	return Part1.Value;
}

//****************************************
//		 Get the expression type
//****************************************

char * GetExpCodeRef()
{
	if (CodeRef)
		return CodeRefName;
		
	return 0;
}

//****************************************
//		 Get the expression type
//****************************************

int GetExpType()
{
	// Return 0 for pure numeric
	// or the section in which the address is.

	return ExpType;
}

//****************************************
//		 Get the expression type
//****************************************

int GetExpSection()
{
	// Return 0 for pure numeric
	// or the section in which the address is.

	return ExpSection;
}

//****************************************
//		 Get the expression type
//****************************************

int GetExpResolved()
{
	// Return -1 for unresolved symbols

	return ExpResolved;
}

//****************************************
//			 MAHandle Assign
//****************************************

#define CopyRType() Part2.RType = Part1->RType

void assign(EVAL *Part1)		/* MAHandle assignments (=) */
{
//	EVAL Part2;

	logor(Part1);
	GetOper();

	if (IsOper("="))
	{
		SkipOper();
		
		Error(Error_Skip, "'=' is not implemented yet");
		ExitApp(1);
	}
}

//****************************************
//
//****************************************

void logor(EVAL *Part1)
{
	EVAL Part2;

	logand(Part1);

	GetOper();

	if (IsOper("||"))
	{
		SkipOper();

		logor(&Part2);

		Part1->Value = (Part1->Value || Part2.Value);
		return;

		return;
	}
}

//****************************************
// 
//****************************************

void logand(EVAL *Part1)				/* MAHandle addition and substraction */
{
	EVAL Part2;

	xor_ev(Part1);

	GetOper();

	if (IsOper("&&"))
	{
		SkipOper();
		logand(&Part2);
		
		Part1->Value = (Part1->Value && Part2.Value);
		return;
	}
}

//****************************************
// 
//****************************************

void xor_ev(EVAL *Part1)				/* MAHandle addition and substraction */
{
	EVAL Part2;

	or_ev(Part1);

	GetOper();

	if (IsOper("^"))
	{
		SkipOper();
		xor_ev(&Part2);
		
		Part1->Value ^= Part2.Value;
		return;
	}
}

//****************************************
// 
//****************************************

void or_ev(EVAL *Part1)				/* MAHandle addition and substraction */
{
	EVAL Part2;

	and_ev(Part1);

	GetOper();

	if (IsOper("|"))
	{
		SkipOper();
		or_ev(&Part2);
		
		Part1->Value |= Part2.Value;
		return;
	}
}

//****************************************
// 
//****************************************

void and_ev(EVAL *Part1)				/* MAHandle addition and substraction */
{
	EVAL Part2;

	GreaterLess(Part1);

	GetOper();

	if (IsOper("&"))
	{
		SkipOper();
		and_ev(&Part2);
		
		Part1->Value &= Part2.Value;
		return;
	}
	
	if (IsOper("!="))
	{
		SkipOper();

		and_ev(&Part2);
		
		Part1->Value = ( Part1->Value != Part2.Value);
		return;
	}	
	
	if (IsOper("=="))
	{
		SkipOper();

		and_ev(&Part2);

		Part1->Value = (Part1->Value == Part2.Value);
		return;
	}
	
}

//****************************************
// 
//****************************************

void GreaterLess(EVAL *Part1)
{
	EVAL Part2;

	shifts(Part1);

	GetOper();

	if (IsOper(">"))
	{
		SkipOper();

		GreaterLess(&Part2);
		
		Part1->Value = (Part1->Value > Part2.Value);
		return;
	}

	if (IsOper("<"))
	{
		SkipOper();

		GreaterLess(&Part2);
		
		Part1->Value = (Part1->Value < Part2.Value);
		return;
	}

	if (IsOper(">="))
	{
		SkipOper();

		GreaterLess(&Part2);
		
		Part1->Value = (Part1->Value >= Part2.Value);
		return;
	}

	if (IsOper("<="))
	{
		SkipOper();

		GreaterLess(&Part2);
		
		Part1->Value = (Part1->Value <= Part2.Value);
		return;
	}

}

//****************************************
// 
//****************************************

void shifts(EVAL *Part1)
{
	EVAL Part2;

	plus(Part1);

	GetOper();

	if (IsOper(">>"))
	{
		SkipOper();

		shifts(&Part2);
		
		Part1->Value >>= Part2.Value;
		return;
	}

	if (IsOper("<<"))
	{
		SkipOper();

		shifts(&Part2);
		
		Part1->Value <<= Part2.Value;
		return;
	}

	if (IsOper(">>>"))
	{
		unsigned int l1,l2;
		
		SkipOper();

		shifts(&Part2);
		
		l1 = Part1->Value;
		l2 = Part2.Value;

		Part1->Value = l1 >> l2;
		return;
	}

}

//****************************************
//    MAHandle Addision and Subtraction
//****************************************

void plus(EVAL *Part1)				/* MAHandle addition and substraction */
{
	EVAL Part2;

	mult(Part1);

	GetOper();

	if (IsOper("+"))
	{
		SkipOper();

		plus(&Part2);
		
		Part1->Value += Part2.Value;
		return;
	}

	if (IsOper("-"))
	{
		SkipOper();

		plus(&Part2);

		Part1->Value -= Part2.Value;
		return;
	}
}

//****************************************
//     MAHandle Multiply and Divide
//****************************************

void mult(EVAL *Part1)		/* MAHandle multiplication, division, modulo */
{
	EVAL Part2;

	unary(Part1);

	GetOper();

	if (IsOper("*"))
	{
		SkipOper();

		mult(&Part2);

		Part1->Value *= Part2.Value;
		return;
	}

	if (IsOper("/"))
	{
		SkipOper();

		mult(&Part2);

		Part1->Value /= Part2.Value;
		return;
	}

	if (IsOper("%"))
	{
		SkipOper();
		mult(&Part2);

		Part1->Value %= Part2.Value;
		return;
	}
}

//****************************************
//		  MAHandle Unary operators
//****************************************

void unary(EVAL *Part1)
{
	SkipWhiteSpace();
	GetOper();

	// Process pre unary

	if (IsOper("-"))
	{
		SkipOper();		
		unary(Part1);

		Part1->Value = -Part1->Value;
		return;
	}

	if (IsOper("+"))
	{
		SkipOper();		
		unary(Part1);

		Part1->Value = +Part1->Value;
		return;
	}
	
	if (IsOper("!"))
	{
		SkipOper();		
		unary(Part1);

		Part1->Value = !Part1->Value ;
		return;
	}
	
	if (IsOper("~"))
	{
		SkipOper();		
		unary(Part1);

		Part1->Value = ~Part1->Value;
		return;
	}	

	// Now process identifiers and numerics

	Identifier(Part1);

	// Process post unary

/*	GetOper();

	if (IsOper("--"))
	{
		SkipOper();		
		SkipWhiteSpace();

		Part1->Value--;
		return;
	} 
*/

}

//****************************************
// 		 MAHandle Primary types
//   Constants, Strings & Indentifiers
//****************************************

void Identifier(EVAL *Part1)
{
	while(1)
	{
	
		if (*FilePtr == '(')
		{
			FilePtr++;
			assign(Part1);
						
			NeedToken(")");
			break;		
		}

		if (isdigit(*FilePtr) || *FilePtr == '\'')
		{	
			Part1->Value	= GetNum();
			Part1->Sym		= NULL;
			Part1->Type		= 0;
			break;
		}

		if (*FilePtr == '\"')						// Get Strings
		{
			Part1->Value	= (int) GetStr();
			Part1->Sym		= NULL;
			Part1->Type		= 0;
			break;
		}

		if (iscsymf(*FilePtr))
		{
			GetIdentifier(Part1);
			break;		
		}

		Error(Error_Skip, "Unknown Operator '%s'",ThisOp);
		ExitApp(1);
	}
	
	SkipWhiteSpace();
}

//****************************************
//			Init Virtual Index
//****************************************

int VirtualIndex;

void InitVirtualIndex()
{
	VirtualIndex = 0xE0000000;
}

//****************************************
//			Init Virtual Index
//****************************************

int GetVirtualIndex()
{
	return VirtualIndex & 0xffff;
}

//****************************************
// 			Get Identifiers
//****************************************

extern int ArgJavaNative;

extern int MaxDataIP;
extern int ArgRes;

void GetIdentifier(EVAL *Part1)
{
	char	*ThisLabel;
//	int		Global = 0;
	int		scope;
	
	SYMBOL	*sym;
	
	ThisLabel = GetSym();
	
	// Set default scope

	scope = LocalScope;

	//----------------------------------------
	//		  Search local scope
	//----------------------------------------
	
	sym = FindSymbols(ThisLabel,section_Enum,section_Enum, scope);
	
	//----------------------------------------
	//		  Not in local scope
	//----------------------------------------

	if (!sym)
	{
		// Look for a symbol in the global scope

		sym = FindSymbols(ThisLabel,section_Enum,section_Enum, 0);

		if (sym)
		{
			// In global scope, so get the original scope

			scope = sym->Value;

			sym = FindSymbols(ThisLabel,section_Enum,section_Enum, scope);
		}
	}
	
	//----------------------------------------
	//		  local/global scope
	//----------------------------------------

	if (sym)
	{		
		Part1->Sym = 0;
		Part1->Value = sym->Value;

		if (ExpIsPure == 0)					// If this is a not pure expression
		{									// Then check code refs
			if (sym->Type == SECT_code)
			{
				SetLastSymbolRef(sym);

				// Must be a code reference
		
				CodeRef = 1;
				strcpy(CodeRefName, ThisLabel);
						
				//printf("\nCodeRef '%s'\n", ThisLabel);
				
				// It must be a function, so make it virtual

				if (sym->LabelType == label_Function)
				{
					// Promote to virtual
					sym->LabelType = label_Virtual;
					sym->VirtualIndex = VirtualIndex++;
				}			

				// Only convert to virtual index if in java mode

				if (ArgJavaNative)
				if (sym->LabelType == label_Virtual)
				{
					// Change the value, to reflect functions virtual reference

					Part1->Value = sym->VirtualIndex;
				}

				CodeValue = Part1->Value;
			}
		}

		if (sym->Type == SECT_data)
		{
			// Special case for bss

			SetLastSymbolRef(sym);

			Part1->Value = sym->Value;
		}
	
		if (sym->Type == SECT_bss)
		{
			// Special case for bss

			SetLastSymbolRef(sym);

			Part1->Value = sym->Value + MaxDataIP;
		}
		
		
		Part1->Type = EXP_numeric;

		if (sym->Type)
		{
			if (ExpSection == 0)
			{
				ExpSection = sym->Type;
				return;
			}

			if (ExpSection == sym->Type)
				return;
			
			Error(Error_Fatal, "Cannot evaluate section conflict in expression");
		}
		return;
	}

	//----------------------------------------
	//		Symbols in resource mode
	//----------------------------------------

	if (ArgRes)
	{
		// Check for resource symbols

		sym = FindSymbolsOld(ThisLabel,section_Resource,section_Resource);
		
		if (sym)
		{
			Part1->Value = sym->Value;
			return;
		}
	}

	//----------------------------------------
	//		Symbols in script mode
	//----------------------------------------

	sym = FindSymbolsOld(ThisLabel,section_Script,section_Script);
	
	if (sym)
	{
		Part1->Value = sym->Value;
		return;
	}

	//----------------------------------------
	// 		error unresolved external
	//----------------------------------------
	
	// fake it in pass1

	if (Pass == 1)
	{	
		Part1->Sym = 0;
		Part1->Value = 0;
		Part1->Type = EXP_numeric;
		ExpResolved = -1;
		return;
	}

	doCannotEvaluate(ThisLabel);
}

//****************************************
//
//****************************************


void doCannotEvaluate(const char* ThisLabel)
{
	char* demangled=NULL;
	const char* output = ThisLabel;

	if(ThisLabel[0] == '_' && ThisLabel[1] == 'Z')			//C++ mangled name
	{
		//demangle

		//allocates the buffer if successful; must free().
		output = demangled = my_demangle (ThisLabel);

		if(demangled == NULL)
		{
			Error(Error_Fatal, "Error demangling %s\n", ThisLabel);
		}
	}

	Error(Error_Skip, "Unresolved symbol '%s'", output);

	if(demangled)
		DisposePtr(demangled);
}
