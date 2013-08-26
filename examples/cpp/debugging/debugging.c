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
