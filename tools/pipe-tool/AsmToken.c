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
//				         PIP-e II Token Handling
// 						   Written by A.R.Hartley
//*********************************************************************************************

#include "compile.h"

//****************************************
// 		 Build token value table
//****************************************

#undef TOKENTAB
#undef TOKENEQL

#define TOKENTAB(str, num, equal)	num,
#define TOKENEQL(str, num, equal)	num,

int TokenValue[] =
{
#include "tokentable.h"
};	   


//****************************************
// 		 Build token string table
//****************************************

#undef TOKENTAB
#undef TOKENEQL

#define TOKENTAB(str, num, equal)	str,
#define TOKENEQL(str, num, equal)	str,

char *TokenStrings[] =
{
#include "tokentable.h"
	0
};


//****************************************
//		  Init Token Symbols
//****************************************

void InitAsmTokens()
{
	char *tok;
	int val;
	int n = 0;
	
	while(1)
	{
		tok = TokenStrings[n];

		if (tok == 0)
			break;

		val = TokenValue[n];
		AddToken(tok,val);
		n++;
	}
}

//****************************************
//			Get Token Value
//****************************************

int ScanAsmToken(void)
{
	SYMBOL	*theSym;
	
	theSym = FindSymbolsOld(Name, section_Token,section_Token);
	
	if (!theSym)
		return 0;
	
	return	theSym->Value;
}


/*
TOKENTAB(".extern",      		dir_extern,             0)
TOKENEQL(".extrn",       		dir_extrn,				dir_extern)
TOKENTAB(".byte",               dir_byte,               0)
TOKENTAB(".half",               dir_half,               0)
TOKENTAB(".word",               dir_word,               0)
TOKENTAB(".space",              dir_space,              0)
TOKENTAB(".string",             dir_string,             0)
TOKENTAB(".comm",               dir_comm,               0)
*/
#if 0
void AsmTokenTest()
{
	char *str;
	int v; 
	
	v = opc_call;
	str = "call";
	printf("%s = %d (%d)\n", str, ScanAsmToken(str), v);


	
}
#endif
