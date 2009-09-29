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

#include <istream>
#include <iostream>
#include <string>

#include "Thread.h"
#include "async.h"
#include "helpers.h"
#include "userInputThread.h"

using namespace std;

static MoSyncSemaphore sPause;

int SDLCALL userInputThreadFunc(void* arg) {
	istream* input = (istream*)arg;
	while(1) {
		DebuggerEvent* de = new DebuggerEvent;
		de->type = DebuggerEvent::eUserInput;
		getline(*input, de->str);
		while(!input->good()) {
			if(!input->eof()) {
				eprintf("Input fail! Exiting...\n");
				return 1;
			}
			if(input != &cin) {
				//delete input;	//not so good with multi-threading, maybe.
				input = &cin;
				getline(*input, de->str);
			} else {
				return 0;
			}
		}
		if(input != &cin) {
			eprintf("%s\n", de->str.c_str());
		}
		putEvent(de);
		sPause.wait();
	}
}

void resumeUserInput() {
	sPause.post();
}
