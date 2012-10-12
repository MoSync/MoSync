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
 * @file FunTable.h
 * @author Mikael Kindborg
 *
 * Class that abstracts away some of the details of processing
 * messages from Wormhole (JavaScript).
 */

#ifndef WORMHOLE_FUNCTION_TABLE_H_
#define WORMHOLE_FUNCTION_TABLE_H_

#include <NativeUI/WebView.h>
#include <Wormhole/WebAppMoblet.h>
#include <Wormhole/MessageProtocol.h>
#include <Wormhole/MessageStream.h>
#include <Wormhole/Libs/JSONMessage.h>
#include <Wormhole/Libs/PhoneGap/PhoneGapMessageHandler.h>
#include <Wormhole/Libs/JSNativeUI/NativeUIMessageHandler.h>
#include <Wormhole/Libs/JSNativeUI/ResourceMessageHandler.h>
#include <Wormhole/Encoder.h>
#include <MAUtil/String.h>
#include <MAUtil/HashMap.h>

namespace Wormhole
{

/**
 * Abstract superclass for classes that has callback methods
 * to be invoked from JavaScript.
 */
class FunObject
{
};

/**
 * Handler for Wormhole messages of type "Custom".
 */
class FunTable
{
public:
	/**
	 * Message handling function type.
	 */
	typedef void (FunObject::*MessageHandlerFun)
		(Wormhole::MessageStream& stream);

	FunTable();

	virtual ~FunTable();

	void addMessageFun(
		const char* command,
		MessageHandlerFun fun);

	void callMessageFun(
		const char* command,
		Wormhole::MessageStream& stream,
		FunObject* object);

private:
	/**
	 * Dictionary with pointers to message handler functions.
	 * command -> fun
	 */
	MAUtil::HashMap<MAUtil::String, MessageHandlerFun> mFunMap;
};

} // namespace

#endif
