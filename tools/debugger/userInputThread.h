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

#ifndef USERINPUTTHREAD_H
#define USERINPUTTHREAD_H

#include "Thread.h"

/**
 * Reads GDB MI commands from the GDB session and puts them in event queue. 
 * When one command has been read no more commands will be read until 
 * resumeUserInput has been called.
 *
 * @param arg The input stream to read commands from.
 * @return when an error has occurred. A return value of 0 indicates a 
 *         generic error with the input stream, 1 means that eof has been
 *         reached.
 */
int SDLCALL userInputThreadFunc(void* arg);

/**
 * Resumes reading commands from the GDB session.
 */
void resumeUserInput();

/**
 * Stops the execution of the thread.
 */
void stopUserThread();

#endif	//USERINPUTTHREAD_H
