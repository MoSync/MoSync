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

/** \file matask.h
* \brief Task control system
* \author Fredrik Eldh
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
void maYield(void);
void maKillTask(void);


/** \brief Initialise the task system
* \param max_tasks Set the maximum amount of tasks.
* \return Returns TRUE if all was well.
*/

int InitTasks(int max_tasks);

/** \brief Dispose the task system
*/

void DisposeTasks(void);


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

void RunAllTasks(void);

#ifdef __cplusplus
}	//extern "C"
#endif

#endif // _MATASK_
