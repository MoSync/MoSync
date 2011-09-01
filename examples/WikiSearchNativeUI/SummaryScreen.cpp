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
 * @file SummaryScreen.cpp
 * @author Emma Tresanszki
 *
 * This This file contains the third screen of the application.
 * This screen displays all the text snippets.
 * By clicking on each snippet, a Web view is shown in next screen:WebScreen.
 * A StartOver button is available.
 */

// Include library for string conversions.
#include <mastdlib.h>

// Include util class for integer to string conversions.
#include <MAUtil/util.h>

// Include the engine class.
#include "WikiEngine.h"

// Include the application's moblet.
#include "WikiMoblet.h"

// Utility functions.
#include "Util.h"

// Include this screen's header file.
#include "SummaryScreen.h"

namespace WikiNativeUI {

/**
 * Constructor.
 * Pass the parent screen and the engine.
 */
SummaryScreen::SummaryScreen(TitleScreen *parentScreen,MediaWiki* engine):
		BasicScreen(),
		mPrevScreen(parentScreen),
		mScreen(-1),
		mMainLayout(-1),
		mLabel(-1),
		mHomeButton(-1),
		mBackButton(-1),
		mListView(-1),
		mExitButton(-1),
		mWiki(engine)
{
	// Initialize the native UI.
	setupUI();

	// Create the next screen.
	mWebScreen = new WebScreen(this);

	// Make the  screen listen for events.
	MAUtil::Environment::getEnvironment().addCustomEventListener(this);
}

/**
 * Destructor.
 * Destroy the main widget, and all it's children will be destroyed also
 */
SummaryScreen::~SummaryScreen()
{
	if (mWebScreen){
		delete mWebScreen;
		mWebScreen = NULL;
	}
	// The base d-tor is called, and it destroys  the mMainLayout;
}

/**
 * Lay out the widgets (portrait mode).
 */
void SummaryScreen::setupUI()
{

	// Get the handle to the main layout and the screen.
	mMainLayout = getMainLayout();
	mScreen = getScreen();
	mLabel = getTopLabel();
	mHomeButton = getTopButtonRight();
	mBackButton = getTopButtonLeft();

	// Set the text for the widget in the top layout: the label and button.
	setButtonText(mBackButton, " BACK ");
	setButtonText(mHomeButton, " New Search ");

	// Add the list view that will contain the snippets.
	// It will be filled when this screen is displayed, based on the checked
	// titles from TitlesScreen.
	// The height is ScreenHeight - the size of the top layout.
	mListView = createListView(mScreenWidth, 7*mScreenHeight/8);
	maWidgetAddChild(mMainLayout, mListView);
}

/**
 *  Show the screen.
 *  If it is called from WebScreen it does not need refresh,
 *  but if from TitleScreen the list box needs refresh.
 */
void SummaryScreen::showScreen(bool needsRefresh) {

	// Each time this screen is shown, refresh the list taken from the engine.
	if ( needsRefresh ){
		fillListBox();
	}

	// Display the current screen.
	BasicScreen::showScreen();
}

/**
 * Show the home screen.
 */
void SummaryScreen::showHomeScreen()
{
	mPrevScreen->showHomeScreen();
}

/**
 * Fill the list box with data provided by the engine.
 */
void SummaryScreen::fillListBox()
{
	// Clear previous data.
	mSnippetLabels.clear();
	mSnippets.clear();

	// For each snippet we need to know the article title.
	mSnippets = mWiki->getAllSnippetsWithTitle();

	// Destroy the list view, and recreate it.
	if ( mListView != -1){
		maWidgetDestroy(mListView);
	}
	mListView = createListView(mScreenWidth, 7*mScreenHeight/8);
	setWidgetProperty(mListView, MAW_WIDGET_BACKGROUND_COLOR, DARK_WHITE, 16);
	maWidgetAddChild(mMainLayout, mListView);

	// If no titles are selected, display a short message.
	if (mSnippets.size() == 0)
	{
		MAWidgetHandle listItem = maWidgetCreate(MAW_LIST_VIEW_ITEM);
		maWidgetSetProperty(
				listItem,
				MAW_LIST_VIEW_ITEM_TEXT,
				WARNING_NOTHING_SELECTED.c_str() );
		maWidgetAddChild(mListView, listItem);
	}

	// Update the UI.
	for (int i=0; i < mSnippets.size(); i++)
	{

		MAWidgetHandle listItem = maWidgetCreate(MAW_LIST_VIEW_ITEM);
		maWidgetSetProperty(
				listItem,
				MAW_LIST_VIEW_ITEM_TEXT,
				mSnippets[i].c_str() );

		mSnippetLabels.add(listItem);
		maWidgetAddChild(mListView,mSnippetLabels[i]);
	}
}

/**
 * from CustomEventListener
 * The custom event listener interface.
 */
void SummaryScreen::customEvent(const MAEvent& event)
{
	// If the event does not come from a widget, we just ignore it.
	if (event.type != EVENT_TYPE_WIDGET) {
		return;
	}

	// Get the information sent by the widget.
	MAWidgetEventData *widgetEventData = (MAWidgetEventData *) event.data;

	// Check that the event was sent from a widget...
	if (widgetEventData->eventType == MAW_EVENT_CLICKED)
	{
		// handle the event emitted by the widget
		widgetClicked(widgetEventData->widgetHandle);
	}
	// Check the event that was sent from a list view.
	else if (widgetEventData->eventType == MAW_EVENT_ITEM_CLICKED )
	{
		// By clicking on an item, the corresponding web view is opened
		// in the WebSsreen.
		mWebScreen->showScreen();
		mWebScreen->openWebView(
			mWiki->getTitleForIndex( widgetEventData->listItemIndex) );
	}
	return;
}

/**
 * Handle events on screen's widgets.
 */
void SummaryScreen::widgetClicked(MAHandle widgetHandle)
{
	if (widgetHandle == mHomeButton)
	{
		// Go back to home screen.
		mPrevScreen->showHomeScreen();
	}
	else if (widgetHandle == mBackButton)
	{
		// Go back to the previous screen.
		// And there is no need for refreshing the results list.
		mPrevScreen->showScreen(false);
	}
}

} // namespace WikiNativeUI
