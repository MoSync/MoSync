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
 * @file MessageHandler.h
 * @author Mikael Kindborg
 *
 * Class that abstracts away some of the details of processing
 * messages from Wormhole (JavaScript).
 */

#ifndef WORMHOLE_MESSAGE_HANDLER_H_
#define WORMHOLE_MESSAGE_HANDLER_H_

#include <MAUtil/String.h>
#include <MAUtil/HashMap.h>
#include <NativeUI/WebView.h>
#include "WebAppMoblet.h"
#include "MessageProtocol.h"
#include "MessageStream.h"
#include "Libs/JSONMessage.h"
#include "Libs/PhoneGap/PhoneGapMessageHandler.h"
#include "Libs/JSNativeUI/NativeUIMessageHandler.h"
#include "Libs/JSNativeUI/ResourceMessageHandler.h"
#include "Libs/Extensions/ExtensionMessageHandler.h"
#include "Libs/Orientation/OrientationMessageHandler.h"
#include "Encoder.h"
#include "FunTable.h"

namespace Wormhole
{

class HybridMoblet;

/**
 * Handler for Wormhole messages of type "Custom".
 *
 * Members are made virtual and public by design, to give
 * maximum flexibility to users of this class. Do not access
 * instance variables directly, unless absolutely needed.
 */
class MessageHandler
{
public:
	/**
	 * Constructor.
	 */
	MessageHandler();

	/**
	 * Destructor.
	 */
	virtual ~MessageHandler();

	/**
	 * Perform initialization of the message handler.
	 */
	virtual void initialize(Wormhole::HybridMoblet* moblet);

	/**
	 * Get the PhoneGap message handler.
	 */
	virtual PhoneGapMessageHandler* getPhoneGapMessageHandler();

	/**
	 * Set the PhoneGap message handler.
	 */
	virtual void setPhoneGapMessageHandler(PhoneGapMessageHandler* handler);

	/**
	 * Get the Native UI message handler.
	 */
	virtual NativeUIMessageHandler* getNativeUIMessageHandler();

	/**
	 * Set the Native UI message handler.
	 */
	virtual void setNativeUIMessageHandler(NativeUIMessageHandler* handler);

	/**
	 * Get the Resource message handler.
	 */
	virtual ResourceMessageHandler* getResourceMessageHandler();

	/**
	 * Set the Resource message handler.
	 */
	virtual void setResourceMessageHandler(ResourceMessageHandler* handler);

	/**
	 * Set the object to get notified when log messages are sent.
	 *
	 * Note that the MessageHandler will take ownership of
	 * the listener and delete it upon destruction. Also, when a new
	 * listener is set, the old listener will be deleted.
	 *
	 * @param listener The log message listener.
	 */
	virtual void setLogMessageListener(LogMessageListener* listener);

	/**
	 * Called when document in main WebView is loaded. Here we
	 * perform initialization of Wormhole that needs to be done
	 * when all JS has been loaded.
	 */
	virtual void openWormhole(
		MAWidgetHandle webViewHandle,
		Wormhole::HybridMoblet* moblet);

	/**
	 * Set the sound used by the PhoneGap beep notification.
	 */
	virtual void setBeepSound(MAHandle beepSound);

	/**
	 * Turn on processing of Native UI events.
	 */
	virtual void nativeUIEventsOn();

	/**
	 * Turn off processing of Native UI events.
	 */
	virtual void nativeUIEventsOff();

	/**
	 * Add a message function callback to be invoked from
	 * JavaScript using the "Custom" protocol.
	 */
	virtual void addMessageFun(
		const char* command,
		Wormhole::FunTable::MessageHandlerFun fun);

	/**
	 * Called a registered message function invoked
	 * via the "Custom" protocol. Used internally.
	 */
	virtual void callMessageFun(
		const char* command,
		Wormhole::MessageStream& stream,
		Wormhole::HybridMoblet* moblet);

	/**
	 * Handle key press events.
	 */
	virtual void keyPressEvent(int keyCode, int nativeCode);

	/**
	 * This method handles messages sent from the WebView.
	 *
	 * Note that the data object will be valid only during
	 * the life-time of the call of this method, then it
	 * will be deallocated.
	 *
	 * @param webViewHandle The NativeUI widget handle of
	 * the WebView that sent the message.
	 * @param data Data object that holds message content.
	 * @param moblet The moblet object.
	 */
	virtual void handleWebViewMessage(
		MAWidgetHandle webViewHandle,
		MAHandle data,
		Wormhole::HybridMoblet* moblet);

	/**
	 * Handle messages in JSON format. PhoneGap
	 * uses this format.
	 */
	virtual void handleMessageStreamJSON(
		MAWidgetHandle webViewHandle,
		MAHandle data);

	/**
	 * Handle messages in string stream format.
	 */
	virtual void handleMessageStream(
		MAWidgetHandle webViewHandle,
		MAHandle data,
		Wormhole::HybridMoblet* moblet);

	/**
	 * Handle messages with the "MoSync" protocol.
	 */
	virtual void handleMoSyncMessage(
		Wormhole::MessageStream& message,
		MAWidgetHandle webViewHandle,
		Wormhole::HybridMoblet* moblet);

	/**
	 * Invoked from JavaScript using the "CallJS" protocol
	 * to evaluate a script in a WebView.
	 */
	virtual void handleCallJSMessage(
		Wormhole::MessageStream& message,
		Wormhole::HybridMoblet* moblet);

protected:
	/**
	 * Handler for PhoneGap messages.
	 */
	Wormhole::PhoneGapMessageHandler* mPhoneGapMessageHandler;

	/**
	 * Handler for NativeUI messages.
	 */
	Wormhole::NativeUIMessageHandler* mNativeUIMessageHandler;

	/**
	 * Handler for resource messages used for NativeUI.
	 */
	Wormhole::ResourceMessageHandler* mResourceMessageHandler;

	/**
	 * Handler for extension messages used for JS extensions.
	 * This handler cannot be replaced.
	 */
	Wormhole::ExtensionMessageHandler* mExtensionMessageHandler;

	/**
	 * Dictionary with pointers to message handler functions.
	 * command -> fun
	 */
	Wormhole::FunTable mFunTable;

	/**
	 * Handler for orientation related messages.
	 */
	Wormhole::OrientationMessageHandler* mOrientationMessageHandler;
};

} // namespace

#endif
