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
 * @file WebScreen.cpp
 * @author Emma Tresanszki
 *
 * This file contains the last screen of the application
 * It contains a top layout with a hint label and two buttons, and a web view.
 */

// Include library for string conversions.
#include <mastdlib.h>

// Include util class for integer to string conversions.
#include <MAUtil/util.h>

// Include the resources for images.
//#include "MAHeaders.h"

// Utility functions.
#include "Util.h"

// The previous screen.
#include "SummaryScreen.h"

// The header of this class.
#include "WebScreen.h"

namespace WikiNativeUI {

/**
 * Constructor.
 * Pass the parent screen.
 */
WebScreen::WebScreen(SummaryScreen *parentScreen):
		BasicScreen(),
		mPrevScreen(parentScreen),
		mScreen(-1),
		mMainLayout(-1),
		mLabel(-1),
		mBackButton(-1),
		mNewSearchButton(-1)
{
	// Initialize the native UI.
	setupUI();

	//Make the  screen listen for events.
	MAUtil::Environment::getEnvironment().addCustomEventListener(this);
}

/**
 * Destructor.
 */
WebScreen::~WebScreen()
{
	// The base d-tor is called, and it destroys the mMainLayout;
}

/**
 * Lay out the widgets (portrait mode).
 */
void WebScreen::setupUI()
{

	// Get the handle to the main layout and the screen.
	mMainLayout = getMainLayout();
	mScreen = getScreen();
	mLabel = getTopLabel();
	mBackButton = getTopButtonLeft();
	mNewSearchButton = getTopButtonRight();

	setButtonText(mBackButton, " BACK ");
	setButtonText(mNewSearchButton, "New Search");

	// Add the web view that contains the article.
	// It will be filled when this screen is displayed,
	// based on the checked titles from TitlesScreen.
	mWebView = createWebView(
		MAW_CONSTANT_FILL_AVAILABLE_SPACE, MAW_CONSTANT_FILL_AVAILABLE_SPACE);

	maWidgetAddChild(mMainLayout, mWebView);
}

/**
 * Show the screen.
 */
void WebScreen::showScreen()
{
	// Make the screen listen for key events.
	MAUtil::Environment::getEnvironment().addKeyListener(this);
	// When this screen is shown, the url is set to the web view.
	BasicScreen::showScreen();
}

/**
 * Open a web view for a certain title
 * compose the url and display
 * @param The article title for which we want to open a wikipedia definition.
 */
void WebScreen::openWebView(MAUtil::String title)
{
	MAUtil::String url = "http://en.wikipedia.org/wiki/" + title;

	// Replace spaces.
	replaceString(url," ","%20");

	// Display the article.
	maWidgetSetProperty(mWebView,MAW_WEB_VIEW_URL,url.c_str() );
}

/**
 * From KeyListener.
 * This function is called with a \link #MAK_FIRST MAK_ code \endlink when
 * a key is pressed.
 */
void WebScreen::keyPressEvent(int keyCode, int nativeCode)
{
    // Go back to the SummaryScreeb if the back key is pressed.
    if(MAK_BACK == keyCode)
    {
		// Unregister from key listener.
		MAUtil::Environment::getEnvironment().removeKeyListener(this);
		// Go back, and do not refresh the list in the SummaryScreen.
		mPrevScreen->showScreen(false);
    }
}

/**
 * from CustomEventListener
 * The custom event listener interface.
 */
void WebScreen::customEvent(const MAEvent& event)
{
	//If the event does not come from a widget, we just ignore it.
	if (event.type != EVENT_TYPE_WIDGET) {
		return;
	}

	// Get the information sent by the widget.
	MAWidgetEventData *widgetEventData = (MAWidgetEventData *) event.data;

	// Check that the event was sent from a widget...
	if (widgetEventData->eventType == MAW_EVENT_CLICKED) {
		// Handle the event emitted by the widget.
		widgetClicked(widgetEventData->widgetHandle);
	}
	else if (MAW_EVENT_WEB_VIEW_CONTENT_LOADING == widgetEventData->eventType ){
		// Refresh text in the top label.
		setLabelText(mLabel, MESSAGE_BACK_HINT.c_str());
	}
	else if ( MAW_EVENT_WEB_VIEW_URL_CHANGED == widgetEventData->eventType){
		 switch (widgetEventData->status)
		 {
		 case MAW_CONSTANT_STARTED:
			 setLabelText(mLabel, MESSAGE_PAGE_LOADING.c_str());
			 break;
		 case MAW_CONSTANT_STOPPED:
			 setLabelText(mLabel, MESSAGE_LOADING_STOPPED.c_str());
			 break;
		 case MAW_CONSTANT_ERROR:
			 setLabelText(mLabel, MESSAGE_LOADING_ERROR.c_str());
			 break;
		 case MAW_CONSTANT_DONE:
			 setLabelText(mLabel, MESSAGE_BACK_HINT.c_str());
			 break;
		 }
	}
	return;
}

/**
 * Handle events on screen's widgets.
 */
void WebScreen::widgetClicked(MAHandle widgetHandle)
{
	if ( widgetHandle == mBackButton )
	{

		// Go back, and do not refresh the list in the SummaryScreen.
		mPrevScreen->showScreen(false);
	}
	else if ( widgetHandle == mNewSearchButton )
	{
		// Unregister from key listener.
		MAUtil::Environment::getEnvironment().removeKeyListener(this);
		// Go back to HomeScreen.
		mPrevScreen->showHomeScreen();
	}
}

} // namespace WikiNativeUI
