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
 * @file ScreenSettings.cpp
 * @author Emma Tresanszki
 *
 * Settings screen that manages settings in the application.
 * Sets the homepage for the Web screen.
 * Turns on/off the alpha blending for the Spinning cube screen.
 */

#include <mavsprintf.h>
#include <maprofile.h>

// Include the resources for images.
#include "MAHeaders.h"

#include "ScreenSettings.h"

using namespace MoSync::UI;

/*
 * Constructor.
 */
ScreenSettings::ScreenSettings():
    // Initialize member variables.
	mHomepageUrl(WEB_VIEW_HOMEPAGE),
	mCheckBoxState(false)
{

}

/*
 * Destructor.
 */
ScreenSettings::~ScreenSettings()
{

}
/**
 * TODO: Add comment.
 */
Screen* ScreenSettings::create()
{
	ScreenSettings* screen = new ScreenSettings();
	screen->setupUI();

	return screen;
}

/*
 * Setup all widgets.
 */
void ScreenSettings::setupUI()
{
	// Setup the screen.
	setTitle("Settings");
	setIcon(RES_TAB_ICON_SETTINGS);

//	mScreenWidth = getPropertyInt(MAW_WIDGET_WIDTH);
//	mScreenHeight = getPropertyInt(MAW_WIDGET_HEIGHT);
	MAExtent screenSize = maGetScrSize();
	mScreenWidth = EXTENT_X(screenSize);
	mScreenHeight = EXTENT_Y(screenSize);

	// Create the main layout.
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(190, 190, 190);

	// On iPhone add a NavigationBar.
	NavigationBar* navBar = new NavigationBar();
	navBar->setTitle("Settings");
	navBar->setBackButtonTitle("");
	mMainLayout->addChild(navBar);

	// Add a small spacer before the list view.
	VerticalLayout* spacer1 = new VerticalLayout();
	spacer1->setSize(
			MAW_CONSTANT_FILL_AVAILABLE_SPACE,
			SPACER_HEIGHT);
	mMainLayout->addChild(spacer1);

	Label* infoLabel = new Label();
	infoLabel->setText(INFO_LABEL);
	infoLabel->setFontColor(BLACK);
	infoLabel->setFontSize(SETTINGS_FONT_SMALL);
	mMainLayout->addChild(infoLabel);

	// Add a small spacer before the list view.
	VerticalLayout* spacer2 = new VerticalLayout();
	spacer2->setSize(
			MAW_CONSTANT_FILL_AVAILABLE_SPACE,
			SPACER_HEIGHT);
	mMainLayout->addChild(spacer2);

	// Add the list view in a Relative layout, and set a Padding.
	RelativeLayout* listViewLayout = new RelativeLayout();

	ListView* listView = new ListView();

	listView->setSize(mScreenWidth - 2*PADDING, MAW_CONSTANT_FILL_AVAILABLE_SPACE);
	listView->setBackgroundColor(255,255,255);

	// Add a layout with 2 labels and an edit box.
	listView->addChild(createWebSetting());

	// Alpha blending settings
	listView->addChild(createCubeSetting());

	listViewLayout->addChild(listView);
	listView->setPosition(PADDING,PADDING);

	mMainLayout->addChild(listViewLayout);

	// Add a small spacer before the reset layout.
	RelativeLayout* spacer3 = new RelativeLayout();
	spacer3->setSize(
			MAW_CONSTANT_FILL_AVAILABLE_SPACE,
			SPACER_HEIGHT);
	mMainLayout->addChild(spacer3);

	// Vertical layout with Reset button and label.
	VerticalLayout* resetLayout = new VerticalLayout();
	resetLayout->setProperty(MAW_VERTICAL_LAYOUT_CHILD_HORIZONTAL_ALIGNMENT, MAW_ALIGNMENT_CENTER);

	mResetButton = new ImageButton();
	mResetButton->setImage(RES_SETTINGS_VIEW_RESET_IMAGE);
	resetLayout->addChild(mResetButton);

	Label* resetLabel = new Label();
	resetLabel->setText("Reset all settings");
	infoLabel->setFontColor(LIGHT_GRAY);
	infoLabel->setFontSize(SETTINGS_FONT_SMALL);
	resetLayout->addChild(resetLabel);

	mMainLayout->addChild(resetLayout);

	setMainWidget(mMainLayout);
}

/*
 * Create the settings for the web view.
 * @return the list view item.
 */
ListViewItem* ScreenSettings::createWebSetting()
{
	ListViewItem* webSettingItem = new ListViewItem();

	// Add a horizontal layout to the list view.
	// It contains two labels and an edit box.
	HorizontalLayout* hLayout = new HorizontalLayout();

	VerticalLayout* labelLayout = new VerticalLayout();

	Label* editBoxLabel = new Label();
	editBoxLabel->setText(WEB_VIEW_SETTINGS);
	editBoxLabel->setFontSize(SETTINGS_FONT_SMALL);
	editBoxLabel->setFontColor(BLACK);
	labelLayout->addChild(editBoxLabel);

	Label* infoLabel = new Label();
	infoLabel->setText(WEB_VIEW_SETTINGS_LABEL);
	infoLabel->setFontSize(SETTINGS_FONT_MEDIUM);
	infoLabel->setFontColor(LIGHT_GRAY);
	labelLayout->addChild(infoLabel);

	hLayout->addChild(labelLayout);

	// Create an edit box.
	mEditBox  = new EditBox();
	mEditBox->setProperty(
			MAW_EDIT_BOX_TEXT,
			WEB_VIEW_HOMEPAGE);
	mEditBox->setPlaceholder(EDIT_BOX_PLACEHOLDER);
	mEditBox->setTextInputMode();
	hLayout->addChild(mEditBox);

	webSettingItem->addChild(hLayout);

	return webSettingItem;
}

/*
 * Create the settings for the 3D cube view.
 * @return the list view item.
 */
ListViewItem* ScreenSettings::createCubeSetting()
{
	ListViewItem* cubeSettingItem = new ListViewItem();

	// Add a horizontal layout to the list view.
	// It contains a vertical layout of two labels, a spacer and a check box.
	HorizontalLayout* hLayout = new HorizontalLayout();

	// Vertical layout with a title and an info label.
	VerticalLayout* labelLayout = new VerticalLayout();

	Label* alphaTitle = new Label();
	alphaTitle->setText(CUBE_VIEW_SETTINGS);
	alphaTitle->setFontColor(BLACK);
	alphaTitle->setFontSize(SETTINGS_FONT_SMALL);
	labelLayout->addChild(alphaTitle);

	Label* alphaInfo = new Label();
	alphaInfo->setText(CUBE_VIEW_SETTINGS_LABEL);
	alphaInfo->setFontSize(SETTINGS_FONT_MEDIUM);
	alphaInfo->setFontColor(LIGHT_GRAY);
	labelLayout->addChild(alphaInfo);

	hLayout->addChild(labelLayout);

	// The check box is unchecked by default.
	mCheckBox = new CheckBox();
	hLayout->addChild(mCheckBox);

	cubeSettingItem->addChild(hLayout);

	return cubeSettingItem;
}

/**
 * This method is called when there is an event for this widget.
 * @param widget The widget object of the event.
 * @param widgetEventData The low-level event data.
 */
void ScreenSettings::handleWidgetEvent(Widget* widget,
		MAWidgetEventData* widgetEventData) {
	// Is this a click event?
	if (MAW_EVENT_CLICKED == widgetEventData->eventType || MAW_EVENT_POINTER_PRESSED == widgetEventData->eventType )
	{
		if ( widget == mResetButton )
		{
			// Reset the alpha blending to false, and reset the Homepage to default.
			resetToDefaultSettings();
		}
		else if ( widget == mCheckBox )
		{
			setCheckboxStateChanged( mCheckBox->getPropertyString("checked") == "true");
		}
		else if ( widget == mEditBox )
		{
			setHomepage( getCurrentHomepage());
		}

	}

}

/*
 * Reset settings to default.
 */
void ScreenSettings::resetToDefaultSettings()
{
	// Set the default homepage.
	mHomepageUrl = WEB_VIEW_HOMEPAGE;
	mEditBox->setProperty(MAW_EDIT_BOX_TEXT, mHomepageUrl);

	// Set alpha blending setting to unchecked.
	mCheckBoxState = false;
	mCheckBox->setProperty(MAW_CHECK_BOX_CHECKED, mCheckBoxState);
}

/*
 * Set the current value of the check box state,
 * and notify the screen that needs this setting.
 * @param checked The state.
 */
void ScreenSettings::setCheckboxStateChanged(bool checked)
{
	mCheckBoxState = checked;
	// TODO notify cube screen.
//	ScreenSpinningCube::toggleBlending(mCheckBoxState);
}

/*
 * Set the current value of the homepage,
 * and notify the screen that needs this setting.
 * @param url A null terminated string.
 */
void ScreenSettings::setHomepage(const MAUtil::String url)
{
	mHomepageUrl = url;
	//TODO notify web screen.
//	ScreenWebView::setHomepage(mHomepageUrl);
}

/*
 * Get the homepage.
 *@return The current homepage.
 */
const MAUtil::String ScreenSettings::getCurrentHomepage() const
{
	// Or return mHomepageUrl.
	return mEditBox->getPropertyString(MAW_EDIT_BOX_TEXT);
}

/*
 * Get the current state of the check box.
 *@param The current check box state.
 */
const bool ScreenSettings::getAlphaEnabledState() const
{
	return mCheckBoxState;
}
