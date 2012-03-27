/*
 Copyright (C) 2012 MoSync AB

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
 * @file SettingsScreen.cpp
 * @author Bogdan Iusco
 * @date 23 Mar 2012
 *
 * @brief Shows settings for the segmented list.
 * Screen shown in the application's second tab.
 */

#define SCREEN_TITLE "Settings"

#define LIST_PADDING_TOP 20
#define LIST_PADDING_LEFT 30
#define LIST_PADDING_RIGHT 30

#define LABEL_PADDING_LEFT 20
#define LABEL_PADDING_TOP 10
#define EDIT_LABEL_TEXT "Allow editing"
#define MOVE_LABEL_TEXT "Allow moving"

#include <NativeUI/RelativeLayout.h>
#include <NativeUI/VerticalLayout.h>
#include <NativeUI/Label.h>

#include "SettingsScreen.h"
#include "../wrapper/SegmentedListViewItem.h"

/**
 * Constructor.
 */
SettingsScreen::SettingsScreen() :
	Screen(),
	mAllowEditing(NULL),
	mAllowMoving(NULL)
{
	this->setTitle(SCREEN_TITLE);
	this->createUI();

	mAllowEditing->addCheckBoxListener(this);
	mAllowMoving->addCheckBoxListener(this);
}

/**
 * Destructor.
 */
SettingsScreen::~SettingsScreen()
{
	mAllowEditing->removeCheckBoxListener(this);
	mAllowMoving->removeCheckBoxListener(this);
}

/**
 * Create widgets and add them to screen.
 */
void SettingsScreen::createUI()
{
	SegmentedListView* list = new SegmentedListView(SegmentedListViewTypeGrouped);
	list->fillSpaceHorizontally();
	list->fillSpaceVertically();
	this->setMainWidget(list);

	this->createListSection(*list);
	list->reloadData();

}

/**
 * Create and add a list section to segmented list.
 * @param list A  segmented list where to add the section.
 */
void SettingsScreen::createListSection(SegmentedListView& list)
{
	SegmentedListViewSection* section = new SegmentedListViewSection();
	list.addSection(section);

	this->createEditOptionRow(*section);
	this->createMoveOptionRow(*section);
}

/**
 * Create and add a list row containing a check box for enabling/disabling
 * list edit option.
 * @param section List section where to add the list item/row.
 */
void SettingsScreen::createEditOptionRow(SegmentedListViewSection& section)
{
	SegmentedListViewItem* item = new SegmentedListViewItem();
	section.addItem(item);

	RelativeLayout* layout = new RelativeLayout();
	layout->fillSpaceHorizontally();
	layout->fillSpaceVertically();
	item->addChild(layout);

	Label* label = new Label();
	label->setTopPosition(LABEL_PADDING_TOP);
	label->setLeftPosition(LABEL_PADDING_LEFT);
	label->fillSpaceHorizontally();
	label->setText(EDIT_LABEL_TEXT);
	layout->addChild(label);

	mAllowEditing = new CheckBox();
	int leftCoord = item->getWidth() - 3 * LABEL_PADDING_LEFT -
		mAllowEditing->getWidth();
	mAllowEditing->setLeftPosition(leftCoord);
	mAllowEditing->setTopPosition(LABEL_PADDING_TOP);
	layout->addChild(mAllowEditing);
}

/**
 * Create and add a list row containing a check box for enabling/disabling
 * list move option.
 * @param section List section where to add the list item/row.
 */
void SettingsScreen::createMoveOptionRow(SegmentedListViewSection& section)
{
	SegmentedListViewItem* item = new SegmentedListViewItem();
	section.addItem(item);

	RelativeLayout* layout = new RelativeLayout();
	item->addChild(layout);

	Label* label = new Label();
	label->setTopPosition(LABEL_PADDING_TOP);
	label->setLeftPosition(LABEL_PADDING_LEFT);
	label->setText(MOVE_LABEL_TEXT);
	layout->addChild(label);

	mAllowMoving = new CheckBox();
	int leftCoord = item->getWidth() - 3* LABEL_PADDING_LEFT -
			mAllowMoving->getWidth();
	mAllowMoving->setLeftPosition(leftCoord);
	mAllowMoving->setTopPosition(LABEL_PADDING_TOP);
	layout->addChild(mAllowMoving);
}

/**
 * This method is called when the state of the check box was changed
 * by the user.
 * @param checkBox The check box object that generated the event.
 * @param state True if the check box is checked, false otherwise.
 */
void SettingsScreen::checkBoxStateChanged(
    CheckBox* checkBox,
    bool state)
{

}
