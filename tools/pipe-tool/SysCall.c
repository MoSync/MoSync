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
	memset(g_SysCallMap, 0, sizeof(g_SysCallMap));
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
	
	if (g_SysCallMap[v])
	{
		printf("*Warning* SysCall '%s' is remapping slot %d\n", name, v);
	}

	AddSysCall(name, v, param, rettype, istr);
	g_SysCallMap[v] = 1;
}


