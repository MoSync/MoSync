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
typedef void (*Command)(const std::string& args);

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