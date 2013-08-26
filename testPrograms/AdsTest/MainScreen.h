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
 * @file MainScreen.h
 * @author Emma Tresanszki and Bogdan Iusco
 *
 * @brief Screen used for displaying a banner.
 * Contains widgets(e.g. check box, button) used for setting and getting
 * values for banner.
 */

#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

// Include all the wrappers.
#include <NativeUI/Widgets.h>

#include <maapi.h>

#include <Ads/Banner.h>
#include <Ads/BannerListener.h>

using namespace NativeUI;
using namespace Ads;

/**
 * @brief Screen used for displaying a banner.
 * Contains widgets(e.g. check box, button) used for setting and getting
 * values for banner.
 */
class MainScreen:
	public Screen,
	public ButtonListener,
	public CheckBoxListener,
	public BannerListener
{

public:
	/**
	 * Constructor.
	 */
	MainScreen();

	/**
	 * Destructor.
	 */
	~MainScreen();

private:
	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();

	/**
	 * Create a new row for settings.
	 * It contains a check box/button and a label.
	 */
	HorizontalLayout* createRow(Widget* check, Label* label);

    /**
     * Called when a banner view fails to load a new advertisement.
     * @param banner The banner view that failed to load an advertisement.
     * @param error Describes the problem.
     */
    virtual void bannerFailedLoad(
			Banner* banner,
			int error);

    /**
     * Called when a new banner advertisement is loaded.
     * @param banner The banner that failed to load an advertisement.
     */
    virtual void bannerLoaded(Banner* banner);

    /**
     * Called when the user taps the banner view.
     * The application is moved to background.
	 * @param banner The banner that the user tapped.
     */
    virtual void bannerOnLeaveApplication(Banner* banner);

    /**
     * Called after a banner view finishes executing an action that
	 * covered your application's user interface.
	 * @param banner The banner view that finished executing an action.
     */
    virtual void bannerOnDismiss(Banner* banner);

    /**
     * This method is called if the touch-up event was inside the
     * bounds of the button.
     * @param button The button object that generated the event.
     */
    virtual void buttonClicked(Widget* button);

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
     * First widget displayed on the screen.
     */
	Banner* mBanner;

	/**
	 * Displays banner's status.
	 */
	Label* mBannerMessage;

	CheckBox* mEnableCheckBox;
	Button* mRefreshEnableButton;
	Label* mIsBannerEnabled;

	/**
	 * Widgets for setting & getting visible property for banner.
	 */
	CheckBox* mShowBannerCheckBox;
	Button* mRefreshVisibleButton;
	Label* mIsBannerVisible;

	/**
	 * Widgets for getting size property for banner.
	 */
	Button* mGetSizeButton;
	Label* mSizeLabel;
};

#endif /* MAINSCREEN_H_ */
