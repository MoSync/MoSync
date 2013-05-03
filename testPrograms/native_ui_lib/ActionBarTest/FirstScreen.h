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
