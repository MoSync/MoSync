#include "Command.h"
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
