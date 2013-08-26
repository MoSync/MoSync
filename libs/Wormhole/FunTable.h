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

protected:
	/**
	 * Dictionary with pointers to message handler functions.
	 * command -> fun
	 */
	MAUtil::HashMap<MAUtil::String, MessageHandlerFun> mFunMap;
};

} // namespace

#endif
