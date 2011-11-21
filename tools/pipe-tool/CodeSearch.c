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
//				  			  	Code Dependency Search
// 						   		Written by A.R.Hartley
//*********************************************************************************************

#include "compile.h"

#ifdef INCLUDE_DEPEND_SEARCH

//****************************************
//				Defines
//****************************************

//#define CD_DEBUG

#ifdef CD_DEBUG
#define CDPRINTF	if (1) printf
#endif

//****************************************
//				Variables
//****************************************

#ifdef CD_DEBUG

void CDTAB(int depth)
{
	int n;

	for (n=0;n<depth;n++)
		CDPRINTF(" ");
}

#endif

//****************************************
//		  Search a function for:
// 1. Functions it uses
// 2. Accesses to data it uses
//****************************************

void SearchDep_Function(SYMBOL *sym, int depth)
{
	int ip, ip_end;

	// Is symbol valid

	if (!sym)
	{
		printf("Error: null symbol in SearchDep_Function");
		return;
	}

	// Is this sym already marked

	if (sym->Flags & SymFlag_Ref)
		return;

	// Check section

	if (sym->Type != SECT_code)
		return;

	// Mark as found

	sym->Flags |= SymFlag_Ref;

	// Dont scan local labels (but mark them as found).

	if (sym->LabelType == label_Local)
	{
		// Find labels function scope

		SYMBOL *func = FunctionSymFromIP(sym);

		sym->EndIP = func->EndIP;
	}

	// print info

#ifdef CD_DEBUG
	CDPRINTF("\n");
	CDTAB(depth);
	CDPRINTF("enter %s(0x%x,0x%x)\n", sym->Name, sym->Value, sym->EndIP);
	CDTAB(depth);
	CDPRINTF("{\n");
#endif

	ip		= sym->Value;
	ip_end	= sym->EndIP;

	SearchDep_Function_Inner(ip, ip_end, depth);

#ifdef CD_DEBUG
	CDTAB(depth);
	CDPRINTF("} // %s\n", sym->Name);
#endif

}

//****************************************
//      Search within a function
//****************************************

int SearchDep_Function_Inner(int ip, int ip_end, int depth)
{
	OpcodeInfo thisOpcode;
	SYMBOL *thisSym;

	unsigned int ip_next, v;

	// Fetch the current opcode, but don't advance ip yet !!


	do
	{
		ip_next = DecodeOpcodeIP(&thisOpcode, ip);

		if (ip_next > 0x80000000)
			ErrorOnIP(Error_Fatal, ip, "SearchDep_Function_Inner: ip_next > 0x80000000 (Contact MoSync)");

#ifdef CD_DEBUG
		{
			char buf[2560];
			buf[0] = 0;
			DisassembleFromSource(ip, buf);
			CDTAB(depth);
			CDPRINTF("%s\n", buf);
		}
#endif
		if (ip > ip_end)
			return ip;

		// Only examine this code if its untouched

		if (ArrayGet(&CodeTouchArray, ip) == 0)
		{
			// Say this opcode has been touched

			ArraySet(&CodeTouchArray, ip, 1);

			// Check CallArray

			v = ArrayGet(&CallArray, ip);

			if (v)
			{
				// Investigate function at this symbol

				thisSym = (SYMBOL *) v;
				SearchDep_Function(thisSym, depth+1);
			}

			// Check DataAccessArray

			v = ArrayGet(&DataAccessArray, ip);

			if (v)
			{
				// Investigate memory at this symbol

				thisSym = (SYMBOL *) v;
				SearchDep_Memory(thisSym, depth+1);
			}

		}

#ifdef INCLUDE_INSTRUCTION_ELIM

		switch (thisOpcode.op)
		{
			// if we found a goto then follow it

			case _JC_EQ:
			case _JC_NE:
			case _JC_GE:
			case _JC_GEU:
			case _JC_GT:
			case _JC_GTU:
			case _JC_LE:
			case _JC_LEU:
			case _JC_LT:
			case _JC_LTU:
			{
				int new_ip = thisOpcode.imm;

				// Test if the target ip is alreay touched, quit if so

				if (ArrayGet(&CodeTouchArray, new_ip) == 0)
				{
					// Search the new ip

					SearchDep_Function_Inner(new_ip, ip_end, depth+1);

					// Continue since jump was conditional
				}

				ip = ip_next;
				continue;
			}

			case _JPI:
			{
				int new_ip = thisOpcode.imm;

				// Test if the target ip is alreay touched, quit if so

				if (ArrayGet(&CodeTouchArray, new_ip) == 1)
					return ip;

				// Follow this new ip

				ip = new_ip;
				continue;
			}

			case _CASE:
			{
				int /*start,*/ len, data_ip, def_ip, i;
				int lab_ip;

				data_ip = thisOpcode.imm;

				/*start	=*/ GetDataMemLong(data_ip++);
				len		= GetDataMemLong(data_ip++);
				def_ip	= GetDataMemLong(data_ip++);	// default case

				for (i=0;i<len+1;i++)
				{
					lab_ip = GetDataMemLong(data_ip++);

					if (ArrayGet(&CodeTouchArray, lab_ip) == 0)
						SearchDep_Function_Inner(lab_ip, ip_end, depth+1);
				}

				if (ArrayGet(&CodeTouchArray, def_ip) == 1)
					return ip;

				ip = def_ip;
				continue;
			}

			// if we found a return then break out

			case _RET:
				return ip;

			// Next ip
			default:
				ip = ip_next;
				continue;

		}

		break;

#endif

		ip = ip_next;
	}
	while(1);

	return 0;
}


//****************************************
//		  Search memory for:
// 1. Functions pointers
// 2. Data pointers
//****************************************

void SearchDep_Memory(SYMBOL *sym, int depth)
{
	SYMBOL *thisSym;

	int ip;
	uint n, v;

	// Valid symbol

	if (!sym)
	{
		printf("Error: null symbol in SearchDep_Memory");
		return;
	}

	// has this symbol been marked, exit if so

	if (sym->Flags & SymFlag_Ref)
		return;

	// If this symbol is in code, search there instead

	if (sym->Type == SECT_code)
	{
		SearchDep_Function(sym, depth);
		return;
	}

	// Mark this symbol as found

	sym->Flags |= SymFlag_Ref;

	// print info

#ifdef CD_DEBUG
	CDPRINTF("\n");
	CDTAB(depth);
	CDPRINTF("%s[]=\n", sym->Name);
	CDTAB(depth);
	CDPRINTF("{\n");
#endif

	// set ip to the address of the memory to be searched

	ip	= sym->Value;

	// if this address is in bss then we must place it at the
	// end of the data section

	if (sym->Type == SECT_bss)
		ip = sym->Value + MaxDataIP;

	// Byte search of memory

	for (n=ip;n<LabelArray.hi;n++)
	{
		// Check DataArray

		v = ArrayGet(&DataArray, n);

		if (v)
		{
			thisSym = (SYMBOL *) v;


			// only explore data section variables, since bss contains
			// no initialized data.

			// if dataref then follow that

			if (thisSym->Type == SECT_data || thisSym->Type == SECT_bss)
				SearchDep_Memory(thisSym, depth+1);

			// if we found a function pointer we must follow it

			if (thisSym->Type == SECT_code)
				 SearchDep_Function(thisSym, depth+1);

		}


		// Check if the data symbol has hit alignment padding

		v = ArrayGet(&PaddingArray, n);

		if (v)
			break;

		// Check if the data symbol is finished, i.e it bumps
		// into the next symbol

		if (n != (uint)ip)
		{
			v = ArrayGet(&LabelArray, n);

			if (v)
				break;
		}

		// Next ip
	}

	// Must be data section only

	if ((sym->Type == SECT_data) || (sym->Type == SECT_bss))
	{
		// Use EndIP as the data length

		sym->EndIP = n - ip;

#ifdef CD_DEBUG
		CDTAB(depth);
		CDPRINTF("} // %s (len %d)\n", sym->Name, sym->EndIP);
#endif
	}

	return;
}

//****************************************
//
//****************************************

void SearchDep_Main()
{
	SYMBOL *ip;

	ArrayClear(&CodeTouchArray);

	// Clear the ref flag in for the whole symbol table

	ClearSymbolFlags(SymFlag_Ref);

	ip = GetGlobalSym(Code_EntryPoint);

	if (ip)
		SearchDep_Function(ip, 0);
}

//****************************************
// Finds the function above the input ip
//****************************************

SYMBOL * FunctionSymFromIP(SYMBOL *localsym)
{
	SYMBOL *sym;
	int in_ip;

	if (!localsym)
		ErrorOnIP(Error_Fatal, 0, "FunctionSymFromIP sym=0");

	in_ip = localsym->Value;

// !! ARH rewrote calls FunctionAboveIP instead !!

	sym = FunctionAboveIP(localsym->Value);

	if (!sym)
		ErrorOnIP(Error_Fatal, in_ip, "FunctionAboveIP can't find function top IP=0x%x Label '%s'", in_ip, localsym->Name);

	return sym;
}

//****************************************
// Finds the function above the input ip
//****************************************

SYMBOL * FunctionAboveIP(int ip)
{
	SYMBOL *sym;

	if (ip > (int) CodeLabelArray.hi)
		return 0;

	while(1)
	{
//		if (ip < (int) CodeLabelArray.lo) !! ARH looks incorrect !!

		if (ip < 0)
			break;

		sym = (SYMBOL *) ArrayGet(&CodeLabelArray, ip);

		if (sym)
		{

			if (sym->LabelType >= label_Function)		// Include virtuals
				return sym;
		}

		ip--;
	}

	return 0;
}


//****************************************
//  Find the extent of a data entity
//****************************************

int FindLabelExtent(int ip)
{
//	SYMBOL *thisSym;

	uint n, v;

	// Byte search of memory

	for (n=ip;n<LabelArray.hi;n++)
	{
		// Check if the data symbol has hit alignment padding

		v = ArrayGet(&PaddingArray, n);

		if (v)
			break;

		// Check if the data symbol is finished, i.e it bumps
		// into the next symbol

		if (n != (uint)ip)
		{
			v = ArrayGet(&LabelArray, n);

			if (v)
				break;
		}

		// Next ip
	}

	v = n - ip;
	return v;
}

//****************************************

#endif // INCLUDE_DEPEND_SEARCH
