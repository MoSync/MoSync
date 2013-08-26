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
