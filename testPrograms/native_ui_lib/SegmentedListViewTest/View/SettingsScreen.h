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
 * @file SettingsScreen.h
 * @author Bogdan Iusco
 * @date 23 Mar 2012
 *
 * @brief Shows settings for the segmented list.
 * Screen shown in the application's second tab.
 */

#ifndef SETTINGSSCREEN_H_
#define SETTINGSSCREEN_H_

#include <NativeUI/Screen.h>
#include <NativeUI/CheckBox.h>
#include <NativeUI/CheckBoxListener.h>

#include "../wrapper/SegmentedListView.h"
#include "../wrapper/SegmentedListViewSection.h"
#include "../wrapper/SegmentedListViewItem.h"
#include "../wrapper/SegmentedListViewListener.h"

using namespace NativeUI;

class SettingsScreen :
	public Screen,
	public CheckBoxListener,
	public SegmentedListViewListener
{
public:
	/**
	 * Constructor.
	 */
	SettingsScreen();

	/**
	 * Destructor.
	 */
	~SettingsScreen();

private:
	/**
	 * Create widgets and add them to screen.
	 */
	void createUI();

	/**
	 * Create and add a list section to segmented list.
	 * @param list A  segmented list where to add the section.
	 */
	void createListSection(SegmentedListView& list);

	/**
	 * Create and add a list section to segmented list.
	 * The new section will allow the user to set the list mode.
	 * @param list A  segmented list where to add the section.
	 */
	void createListSectionForListMode(SegmentedListView& list);

	/**
	 * Create and add a list row containing a check box for enabling/disabling
	 * list edit option.
	 * @param section List section where to add the list item/row.
	 */
	void createEditOptionRow(SegmentedListViewSection& section);

	/**
	 * Create and add a list row containing a check box for enabling/disabling
	 * list move option.
	 * @param section List section where to add the list item/row.
	 */
	void createMoveOptionRow(SegmentedListViewSection& section);

	/**
	 * Create and add a list rows for setting the list mode.
	 * @param section List section where to add the list items/rows.
	 */
	void createModeOptionRows(SegmentedListViewSection& section);

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
	 * Called when a segmented list view item is about to be selected.
	 * Platform: iOS.
	 * @param segmentedListView The segmented list view item that
	 * generated the event.
	 * @param segmentedListViewItem List item that will be selected.
	 * @param segmentedListViewItemIndex List item index that will be
	 * selected.
	 */
	virtual void segmentedListViewItemWillSelect(
		SegmentedListView* segmentedListView,
		SegmentedListViewItem* segmentedListViewItem,
		const int segmentedListViewItemIndex);

	/**
	 * Called after a segmented list view item is selected.
	 * Platform: iOS.
	 * @param segmentedListView The segmented list view item that
	 * generated the event.
	 * @param segmentedListViewItem List item that was selected.
	 * @param segmentedListViewItemIndex List item index that was selected.
	 */
	virtual void segmentedListViewItemSelected(
		SegmentedListView* segmentedListView,
		SegmentedListViewItem* segmentedListViewItem,
		const int segmentedListViewItemIndex);
private:
	/**
	 * Allow cells to be edited.
	 */
	CheckBox* mAllowEditing;

	/**
	 * Allow cells to be moved.
	 */
	CheckBox* mAllowMoving;

	/**
	 * Used to set the list mode to display.
	 */
	SegmentedListViewItem* mDisplayMode;

	/**
	 * Used to set the list mode to edit.
	 */
	SegmentedListViewItem* mEditMode;
};

#endif /* SETTINGSSCREEN_H_ */
