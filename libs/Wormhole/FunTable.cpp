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
