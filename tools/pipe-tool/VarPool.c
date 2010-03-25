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
//				  PIP-e II Assembler Variable Pool Routines
// 						   Written by A.R.Hartley
//			This code is based on the GameTasker v2.0 Module System
//*********************************************************************************************

#include "compile.h"

#define VARMAX 32768			//16384

//****************************************
//		Initialise the constant pool
//****************************************

int InitVarPool()
{
	g_VarPool = (int *) NewPtrClear( (int) (sizeof(int) * VARMAX));

	if (!g_VarPool)
		return 0;

	g_VarFreq = (int *) NewPtrClear( (int) (sizeof(int) * VARMAX));

	if (!g_VarFreq)
	{
		DisposePtr((char *) g_VarPool);
		return 0;
	}

	g_ThisVar = g_VarPool;
	g_VarCount = 0;
	
	StoreVarPool(0);			// Store defualt 0
	return 1;
}

//****************************************
//		 Close the constant pool
//****************************************

void CloseVarPool()
{
	if (g_VarPool)
		DisposePtr((char *) g_VarPool);

	if (g_VarFreq)
		DisposePtr((char *) g_VarFreq);

	g_VarCount = 0;
	g_ThisVar = 0;
}

//****************************************
//		  Search var pool entry
//****************************************

void ClearVarPool()
{
	g_ThisVar = g_VarPool;
	g_VarCount = 0;
	
	StoreVarPool(0);			// Store defualt 0
}

//****************************************
//		  Search var pool entry (Slow)
//****************************************

int SearchVarPool(int v)
{
	int *vptr;
	int n;

	// Failed so look again

	vptr = g_VarPool;
	
	for (n=0;n<g_VarCount;n++)
	{
		if (v == *vptr++)
			return n;
	
	}

	return -1;
}

//****************************************
//		  Search var pool entry
//****************************************

int StoreVarPool(int v)
{
	int idx;
	
	if (g_VarCount >= VARMAX)
		Error(Error_Fatal, "Var Pool is full");

	*g_ThisVar++ = v;
	
	idx = g_VarCount;
	g_VarCount++;
	return idx;	
}

//****************************************
// 		Find a constant pool entry
//****************************************

int FindVar(int v)
{
	int idx;
		
	idx = SearchVarPool(v);
	
	if (idx == -1)
	{
//		if (Pass == 2)
//			Error("Constant int not found in second asm pass, something is wrong !!!");

		idx = StoreVarPool(v);
	}
	// Increase the frequency of access
	
	if (g_Pass == 1)
		g_VarFreq[idx]++;

	return idx;
}

//****************************************
//			Sort the int pool (Slow)
//****************************************

#if 1

#define INV_IND(ind) (g_VarCount-ind-1)

void SortVarPool()
{
  varpool_q_sort(0, g_VarCount - 1);
}

void varpool_q_sort(int left, int right)
{
	int pivot, pivot_pool, l_hold, r_hold;

	l_hold = left;
	r_hold = right;

	pivot = g_VarFreq[INV_IND(left)];
	pivot_pool = g_VarPool[INV_IND(left)];

	while (left < right)
	{
		while ((g_VarFreq[INV_IND(right)] >= pivot) && (left < right))
			right--;

		if (left != right)
		{
			g_VarFreq[INV_IND(left)] = g_VarFreq[INV_IND(right)];
			g_VarPool[INV_IND(left)] = g_VarPool[INV_IND(right)];
			left++;
		}

		while ((g_VarFreq[INV_IND(left)] <= pivot) && (left < right))
			left++;

		if (left != right)
		{
			g_VarFreq[INV_IND(right)] = g_VarFreq[INV_IND(left)];
			g_VarPool[INV_IND(right)] = g_VarPool[INV_IND(left)];
			right--;
		}
	}

	g_VarFreq[INV_IND(left)] = pivot;
	g_VarPool[INV_IND(left)] = pivot_pool;

	pivot = left;
	left = l_hold;
	right = r_hold;

	if (left < pivot)
		varpool_q_sort(left, pivot-1);

	if (right > pivot)
		varpool_q_sort(pivot+1, right);
}

#endif
//****************************************
//			Sort the int pool (Slow)
//****************************************

#if 0

void SortVarPool()
{
	int n, mod=1, tmp;
	
	if (g_VarCount < 3)
		return;

	while(mod)
	{	
		mod = 0;

		for (n=0;n<g_VarCount-1;n++)
		{
			if (g_VarFreq[n+1] > g_VarFreq[n])
			{
				// Swap const's

				tmp = g_VarFreq[n];
				g_VarFreq[n] = g_VarFreq[n+1];
				g_VarFreq[n+1] = tmp;

				tmp = g_VarPool[n];
				g_VarPool[n] = g_VarPool[n+1];
				g_VarPool[n+1] = tmp;
				
				mod = 1;			
			}
		}
	}

#ifdef POOL_HASHING
	IndexVarPool();
#endif
}

#endif

//****************************************
//			Sort the int pool (Slow)
//****************************************

#if 0

void SortVarPool()
{
	int i, j, inc, tmp1, tmp2;
	
	inc = 3;

	while (inc > 0)
	{
		for (i=0; i < g_VarCount; i++)
		{
			j = i;

			tmp1 = g_VarFreq[i];
			tmp2 = g_VarPool[i];

			while ((j >= inc) && (g_VarFreq[j-inc] > tmp1))
			{
				g_VarFreq[j] = g_VarFreq[j - inc];
				g_VarPool[j] = g_VarPool[j - inc];
				j = j - inc;
			}

			g_VarFreq[j] = tmp1;
			g_VarPool[j] = tmp2;
		}

		if (inc/2 != 0)
			inc = inc/2;
		else if (inc == 1)
			inc = 0;
		else
			inc = 1;
	}
}

#endif

//****************************************
//		  Fetch Var Pool Entry
//****************************************

int GetVarPoolEntry(int n)
{
	if (n >= g_VarCount)
		return 0;

	return g_VarPool[n];
}

//****************************************
//
//****************************************

#if 1

void OuputVarPool()
{
}

//****************************************
//			Dump var pool
//****************************************

void DumpVarPool()
{
	int n;

	printf("\nInteger Pool\n\n");
	
	for (n=0;n<g_VarCount;n++)
	{
		printf("%d: 0x%x Freq %d\n", n, g_VarPool[n], g_VarFreq[n]);
	}

	printf("VarCount = %d\n", g_VarCount);

	return;
}

//****************************************
//		  Search var pool entry (Slow)
//****************************************

int HashVarPool()
{
	int *vptr = g_VarPool;
	int n;
	int v = 41;
	
	for (n=0;n<g_VarCount;n++)
	{
		v += HASHFUNC(*vptr);
		vptr++;
	}

	return v;	
}

//****************************************
//			Test var pool
//****************************************

void TestVarPool()
{
	int n,v,i;

	InitVarPool();
	
	for (n=0;n<20;n++)
	{
		v = rand() & 7;
		i = FindVar(v);
		printf("inserted %d\n",  v);
	}
	
	DumpVarPool();

	printf("\nSorted");
	SortVarPool();
	DumpVarPool();
	CloseVarPool();
}

#endif
