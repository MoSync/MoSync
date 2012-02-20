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
 * @author Bogdan Iusco..
 */

#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

#include <maapi.h>

#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>

using namespace NativeUI;

/**
 * Class that creates a screen that displays all the contacts.
 */
class MainScreen:
	public Screen,
	public ButtonListener,
	public EditBoxListener,
	public ListViewListener
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
     * This method is called when there is an touch-down event for
     * a button.
     * Only for iphone platform.
     * @param button The button object that generated the event.
     */
    virtual void buttonPressed(Widget* button) {};

    /**
     * This method is called when there is an touch-up event for
     * a button.
     * Only for iphone platform.
     * @param button The button object that generated the event.
     */
    virtual void buttonReleased(Widget* button) {};

    /**
     * This method is called if the touch-up event was inside the
     * bounds of the button.
     * @param button The button object that generated the event.
     */
    virtual void buttonClicked(Widget* button);

    /**
     * This method is called when the user selects an item from
     * the list view
     * @param listView The list view object that generated the event
     * @param listViewItem The ListViewItem object that was clicked.
     */
    virtual void listViewItemClicked(ListView* listView, ListViewItem* listViewItem);

    /**
     * This method is called when an edit box gains focus.
     * The virtual keyboard is shown.
     * Only for iphone platform.
     * @param editBox The edit box object that generated the event.
     */
    virtual void editBoxEditingDidBegin(EditBox* editBox);

    /**
     * This method is called when an edit box loses focus.
     * The virtual keyboard is hidden.
     * Only for iphone platform.
     * @param editBox The edit box object that generated the event.
     */
    virtual void editBoxEditingDidEnd(EditBox* editBox);

    /**
     * This method is called when the edit box text was changed.
     * Only for iphone platform.
     * @param editBox The edit box object that generated the event.
     * @param text The new text.
     */
    virtual void editBoxTextChanged(
        EditBox* editBox,
        const MAUtil::String& text);

    /**
     * This method is called when the return button was pressed.
     * On iphone platform the virtual keyboard is not hidden after
     * receiving this event.
     * Only for iphone platform.
     * @param editBox The edit box object that generated the event.
     */
    virtual void editBoxReturn(EditBox* editBox);

	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();

	/**
	 * Creates the input mode list view
	 * @param mainLayout Widgets will be added to it.
	 */
	void createInputModeListView(ListView* mainListView);

	/**
	 * Creates the input flag list view
	 * @param mainLayout Widgets will be added to it.
	 */
	void createInputFlagListView(ListView* mainListView);

	/**
	 * Creates a horizontal layout, adds it to the main layout, initializes the
	 * edit box and a label and adds it to the horizontal layout.
	 * @param editBox The editbox to be created and added on the screen.
	 * @param mainLayout Widgets will be added to it.
	 * @param text The text that will fill the label.
	 */
	void createDecimalEditBoxView(EditBox* &editBox, ListView* mainListView, MAUtil::String text);

	/**
	 * Gets the index of a string. Returns -1 if the string is not inside the array
	 * @param array The String array in which to search
	 * @param text The list view item text
	 */
	int getIndexForString(const MAUtil::String* array, MAUtil::String text);

	/**
	 * Set the input mode of the edit box
	 * @param mode The input mode code
	 */
	void setInputMode(int mode);

	/**
	 * Set the input flag of the edit box
	 * @param flag The input flag code
	 */
	void setInputFlag(int flag);

private:
	/**
	 * Main layout.
	 */
	ListView* mMainListView;
	VerticalLayout* mMainLayout;
	EditBox* mEditBox;

	Button* mSetTextButton;
	Button* mGetTextButton;
	Label* mGetTextLabel;
	Button* mKeyboardButton;

	/**
	 * The max input text length for mEditBox.
	 */
	EditBox* mMaxTextLengthEditBox;
	EditBox* mPasswordEditBox;

	ListView* mInputModeListView;
	ListView* mInputFlagListView;

	/**
	 * The maximum number of lines for mEditBox
	 */
	EditBox* mMaxLinesEditBox;
	Label* mMaxLinesLabel;

	/**
	 * The minimum number of lines for mEditBox
	 */
	EditBox* mMinLinesEditBox;
	Label* mMinLinesLabel;

	/**
	 * The placeholder color for mEditBox
	 */
	EditBox* mPlaceholderColorEditBox;
	Label* mPlaceholderColorLabel;

	/**
	 * The lines number for mEditBox
	 */
	EditBox* mLinesNumberEditBox;
	Label *mLinesNumberLabel;

	bool mKeyboard;

	/** Screen's width. */
	int mScreenWidth;

	/** Screen's height. */
	int mScreenHeight;

	int mFontSize;
};


#endif /* MAINSCREEN_H_ */
