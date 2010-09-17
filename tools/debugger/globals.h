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

#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>
#include <helpers/types.h>

extern std::string gProgramFilename, gResourceFilename, gSldFilename;

extern bool gTestWaiting;

typedef struct _MA_HEAD
{
	int	Magic;				// Header magic
	int	CodeLen;			// Length of code section
	int	DataLen;			// Length of data section

	int	DataSize;			// Prefurred data memory size	
	int	StackSize;		// Stack size
	int	HeapSize;			// Heap size

	int	AppCode;			// App Code (App MIME)
	int AppID;				// App ID

	int	EntryPoint;		// Program entry point
	int	IntLen;
} MA_HEAD;

extern MA_HEAD gHead;
extern byte* gMemCs;
extern int* gMemCp;

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
extern HANDLE gSimProcess;
#endif

bool execIsRunning();
void varErrorFunction();

#endif	//GLOBALS_H
