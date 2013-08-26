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

/*! \addtogroup WormHoleGroup
 *  @{
 */

/** @defgroup WormHoleGroup Wormhole Library
 *  @{
 */

/**
 * @file OrientationMessageHandler.h
 * @author Bogdan Iusco
 *
 * @brief Handles Javascript orientation calls.
 */

#ifndef WORMHOLE_ORIENTATION_MESSAGE_HANDLER_H_
#define WORMHOLE_ORIENTATION_MESSAGE_HANDLER_H_

#include <MAUtil/Environment.h>

namespace NativeUI
{
	class WebView;
}

namespace Wormhole
{

	class MessageStream;

	/**
	 * @brief Handles Javascript orientation calls.
	 */
	class OrientationMessageHandler:
		public MAUtil::OrientationListener
	{
	public:
		/**
		 * Constructor.
		 * @param webView WebView used to communicate with the Javascript methods.
		 */
		OrientationMessageHandler(NativeUI::WebView* webView);

		/**
		 * Destructor.
		 */
		virtual ~OrientationMessageHandler();

		/**
		 * Implementation of the orientation syscalls exposed to Javascript.
		 * @param message Message containing the JS request.
		 */
		virtual void handleMessage(Wormhole::MessageStream& message);

		/**
		* Called after the screen has finished rotating.
		* \param 'screenOrientation' One of the
		* \link #MA_SCREEN_ORIENTATION_PORTRAIT MA_SCREEN_ORIENTATION \endlink codes.
		*/
		virtual void orientationChanged(int orientation);

		/**
		* Send by current screen just before it begins rotating.
		* Note: available only on iOS platform.
		*/
		virtual void orientationWillChange();

	protected:
		/**
		 * Set application supported orientations.
		 * @param orientation Orientation mask.
		 * The bitmask is created using \link #MA_SCREEN_ORIENTATION_PORTRAIT
		 * MA_SCREEN_ORIENTATION \endlink values.
		 */
		void setSupportedOrientations(const char* orientation);

		/**
		 * Get application supported orientations.
		 * The value will be sent to a Javascript method.
		 */
		void getSupportedOrientations();

		/**
		 * Get application current orientation.
		 * The value will be sent to a Javascript method.
		 */
		void getCurrentOrientation();

		/**
		 * Check if it's needed to register for orientation events. If so, this
		 * will be registered.
		 */
		void registerListener();

		/**
		 * Check if it's needed to unregister for orientation events. If so, this
		 * will be unregistered.
		 */
		void unregisterListener();

	private:
		/**
		 * WebView used to communicate with the Javascript methods.
		 */
		NativeUI::WebView& mWebView;

		/**
		 * Flag to indicate if object is registered for receiving orientation
		 * events.
		 */
		bool mIsListenerRegistered;

		/**
		 * Flag to indicate if there are listeners for orientation did change
		 * event on the JS side.
		 */
		bool mRegisteredForDidChange;

		/**
		 * Flag to indicate if there are listeners for orientation will change
		 * event on the JS side.
		 */
		bool mRegisteredForWillChange;
	};

} // end of Wormhole namespace

#endif

/*! @} */
