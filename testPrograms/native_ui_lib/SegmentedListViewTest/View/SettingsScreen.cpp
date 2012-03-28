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

#define DISPLAY_MODE_LABEL_TEXT "Display"
#define EDIT_MODE_LABEL_TEXT "Edit"

#define FIRST_SECTION_HEADING_TEXT "Options allowed in edit mode"
#define MODE_SECTION_HEADING_TEXT "List mode"


#include <NativeUI/RelativeLayout.h>
#include <NativeUI/VerticalLayout.h>
#include <NativeUI/Label.h>

#include "SettingsScreen.h"

/**
 * Constructor.
 */
SettingsScreen::SettingsScreen() :
	Screen(),
	mAllowEditing(NULL),
	mAllowMoving(NULL),
	mDisplayMode(NULL),
	mEditMode(NULL)
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
	list->addSegmentedListViewListener(this);
	list->fillSpaceHorizontally();
	list->fillSpaceVertically();
	this->setMainWidget(list);

	this->createListSection(*list);
	this->createListSectionForListMode(*list);
	list->reloadData();
}

/**
 * Create and add a list section to segmented list.
 * @param list A  segmented list where to add the section.
 */
void SettingsScreen::createListSection(SegmentedListView& list)
{
	SegmentedListViewSection* section = new SegmentedListViewSection();
	section->setHeaderText(FIRST_SECTION_HEADING_TEXT);
	list.addSection(section);

	this->createEditOptionRow(*section);
	this->createMoveOptionRow(*section);
}

/**
 * Create and add a list section to segmented list.
 * The new section will allow the user to set the list mode.
 * @param list A  segmented list where to add the section.
 */
void SettingsScreen::createListSectionForListMode(SegmentedListView& list)
{
	SegmentedListViewSection* section = new SegmentedListViewSection();
	section->setHeaderText(MODE_SECTION_HEADING_TEXT);
	list.addSection(section);

	this->createModeOptionRows(*section);
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
 * Create and add a list rows for setting the list mode.
 * @param section List section where to add the list items/rows.
 */
void SettingsScreen::createModeOptionRows(SegmentedListViewSection& section)
{
	// Create and add the display list item.
	mDisplayMode = new SegmentedListViewItem();
	RelativeLayout* layout = new RelativeLayout();
	mDisplayMode->addChild(layout);
	section.addItem(mDisplayMode);

	Label* label = new Label();
	label->setText(DISPLAY_MODE_LABEL_TEXT);
	label->setTopPosition(LABEL_PADDING_TOP);
	label->setLeftPosition(LABEL_PADDING_LEFT);
	layout->addChild(label);

	// Create and add the display list item.
	mEditMode = new SegmentedListViewItem();
	layout = new RelativeLayout();
	mEditMode->addChild(layout);
	section.addItem(mEditMode);

	label = new Label();
	label->setText(EDIT_MODE_LABEL_TEXT);
	label->setTopPosition(LABEL_PADDING_TOP);
	label->setLeftPosition(LABEL_PADDING_LEFT);
	layout->addChild(label);
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

/**
 * Called when a segmented list view item is about to be selected.
 * Platform: iOS.
 * @param segmentedListView The segmented list view item that
 * generated the event.
 * @param segmentedListViewItem List item that will be selected.
 * @param segmentedListViewItemIndex List item index that will be
 * selected.
 */
void SettingsScreen::segmentedListViewItemWillSelect(
	SegmentedListView* segmentedListView,
	SegmentedListViewItem* segmentedListViewItem,
	const int segmentedListViewItemIndex)
{
	printf("SettingsScreen::segmentedListViewItemWillSelect");
}

/**
 * Called after a segmented list view item is selected.
 * Platform: iOS.
 * @param segmentedListView The segmented list view item that
 * generated the event.
 * @param segmentedListViewItem List item that was selected.
 * @param segmentedListViewItemIndex List item index that was selected.
 */
void SettingsScreen::segmentedListViewItemSelected(
	SegmentedListView* segmentedListView,
	SegmentedListViewItem* segmentedListViewItem,
	const int segmentedListViewItemIndex)
{
	printf("SettingsScreen::segmentedListViewItemSelected");
}
