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
