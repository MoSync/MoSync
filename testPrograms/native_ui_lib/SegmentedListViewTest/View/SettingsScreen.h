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

using namespace NativeUI;

class SettingsScreen :
	public Screen,
	public CheckBoxListener
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
	 * Allow cells to be edited.
	 */
	CheckBox* mAllowEditing;

	/**
	 * Allow cells to be moved.
	 */
	CheckBox* mAllowMoving;
};

#endif /* SETTINGSSCREEN_H_ */
