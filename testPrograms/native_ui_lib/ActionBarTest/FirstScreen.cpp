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
 * @file FirstScreen.cpp
 * @author emma
 */

#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <mastdlib.h>

#include "FirstScreen.h"

#define CLEAR_MENU "Clear Menu"
#define RECREATE_MENU "Recreate Menu"

/**
 * Constructor.
 */
FirstScreen::FirstScreen() :
		Screen(), mMainLayout(NULL), mEventsList(NULL)

{
	// First screen has some default menu items.
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(0x104E8B);
	Screen::setMainWidget(mMainLayout);
	this->setTitle("First");

	createUI();

	//mAddItemDialog = new ItemDialog();

	Screen::addScreenListener(this);
	mTitleSet->addButtonListener(this);
	mGetHeightBtn->addButtonListener(this);
	mShowCheckBox->addCheckBoxListener(this);
	mHideCheckBox->addCheckBoxListener(this);
	mGetVisibilityBtn->addButtonListener(this);
}

void FirstScreen::createUI()
{
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(0x104E8B);
	Screen::setMainWidget(mMainLayout);


	MA_ACTION_BAR_ITEM_ICON icon;
	icon.iconPredefinedId = 17301555;
	icon.iconHandle = -1;

	maActionBarAddMenuItem(getWidgetHandle(), "Add", &icon, 2);
	icon.iconPredefinedId = 17301564;
	maActionBarAddMenuItem(getWidgetHandle(), "Delete", &icon, 2);
	icon.iconPredefinedId = 17301586;
	maActionBarAddMenuItem(getWidgetHandle(), "Share", &icon, 1);
	icon.iconPredefinedId = 17301582;
	maActionBarAddMenuItem(getWidgetHandle(), "Save", &icon, 1);
	icon.iconPredefinedId = 17301566;
	maActionBarAddMenuItem(getWidgetHandle(), "Edit", &icon, 1);

	// By default the title is the activity name.

	HorizontalLayout* setTitleLayout = new HorizontalLayout();
	mTitleEditBox = new EditBox();
	mTitleSet = new Button();
	mTitleSet->setText("Apply title");
	setTitleLayout->addChild(mTitleEditBox);
	setTitleLayout->addChild(mTitleSet);

	HorizontalLayout* getHeightLayout = new HorizontalLayout();
	mGetHeightBtn = new Button();
	mGetHeightBtn->setText("Get Action Bar height");
	mHeightLbl = new Label();
	getHeightLayout->addChild(mGetHeightBtn);
	getHeightLayout->addChild(mHeightLbl);

	//todo replace with spinner.
	HorizontalLayout* showLayout = new HorizontalLayout();
	mShowCheckBox = new CheckBox();
	mShowCheckBox->setState(true);
	Label* showLbl = new Label("Show action bar");
	showLayout->addChild(mShowCheckBox);
	showLayout->addChild(showLbl);

	HorizontalLayout* hideLayout = new HorizontalLayout();
	mHideCheckBox = new CheckBox();
	mHideCheckBox->setState(false);
	Label* hideLbl = new Label("Hide action bar");
	hideLayout->addChild(mHideCheckBox);
	hideLayout->addChild(hideLbl);

	HorizontalLayout* getVisibility = new HorizontalLayout();
	mGetVisibilityBtn = new Button();
	mGetVisibilityBtn->setText("Get visibility");
	mVisibilityLbl = new Label("Action bar is visible");
	getVisibility->addChild(mGetVisibilityBtn);
	getVisibility->addChild(mVisibilityLbl);

	mMainLayout->addChild(setTitleLayout);
	mMainLayout->addChild(getHeightLayout);
	mMainLayout->addChild(showLayout);
	mMainLayout->addChild(hideLayout);
	mMainLayout->addChild(getVisibility);
}
/**
 * Destructor.
 */
FirstScreen::~FirstScreen() {
	this->removeScreenListener(this);
//	mClearOptions->removeButtonListener(this);
	mTitleSet->removeButtonListener(this);
	mGetHeightBtn->removeButtonListener(this);
	mShowCheckBox->removeCheckBoxListener(this);
	mHideCheckBox->removeCheckBoxListener(this);
	mGetVisibilityBtn->removeButtonListener(this);
}

/**
 * This method is called when the OptionsMenu is being closed
 * (either by the user canceling the menu with the back/menu
 * button, or when an item is selected.
 * @param Screen The screen that generated the event.
 */
void FirstScreen::optionsMenuClosed(Screen* screen) {

	ListViewItem* item = new ListViewItem();
	item->setText("First Screen event: Menu closed.");
	mEventsList->addChild(item);
}

/**
 * This method is called when an options menu item is selected.
 * @param index The index on which the item is placed into the
 * OptionsMenu.
 *  @param Screen The screen that generated the event.
 */
void FirstScreen::optionsMenuItemSelected(Screen* screen, int index) {
	ListViewItem* item = new ListViewItem();
	item->setText("First Screen event: Menu Item selected: index " + MAUtil::integerToString(index));
	mEventsList->addChild(item);
	MAUtil::String message = "First Screen event: Menu Item selected: index " + MAUtil::integerToString(index);
	maToast(message.c_str(), MA_TOAST_DURATION_LONG);
}

/**
 * This method is called when the state of the check box was changed
 * by the user.
 * @param checkBox The check box object that generated the event.
 * @param state True if the check box is checked, false otherwise.
 */
void FirstScreen::checkBoxStateChanged(
    CheckBox* checkBox,
    bool state)
{
	if ( mShowCheckBox == checkBox && state)
	{
		mHideCheckBox->setState(false);
		maActionBarSetVisibility(1);
	}
	else if( mHideCheckBox == checkBox && state)
	{
		mShowCheckBox->setState(false);
		maActionBarSetVisibility(0);
	}
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * Platform: iOS and Android.
 * @param button The button object that generated the event.
 */
void FirstScreen::buttonClicked(Widget* button)
{
	if ( mTitleSet == (Button*) button)
	{
		maActionBarSetTitle(mTitleEditBox->getText().c_str());
	}
	else if( mGetHeightBtn == button)
	{
		mHeightLbl->setText( MAUtil::integerToString(maActionBarGetHeight()));
	}
	else if( mGetVisibilityBtn == button)
	{
		if ( maActionBarIsShowing() == 1)
			mVisibilityLbl->setText("Is visible");
		else
			mVisibilityLbl->setText("Is hidden");
	}
}
