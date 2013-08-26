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
 * @author Bogdan Iusco
 *
 * @brief Screen used for displaying a list filled with available video quality
 * constants.
 * The user can change current video quality by selecting a list item.
 */

#ifndef VIDEOQUALITYSCREEN_H_
#define VIDEOQUALITYSCREEN_H_

// Include all the wrappers.
#include <NativeUI/Widgets.h>
#include <maapi.h>

using namespace NativeUI;
using namespace MAUtil;

/**
 * @brief Screen used for displaying a list filled with available video quality
 * constants.
 * The user can change current video quality by selecting a list item.
 */
class VideoQualityScreen:
	public Screen,
	public ListViewListener
{
public:
	/**
	 * Constructor.
	 */
	VideoQualityScreen();

	/**
	 * Destructor.
	 */
	virtual ~VideoQualityScreen();

private:
	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();

    /**
     * This method is called when a list view item is clicked.
     * @param listView The list view object that generated the event.
     * @param listViewItem The ListViewItem object that was clicked.
     */
    virtual void listViewItemClicked(
        ListView* listView,
        ListViewItem* listViewItem);

    /**
     * Select and highlight a default quality value.
     */
    void highlightDefaultSetting(ListViewItem* item);

private:
    /**
     * Used for displaying the video quality constants.
     */
    ListView* mListView;

    /**
     * Used for displaying video quality low option.
     */
    ListViewItem* mQualityLowListItem;

    /**
     * Used for displaying video quality medium option.
     */
    ListViewItem* mQualityMediumListItem;

    /**
     * Used for displaying video quality high option.
     */
    ListViewItem* mQualityHighListItem;

    /**
     * Used for storing currently selected list item.
     */
    ListViewItem* mSelectedListItem;
};

#endif /* VIDEOQUALITYSCREEN_H_ */
