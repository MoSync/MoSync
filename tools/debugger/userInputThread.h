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

#endif	//USERINPUTTHREAD_H
