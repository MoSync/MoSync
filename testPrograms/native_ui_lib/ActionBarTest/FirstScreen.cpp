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
 * @file FirstScreen.cpp
 * @author emma
 */

#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <mastdlib.h>

#include "FirstScreen.h"

/**
 * Constructor.
 */
FirstScreen::FirstScreen() :
		Screen(), mMainLayout(NULL)

{
	// First screen has some default menu items.
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(0x104E8B);
	Screen::setMainWidget(mMainLayout);
	this->setTitle("First");

	createUI();
	// By default the title is the activity name.
	maActionBarSetEnabled(1);
	addDefaultActionBarItems();

	createAddItemDialog();
	createRemoveItemDialog();

	Screen::addScreenListener(this);
	mTitleSet->addButtonListener(this);
	mGetHeightBtn->addButtonListener(this);
	mShowCheckBox->addCheckBoxListener(this);
	mHideCheckBox->addCheckBoxListener(this);
	mGetVisibilityBtn->addButtonListener(this);
	mEnableTitle->addCheckBoxListener(this);
	mDisableTitle->addCheckBoxListener(this);
	mEnableLogo->addCheckBoxListener(this);
	mDisableLogo->addCheckBoxListener(this);
	mDisplayUpIndicator->addButtonListener(this);
	mAddItemBtn->addButtonListener(this);
	mRemoveItemBtn->addButtonListener(this);
	mAddItemDone->addButtonListener(this);
	mRemoveAllItemsDone->addButtonListener(this);
}

void FirstScreen::addDefaultActionBarItems()
{
	int handle = addActionBarMenuItem("Delete", 17301564, -1, SHOW_IF_ROOM);
	mMenuItems.add(handle);
}

void FirstScreen::createUI()
{
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(0x104E8B);
	Screen::setMainWidget(mMainLayout);
	mListView = new ListView();
	mListView->fillSpaceHorizontally();
	mListView->fillSpaceVertically();
	mMainLayout->addChild(mListView);

	HorizontalLayout* setTitleLayout = new HorizontalLayout();
	mTitleEditBox = new EditBox();
	mTitleEditBox->fillSpaceHorizontally();
	mTitleSet = new Button();
	mTitleSet->setText("Apply title");
	setTitleLayout->addChild(mTitleSet);
	setTitleLayout->addChild(mTitleEditBox);

	HorizontalLayout* enableTitleLayout = new HorizontalLayout();
	mEnableTitle = new CheckBox();
	mEnableTitle->setState(true);
	Label* enableLbl = new Label("Enable Title");
	enableTitleLayout->addChild(mEnableTitle);
	enableTitleLayout->addChild(enableLbl);

	HorizontalLayout* disableTitleLayout = new HorizontalLayout();
	mDisableTitle = new CheckBox();
	mDisableTitle->setState(false);
	Label* disableLbl = new Label("Disable Title");
	disableTitleLayout->addChild(mDisableTitle);
	disableTitleLayout->addChild(disableLbl);

	HorizontalLayout* enableLogoLayout = new HorizontalLayout();
	mEnableLogo = new CheckBox();
	mEnableLogo->setState(true);
	Label* enableLogoLbl = new Label("Enable Logo");
	enableLogoLayout->addChild(mEnableLogo);
	enableLogoLayout->addChild(enableLogoLbl);

	HorizontalLayout* disableLogoLayout = new HorizontalLayout();
	mDisableLogo = new CheckBox();
	mDisableLogo->setState(false);
	Label* disableLogoLbl = new Label("Disable Logo");
	disableLogoLayout->addChild(mDisableLogo);
	disableLogoLayout->addChild(disableLogoLbl);

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

	mDisplayUpIndicator = new Button();
	mDisplayUpIndicator->fillSpaceHorizontally();
	mDisplayUpIndicator->setText("Display Home As Up");

	mAddItemBtn = new Button();
	mAddItemBtn->setText("ADD MENU ITEM");
	mRemoveItemBtn = new Button();
	mRemoveItemBtn->setText("REMOVE MENU ITEM");

	mListView->addChild(setTitleLayout);
	mListView->addChild(getHeightLayout);
	mListView->addChild(showLayout);
	mListView->addChild(hideLayout);
	mListView->addChild(getVisibility);
	mListView->addChild(enableTitleLayout);
	mListView->addChild(disableTitleLayout);
	mListView->addChild(enableLogoLayout);
	mListView->addChild(disableLogoLayout);
	mListView->addChild(mDisplayUpIndicator);
	mListView->addChild(mAddItemBtn);
	mListView->addChild(mRemoveItemBtn);
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
	mEnableTitle->removeCheckBoxListener(this);
	mDisableTitle->removeCheckBoxListener(this);
	mEnableLogo->removeCheckBoxListener(this);
	mDisableLogo->removeCheckBoxListener(this);
	mDisplayUpIndicator->removeButtonListener(this);
	mAddItemBtn->removeButtonListener(this);
	mRemoveItemBtn->removeButtonListener(this);
	mAddItemDone->removeButtonListener(this);
	mRemoveAllItemsDone->removeButtonListener(this);
	mMenuItems.clear();
}

/**
 * This method is called when a menu item from the Action Bar is selected.
 * @param screen The screen that generated the event.
 * @param itemHandle The handle of the menu item that was selected.
 */
void FirstScreen::actionBarItemSelected(Screen* screen, int itemHandle)
{
	// convert handle to index.
	int itemIndex =0;
	for(int i=0; i<mMenuItems.size(); i++)
	{
		if (mMenuItems[i] == itemHandle)
		{
			itemIndex = i;
			break;
		}
	}
	MAUtil::String message = "ACTION BAR: Item on position " + MAUtil::integerToString(itemIndex) + " was selected";
	maToast(message.c_str(), MA_TOAST_DURATION_SHORT);
}

/**
 * This method is called when the Up indicator from the Action Bar is selected.
 * @param screen The screen that generated the event.
 */
void FirstScreen::actionBarUpSelected(Screen* screen)
{
	maToast("ACTION BAR: Up was selected", MA_TOAST_DURATION_SHORT);
}

void FirstScreen::createAddItemDialog()
{
	mAddItemDialog = new Dialog();
	VerticalLayout* dialogLayout = new VerticalLayout();
	mAddItemDialog->setMainWidget(dialogLayout);

	HorizontalLayout* titleLayout = new HorizontalLayout();
	Label* infoTitle = new Label("Title");
	mAddItemTitle = new EditBox();
	mAddItemTitle->fillSpaceHorizontally();
	titleLayout->addChild(infoTitle);
	titleLayout->addChild(mAddItemTitle);

	mFlagPicker = new CustomPicker();
	HorizontalLayout* flagIfRoomLayout = new HorizontalLayout();
	Label* flagIfRoom = new Label("Show If Room");
	flagIfRoomLayout->addChild(flagIfRoom);
	HorizontalLayout* flagAlwaysLayout = new HorizontalLayout();
	Label* flagAlways = new Label("Show Always");
	flagAlwaysLayout->addChild(flagAlways);
	HorizontalLayout* flagWithTextLayout = new HorizontalLayout();
	Label* flagText = new Label("Show with text");
	flagWithTextLayout->addChild(flagText);
	HorizontalLayout* flagCollapseLayout = new HorizontalLayout();
	Label* flagCollapse = new Label("Show collapse action view");
	flagCollapseLayout->addChild(flagCollapse);

	mFlagPicker->addChild(flagIfRoomLayout);
	mFlagPicker->addChild(flagAlwaysLayout);
	mFlagPicker->addChild(flagWithTextLayout);
	mFlagPicker->addChild(flagCollapseLayout);

	HorizontalLayout* predefinedLayout = new HorizontalLayout();

	Label* predefinedIconLabel = new Label("Use predefined image");
	mUsePredefinedIcon = new CustomPicker();
	HorizontalLayout* addIconLayout = new HorizontalLayout();
	Label* addIcon = new Label("Add");
	addIconLayout->addChild(addIcon);
	HorizontalLayout* editIconLayout = new HorizontalLayout();
	Label* editIcon = new Label("Edit");
	editIconLayout->addChild(editIcon);
	HorizontalLayout* saveIconLayout = new HorizontalLayout();
	Label* saveIcon = new Label("Save");
	saveIconLayout->addChild(saveIcon);
	HorizontalLayout* searchIconLayout = new HorizontalLayout();
	Label* searchIcon = new Label("Search");
	searchIconLayout->addChild(searchIcon);

	mUsePredefinedIcon->addChild(addIconLayout);
	mUsePredefinedIcon->addChild(editIconLayout);
	mUsePredefinedIcon->addChild(saveIconLayout);
	mUsePredefinedIcon->addChild(searchIconLayout);

	predefinedLayout->addChild(predefinedIconLabel);
	predefinedLayout->addChild(mUsePredefinedIcon);

	mAddItemDone = new Button();
	mAddItemDone->fillSpaceHorizontally();
	mAddItemDone->setText("Done");

	dialogLayout->addChild(titleLayout);
	dialogLayout->addChild(mFlagPicker);
	dialogLayout->addChild(predefinedLayout);
	dialogLayout->addChild(mAddItemDone);
}

void FirstScreen::createRemoveItemDialog()
{
	mRemoveItemDialog = new Dialog();
	VerticalLayout* removeDialogLayout = new VerticalLayout();
	mRemoveItemDialog->addChild(removeDialogLayout);

	mItemsPicker = new CustomPicker();
	for (int i=0; i<mMenuItems.size(); i++)
	{
		HorizontalLayout* itemLayout = new HorizontalLayout();
		Label* info = new Label("Item on position" +MAUtil::integerToString(i));
		itemLayout->addChild(info);
		mItemsPicker->addChild(itemLayout);
	}

	mRemoveItemDone = new Button();
	mRemoveItemDone->setText("Remove selected item");
	mRemoveAllItemsDone = new Button();
	mRemoveAllItemsDone->setText("Remove ALL items");

	removeDialogLayout->addChild(mItemsPicker);
	removeDialogLayout->addChild(mRemoveItemDone);
	removeDialogLayout->addChild(mRemoveAllItemsDone);
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
	if ( mShowCheckBox == checkBox && state == true)
	{
		mHideCheckBox->setState(false);
		maActionBarSetVisibility(1);
	}
	if( mHideCheckBox == checkBox && state == true)
	{
		mShowCheckBox->setState(false);
		maActionBarSetVisibility(0);
	}
	if (mEnableTitle == checkBox && state == true)
	{
		maActionBarShowTitleEnabled(1);
		mDisableTitle-> setState(false);
	}
	if (mDisableTitle == checkBox && state == true)
	{
		maActionBarShowTitleEnabled(0);
		mEnableTitle->setState(false);
	}
	if(mEnableLogo == checkBox && state == true)
	{
		maActionBarShowLogoEnabled(1);
		mDisableLogo->setState(false);
	}
	if(mDisableLogo == checkBox && state == true)
	{
		maActionBarShowLogoEnabled(0);
		mEnableLogo->setState(false);
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
		int res = maActionBarSetTitle(mTitleEditBox->getText().c_str());
		if ( res != MAW_RES_OK )
			maMessageBox("Set title failed", "with error " + res);
	}
	else if( mGetHeightBtn == button)
	{
		mHeightLbl->setText( MAUtil::integerToString(maActionBarGetHeight()));
	}
	else if( mGetVisibilityBtn == button)
	{
		if ( maActionBarIsShowing() == 1)
		{
			mVisibilityLbl->setText("ActionBar is visible");
		}
		else
		{
			mVisibilityLbl->setText("ActionBar is hidden");
		}
	}
	else if (mDisplayUpIndicator == button)
	{
		if (strcmp( mDisplayUpIndicator->getText().c_str(), "Display Home As Up") == 0 )
		{
			mDisplayUpIndicator->setText("Disable Home As Up");
			maActionBarSetDisplayHomeAsUpEnabled(1);
		}
		else
		{
			mDisplayUpIndicator->setText("Display Home As Up");
			maActionBarSetDisplayHomeAsUpEnabled(0);
		}
	}
	else if (mAddItemBtn == button)
	{
		mAddItemDialog->show();
	}
	else if (mRemoveItemBtn == button)
	{
		mRemoveItemDialog->show();
	}
	else if (mRemoveAllItemsDone == button)
	{
		int res = removeActionBarItems();
		if ( res >= 0)
		{
			mMenuItems.clear();
			ActionBar::getInstance()->refresh();
		}
		else
		{
			maMessageBox("Error", "Cannot remove all menu items." );
		}
		mRemoveItemDialog->hide();
	}
	else if (mRemoveItemDone == button)
	{
		int index = mItemsPicker->getSelectedItemIndex();
		int res = removeActionBarMenuItem(mMenuItems[index]);
		if ( res >= 0)
		{
			mMenuItems.remove(index);
			ActionBar::getInstance()->refresh();
		}
		else
		{
			maMessageBox("Error", "Cannot remove selected menu item." );
		}
		mRemoveItemDialog->hide();
	}
	else if (mAddItemDone == button)
	{
		// Add the menu item and dismiss the dialog.
		int predefinedId;
		predefinedId = mUsePredefinedIcon->getSelectedItemIndex();
		switch(predefinedId)
		{
		case 0:
		default:
			predefinedId = 17301555;
			break;
		case 1:
			predefinedId = 17301566;
			break;
		case 2:
			predefinedId = 17301582;
			break;
		case 3:
			predefinedId = 17301583;
			break;
		}

		ActionBarMenuItemDisplayFlag flag;
		int flagIndex = mFlagPicker->getSelectedItemIndex();
		switch(flagIndex)
		{
		case 0:
		default:
			flag = SHOW_IF_ROOM;
			break;
		case 1:
			flag = SHOW_ALWAYS;
			break;
		case 2:
			flag = SHOW_WITH_TEXT;
			break;
		case 3:
			flag = SHOW_COLLAPSE_ACTION_VIEW;
			break;
		}
		int handle = addActionBarMenuItem(mAddItemTitle->getText().c_str(), predefinedId, -1, flag);
		if ( handle >= 0)
		{
			mMenuItems.add(handle);
			ActionBar::getInstance()->refresh();
		}
		else
		{
			maMessageBox("Error", "Cannot add menu item." );
		}

		mAddItemDialog->hide();
	}
}
