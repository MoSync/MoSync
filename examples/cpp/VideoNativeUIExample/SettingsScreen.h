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
 * @file SettingsScreen.h
 * @author Emma Tresanszki
 *
 * The screen that handles settings, like:
 *  - Display video file duration.
 *  - The default link/path to load at application startup.
 *  - Reload the default link.
 */

#ifndef SETTINGSSCREEN_H_
#define SETTINGSSCREEN_H_

#include <maapi.h>
#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>

using namespace NativeUI;

class SettingsScreen:
	public Screen,
	public ListViewListener,
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

	/**
	 * Get the home url that is set in the list view item.
	 */
	MAUtil::String getDefaultUrl();

	/**
	 * Get the checked state of the duration setting item.
	 * @return true if it is checked, false otherwise.
	 */
	bool getDurationSettingValue();

	/**
	 * Set the default value for the homepage.
	 * It is different depending on the platform, as they can play different
	 * media formats.
	 */
	void setDefaultHomepage();

	ListView* getSettingsListView();
	ListViewItem* getResetListViewItem();
private:

	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();

    /** from ListViewListener
     * This method is called when a list view item is clicked.
     * @param listView The list view object that generated the event.
     * @param listViewItem The ListViewItem object that was clicked.
     */
    virtual void listViewItemClicked(
        ListView* listView,
        ListViewItem* listViewItem);

    /** from CheckBoxListener
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

	/**
	 * List View with all settings.
	 */
	ListView* mListView;

	/**
	 * Check box for for displaying video duration.
	 * Checked by default.
	 */
	CheckBox* mDisplayDuration;

	/**
	 * Label for the home url, that is loaded at app startup.
	 * It has different default values for Android and iOS.
	 */
	Label* mHomeUrl;

	/**
	 * List view item for reloading the homepage source.
	 */
	ListViewItem* mReloadItem;

};

#endif /* SETTINGSSCREEN_H_ */
