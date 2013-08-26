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
