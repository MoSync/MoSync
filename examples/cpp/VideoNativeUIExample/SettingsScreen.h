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

	/**
	 * This method is called when a list view item is clicked.
	 * @param listView The list view object that generated the event.
	 * @param listViewSection The section object that contains the selected item.
	 * Will be null for default type list views.
	 * @param listViewItem The item object that was clicked.
	 */
	virtual void listViewItemClicked(
		ListView *listView,
		ListViewSection *listViewSection,
		ListViewItem *listViewItem);

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
