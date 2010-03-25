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
//							   MoSync Task Control System
//*********************************************************************************************

#include "matask.h"
#include "maheap.h"

char *MainSP;
int CurrentRunningTask = 0;

#define ThisTask()	CurrentRunningTask;
#define task_t		char *

#ifdef _WIN32
#define DEFAULT_STACK_SIZE (28*1024)
#else
#define DEFAULT_STACK_SIZE (48*1024)//4096
#endif

int DefaultStackSize = DEFAULT_STACK_SIZE;

//****************************************
//	    X86 Low-Level Task Asm
//****************************************

#ifdef _WIN32

typedef struct
{
	int _ebp;
	int _edi;
	int _esi;
	int _ebx;
	int rt;
} TaskStack;

int para0 = 0, para1 = 0;

_declspec(naked) int maRunTaskInit(int SP, int p0, int p1)
{
	_asm
	{
		pop		ecx

		pop		eax			// get param
		pop		[para0]
		pop		[para1]

		push	[para1]
		push	[para0]
		push	eax			// stack param


		push	ecx			// re-push return address

		push	ebx
		push	esi
		push	edi
		push	ebp
		mov		[MainSP], esp

		// Switch context

		mov		esp,eax
		pop		ebp
		pop		edi
		pop		esi
		pop		ebx

		pop		ecx			// return addr

		push	[para1]
		push	[para0]
		push	maKillTask			//	sub		esp, 4

		jmp ecx
	}
}
_declspec(naked) int maRunTask(int SP)
{
	_asm
	{
		pop		ecx

		pop		eax			// get param1
		push	eax

		push	ecx			// re-push return address

		push	ebx
		push	esi
		push	edi
		push	ebp
		mov		[MainSP], esp

		// Switch context

		mov		esp,eax
		pop		ebp
		pop		edi
		pop		esi
		pop		ebx
		ret
	}
}

_declspec(naked) void maYield()
{
	_asm
	{
		push	ebx
		push	esi
		push	edi
		push	ebp
		mov		eax,esp					// Save task context in return reg

		mov		esp,[MainSP]			// Retrieve main context
		pop		ebp
		pop		edi
		pop		esi
		pop		ebx
		ret
	}
}

_declspec(naked) void maKillTask()
{
	_asm
	{
		mov		eax,0					// Don't save task context, return sp at null

		mov		esp,[MainSP]			// Retrieve main context
		pop		ebp
		pop		edi
		pop		esi
		pop		ebx
		ret
	}
}

#else

//****************************************
//	    PIPE Low-Level Task Asm
//****************************************

#define ASM(str)		__asm__(str "\n\r");

//****************************************
//			Stack structure
//****************************************

typedef struct
{
	int d7;
	int d6;
	int d5;
	int d4;
	int d3;
	int d2;
	int d1;
	int d0;
	int fr;
	int rt;
} TaskStack;

//****************************************
//			  Run Task Init
//         i0 = stack location
//         i1 = param1
//         i2 = param2
//****************************************

int maRunTaskInit(int SP, int p0, int p1)
{
	ASM("push rt,d7			")		// Save main context
	ASM("ld	[&_MainSP],sp	")

	ASM("ld	sp, i0			")		// retrieve task context
	ASM("pop	rt,d7		")

	ASM("ld	i0,i1			")		// Load up parameters
	ASM("ld	i1,i2			")
	ASM("ld	i2,i3			")

	ASM("ld	i3,rt			")		// Save task address in i3

	ASM("ld	rt,&_maKillTask	")		// Set up kill task as a return address
	ASM("jp	i3				")		// Run Task

	return 0;	//never gonna happen
}

//****************************************
//			  Run Task Once
//         i0 = stack location
//****************************************

int maRunTask(int SP)
{
	ASM("push rt,d7			")		// Save main context
	ASM("ld	[&_MainSP],sp	")

	ASM("ld	sp, i0			")		// retrieve task context
	ASM("pop rt,d7			")

	ASM("ret				")		// Run task

	return 0;	//never gonna happen
}

//****************************************
//				Run Task
//         returns stack location
//****************************************

void maYield(void)
{
	ASM("push rt,d7			");		// Save task context
	ASM("ld	r14,sp			");

	ASM("ld	sp,[&_MainSP]	");		// Retrieve main context
	ASM("pop rt,d7			");
	ASM("ret				");
}

//****************************************
//				Kill Task
//         returns stack location
//****************************************

void maKillTask(void)
{
	ASM("ld	r14,zr			");		// return sp as null

	ASM("ld	sp,[&_MainSP]	");		// Retrieve main context
	ASM("pop rt,d7			");
	ASM("ret				");
}


#endif


//***************************************
//
//***************************************

#define MAXTASK			4		// Maximum allowed tasks

typedef struct
{
	int		SP;
	int		StackBase;		// Stack Base
} TaskEntry;

//***************************************
//		  Set up task buffers
//***************************************

TaskEntry*	TaskData;			// Main task table
int		TaskCount;			// Global task count

//***************************************
//		Initialise task entrys
//***************************************

int InitTasks(int max_tasks)
{
	TaskEntry	*Task;
	long n;

	TaskData = (TaskEntry *) malloc(max_tasks * sizeof(TaskEntry));

	if (!TaskData)
		return 0;

	Task = TaskData;

	for(n=0;n<MAXTASK;n++)
	{
		Task->SP		= 0;
		Task->StackBase	= 0;
		Task++;
	}

	TaskCount = 0;
	DefaultStackSize = DEFAULT_STACK_SIZE;
	return 1;
}

//***************************************
// 			Kill all tasks
//***************************************

void DisposeTasks(void)
{
	short	n;

	for(n=0;n<MAXTASK;n++)
		DisposeTask(n);

	if (TaskData)
		free((char *) TaskData);

	TaskData = 0;

	TaskCount = 0;
}

//***************************************
//		 Dispose a thread
//***************************************

void DisposeTask(short task)
{
	TaskEntry	*Task;

	Task = &TaskData[task];

	if (Task->SP)
		free((char *)Task->StackBase);

	Task->SP		= 0;
	Task->StackBase = 0;
}

//***************************************
//	  Change default stack size
//***************************************

void SetTaskStackSize(int size)
{
	if (size > 32)
		DefaultStackSize = size;
}

//***************************************
//		  Create a new task
//***************************************

long CreateTask(char *TaskAddr, int p0, int p1)
{
	TaskEntry	*Task;
	TaskStack	*NewStack;
	char		*mem;
	int			n,Old;

	// Search for free task buffer

	Task = TaskData;

	for(n=0;n<MAXTASK;n++)
	{
		if (Task->SP == 0)
			break;

		Task++;
	}

	if (n == MAXTASK)
		return -1;

	// Set start of execution and clear backchannel

	mem = (char *) malloc(DefaultStackSize);

	if (!mem)
		return -1;

	NewStack = (TaskStack *) (mem + DefaultStackSize - sizeof(TaskStack) - 16);

//	NewStack = (TaskStack *) (mem + (DefaultStackSize / 2));

	Task->StackBase	= (int)	mem;
	Task->SP		= (int) NewStack ;
	NewStack->rt	= (int) TaskAddr;

	TaskCount++;									// Add new task to count

	Old = CurrentRunningTask;						// Save the current task num

	CurrentRunningTask = n;							// Set the new task num

	// Run the task

	Task->SP = (int) maRunTaskInit(Task->SP, p0, p1);

	CurrentRunningTask = Old;						// Restore our task num
	return n;
}

//***************************************
// 			Run all tasks
//***************************************

void RunAllTasks(void)
{
	TaskEntry	*Task;
	short	n;

	Task = &TaskData[0];

	for(n=0;n<MAXTASK;n++)
	{
		if (Task->SP)
		{
			CurrentRunningTask = n;
			Task->SP = (int) maRunTask(Task->SP);

			if (!Task->SP)			// Check if Task killed
			{
				TaskCount--;										// One less task active
				DisposeTask(CurrentRunningTask);
			}
		}

		Task++;
	}
}

