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

/*! \addtogroup NativeUILib
 *  @{
 */

/**
 *  @defgroup NativeUILib Native UI Library
 *  @{
 */

/**
* @file WebViewListener.h
* @author Emma Tresanszki
*
* \brief Listener for WebView events.
*
*/

#ifndef NATIVEUI_WEB_VIEW_LISTENER_H_
#define NATIVEUI_WEB_VIEW_LISTENER_H_

#include <MAUtil/String.h>

namespace NativeUI
{
	// Forward declaration.
	class WebView;

	/**
	* \brief Listener for WebView events.
	*/
	class WebViewListener
	{
	public:
		/**
		 * Constructor.
		 */
		WebViewListener()
		{
		}

		/**
		 * We need to define an virtual destructor for
		 * the library to build in Debug mode.
		 */
		virtual ~WebViewListener()
		{
		}

		/**
		* This method is called when MAW_EVENT_WEB_VIEW_HOOK_INVOKED is received.
		* @param webView The web view object that generated the event.
		* @param hookType The type of hook that has been invoked.
		* One of:
		*  -#MAW_CONSTANT_SOFT
		*  -#MAW_CONSTANT_HARD
		*
		* @param urlData The handle to url data. This is string data,
		* there is NO null terminating character. The encoding
		* of the data is determined by the application. The data
		* object is only valid during the duration of this call.
		*/
		virtual void webViewHookInvoked(
			WebView* webView,
			int hookType,
			MAHandle urlData)
		{
		}

		/**
		* This method is called when the web view content is loading.
		* @param webView The web view object that generated the event.
		* @param webViewState  Any of the following constants:
		* - #MAW_CONSTANT_STARTED
		* - #MAW_CONSTANT_DONE
		* - #MAW_CONSTANT_STOPPED
		* - #MAW_CONSTANT_ERROR
		*/
		virtual void webViewContentLoading(
			WebView* webView,
			const int webViewState)
		{
		}
	};

} // namespace NativeUI

#endif /* NATIVEUI_WEB_VIEW_LISTENER_H_ */

/*! @} */
