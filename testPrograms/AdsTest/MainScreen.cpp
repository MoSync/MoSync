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
 * @file MainScreen.cpp
 * @author Emma Tresanszki and Bogdan Iusco
 *
 * @brief Screen used for displaying a banner.
 * Contains widgets(e.g. check box, button) used for setting and getting
 * values for banner.
 */

#include <conprint.h>
#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <maxtoa.h>
#include <MAUtil/util.h>

#include "MainScreen.h"
#include "Util.h"

#include "MainScreen.h"

/**
 * Constructor.
 */
MainScreen::MainScreen() :
	Screen(),
	mBanner(NULL),
	mBannerMessage(NULL),
	mEnableCheckBox(NULL),
	mRefreshEnableButton(NULL),
	mIsBannerEnabled(NULL),
	mGetSizeButton(NULL),
	mSizeLabel(NULL),
	mRefreshVisibleButton(NULL),
	mIsBannerVisible(NULL)
{
	createMainLayout();

	mEnableCheckBox->addCheckBoxListener(this);
	mShowBannerCheckBox->addCheckBoxListener(this);
	mBanner->addBannerListener(this);
	mGetSizeButton->addButtonListener(this);
	mRefreshEnableButton->addButtonListener(this);
	mRefreshVisibleButton->addButtonListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
	mEnableCheckBox->removeCheckBoxListener(this);
	mShowBannerCheckBox->removeCheckBoxListener(this);
	mBanner->removeBannerListener(this);
	mGetSizeButton->removeButtonListener(this);
	mRefreshEnableButton->removeButtonListener(this);
	mRefreshVisibleButton->removeButtonListener(this);
}

/**
 * Create a new row for settings.
 * It contains a check box/button and a label.
 */
HorizontalLayout* MainScreen::createRow(Widget* check, Label* label)
{
	HorizontalLayout* hLayout = new HorizontalLayout();
	if ( check != NULL )
	{
		hLayout->addChild(check);
		HorizontalLayout* space = new HorizontalLayout();
		space->setWidth(10);
		hLayout->addChild(space);
	}
	hLayout->addChild(label);
	return hLayout;
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout() {
	// Create and add the main layout to the screen.
	VerticalLayout* mainLayout = new VerticalLayout();
	Screen::setMainWidget(mainLayout);
	mBanner = new Banner("a14dbba084368db");
	mBanner->requestContent(true);
	mainLayout->addBanner(mBanner);

	Label* label;

	// Add banner status to list item
	mBannerMessage = new Label();
	mBannerMessage->setText("before loading Ad");
	mBannerMessage->setFontColor(0xFF0000);
	mainLayout->addChild(mBannerMessage);

	// Add "Enable/Disable banner" list item
	label = new Label();
	label->setText("Enable click on banner");
	mEnableCheckBox = new CheckBox();
	mEnableCheckBox->setState(true);
	mainLayout->addChild(createRow(mEnableCheckBox, label));

	// Add "Is banner enabled/disabled status" into a list item
	mRefreshEnableButton = new Button();
	mRefreshEnableButton->setText("Is Enabled");
	mRefreshEnableButton->wrapContentHorizontally();
	mIsBannerEnabled = new Label();
	mIsBannerEnabled->setText("Banner is enabled");
	mainLayout->addChild(createRow(mRefreshEnableButton, mIsBannerEnabled));

	// Add "Show/Hide banner" list item
	label = new Label();
	label->setText("Show banner");
	mShowBannerCheckBox = new CheckBox();
	mShowBannerCheckBox->setState(true);
	mainLayout->addChild(createRow(mShowBannerCheckBox, label));

	// Add "Is banner visible status" into a list item
	mRefreshVisibleButton = new Button();
	mRefreshVisibleButton->setText("Is Visible");
	mRefreshVisibleButton->wrapContentHorizontally();
	mIsBannerVisible = new Label();
	mIsBannerVisible->setText("Banner is visible");
	mainLayout->addChild(createRow(mRefreshVisibleButton, mIsBannerVisible));

	// Add "Get banner size" list item
	mGetSizeButton = new Button();
	mGetSizeButton->setText("Get size");
	mGetSizeButton->wrapContentHorizontally();
	mSizeLabel = new Label();
	mSizeLabel->setText("Banner size: 0 x 0 px");
	mainLayout->addChild(createRow(mGetSizeButton, mSizeLabel));
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonClicked(Widget* button)
{
	if (button == mGetSizeButton)
	{
		int height = mBanner->getHeight();
		int width = mBanner->getWidth();
		char buffer[124];
		sprintf(buffer, "Banner size: %d x %d px", width, height);
		mSizeLabel->setText(buffer);
	}
	else if (button == mRefreshEnableButton)
	{
		bool isEnabled = mBanner->isEnabled();
		if (isEnabled)
		{
			mIsBannerEnabled->setText("Banner is enabled");
		}
		else
		{
			mIsBannerEnabled->setText("Banner is disabled");
		}
	}
	else if (button == mRefreshVisibleButton)
	{
		bool isVisible = mBanner->isVisible();
		if (isVisible)
		{
			mIsBannerVisible->setText("Banner is visible");
		}
		else
		{
			mIsBannerVisible->setText("Banner is not visible");
		}
	}
}

/**
 * Called when a banner view fails to load a new advertisement.
 * @param banner The banner view that failed to load an advertisement.
 * @param error Describes the problem.
 */
void MainScreen::bannerFailedLoad(
		Banner* banner,
		int error)
{
	printf("MainScreen::bannerFailedLoad error code = %d", error);
	char buf[4];
	itoa(error, buf, 10);
	MAUtil::String text = "bannerFailedLoad error code = ";
	text += buf;
	mBannerMessage->setText("bannerFailedLoad");
}

/**
 * Called when a new banner advertisement is loaded.
 * @param banner The banner that failed to load an advertisement.
 */
void MainScreen::bannerLoaded(Banner* banner)
{
	printf("MainScreen::bannerLoaded");
	mBannerMessage->setText("bannerLoaded");
}

/**
 * Called when the user taps the banner view.
 * The application is moved to background.
 * @param banner The banner that the user tapped.
 */
void MainScreen::bannerOnLeaveApplication(Banner* banner)
{
	printf("MainScreen::bannerOnLeaveApplication");
	mBannerMessage->setText("bannerOnLeaveApplication");
}

/**
 * Called after a banner view finishes executing an action that
 * covered your application's user interface.
 * @param banner The banner view that finished executing an action.
 */
void MainScreen::bannerOnDismiss(Banner* banner)
{
	printf("MainScreen::bannerOnDismiss");
	mBannerMessage->setText("bannerOnDismiss");
}

/**
 * This method is called when the state of the check box was changed
 * by the user.
 * @param checkBox The check box object that generated the event.
 * @param state True if the check box is checked, false otherwise.
 */
void MainScreen::checkBoxStateChanged(
    CheckBox* checkBox,
    bool state)
{
	if (checkBox == mEnableCheckBox)
	{
		mBanner->setEnabled(mEnableCheckBox->isChecked() ? true : false );
	}
	else if (checkBox == mShowBannerCheckBox)
	{
		mBanner->setVisible(mShowBannerCheckBox->isChecked() ? true : false );
	}
}
