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
//				       Const Register Routines
// 						   Written by A.R.Hartley
//			This code is based on the GameTasker v2.0 Module System
//*********************************************************************************************

#include "compile.h"

//***************************************
//
//***************************************

void GenRegConst()
{

#if CONST_REG_SIZE == 128

//-------------
// 128 Entries
//-------------

	int n, p;
	int mask;

	p = 32;
		
	memset(g_ConstTable,0, CONST_REG_SIZE * sizeof(int));

	for (n=1;n<17;n++)
	{
		g_ConstTable[p++] = n;
		g_ConstTable[p++] = -n;
	}

	mask = 0x20;

	for (n=0;n<32-5;n++)
	{
		g_ConstTable[p++] = mask-1;
		g_ConstTable[p++] = mask;
		mask <<= 1;
	}

	mask = 0x10;

	for (n=0;n<10;n++)
	{
		g_ConstTable[p++] = mask ^ 0xffffffff;
		mask <<= 1;
	}

	return;

//-------------

#elif CONST_REG_SIZE == 256

//-------------
// 256 Entries
//-------------

	int n;

	memset(g_ConstTable,0, 256 * sizeof(int));

	for (n=1;n<49;n++)
	{
		g_ConstTable[n + 31] = n;
		g_ConstTable[n + 79] = -n;
	}

	for (n=128;n<256;n++)
	{
		int mask = 0x01;
		int m;

		for (m=0;m<(n & 0x1f);m++)
		{
			mask <<= 1;

			if (n & 0x40)
				mask |= 0x01;
		}

		if (n & 0x20)
			mask ^= 0xffffffff;
	
		g_ConstTable[n] =  mask;
	}
	
	return;

//-------------
#else
#error
#endif
}
//***************************************
//
//***************************************

int IsConst(int v)
{
	int *ConstPtr;
	int n;

	// If constant optimization is off return not found

	if (g_ArgConstOpt == 0)
		return -1;
			
	// If value == 0 then return reg0
	
	if (v == 0)
		return 0;

	ConstPtr = &g_ConstTable[32];
	
	for (n=32;n<CONST_REG_SIZE;n++)
	{	
		if (v == *ConstPtr++)
			return n;				// Return reg number
	}
	
	return -1;						// Not in table
}
//***************************************
//
//***************************************

int IsRegConst(int reg)
{
	if (reg >= CONST_REG_SIZE)
		return 0;

	if (reg >= 32)
		return 1;

	if (reg == 0)
		return 1;
		
	return 0;
}

//***************************************
//
//***************************************

int ConstRegValue(int reg)
{
	if (reg == 0)
		return 0;
		
	return g_ConstTable[reg];
}


