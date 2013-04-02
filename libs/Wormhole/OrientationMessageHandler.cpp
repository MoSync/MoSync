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
 * @file OrientationMessageHandler.cpp
 * @author Bogdan Iusco
 *
 * @brief Handles Javascript orientation calls.
 */

#include <NativeUI/WebView.h>
#include <MAUtil/util.h>

#include "MessageStream.h"
#include "OrientationMessageHandler.h"

namespace Wormhole
{

	/**
	 * Constructor.
	 * @param webView WebView used to communicate with the Javascript methods.
	 */
	OrientationMessageHandler::OrientationMessageHandler(NativeUI::WebView* webView):
		mWebView(*webView),
		mIsListenerRegistered(false),
		mRegisteredForDidChange(false),
		mRegisteredForWillChange(false)
	{

	}

	/**
	 * Destructor.
	 */
	OrientationMessageHandler::~OrientationMessageHandler()
	{
		if (mIsListenerRegistered)
		{
			MAUtil::Environment::getEnvironment().removeOrientationListener(this);
		}
	}

	/**
	 * Implementation of the orientation syscalls exposed to Javascript.
	 * @param message Message containing the JS request.
	 */
	void OrientationMessageHandler::handleMessage(Wormhole::MessageStream& message)
	{
		const char* functionality = message.getNext();
		if (0 == strcmp(functionality, "ScreenSetOrientation"))
		{
			setSupportedOrientations(message.getNext());
		}
		else if (0 == strcmp(functionality, "ScreenGetOrientation"))
		{
			getSupportedOrientations();
		}
		else if (0 == strcmp(functionality, "ScreenGetCurrentOrientation"))
		{
			getCurrentOrientation();
		}
		else if (0 == strcmp(functionality, "RegisterOrientationDidChange"))
		{
			mRegisteredForDidChange = true;
			registerListener();
		}
		else if (0 == strcmp(functionality, "UnregisterOrientationDidChange"))
		{
			mRegisteredForDidChange = false;
			unregisterListener();
		}
		else if (0 == strcmp(functionality, "RegisterOrientationWillChange"))
		{
			mRegisteredForWillChange = true;
			registerListener();
		}
		else if (0 == strcmp(functionality, "UnregisterOrientationWillChange"))
		{
			mRegisteredForWillChange = false;
			unregisterListener();
		}
	}

	/**
	* Called after the screen has finished rotating.
	* \param 'screenOrientation' One of the
	* \link #MA_SCREEN_ORIENTATION_PORTRAIT MA_SCREEN_ORIENTATION \endlink codes.
	*/
	void OrientationMessageHandler::orientationChanged(int orientation)
	{
		char buffer[1024];
		sprintf(buffer, "try{mosync.app.orientationDidChange(%d)}catch(e){}",
				orientation);
		mWebView.callJS(buffer);
	}

	/**
	* Send by current screen just before it begins rotating.
	* Note: available only on iOS platform.
	*/
	void OrientationMessageHandler::orientationWillChange()
	{
		char buffer[1024];
		sprintf(buffer, "try{mosync.app.orientationWillChange()}catch(e){}");
		mWebView.callJS(buffer);
	}

	/**
	 * Set application supported orientations.
	 * @param orientation Orientation mask.
	 * The bitmask is created using \link #MA_SCREEN_ORIENTATION_PORTRAIT
	 * MA_SCREEN_ORIENTATION \endlink values.
	 */
	void OrientationMessageHandler::setSupportedOrientations(const char* orientation)
	{
		int orientationMask = MAUtil::stringToInteger(orientation);
		maScreenSetSupportedOrientations(orientationMask);
	}

	/**
	 * Get application supported orientations.
	 * The value will be sent to a Javascript method.
	 */
	void OrientationMessageHandler::getSupportedOrientations()
	{
		int supportedOrientations = maScreenGetSupportedOrientations();
		char buffer[1024];
		sprintf(buffer, "try{mosync.app.supportedOrientation(%d)}catch(e){}",
				supportedOrientations);
		mWebView.callJS(buffer);
	}

	/**
	 * Get application current orientation.
	 * The value will be sent to a Javascript method.
	 */
	void OrientationMessageHandler::getCurrentOrientation()
	{
		int currentOrientation = maScreenGetCurrentOrientation();
		char buffer[1024];
		sprintf(buffer, "try{mosync.app.currentOrientation(%d)}catch(e){}",
				currentOrientation);
		mWebView.callJS(buffer);
	}

	/**
	 * Check if it's needed to register for orientation events. If so, this
	 * will be registered.
	 */
	void OrientationMessageHandler::registerListener()
	{
		if (!mIsListenerRegistered &&
			(mRegisteredForDidChange || mRegisteredForWillChange))
		{
			MAUtil::Environment::getEnvironment().addOrientationListener(this);
			mIsListenerRegistered = true;
		}
	}

	/**
	 * Check if it's needed to unregister for orientation events. If so, this
	 * will be unregistered.
	 */
	void OrientationMessageHandler::unregisterListener()
	{
		if (mIsListenerRegistered    &&
			!mRegisteredForDidChange &&
			!mRegisteredForWillChange)
		{
			MAUtil::Environment::getEnvironment().removeOrientationListener(this);
			mIsListenerRegistered = false;
		}
	}

} // end of Wormhole namespace
