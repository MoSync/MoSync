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
 * @file HybridMoblet.h
 *
 * @author Mikael Kindborg
 */

#ifndef WORMHOLE_CUSTOM_WEB_APP_MOBLET_H_
#define WORMHOLE_CUSTOM_WEB_APP_MOBLET_H_

#include <ma.h>
#include <maassert.h>
#include <MAUtil/Environment.h>
#include <MAUtil/Vector.h>

#include "WebAppMoblet.h"
#include "MessageHandler.h"

namespace Wormhole
{

/**
 * Moblet designed to make it easy to add custom C++ code that
 * can be invoked from JavaScript.
 *
 * Members are made virtual and public by design, to give
 * maximum flexibility to users of this class. Do not access
 * instance variables directly, unless absolutely needed.
 */
class HybridMoblet :
	public WebAppMoblet,
	public FunObject
{
public:
	/**
	 * Constructor.
	 */
	HybridMoblet();

	/**
	 * Destructor.
	 */
	virtual ~HybridMoblet();

	/**
	 * Display a page. Will automatically call initialize()
	 * if it has not been called.
	 */
	virtual void showPage(const MAUtil::String& url);

	/**
	 * Initialize the moblet.
	 */
	virtual void initialize();

	/**
	 * Initialize the Wormhole JS library. Should be
	 * called after the page has been displayed.
	 */
	virtual void openWormhole(
		Wormhole::HybridMoblet* moblet);

	/**
	 * Set the sound used for the PhoneGap beep notification.
	 *
	 * @param beepSoundResource Handle to the "beep" sound
	 * played by the PhoneGap API.
	 */
	virtual void setBeepSound(MAHandle beepSoundResource);

	/**
	 * Add a function invoked when a message is sent from JavaScript.
	 *
	 * Invoke from JavaScript using this format:
	 *
	 *   mosync.bridge.send(["Custom", "Vibrate"]);
	 *
	 * First parameter must always be "Custom". Then the string
	 * (command name) used to identify the function. Optionally
	 * you can send additional parameters and retrieve them
	 * from the message stream.
	 *
	 * @param command String identifying the command.
	 * @param fun Function of type MessageHandlerFun. This function must
	 * be a member of a subclass of WebAppMoblet.
	 */
	virtual void addMessageFun(
		const char* command,
		Wormhole::FunTable::MessageHandlerFun fun);

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
	virtual void handleWebViewMessage(
		NativeUI::WebView* webView,
		MAHandle data);

	/**
	 * This method is called when a key is pressed.
	 * Eventually forwards the event to PhoneGapMessageHandler.
	 */
	virtual void keyPressEvent(int keyCode, int nativeCode);

	/**
	 * Handles messages sent from JavaScript.
	 */
	MessageHandler mMessageHandler;

	/**
	 * Is the moblet initialized?
	 */
	bool mInitialized;

}; // class

} // namespace

#endif
