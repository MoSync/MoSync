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
//				       Thunk Register Routines
// 						   Written by A.R.Hartley
//*********************************************************************************************

#include "compile.h"

//SYMBOL *ThunkTable[128];
SYMBOL *ThunkTable[65536];

//***************************************
//		  Initialize Thunk Table
//***************************************

void InitThunkTable()
{
	memset(ThunkTable, 0, sizeof(ThunkTable));
	return;
}

//***************************************
//			Register a Thunk
//***************************************

void RegisterThunk(int v, SYMBOL *sym)
{
	ThunkTable[v] = sym;
}

//***************************************
//		  Get a Thunk Entry
//***************************************

SYMBOL * GetThunk(int v)
{
	return 	ThunkTable[v];
}
