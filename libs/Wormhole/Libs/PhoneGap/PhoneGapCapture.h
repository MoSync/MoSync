/*
Copyright (C) 2011 MoSync AB

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
 * @file PhoneGapCapture.h
 * @author Iraklis Rossis
 *
 * Implementation of PhoneGap capture calls made from JavaScript.
 */

#ifndef WORMHOLE_LIBS_PHONEGAP_PHONEGAPCAPTURE_H_
#define WORMHOLE_LIBS_PHONEGAP_PHONEGAPCAPTURE_H_

#include <Wormhole/WebViewMessage.h>
#include <NativeUI/WebView.h>

namespace Wormhole
{
	class JSONMessage;
	class PhoneGapMessageHandler;

	/**
	 * Class that implements PhoneGap Capture APIs.
	 */
	class PhoneGapCapture: public MAUtil::CustomEventListener
	{
	public:
		/**
		 * Constructor.
		 */
		PhoneGapCapture(PhoneGapMessageHandler* messageHandler);

		/**
		 * Destructor.
		 */
		virtual ~PhoneGapCapture();

		/**
		 * Implementation of Capture API exposed to JavaScript.
		 * @return true if message was handled, false if not.
		 */
		virtual void handleMessage(JSONMessage& message);

		/**
		 * Event handler for capture events
		 * @param event the event struct.
		 */
		virtual void customEvent(const MAEvent &event);

	protected:
		PhoneGapMessageHandler* mMessageHandler;

		/**
		 * A Pointer to the main webview
		 * Used for communicating with PhoneGap
		 */
		NativeUI::WebView* mWebView;

		/**
		 * Stores the CallbackID of the capture call so the result can be sent
		 * after receiving the media data
		 */
		MAUtil::String mCaptureCallBack;
	};
} // namespace

#endif
