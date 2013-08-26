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
 * @file FirstScreen.h.
 * @author emma
 */

#ifndef FIRSTSCREEN_H_
#define FIRSTSCREEN_H_

#include <maapi.h>
#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>

using namespace NativeUI;

class FirstScreen:
	public Screen,
	public ScreenListener,
	public ButtonListener,
	public CheckBoxListener
{

public:
	/**
	 * Constructor.
	 */
	FirstScreen();

	/**
	 * Destructor.
	 */
	~FirstScreen();

	void createUI();

	void addDefaultActionBarItems();

	void createAddItemDialog();

	void createRemoveItemDialog();

    /**
     * This method is called when a menu item from the Action Bar is selected.
     * @param screen The screen that generated the event.
     * @param itemHandle The handle of the menu item that was selected.
     */
    virtual void actionBarItemSelected(Screen* screen, int itemHandle);

    /**
     * This method is called when the Up indicator from the Action Bar is selected.
     * @param screen The screen that generated the event.
     */
    virtual void actionBarUpSelected(Screen* screen);

    /**
     * This method is called if the touch-up event was inside the
     * bounds of the button.
     * Platform: iOS and Android.
     * @param button The button object that generated the event.
     */
    virtual void buttonClicked(Widget* button);

    /**
     * This method is called when the state of the check box was changed
     * by the user.
     * @param checkBox The check box object that generated the event.
     * @param state True if the check box is checked, false otherwise.
     */
    virtual void checkBoxStateChanged(
        CheckBox* checkBox,
        bool state);
private:
	/**
	 * Main layout.
	 */
	VerticalLayout* mMainLayout;
	ListView* mListView;

	EditBox* mTitleEditBox;
	Button* mTitleSet;
	Button* mTitleGet;
	Label* mTitleLbl;

	CheckBox* mEnableTitle;
	CheckBox* mDisableTitle;

	CheckBox* mEnableLogo;
	CheckBox* mDisableLogo;

	Button* mGetHeightBtn;
	Label* mHeightLbl;

	CheckBox* mShowCheckBox;
	CheckBox* mHideCheckBox;
	Button* mGetVisibilityBtn;
	Label* mVisibilityLbl;

	Button* mDisplayUpIndicator;

	Button* mAddItemBtn;
	Button* mRemoveItemBtn;

	Dialog* mAddItemDialog;
	Dialog* mRemoveItemDialog;

	// Widgets from add dialog
	EditBox* mAddItemTitle;
	CustomPicker* mFlagPicker;
	CustomPicker* mUsePredefinedIcon;
	Button* mAddItemDone;

	// Widgets from remove dialog.
	CustomPicker* mItemsPicker;
	Button* mRemoveItemDone;
	Button* mRemoveAllItemsDone;
	MAUtil::Vector<MAHandle> mMenuItems;
};

#endif /* FIRSTSCREEN_H_ */
