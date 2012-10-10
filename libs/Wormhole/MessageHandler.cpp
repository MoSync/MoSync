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
 * @file MessageHandler.cpp
 * @author Mikael Kindborg
 *
 * Class that abstracts away some of the details of processing
 * messages from Wormhole (JavaScript).
 */

#include "CustomMoblet.h"
#include "MessageHandler.h"
#include "HybridMoblet.h"

namespace Wormhole
{

MessageHandler::MessageHandler() :
	mPhoneGapMessageHandler(NULL),
	mNativeUIMessageHandler(NULL),
	mResourceMessageHandler(NULL)
{
}

MessageHandler::~MessageHandler()
{
	if (mPhoneGapMessageHandler)
	{
		delete mPhoneGapMessageHandler;
		mPhoneGapMessageHandler = NULL;
	}
	if (mNativeUIMessageHandler)
	{
		delete mNativeUIMessageHandler;
		mNativeUIMessageHandler = NULL;
	}
	if (mResourceMessageHandler)
	{
		delete mResourceMessageHandler;
		mResourceMessageHandler = NULL;
	}
}

void MessageHandler::initialize(Wormhole::HybridMoblet* moblet)
{
	NativeUI::WebView* webView = moblet->getWebView();
	mPhoneGapMessageHandler = new PhoneGapMessageHandler(webView);
	mNativeUIMessageHandler = new NativeUIMessageHandler(webView);
	mResourceMessageHandler = new ResourceMessageHandler(webView);
}

void MessageHandler::openWormhole(Wormhole::HybridMoblet* moblet)
{
	// Send the Device Screen size to JavaScript.
	MAExtent scrSize = maGetScrSize();
	int width = EXTENT_X(scrSize);
	int height = EXTENT_Y(scrSize);
	char buf[512];
	sprintf(
		buf,
		"mosync.nativeui.setScreenSize(%d,%d)",
		//"{mosyncScreenWidth=%d; mosyncScreenHeight=%d;}",
		width,
		height);
	moblet->callJS(buf);

	// Initialize PhoneGap.
	mPhoneGapMessageHandler->initializePhoneGap();
}

void MessageHandler::setBeepSound(MAHandle beepSound)
{
	// Set beep sound resource.
	mPhoneGapMessageHandler->setBeepSound(beepSound);
}

void MessageHandler::addMessageFun(
	const char* command,
	FunTable::MessageHandlerFun fun)
{
	mFunTable.addMessageFun(command, fun);
}

void MessageHandler::callMessageFun(
	const char* command,
	Wormhole::MessageStream& stream,
	Wormhole::HybridMoblet* moblet)
{
	mFunTable.callMessageFun(command, stream, moblet);
}

void MessageHandler::keyPressEvent(int keyCode, int nativeCode)
{
	// Forward to PhoneGap MessageHandler.
	mPhoneGapMessageHandler->processKeyEvent(keyCode, nativeCode);
}

/**
 * This method handles messages sent from the WebView.
 *
 * Note that the data object will be valid only during
 * the life-time of the call of this method, then it
 * will be deallocated.
 *
 * @param webView The WebView that sent the message.
 * @param urlData Data object that holds message content.
 */
void MessageHandler::handleWebViewMessage(
	NativeUI::WebView* webView,
	MAHandle data,
	Wormhole::HybridMoblet* moblet)
{
	// Check the message protocol.
	Wormhole::MessageProtocol protocol(data);
	if (protocol.isMessageStreamJSON())
	{
		handleMessageStreamJSON(webView, data);
	}
	else if (protocol.isMessageStream())
	{
		handleMessageStream(webView, data, moblet);
	}
}

/**
 * Handles JSON messages. This is used by PhoneGap.
 *
 * @param webView A pointer to the web view posting this message.
 * @param data The raw encoded JSON message array.
 */
void MessageHandler::handleMessageStreamJSON(
	NativeUI::WebView* webView,
	MAHandle data)
{
	// Create the message object. This parses the message data.
	// The message object contains one or more messages.
	Wormhole::JSONMessage message(webView, data);

	// Loop through messages.
	while (message.next())
	{
		// This detects the PhoneGap protocol.
		if (message.is("PhoneGap"))
		{
			mPhoneGapMessageHandler->handlePhoneGapMessage(message);
		}
	}
}

/**
 * Handles string stream messages (generally faster than JSON messages).
 * This is used by the JavaScript NativeUI system.
 *
 * You can send your own messages from JavaScript and handle them here.
 *
 * @param webView A pointer to the web view posting this message.
 * @param data The raw encoded stream of string messages.
 */
void MessageHandler::handleMessageStream(
	NativeUI::WebView* webView,
	MAHandle data,
	Wormhole::HybridMoblet* moblet)
{
	// Create a message stream object. This parses the message data.
	// The message object contains one or more strings.
	Wormhole::MessageStream stream(webView, data);

	// Pointer to a string in the message stream.
	const char* p;

	// Process messages while there are strings left in the stream.
	while (p = stream.getNext())
	{
		if (0 == strcmp(p, "CallJS"))
		{
			// Call to evaluate JavaScript in another WebView.
			// This is used to communicate between WebViews.
			handleCallJSMessage(stream, webView, moblet);
		}
		else if (0 == strcmp(p, "NativeUI"))
		{
			// Forward NativeUI messages.
			mNativeUIMessageHandler->handleMessage(stream);
		}
		else if (0 == strcmp(p, "Resource"))
		{
			// Forward Resource messages.
			mResourceMessageHandler->handleMessage(stream);
		}
		else if (0 == strcmp(p, "Custom"))
		{
			// Lookup and call function to handle custom message.
			const char* command = stream.getNext();
			if (NULL != command)
			{
				callMessageFun(
					command,
					stream,
					moblet);
			}
		}
		else if (0 == strcmp(p, "MoSync"))
		{
			// Handle messages specific to MoSync.
			handleMoSyncMessage(stream, webView, moblet);
		}
	}
}

void MessageHandler::handleMoSyncMessage(
	Wormhole::MessageStream& message,
	NativeUI::WebView* webView,
	Wormhole::HybridMoblet* moblet)
{
	const char* p = message.getNext();

	if (0 == strcmp(p, "ExitApplication"))
	{
		// Close the application.
		Wormhole::CustomMoblet::close();
	}
	else if (0 == strcmp(p, "SendToBackground"))
	{
		maSendToBackground();
	}
	else if (0 == strcmp(p, "OpenWormhole"))
	{
		moblet->openWormhole(moblet);
	}
}

/**
 * Invoked from JavaScript to evaluate a JS script in a WebView.
 */
void MessageHandler::handleCallJSMessage(
	Wormhole::MessageStream& message,
	NativeUI::WebView* webView,
	Wormhole::HybridMoblet* moblet)
{
	// TODO: Add error handling (missing parameters).

	// Get the native MoSync widget handle for the WebView
	// this call should be forwarded to.
	int webViewHandle = MAUtil::stringToInteger(message.getNext());

	// When the handle is zero, we use the main WebView
	// (which is hidden in a NativeUI app).
	if (0 == webViewHandle)
	{
		webViewHandle = moblet->getWebView()->getWidgetHandle();
	}

	// Evaluate the JavaScript code in the WebView.
	const char* script = message.getNext();
	moblet->callJS(webViewHandle, script);
}

} // namespace
