/*
Copyright (C) 2012 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/**
 * @file FunTable.cpp
 * @author Mikael Kindborg
 *
 * Class that abstracts away some of the details of processing
 * messages from Wormhole (JavaScript).
 */

#include <mastring.h>		// C string functions
#include <mavsprintf.h>		// C string functions
#include <mastdlib.h>		// C string conversion functions

#include "FunTable.h"

namespace Wormhole
{

FunTable::FunTable()
{
}

FunTable::~FunTable()
{
}

void FunTable::addMessageFun(
	const char* command,
	MessageHandlerFun fun)
{
	mFunMap[command] = fun;
}

void FunTable::callMessageFun(
	const char* command,
	Wormhole::MessageStream& stream,
	FunObject* object)
{
	if (mFunMap.find(command) != mFunMap.end())
	{
		MessageHandlerFun fun = mFunMap[command];
		(object->*fun)(stream);
	}
	else
	{
		const char* message = "@@@ MoSync: Could not find Wormhole message function:";
		maWriteLog(message, strlen(message));
		maWriteLog(command, strlen(command));
	}
}

} // namespace
