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

#ifndef __COMMAND_H_
#define __COMMAND_H_

#include <map>
#include <string>

/* Defines a function that handles a GDB MI command. */
/*
typedef void (*Command)(const std::string& args);
*/

// Niklas: changed it to this, because a lot of commands has a loaded stack as a prerequisite.
// This makes it possible to add a mask of prerequisites to the commands in the initCommands.cpp list.
// and assures you for instance that the stack is loaded.
// TODO: refactor so that all commands that need the stack, not loads the stack manually, 
// just add the LOAD_STACK bit to the mask for it.

class Command {
public:
	enum {
		LOAD_STACK = 1
	};

	typedef void (*Proc)(const std::string& args);

	Command();
	Command(Proc p);
	Command(Proc p, int options);
	void operator()(const std::string& args);

private:
	static Command sCommand;
	static void cmdStackLoaded();

	Proc mProc;
	std::string mArgs;
	int mOptions;
};

typedef std::map<std::string, Command>::iterator CommandIterator;
typedef std::pair<std::string, Command> CommandPair;

/**
 * Maps a gdb MI command string to the procedure that
 * should handle the command.
 */
extern std::map<std::string, Command> sCommands;

/**
 * Initializes the command module.
 */
void initCommands();

#endif
