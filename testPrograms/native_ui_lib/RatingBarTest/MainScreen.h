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
 * @author emma
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
	public EditBoxListener,
	public ButtonListener,
	public RatingBarListener
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

    /**
     * This method is called when the value of the rating bar was changed.
     * @param ratingBar The rating bar object that generated the event.
     * @param value The new value of the rating bar.
     * @param romUser True if the value was changed by the user, false if it was
     * set programmaticaly.
     */
    void ratingChanged(
        RatingBar* ratingBar,
        float value,
        bool fromUser);

    /**
     * This method is called when the return button was pressed.
     * On iphone platform the virtual keyboard is not hidden after
     * receiving this event.
     * @param editBox The edit box object that generated the event.
     */
    virtual void editBoxReturn(EditBox* editBox);

    /**
     * This method is called when the edit box text was changed.
     * @param editBox The edit box object that generated the event.
     * @param text The new text.
     */
    virtual void editBoxTextChanged(
        EditBox* editBox,
        const MAUtil::String& text);

    /**
     * This method is called if the touch-up event was inside the
     * bounds of the button.
     * Platform: iOS and Android.
     * @param button The button object that generated the event.
     */
    virtual void buttonClicked(Widget* button);
private:

	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();

	/**
	 * Add rows:
	 * On the first line an info label and the edit box.
	 * On the second line a button for getting the set info, and a label
	 * for displaying the value.
	 */
	void addRow(MAUtil::String labelText, EditBox* &editBox,
			Button* &button, MAUtil::String buttonText,Label* &getValueLabel);
private:
	/**
	 * Main layout.
	 */
	VerticalLayout* mMainLayout;

	RatingBar* mRatingBar;
	EditBox* mRating;
	Label* mRatingLabel;
	Button* mGetRating;
	EditBox* mGranularity;
	Label* mGranularityLabel;
	Button* mGetGranularity;
	EditBox* mStars;
	Label* mStarsLabel;
	Button* mGetStars;
	Label* mEventLabel;
};


#endif /* MAINSCREEN_H_ */
