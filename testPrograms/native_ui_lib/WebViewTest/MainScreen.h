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
