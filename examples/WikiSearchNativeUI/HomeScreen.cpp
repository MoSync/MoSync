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
 * @file HomeScreen.cpp
 *
 * This file contains the home screen of the application.
 * When application is launched, this screen is displayed by default.
 * It contains edit box field for user input, a selection of categories,
 * a progress bar and a slider control.
 * After the user presses Search button, next screen with the
 * available article titles is displayed.
 *
 * @author Emma Tresanszki
 */

// Include library for string conversions.
#include <mastdlib.h>

// Include the engine that performs the wiki requests.
#include "WikiEngine.h"

// Include application's moblet.
#include "WikiMoblet.h"

// Include the class that has utility functions.
#include "Util.h"

// Include the header file of this screen.
#include "HomeScreen.h"

namespace WikiNativeUI
{

/**
 * Constructor for the home screen class.
 * Here instance variables are initialized.
 */
HomeScreen::HomeScreen():
		BasicScreen(),
		mScreen(-1),
		mMainLayout(-1),
		mLabel(-1),
		mSearchButton(-1),
		mProgressBar(-1),
		mProgressLabel(-1),
		mSteps(0),
		mEditBox(-1),
		mSlider(-1),
		mSliderValue(0)
{
	// Initialize the native UI.
	setupUI();

	// Initialize the wikipedia engine.
	mWiki = new MediaWiki(this);

	// Initialize next screen ( without filling it ).
	mTitlesScreen = new TitleScreen(this, mWiki);

	// Make the home screen listen for events.
	MAUtil::Environment::getEnvironment().addCustomEventListener(this);
}

/*
 * dtor
 * Delete the main widget, and all it's children will be deleted also.
 */
HomeScreen::~HomeScreen() {
	// Delete the engine.
	if (mWiki)
	{
		delete mWiki;
		mWiki = NULL;
	}

	// Delete the screens.
	delete mTitlesScreen;
	mTitlesScreen = NULL;

	// The main layout is destroyed in base class destructor.
}

/*
 * Create the layout with checkable categories.
 * It is parented by the main layout.
 * For the moment, there are only 6 available categories. ( These could be expanded also by new user categories.)
 * @return The layout that contains the categories.
 */
MAWidgetHandle HomeScreen::createCategoriesLayout()
{
	// Add a layout for categories.
	MAWidgetHandle categoriesLayout = maWidgetCreate(MAW_VERTICAL_LAYOUT);

	// Set layout's size.
	setWidgetSize(
			categoriesLayout,
			MAW_CONSTANT_FILL_AVAILABLE_SPACE,
			mScreenHeight/3); //or MAW_CONSTANT_WRAP_CONTENT

	 // Add a horizontal line.
	MAWidgetHandle line1 = createSpacer(mScreenWidth,BREAKLINE_HEIGHT);
	setWidgetProperty(
			line1,
			MAW_WIDGET_BACKGROUND_COLOR,
			DARK_GREY, 16);

	MAWidgetHandle label = createLabel(
			MAW_CONSTANT_FILL_AVAILABLE_SPACE,
			"  Categories  ",
			BLUE,
			mFontSize);
	maWidgetSetProperty(
			label,
			MAW_LABEL_TEXT_HORIZONTAL_ALIGNMENT,
			MAW_ALIGNMENT_LEFT);
	maWidgetAddChild(categoriesLayout, label);

	 // Add another horizontal line.
	MAWidgetHandle line2 = createSpacer(mScreenWidth,BREAKLINE_HEIGHT);
	setWidgetProperty(line2, MAW_WIDGET_BACKGROUND_COLOR, DARK_GREY, 16);
	maWidgetAddChild(categoriesLayout, line2);

	// Available categories: All ( enabled by default), Nature, Sports, Movies, News, Science.
	mCategoriesStrings.clear();
	mCategoriesStrings.add(CATEGORY_ALL);
	mCategoriesStrings.add(CATEGORY_NATURE);
	mCategoriesStrings.add(CATEGORY_SPORTS);
	mCategoriesStrings.add(CATEGORY_NEWS);
	mCategoriesStrings.add(CATEGORY_MOVIES);
	mCategoriesStrings.add(CATEGORY_SCIENCE);

	setWidgetProperty(categoriesLayout, MAW_WIDGET_BACKGROUND_COLOR, DARK_WHITE, 16);

	int index(-1);

	// Each category has a check box and label.
	for ( int i=0; i<mCategoriesStrings.size()/2; i++)
	{
		// Arrange 2 categories per row.
		MAWidgetHandle categoryLayout = maWidgetCreate(MAW_HORIZONTAL_LAYOUT);

		// Set layout's size.
		setWidgetSize(categoryLayout,mScreenWidth, MAW_CONSTANT_FILL_AVAILABLE_SPACE);

		MAWidgetHandle checkBox1 = createCheckBox();
		// By default, the first category (All) is checked,
		// so all of them are checked also.
		maWidgetSetProperty(checkBox1,MAW_CHECK_BOX_CHECKED,"true");

		// We hold the array of check boxes, so we can construct
		// the request url based on the chosen categories.
		mCategoryBoxes.add(checkBox1);
		maWidgetAddChild(categoryLayout, checkBox1);

		index++;
		MAWidgetHandle label1 = createLabel(
				MAW_CONSTANT_FILL_AVAILABLE_SPACE,
				mCategoriesStrings[index].c_str(),
				BLUE,
				mFontSize );
		// Arrange the text to the left.
		maWidgetSetProperty(label1, MAW_LABEL_TEXT_HORIZONTAL_ALIGNMENT, MAW_ALIGNMENT_LEFT);
		maWidgetAddChild(categoryLayout, label1);

		// Add another check box & label, on the same row.

		MAWidgetHandle checkBox2 = createCheckBox();
		// By default, the first category (All) is checked,
		// so all of them are checked also.
		maWidgetSetProperty(checkBox2,MAW_CHECK_BOX_CHECKED,"true");

		// We hold the array of check boxes, so we can construct
		// the request url based on the chosen categories.
		mCategoryBoxes.add(checkBox2);
		maWidgetAddChild(categoryLayout, checkBox2);

		index++;
		MAWidgetHandle label2 = createLabel(
				MAW_CONSTANT_FILL_AVAILABLE_SPACE,
				mCategoriesStrings[index].c_str(),
				BLUE,
				mFontSize );
		// Arrange the text to the left.
		maWidgetSetProperty(label2, MAW_LABEL_TEXT_HORIZONTAL_ALIGNMENT, MAW_ALIGNMENT_LEFT);
		maWidgetAddChild(categoryLayout, label2);

		// Add this layout to the main vertical layout.
		maWidgetAddChild(categoriesLayout, categoryLayout);
	}

	 // Add another horizontal line.
	MAWidgetHandle line3 = createSpacer(mScreenWidth,BREAKLINE_HEIGHT);
	setWidgetProperty(line3, MAW_WIDGET_BACKGROUND_COLOR, DARK_GREY, 16);

	maWidgetAddChild(categoriesLayout, line3);

	return categoriesLayout;
}

/*
 * Lay out the widgets (portrait mode).
 */
void HomeScreen::setupUI()
{
	// Get the handle to the main layout and the screen.
	mMainLayout = getMainLayout();
	mScreen = getScreen();
	mLabel = getTopLabel();
	mSearchButton = getTopButtonRight();
	// We do not need a Back button in this screen,
	// so we can dismiss it.
	MAWidgetHandle backBtn = getTopButtonLeft();
	maWidgetDestroy(backBtn);
//	maWidgetRemoveChild(backBtn);

	// The creation of the main layout is already done in the base class constructor,
	// called at derived object creation.
	// So, we can use  a handle for the main layout at any point.

	// Set the text for the widget in the top layout: the label and button.
//	setLabelText(mLabel,MESSAGE_SEARCH_HINT.c_str());
	setButtonText(mSearchButton, " NEXT ");

	// Add a label before the Progress bar.
	mProgressLabel = createLabel(
			mScreenWidth,
			" Please wait...",
			DARK_GREY,
			mFontSize );
	// Show it only after Search is pressed.
	maWidgetSetProperty(mProgressLabel,MAW_WIDGET_VISIBLE, "false");
	maWidgetAddChild(mMainLayout, mProgressLabel);

	// Create a progress bar for the Search action.
	mProgressBar = createProgressBar();

	// Set the range of the progress bar from 0..100
	setWidgetProperty(mProgressBar, MAW_PROGRESS_BAR_MAX, PROGRESS_BAR_MAX_VALUE);
	// Set the progress value to 0.
	setWidgetProperty(mProgressBar, MAW_PROGRESS_BAR_PROGRESS, 0);
	// Hide the widget at first, and display it when a Search is being performed.
	maWidgetSetProperty(mProgressBar, MAW_WIDGET_VISIBLE, "false");

	maWidgetAddChild(mMainLayout, mProgressBar);

	// Next, fill the remaining space with an edit box and some check boxes for the categories.
	MAWidgetHandle hintLabel = createLabel(
			mScreenWidth,
			MESSAGE_EDITBOX_HINT.c_str(),
			BLUE,
			mFontSize);
	maWidgetSetProperty(hintLabel,MAW_LABEL_TEXT_HORIZONTAL_ALIGNMENT,MAW_ALIGNMENT_LEFT);
	maWidgetAddChild(mMainLayout, hintLabel);

	// Add only one edit box.
	mEditBox = createEditBox(mScreenWidth, MAW_CONSTANT_WRAP_CONTENT);
	maWidgetAddChild(mMainLayout, mEditBox);

	// Add a small spacer before the categories.
	maWidgetAddChild(mMainLayout, createSpacer(mScreenWidth, mPaddingSize));

	// Add the layout with checkable categories.
	maWidgetAddChild( mMainLayout, createCategoriesLayout());

	// Add a label for the slider.
	MAWidgetHandle sliderLabel = createLabel(
			mScreenWidth,
			" Please select the results limit. ",
			BLUE,mFontSize);
	maWidgetAddChild(mMainLayout, sliderLabel);

	// Create a slider control for selecting the desired number of results.
	mSlider = createSlider();
	setWidgetProperty(mSlider, MAW_SLIDER_MAX, SLIDER_MAX_VALUE);
	// Set the current slider value to 1.
	setWidgetProperty(mSlider, MAW_SLIDER_VALUE, 1);
	maWidgetAddChild(mMainLayout, mSlider);

	// Add two labels with minimum and maximum value of the slider.
	MAWidgetHandle valuesLayout = maWidgetCreate(MAW_HORIZONTAL_LAYOUT);
	MAWidgetHandle minValue = createLabel(
			mScreenWidth / 2,
			 " 0 ",
			 DARK_GREY,
			 mFontSize);
	maWidgetSetProperty(minValue, MAW_LABEL_TEXT_HORIZONTAL_ALIGNMENT,
			MAW_ALIGNMENT_LEFT);
	maWidgetAddChild(valuesLayout, minValue);

	MAWidgetHandle maxValue = createLabel(
			mScreenWidth / 2,
			"", DARK_GREY,
			mFontSize);
	setWidgetProperty(maxValue, MAW_LABEL_TEXT, SLIDER_MAX_VALUE);
	maWidgetSetProperty(maxValue, MAW_LABEL_TEXT_HORIZONTAL_ALIGNMENT,
			MAW_ALIGNMENT_RIGHT);
	maWidgetAddChild(valuesLayout, maxValue);

	// Add this layout to the main one.
	maWidgetAddChild(mMainLayout, valuesLayout);
}

/*
 * Provide the checked categories in a string, separated by comma.
 */
MAUtil::String HomeScreen::getCheckedCategories()
{
	char buf[6];
	MAUtil::String categories="";

	for (int i=0; i<mCategoryBoxes.size(); i++)
	{
		maWidgetGetProperty(mCategoryBoxes[i], MAW_CHECK_BOX_CHECKED, buf, 6);
		if ( strcmp(buf, "true") == 0)
		{
			if ( i == 0 )
			{
				// All categories are selected, so do not add any search term.
				break;
			}
			// Add category name and commas.
			categories += "," + mCategoriesStrings[i];
		}
	}
	return categories;
}

/*
 * Show the screen.
 */
void HomeScreen::showScreen()
{
	// Reset the progress bar value to 0, and dismiss it.
	maWidgetSetProperty(mProgressLabel,MAW_WIDGET_VISIBLE, "false");
	setWidgetProperty(mProgressBar,MAW_PROGRESS_BAR_PROGRESS, 0);
	maWidgetSetProperty(mProgressBar,MAW_WIDGET_VISIBLE, "false");

	// Reset the slider value.
	setWidgetProperty(mSlider, MAW_SLIDER_VALUE, 1);

	// Clear the edit box.
	maWidgetSetProperty(mEditBox, MAW_EDIT_BOX_TEXT, "");

	// Check all categories.
	for (int i=0; i<mCategoryBoxes.size(); i++)
	{
		maWidgetSetProperty(mCategoryBoxes[i], MAW_CHECK_BOX_CHECKED, "true");
	}

	// Initialize the top text label.
	setLabelText(mLabel, " ");

	// Display this screen.
	BasicScreen::showScreen();
}

/*
 * Notify the screen about wiki engine status
 * when finished, next screen can be displayed.
 */
void HomeScreen::engineFinished()
{
	// Set the progress bar to it's max value.
	setWidgetProperty(mProgressBar,MAW_PROGRESS_BAR_PROGRESS, PROGRESS_BAR_MAX_VALUE);

	// Show next screen: the one with all the available titles and check boxes for each one.
	// And refresh the list view that contains those values.
	mTitlesScreen->showScreen(true);
}

/*
 * Engine notifies this screen that a new chunk of data is received.
 * At each step, the progress value is incremented.
 */
void HomeScreen::engineChunkReceived()
{
	int incrementBy = PROGRESS_BAR_MAX_VALUE/mSteps;
	setWidgetProperty(mProgressBar, MAW_PROGRESS_BAR_INCREMENT_PROGRESS, incrementBy);
}

/*
 * The UI is notified when an engine error occurs.
 */
void HomeScreen::engineError(MAUtil::String errorMessage)
{
	// Some error occurred.
	setLabelText(mLabel, errorMessage.c_str());
	// Hide the progress bar and it's label.
	maWidgetSetProperty(mProgressBar, MAW_WIDGET_VISIBLE, "false");
	maWidgetSetProperty(mProgressLabel,MAW_WIDGET_VISIBLE, "false");
}

/*
 * Notify this screen of the number the chunks of data from HTTP_GET request.
 * After each chunk is received, the progress bar value is incremented.
 *
 */
void HomeScreen::engineNrSteps(int nrSteps)
{
	mSteps = nrSteps;
}

/*
 * from CustomEventListener
 * The custom event listener interface.
 */
void HomeScreen::customEvent(const MAEvent& event) {
	//If the event does not come from a widget, we just ignore it.
	if (event.type != EVENT_TYPE_WIDGET) {
		return;
	}

	// Get the information sent by the widget.
	MAWidgetEventData *widgetEventData = (MAWidgetEventData *) event.data;

	// Check that the event was sent from a widget...
	if (widgetEventData->eventType == MAW_EVENT_CLICKED) {
		// handle the event emitted by the widget
		widgetClicked(widgetEventData->widgetHandle);
	}
	// Check if the event came from a slider value change...
	else if(widgetEventData->eventType == MAW_EVENT_SLIDER_VALUE_CHANGED) {
		// Set the current value of the slider.
		mSliderValue = widgetEventData->sliderValue;
	}
	return;
}

/*
 * Handle events on screen's widgets
 * NOTE: take care to dismiss the Keyboard until you press other buttons, because it is not modal.
 */
void HomeScreen::widgetClicked(MAHandle widgetHandle)
{
	if (widgetHandle == mSearchButton)
	{
		// Close the edit box forced.
		maWidgetSetProperty(mEditBox, MAW_EDIT_BOX_SHOW_KEYBOARD, "false");

		// Declare a buffer for the text contained in the edit box.
		char textBuffer[256];

		// Get the text from the edit box and put it into the buffer.
		int textLength = maWidgetGetProperty(mEditBox, MAW_EDIT_BOX_TEXT,
				textBuffer, 256);

		// If the text does not fit in the buffer, textLength will be set
		// to -1, therefore we need to check that the length is greater than
		// or equal to 0, otherwise we just ignore the event.
		if (textLength == MAW_RES_INVALID_STRING_BUFFER_SIZE || strcmp(textBuffer,"") == 0) {
			return;
		}

		MAUtil::String text = textBuffer;
		// Now we can perform a wiki search.
		// After the result is received and processed, go to next screen.
		if (text.size() > 2)
		{
			// Show the progress bar and it's label.
			maWidgetSetProperty(mProgressLabel,MAW_WIDGET_VISIBLE, "true");
			maWidgetSetProperty(mProgressBar, MAW_WIDGET_VISIBLE, "true");

			// This label is indicating that a search action is in progress.
			maWidgetSetProperty(mProgressLabel, MAW_LABEL_TEXT, MESSAGE_WAITNOTE.c_str());

			// Initiate the search with a result limit.
			mWiki->search(text, mSliderValue);
		}
	}
	else if ( mCategoryBoxes[0] == widgetHandle)
	{
		// If All category is selected, all the other ones are selected also.
		char buf[6];
		maWidgetGetProperty(mCategoryBoxes[0], MAW_CHECK_BOX_CHECKED, buf, 6);
		if (strcmp(buf,"true") == 0)
		{
			// Check the other ones.
			for ( int i=1; i< mCategoryBoxes.size(); i++){
				maWidgetSetProperty(mCategoryBoxes[i], MAW_CHECK_BOX_CHECKED, "true");
			}
		}
		else
		{
			// Uncheck all.
			for ( int i=1; i< mCategoryBoxes.size(); i++){
				maWidgetSetProperty(mCategoryBoxes[i], MAW_CHECK_BOX_CHECKED, "false");
			}
		}
	}
	else
	{
		// If any other category then All is unchecked, than uncheck All also.
		for (int i=1; i< mCategoryBoxes.size(); i++)
		{
			if ( mCategoryBoxes[i] == widgetHandle )
			{
				char buf[6];
				maWidgetGetProperty(mCategoryBoxes[i], MAW_CHECK_BOX_CHECKED, buf, 6);
				if (strcmp(buf,"false") == 0)
				{
					maWidgetSetProperty(mCategoryBoxes[0], MAW_CHECK_BOX_CHECKED, "false");
				}
				break;
			}
		}
	}
}

} // namespace WikiNativeUI
