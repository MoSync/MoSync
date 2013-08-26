/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
