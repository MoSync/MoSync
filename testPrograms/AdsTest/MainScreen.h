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
 * @file MainScreen.h
 * @author Bogdan Iusco.
 */

#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

// Include all the wrappers.
#include <NativeUI/Widgets.h>

#include <IX_PIM.h>
#include <maapi.h>
#include <MAUtil/util.h>

#include <Ads/Banner.h>
#include <Ads/BannerListener.h>

using namespace NativeUI;
using namespace Ads;

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

    /**
     * This method is called when there is an touch-down event for
     * a button.
     * @param button The button object that generated the event.
     */
    virtual void buttonPressed(Widget* button);

    /**
     * This method is called when there is an touch-up event for
     * a button.
     * @param button The button object that generated the event.
     */
    virtual void buttonReleased(Widget* button);

    /**
     * This method is called if the touch-up event was inside the
     * bounds of the button.
     * @param button The button object that generated the event.
     */
    virtual void buttonClicked(Widget* button);

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

	Banner* mBanner;

	Label* mBannerMessage;

	CheckBox* mEnableCheckBox;
	Button* mRefreshEnableButton;
	Label* mIsBannerEnabled;

	CheckBox* mShowBannerCheckBox;
	Button* mRefreshVisibleButton;
	Label* mIsBannerVisible;

	Button* mGetSizeButton;
	Label* mSizeLabel;
};


#endif /* MAINSCREEN_H_ */
