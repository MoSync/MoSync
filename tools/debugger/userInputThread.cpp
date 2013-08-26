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

#include <istream>
#include <iostream>
#include <string>

#include "Thread.h"
#include "async.h"
#include "helpers.h"
#include "userInputThread.h"

using namespace std;

static MoSyncSemaphore sPause;
static bool run = true;

int SDLCALL userInputThreadFunc(void* arg) {
	istream* input = (istream*)arg;
	while(run) {
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
	return 0;
}

void resumeUserInput() {
	sPause.post();
}

void stopUserThread() {
	run = false;
	sPause.post();
}
