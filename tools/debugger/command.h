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
