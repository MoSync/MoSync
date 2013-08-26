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

#include "command.h"
#include "cmd_stack.h"
#include "helpers.h"

Command Command::sCommand;
void Command::cmdStackLoaded() {
	_ASSERT(sCommand.mProc != NULL);
	sCommand.mProc(sCommand.mArgs);
}

Command::Command() : mProc(NULL), mOptions(0) {}
Command::Command(Proc p) : mProc(p), mOptions(0) {}
Command::Command(Proc p, int options) : mProc(p), mOptions(options) {}

void Command::operator()(const std::string& args) {
	mArgs = args;
	sCommand = *this;

	if(mOptions & LOAD_STACK) {
		loadStack(cmdStackLoaded);
	} else {
		sCommand.mProc = NULL;
		mProc(args);
	}
}
