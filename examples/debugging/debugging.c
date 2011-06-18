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

/** \file debugging.c
* This example shows how to use the debug logging and User Panic
* functions in MoSync. It outputs data to MoBuild, if run through that system,
* and to a file, but not to the screen.
*/

#include <ma.h>
#include <mavsprintf.h>

int MAMain(void) {
	int i;

	//Logging print, Formatted, with end of LiNe.
	//This function calls maWriteLog(), which logs binary data.
	lprintfln("Hello World.");
	
	for(i=0; i<10; i++) {
		lprintfln("Number %i", i);
		if(i > 3) {
			//maPanic() works similarly to maExit(), but also displays an alert to the user.
			maPanic(0, "i became greater than 3!");
		}
	}
	
	return 0;
}
