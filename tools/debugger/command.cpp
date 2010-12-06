/* Copyright (C) 2010 MoSync AB

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
