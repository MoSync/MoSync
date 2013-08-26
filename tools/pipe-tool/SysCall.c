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
//				  PIP-e II Assembler SysCall Routines
// 						   Written by A.R.Hartley
//			This code is based on the GameTasker v2.0 Module System
//*********************************************************************************************

#include "compile.h"

//****************************************
//
//****************************************

void InitSysCalls()
{
	memset(SysCallMap, 0, sizeof(SysCallMap));
}

//****************************************
//			Check SysCall
//****************************************

int isSysCall(char *ThisName)
{
	SYMBOL	*sym;
		
	sym = FindSymbolsOld(ThisName,section_SysCall,section_SysCall);
	
	if (sym)
	{
//		sym->Ref++;
		SetLastSymbolRef(sym);

		return sym->Value;
	}	
	return -1;
}

//****************************************
//		Reverse lookup for Syscalls
//****************************************

SYMBOL * FindSysCall(int v)
{
	SYMBOL	*sym;
		
	sym = ReverseFindSymbols(v, section_SysCall,section_SysCall);
	
//	if (sym)
//		return sym->Name;
		
	return sym;
}

//****************************************
//		Map the new SysCall
//****************************************

void MapSysCall(char *name, int v, int param, int rettype, char *istr)
{
	// Test if SysCall already mapped
	
	if (SysCallMap[v])
	{
		printf("*Warning* SysCall '%s' is remapping slot %d\n", name, v);
	}

	AddSysCall(name, v, param, rettype, istr);
	SysCallMap[v] = 1;
}


