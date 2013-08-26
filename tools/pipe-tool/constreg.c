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
		
	memset(ConstTable,0, CONST_REG_SIZE * sizeof(int));

	for (n=1;n<17;n++)
	{
		ConstTable[p++] = n;
		ConstTable[p++] = -n;
	}

	mask = 0x20;

	for (n=0;n<32-5;n++)
	{
		ConstTable[p++] = mask-1;
		ConstTable[p++] = mask;
		mask <<= 1;
	}

	mask = 0x10;

	for (n=0;n<10;n++)
	{
		ConstTable[p++] = mask ^ 0xffffffff;
		mask <<= 1;
	}

	return;

//-------------

#elif CONST_REG_SIZE == 256

//-------------
// 256 Entries
//-------------

	int n;

	memset(ConstTable,0, 256 * sizeof(int));

	for (n=1;n<49;n++)
	{
		ConstTable[n + 31] = n;
		ConstTable[n + 79] = -n;
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
	
		ConstTable[n] =  mask;
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

	if (ArgConstOpt == 0)
		return -1;
			
	// If value == 0 then return reg0
	
	if (v == 0)
		return 0;

	ConstPtr = &ConstTable[32];
	
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
		
	return ConstTable[reg];
}


