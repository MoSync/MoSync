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
 * @file WebViewListener.h
 * @author Emma Tresanszki
 *
 * Listener for WebView events.
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
     * Listener for WebView events.
     */
    class WebViewListener
    {
    public:
        /**
         * Deprecated, use webViewHookInvoked instead.
         * This method is called when the URL has changed.
         * @param webView The web view object that generated the event.
         * @param newURL The new URL.
         */
        virtual void webViewUrlChanged(
            WebView* webView,
            const MAUtil::String& newURL) = 0;

        /**
         * This method is called when MAW_EVENT_WEB_VIEW_HOOK_INVOKED is received.
         * @param hookType The type of hook that has been invoked.
		 * One of:
		 *  -#MAW_CONSTANT_SOFT
		 *  -#MAW_CONSTANT_HARD
		 *
		 * @param urlData The handle to url data. This is string data,
		 * there is NO null terminating character. The encoding
		 * of the data is determined by the application.
		 * The data MUST be deallocated with maDestroyObject after
		 * it hs been used. Each event allocates a new data object
		 * for the url data.
         */
        virtual void webViewHookInvoked(
			WebView* webView,
			int hookType,
			MAHandle urlData) = 0;

        /**
         * This method is called when the web view content is loading.
         * @param webView The web view object that generated the event.
         * Any of the following constants:
         * - #MAW_CONSTANT_STARTED
		 * - #MAW_CONSTANT_DONE
		 * - #MAW_CONSTANT_STOPPED
		 * - #MAW_CONSTANT_ERROR
         */
        virtual void webViewContentLoading(
			WebView* webView,
			const int webViewState) = 0;
    };

} // namespace NativeUI

#endif /* NATIVEUI_WEB_VIEW_LISTENER_H_ */
