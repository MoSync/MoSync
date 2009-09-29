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

#ifndef _MATASK_H_
#define _MATASK_H_

#ifdef __cplusplus
extern "C" {
#endif


// ASM function proto's

int maRunTaskInit(int SP, int p0, int p1);
int maRunTask(int SP);
void maYield();
void maKillTask();


/** \brief Initialise the task system
* \param max_tasks Set the maximum amount of tasks.
* \return Returns TRUE if all was well.
*/

int InitTasks(int max_tasks);

/** \brief Dispose the task system
*/

void DisposeTasks();


/** \brief Dispose a specific task
* \param task The task to dispose.
*/

void DisposeTask(short task);

/** \brief Sets the current task stack size
* \param size The size of the new current stack size
*/

void SetTaskStackSize(int size);

/** \brief Create a new cooperative task
* \param TaskAddr The address of the new task
* \param p0 The first parameter to be passed to the new task
* \param p1 The second parameter to be passed to the new task
* \return Returns a positive taskid if all was well, null on error.
*/

long CreateTask(char *TaskAddr, int p0, int p1);

/** \brief Execute all cooperative tasks
*/

void RunAllTasks();

#ifdef __cplusplus
}	//extern "C"
#endif

#endif // _MATASK_
