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
 * @file MyMessageHandler.h
 * @author Mikael Kindborg
 *
 * Class that abstracts away some of the details of processing
 * messages from Wormhole (JavaScript).
 */

#ifndef MY_MESSAGE_HANDLER_H_
#define MY_MESSAGE_HANDLER_H_

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

/**
 * Message handling function type.
 */
typedef void (Wormhole::WebAppMoblet::*MyMessageHandlerFun)
	(Wormhole::MessageStream& stream);

/**
 * Handler for Wormhole messages of type "Custom".
 */
class MyMessageHandler
{
public:
	MyMessageHandler(NativeUI::WebView* webView);

	virtual ~MyMessageHandler();

	void init(
		MAHandle beepSound,
		Wormhole::WebAppMoblet* moblet);

	void addMessageFun(
		const char* command,
		MyMessageHandlerFun fun);

	void callMessageFun(
		const char* command,
		Wormhole::MessageStream& stream,
		Wormhole::WebAppMoblet* moblet);

	void keyPressEvent(int keyCode, int nativeCode);

	void handleWebViewMessage(
		NativeUI::WebView* webView,
		MAHandle data,
		Wormhole::WebAppMoblet* moblet);

	void handleMessageStreamJSON(
		NativeUI::WebView* webView,
		MAHandle data);

	void handleMessageStream(
		NativeUI::WebView* webView,
		MAHandle data,
		Wormhole::WebAppMoblet* moblet);

private:
	/**
	 * Handler for PhoneGap messages.
	 */
	Wormhole::PhoneGapMessageHandler mPhoneGapMessageHandler;

	/**
	 * Handler for NativeUI messages.
	 */
	Wormhole::NativeUIMessageHandler mNativeUIMessageHandler;

	/**
	 * Handler for resource messages used for NativeUI.
	 */
	Wormhole::ResourceMessageHandler mResourceMessageHandler;

	/**
	 * Dictionary with pointers to message handler functions.
	 * command -> fun
	 */
	MAUtil::HashMap<MAUtil::String, MyMessageHandlerFun> mFunMap;
};

#endif
