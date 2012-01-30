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
 * @file HomeScreen.h
 * @author Emma Tresanszki
 *
 * This file contains the home screen of the application.
 * When application is launched, this screen is displayed by default.
 * It contains edit box field for user input, a selection of categories,
 * a progress bar and a slider control.
 * After the user presses Search button, next screen with the
 * available article titles is displayed.
 */

#ifndef HOMESCREENNUI_H_
#define HOMESCREENNUI_H_

// Include MoSync syscalls collection.
#include <IX_WIDGET.h>

// Include the library for event handling listener.
#include "MAUtil/Environment.h"

// The header file for this screen.
#include "TitlesScreen.h"

// Include the base screen class
#include "BasicScreen.h"

namespace WikiNativeUI
{

// Forward declaration.
class MediaWiki;

/**
 * HomeScreen: native UI screen
 * It is the home screen, where user can add or remove tags, and perform a wiki search based on those tags.
 * It contains list boxes, vertical and horizontal layouts with widgets for user selection.
 */
class HomeScreen : public BasicScreen,
				   public CustomEventListener,
				   public KeyListener
{
public:
	/**
	 * Constructor for the home screen class.
	 * Here instance variables are initialized.
	 */
	HomeScreen();

	/**
	 * Destructor.
	 */
	~HomeScreen();

	/*
	 *  Show the screen.
	 */
	void showScreen();

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
	 * Handle events on screen's widgets
	 * NOTE: take care to dismiss the Keyboard until you press other buttons,
	 * because it is not modal.
	 */
	void widgetClicked(MAHandle widgetHandle);

	/**
	 * Notify the screen about wiki engine status
	 * when finished, next screen can be displayed.
	 */
	void engineFinished();

	/**
	 * Notify this screen of the number the chunks of data the HTTP_GET request.
	 * After each chunk is received, the progress bar value is incremented.
	 *
	 */
	void engineNrSteps(int nrSteps);

	/**
	 * Engine notifies this screen that a new chunk of data is received.
	 * At each step, the progress value is incremented.
	 */
	void engineChunkReceived();

	/**
	 * The UI is notified when an engine error occurs.
	 */
	void engineError(MAUtil::String errorMessage = ERROR_ANY);

	/**
	 * Provide the checked categories in a string, separated by comma.
	 */
	MAUtil::String getCheckedCategories();

private: // methods

	/**
	 * Create the layout with checkable categories.
	 * It is parented by the main layout.
	 * For the moment, there are only 6 available categories.
	 * ( These could be expanded also by new user categories.)
	 * @return The layout that contains the categories.
	 */
	MAWidgetHandle createCategoriesLayout();

	/**
	 * Lay out the widgets (portrait mode).
	 */
	void setupUI();


private: // members

	// Next screen: this screen is shown when the wiki engine finishes. */
	TitleScreen*                mTitlesScreen;

	/** NativeUI widget handles: */
	/** The screen handle. */
	MAWidgetHandle 				   mScreen;

	/** Main vertical layout. **/
	MAWidgetHandle 				   mMainLayout;

	/** Label, next to the button. **/
	MAWidgetHandle			       mLabel;

	/** Search button. **/
	MAWidgetHandle 				   mSearchButton;

	/** Progress bar. */
	MAWidgetHandle                 mProgressBar;

	/** A label for the progress bar. */
	MAWidgetHandle                mProgressLabel;

	/** The number of increments the progress bar needs to reach the max value. */
	int                            mSteps;

	/** Edit box for the tags. **/
	MAWidgetHandle  			   mEditBox;

	/** A list of categories. **/
	MAUtil::Vector<MAWidgetHandle> mCategoryBoxes;

	/** This vector contains the available categories. **/
	MAUtil::Vector<MAUtil::String> mCategoriesStrings;

	/** The slider for the search limit. */
	MAWidgetHandle                 mSlider;

	/** The current value of the slider. */
	int                            mSliderValue;

	/** The engine. **/
	MediaWiki*                     mWiki;
};

} // namespace WikiNativeUI

#endif /* HOMESCREENNUI_H_ */
