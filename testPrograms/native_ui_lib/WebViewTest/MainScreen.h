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
 * @file MainScreen.h
 * @author Bogdan Iusco.
 */

#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

#include <maapi.h>
#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>

using namespace NativeUI;

class MainScreen:
	public Screen,
	public WebViewListener,
	public ButtonListener
{

public:
	/**
	 * Constructor.
	 */
		MainScreen();

	/**
	 * Destructor.
	 */
	~MainScreen();

private:

	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();

	// From WebViewListener
    /**
     * Deprecated, use webViewHookInvoked instead.
     * This method is called when the URL has changed.
     * @param webView The web view object that generated the event.
     * @param newURL The new URL.
     */
    virtual void webViewUrlChanged(
        WebView* webView,
        const MAUtil::String& newURL);

    /**
     * This method is called when MAW_EVENT_WEB_VIEW_HOOK_INVOKED is received.
     * @param hookType The type of hook that has been invoked.
	 * One of:
	 *  -#MAW_CONSTANT_SOFT
	 *  -#MAW_CONSTANT_HARD
	 *
	 *  @param urlData The handle to url data. This is string data,
	 * there is NO null terminating character. The encoding
	 * of the data is determined by the application.
	 * The data MUST be deallocated with maDestroyObject after
	 * it hs been used. Each event allocates a new data object
	 * for the url data.
     */
    virtual void webViewHookInvoked(
		WebView* webView,
		int hookType,
		MAHandle urlData);

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
		const int webViewState);

    // From ButtonListener
    /**
     * This method is called when there is an touch-down event for
     * a button.
     * Only for iphone platform.
     * @param button The button object that generated the event.
     */
    virtual void buttonPressed(Widget* button);

    /**
     * This method is called when there is an touch-up event for
     * a button.
     * Only for iphone platform.
     * @param button The button object that generated the event.
     */
    virtual void buttonReleased(Widget* button);

    /**
     * This method is called if the touch-up event was inside the
     * bounds of the button.
     * @param button The button object that generated the event.
     */
    virtual void buttonClicked(Widget* button);
private:
	/**
	 * Main layout.
	 */
	VerticalLayout* mMainLayout;

	WebView* mWebView;

	Label* mHookReceived;
	Label* mLoadingState;

	Button* mEnableVerticalScroll;
	Button* mEnableHorizontalScroll;

	Button* mOpenUrl;
	Button* mSetHardHook;
	Button* mSetSoftHook;
};


#endif /* MAINSCREEN_H_ */
