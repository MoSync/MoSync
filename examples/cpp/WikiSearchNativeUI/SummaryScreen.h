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
 * @file SummaryScreen.h
 * @author Emma Tresanszki
 *
 * This This file contains the third screen of the application.
 * This screen displays all the text snippets.
 * By clicking on each snippet, a Web view is shown in next screen:WebScreen.
 * A StartOver button is available.
 */

#ifndef SUMMARYSCREENNUI_H_
#define SUMMARYSCREENNUI_H_

// Include the library for event handling listener.
#include "MAUtil/Environment.h"

// Include the class for the next screen.
#include "WebScreen.h"

// Include the base screen class.
#include "BasicScreen.h"

namespace WikiNativeUI
{

// Forward declarations.
class TitleScreen;
class MediaWiki;

/**
 * SummaryScreen: native UI screen
 * It contains the user selected article snippets.
 * Results are displayed in a list view, and each snippet is followed by an anchor.
 * When the anchor is clicked, the whole article becomes available in a web view.
 */
class SummaryScreen : public BasicScreen,
				      public CustomEventListener,
				      public KeyListener
{
public:

	/**
	 * Constructor.
	 * Pass the parent screen and the engine.
	 */
	SummaryScreen(TitleScreen *parentScreen,MediaWiki* engine);

	/**
	 * Destructor.
	 * Destroy the main widget, and all it's children will be destroyed also.
	 */
	~SummaryScreen();

	/**
	 *  Show the screen.
	 *  If it is called from WebScreen it does not need refresh,
	 *  but if from TitleScreen the list box needs refresh.
	 */
	void showScreen(bool needsRefresh);

	/**
	 * Show the home screen.
	 */
	void showHomeScreen();

	/**
	 * From KeyListener.
	 * This function is called with a \link #MAK_FIRST MAK_ code \endlink when
	 * a key is pressed.
	 */
	virtual void keyPressEvent(int keyCode, int nativeCode);

	/**
	 * from CustomEventListener
	 * The custom event listener interface.
	 */
	void customEvent(const MAEvent& event);

	/**
	 * Handle events on screen's widgets.
	 */
	void widgetClicked(MAHandle widgetHandle);

	/**
	 * Fill the list box with data provided by the engine.
	 */
	void fillListBox();

private: // methods

	/**
	 * Lay out the widgets (portrait mode).
	 */
	void setupUI();


private: // members

	/** previous screen. **/
	TitleScreen* mPrevScreen;

	/** Next screen. **/
	WebScreen*   mWebScreen;

	/** NativeUI widgets handles. **/
	MAWidgetHandle mScreen;

	/** Main vertical layout. **/
	MAWidgetHandle mMainLayout;

	/** Label in the top layout. **/
	MAWidgetHandle mLabel;

	/** Button for starting again a search. When clicked, go back to HomeScreen. **/
	MAWidgetHandle mHomeButton;

	/** back button for returning to the previous screen. */
	MAWidgetHandle mBackButton;

	/** The list with all the snippets. **/
	MAWidgetHandle mListView;

	/** The strings for all the snippets in this screen. */
	MAUtil::Vector<MAUtil::String> mSnippets;

	/** The string with all the titles in this screen. */
	MAUtil::Vector<MAUtil::String> mTitles;

	/** All the label widgets containing snippets. */
	MAUtil::Vector<MAWidgetHandle> mSnippetLabels;

	/** Exit button. **/
	MAWidgetHandle mExitButton;

	/** Wiki engine. **/
	MediaWiki*              mWiki;
};

} // namespace WikiNativeUI

#endif /* SUMMARYSCREENNUI_H_ */
