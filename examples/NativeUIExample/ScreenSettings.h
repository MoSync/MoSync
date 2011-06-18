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
 * @file ScreenSettings.h
 * @author Emma Tresanszki
 *
 * Settings screen that manages settings in the application.
 * Sets the homepage for the Web screen.
 * Turns on/off the alpha blending for the Spinning cube screen.
 */


#ifndef SCREEN_SETTINGS_H_
#define SCREEN_SETTINGS_H_

#include "UIWrapper/Widgets.h"

using namespace MoSync::UI;

/**
 * Strings for this screen.
 */
const MAUtil::String INFO_LABEL = " Please select desired settings ";
const MAUtil::String WEB_VIEW_HOMEPAGE = "www.mosync.com";
const MAUtil::String EDIT_BOX_PLACEHOLDER = "Default homepage";

const MAUtil::String WEB_VIEW_SETTINGS = " HOMEPAGE ";
const MAUtil::String CUBE_VIEW_SETTINGS = " Use Alpha blending ";
const MAUtil::String WEB_VIEW_SETTINGS_LABEL = " Web View settings ";
const MAUtil::String CUBE_VIEW_SETTINGS_LABEL = " Settings for 3D Cube ";

/** Utility sizes. */
enum FontSize{
	SETTINGS_FONT_SMALL   = 15 ,
	SETTINGS_FONT_MEDIUM  = 12 ,
	SPACER_HEIGHT         = 10 ,
	PADDING               = 5
};

// Colors used for widgets.
enum Colors
{
	// For title labels.
	BLACK = 0x000000 ,
	// For the background and some labels.
	LIGHT_GRAY = 0x969696
};

/**
 * TODO: Add comment.
 */
class ScreenSettings :
	public Screen,
	public WidgetEventListener
{
public:

	/**
	 * TODO: Add comment.
	 */
	static Screen* create();

	/*
	 * Destructor.
	 */
	virtual ~ScreenSettings();

	/*
	 * Create the settings for the web view.
	 * @return the layout.
	 */
	ListViewItem* createWebSetting();

	/*
	 * Create the settings for the 3D cube view.
	 * @return the layout.
	 */
	ListViewItem* createCubeSetting();

	/**
	 * from WidgetEventListener
	 * This method is called when there is an event for this widget.
	 * @param widget The widget object of the event.
	 * @param widgetEventData The low-level event data.
	 */
	virtual void handleWidgetEvent(
		Widget* widget,
		MAWidgetEventData* widgetEventData);

	/*
	 * Reset settings to default.
	 */
	void resetToDefaultSettings();

	/*
	 * Set the current value of the check box state,
	 * and notify the screen that needs this setting.
	 * @param checked The state.
	 */
	void setCheckboxStateChanged(bool checked);

	/*
	 * Set the current value of the homepage,
	 * and notify the screen that needs this setting.
	 * @param url A null terminated string.
	 */
	void setHomepage(const MAUtil::String url);

	/*
	 * Get the homepage.
	 *@return The current homepage.
	 */
	const MAUtil::String getCurrentHomepage() const;

	/*
	 * Get the current state of the check box.
	 *@param The current check box state.
	 */
	const bool getAlphaEnabledState() const;

private:
	/*
	 * Constructor.
	 */
	ScreenSettings();

	/*
	 * Setup all widgets.
	 */
	void setupUI();

private:

	VerticalLayout* mMainLayout;
	ListViewItem* webSettingItem;
	ListViewItem* cubeSettingItem;

	/** The edit box for the web view homepage. */
	EditBox* mEditBox;

	/** The image button for reseting all settings. */
	ImageButton* mResetButton;

	/** The check box for alpha blending setting. */
	CheckBox* mCheckBox;

	/** The current value of the check box. */
	bool mCheckBoxState;

	/** The current value for the homepage. */
	MAUtil::String mHomepageUrl;

	/*
	 * Screen size.
	 */
	int mScreenWidth;
	int mScreenHeight;
};

#endif
