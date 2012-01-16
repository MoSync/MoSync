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
