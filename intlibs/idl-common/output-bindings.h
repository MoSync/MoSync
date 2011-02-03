/* Copyright (C) 2011 Mobile Sorcery AB

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

/**
 * File: output-bindings.h
 * Author: Mikael Kindborg
 * Summary: Generation of bindings for dynamic languages.
 * Currently generation of Lua bindings in tolua headerfile format.
 * We could also output XML or whatever format that would help
 * dynamic language developers to make bindings to MoSync syscalls.
 *
 * Instructions for Lua:
 *  Output file is in: "tools/idl2/Output/lua_maapi.pkg"
 *  Install tolua: http://www.tecgraf.puc-rio.br/~celes/tolua/
 *  Copy lua_maapi.pkg to the bin folder in the tolua install
 *  Run command: tolua -o lua_maapi.c lua_maapi.pkg
 *  Copy maapi.c to the MobileLua project
 */

#ifndef OUTPUT_BINDINGS_H
#define OUTPUT_BINDINGS_H

#include "idl-common.h"

void lua_outputHeaderFile(
	const Interface& maapi, 
	const vector<string>& ixs,
	const char* destinationFile);

#endif
