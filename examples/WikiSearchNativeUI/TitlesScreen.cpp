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
 * @file TitlesScreen.cpp
 * @author Emma Tresanszki
 *
 * This file contains the second screen of the application.
 * This screen displays all the available article titles.
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

// This class header file.
#include "TitlesScreen.h"

namespace WikiNativeUI {

/**
 * Constructor.
 * Pass the previous screen and the engine.
 */
TitleScreen::TitleScreen(HomeScreen *parentScreen, MediaWiki* engine):
		BasicScreen(),
		mPrevScreen(parentScreen),
		mMainLayout(-1),
		mLabel(-1),
		mNextButton(-1),
		mBackButton(-1),
		mSelectAll(-1),
		mListView(-1),
		mWiki(engine)
{
	// Initialize the native UI.
	setupUI();

	// Create the next screen.
	mSummaryScreen = new SummaryScreen(this, mWiki);

	//Make the screen listen for events.
	MAUtil::Environment::getEnvironment().addCustomEventListener(this);
}

/**
 * D-tor.
 * Delete the main widget, and all it's children will be deleted also.
 */
TitleScreen::~TitleScreen()
{
	if (mSummaryScreen){
		delete mSummaryScreen;
		mSummaryScreen = NULL;
	}
	// The base d-tor is called, and it destroys the mMainLayout;
}

/**
 * Lay out the widgets (portrait mode).
 */
void TitleScreen::setupUI()
{

	// Get the handle to the main layout and the screen.
	mMainLayout = getMainLayout();
	mScreen = getScreen();
	mLabel = getTopLabel();
	mBackButton = getTopButtonLeft();
	mNextButton = getTopButtonRight();

	// Set the text for the widget in the top layout: the label and the buttons.
	setButtonText(mNextButton, " NEXT ");
	setButtonText(mBackButton, " BACK ");

	// The creation of the main layout is already done in the base class
	// constructor, called at derived object creation.
	// So, we can use  a handle for the main layout at any point.

	// Add an empty list view.
	// Each line will have a check box, and the related label with article title
	// List is populated when search is performed, and showScreen() is called.
	// The height is ScreenHeight - the size of the top layout.
	mListView = createListView(mScreenWidth, 7*mScreenHeight/8);
	maWidgetAddChild(mMainLayout, mListView);
}

/**
 *  Show the screen.
 *  If it does not need refresh, it is called from the SummaryScreen.
 *  The list box need refresh when this is called from the home screen.
 */
void TitleScreen::showScreen(bool needsRefresh)
{
	if (needsRefresh)
	{
		// Each time a new search is initiated, refresh the check box list.
		// Data is taken from the engine.
		fillListBox();
	}

	// Display the current screen.
	BasicScreen::showScreen();
}

/**
 * Fill the list box with data provided by the engine.
 */
void TitleScreen::fillListBox()
{
	// Clear previous entries.
	mCheckBoxes.clear();
	mTitleLabels.clear();

	mTitles.clear();
	mSnippets.clear();

	mTitles = mWiki->getAllTitles();
	mSnippets = mWiki->getAllSnippets();

	// Destroy the list view, and recreate it.
	if ( mListView != -1){
		maWidgetDestroy(mListView);
	}
	mListView = createListView(mScreenWidth, MAW_CONSTANT_FILL_AVAILABLE_SPACE);
	maWidgetAddChild(mMainLayout, mListView);

	// Add a Select/Deselect All button.
	MAWidgetHandle selectAllLayout = maWidgetCreate(MAW_HORIZONTAL_LAYOUT);
	// Set layout's size.
	setWidgetSize(
		selectAllLayout, mScreenWidth, MAW_CONSTANT_FILL_AVAILABLE_SPACE);

	mSelectAll = createCheckBox();
	// All titles are deselected by default.
	maWidgetSetProperty(mSelectAll, MAW_CHECK_BOX_CHECKED, "false");
	maWidgetAddChild(selectAllLayout, mSelectAll);

	MAWidgetHandle labelSelectAll = createLabel(
			MAW_CONSTANT_FILL_AVAILABLE_SPACE,
			MESSAGE_SELECT_ALL.c_str(),
			DARK_GREY,
			mFontSize );
	mTitleLabels.add(labelSelectAll);
	maWidgetAddChild(selectAllLayout, labelSelectAll);
	maWidgetAddChild(mListView,selectAllLayout);

	// Update the UI.
	for (int i=0; i < mTitles.size(); i++)
	{
		// Add results in a horizontal layout.
		MAWidgetHandle layout = maWidgetCreate(MAW_HORIZONTAL_LAYOUT);
		setWidgetSize(layout, mScreenWidth, MAW_CONSTANT_FILL_AVAILABLE_SPACE);

		MAWidgetHandle checkbox = createCheckBox();
		mCheckBoxes.add(checkbox);
		maWidgetAddChild(layout, mCheckBoxes[i]);

		MAWidgetHandle label = createLabel(
				MAW_CONSTANT_FILL_AVAILABLE_SPACE,
				mTitles[i].c_str(),
				BLUE,
				mFontSize);
		mTitleLabels.add(label);
		maWidgetAddChild(layout, label);

		setWidgetProperty(layout, MAW_WIDGET_BACKGROUND_COLOR, DARK_WHITE, 16);

		maWidgetAddChild(mListView,layout);
	}
}

/**
 * Update wiki list of titles, with the ones selected by the user.
 */
void TitleScreen::updateWikiTitles()
{
	// For each check box that is unchecked, mark the corresponding entry
	// in the engine. So that when displaying summary screen, those entries
	// will not be displayed.
	mWiki->clearHiddenIndexList();

	char buf[6];
	for (int i =0; i < mCheckBoxes.size(); i++)
	{
		maWidgetGetProperty(mCheckBoxes[i], MAW_CHECK_BOX_CHECKED, buf, 6);
		if ( strcmp(buf, "false") == 0 ){
			mWiki->markAsHidden(i);
		}
	}
}

/**
 * Get all the snippets.
 * @return all snippets with user checked title.
 */
MAUtil::Vector<MAUtil::String> TitleScreen::getAllSnippets()
{
	return mSnippets;
}


/**
 * Called when user wants to start over, but he is in another screen.
 */
void TitleScreen::showHomeScreen()
{
	mPrevScreen->showScreen();
}

/**
 * from CustomEventListener
 * The custom event listener interface.
 */
void TitleScreen::customEvent(const MAEvent& event)
{
	// If the event does not come from a widget, we just ignore it.
	if (event.type != EVENT_TYPE_WIDGET) {
		return;
	}

	// Get the information sent by the widget.
	MAWidgetEventData *widgetEventData = (MAWidgetEventData *) event.data;

	// Check that the event was sent from a widget...
	if (widgetEventData->eventType == MAW_EVENT_CLICKED) {
		// handle the event emitted by the widget.
		widgetClicked(widgetEventData->widgetHandle);
	}
	return;
}

/**
 * Handle events on screen's widgets.
 */
void TitleScreen::widgetClicked(MAHandle widgetHandle)
{
	char buf[6];

	if ( widgetHandle == mNextButton )
	{
		// For selected titles, a short snippet will be displayed ( in a separate screen).
		updateWikiTitles();

		// Show the next screen.
		mSummaryScreen->showScreen(true);
	}
	else if ( widgetHandle == mBackButton )
	{
		// Go back to the home screen.
		showHomeScreen();
	}
	else if ( mSelectAll == widgetHandle )
	{
		// Select all titles, or deselect them.
		maWidgetGetProperty(mSelectAll, MAW_CHECK_BOX_CHECKED, buf, 6);
			for (int i=0; i < mCheckBoxes.size(); i++)
			{
				maWidgetSetProperty(mCheckBoxes[i], MAW_CHECK_BOX_CHECKED, buf);
			}
	}
	else
	{
		// If a check box is unselected, than Select all is unselected too.
		for (int i=0; i < mCheckBoxes.size(); i++)
		{
			if (mCheckBoxes[i] == widgetHandle){
				maWidgetGetProperty(
					mCheckBoxes[i], MAW_CHECK_BOX_CHECKED, buf, 6);
				if (strcmp(buf, "false") == 0)
				{
					maWidgetSetProperty(
						mSelectAll, MAW_CHECK_BOX_CHECKED, "false");
				}
				break;
			}
		}
	}
}

} // namespace WikiNativeUI
