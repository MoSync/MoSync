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
	public ButtonListener,
	public EditBoxListener,
	public ListViewListener,
	public TabScreen,
	public TabScreenListener,
	public CheckBoxListener
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
     * This method is called when the state of the check box was changed
     * by the user.
     * @param checkBox The check box object that generated the event.
     * @param state True if the check box is checked, false otherwise.
     */
    virtual void checkBoxStateChanged(
        CheckBox* checkBox,
        bool state);

	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();

	/**
	 * Creates the input mode list view
	 * @param mainLayout Widgets will be added to it.
	 */
	void createInputModeListView(VerticalLayout* aVerticalLayout);

	/**
	 * Creates the input flag list view
	 * @param mainLayout Widgets will be added to it.
	 */
	void createInputFlagListView(VerticalLayout* aVerticalLayout);

	/**
	 * Creates a horizontal layout, adds it to the main layout, initializes the
	 * edit box and a label and adds it to the horizontal layout.
	 * @param editBox The editbox to be created and added on the screen.
	 * @param mainLayout Widgets will be added to it.
	 * @param text The text that will fill the label.
	 */
	void createDecimalEditBoxView(EditBox* &editBox, VerticalLayout* aVerticalLayout, MAUtil::String text);

	/**
	 * Create an layout containing a label and a check box widget.
	 * The check box will be used to set the edit box #MAW_EDIT_BOX_MODE property.
	 * Platform: iOS.
	 */
	HorizontalLayout* createModePropertyLayout();

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

	/**
	 * This method is called when a tab screen has changed to a new tab.
	 * @param tabScreen The tab screen object that generated the event.
	 * @param tabScreenIndex The index of the new tab.
	 */
	virtual void tabScreenTabChanged(TabScreen* tabScreen, const int tabScreenIndex);

private:
	/**
	 * Main layout.
	 */
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

	/**
	 * Used to test MAW_EDIT_BOX_MODE.
	 * Platform: iOS.
	 */
	CheckBox* mSingleLineCheckBox;

	bool mKeyboard;

	/** Screen's width. */
	int mScreenWidth;

	/** Screen's height. */
	int mScreenHeight;

	int mFontSize;
};


#endif /* MAINSCREEN_H_ */
