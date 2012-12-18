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
 * @file DisclaimerScreen.cpp
 * @author Bogdan Iusco
 *
 * @brief NativeUI Screen used to show the disclaimer text.
 */

#define TITLE_BAR_HEIGHT_PERCENTAGE 11
#define PADDING_LEFT_PERCENTAGE 2
#define SCREEN_TITLE "Disclaimer"

#define LABEL_MAX_LINES 50
#define DISCLAIMER_TEXT "This application uses information from Wikipedia: http://en.wikipedia.org/wiki/List_of_sovereign_states_and_dependent_territories_in_Europe and the specific link for each covered country."

#include <NativeUI/HorizontalLayout.h>
#include <NativeUI/Image.h>
#include <NativeUI/ImageButton.h>
#include <NativeUI/Label.h>
#include <NativeUI/RelativeLayout.h>
#include <NativeUI/VerticalLayout.h>

#include "DisclaimerScreen.h"
#include "DisclaimerScreenObserver.h"
#include "MAHeaders.h"
#include "ViewUtils.h"

namespace EuropeanCountries
{

	/**
	 * Constructor.
	 * @param observer Observer for this screen.
	 */
	DisclaimerScreen::DisclaimerScreen(DisclaimerScreenObserver& observer):
		mObserver(observer),
		mMainLayout(NULL),
		mTitleBarLayout(NULL),
		mBackButton(NULL),
		mTitleBarHeight(0)
	{
		this->createUI();
		if (!isAndroid())
		{
			mBackButton->addButtonListener(this);
		}
	}

	/**
	 * Destructor.
	 */
	DisclaimerScreen::~DisclaimerScreen()
	{
		if (!isAndroid())
		{
			mBackButton->addButtonListener(this);
		}
	}

	/**
	 * This method is called if the touch-up event was inside the
	 * bounds of the button.
	 * @param button The button object that generated the event.
	 */
	void DisclaimerScreen::buttonClicked(NativeUI::Widget* button)
	{
		mObserver.showCountryInfoScreen();
	}

	/**
	 * Create screen UI.
	 */
	void DisclaimerScreen::createUI()
	{
		// Create and add layout to the screen.
		mMainLayout = new NativeUI::RelativeLayout();
		this->setMainWidget(mMainLayout);

		this->createImageWidget();
		this->createTitleBar();
		this->createDisclaimerLabel();
	}

	/**
	 * Create and add an Image widget to the screen's main layout.
	 * It will take the screen size.
	 */
	void DisclaimerScreen::createImageWidget()
	{
		NativeUI::Image* imageWidget = new NativeUI::Image();
		imageWidget->setHeight(gScreenHeight);
		imageWidget->setWidth(gScreenWidth);
		imageWidget->setTopPosition(0);
		imageWidget->setLeftPosition(0);
		imageWidget->setImage(R_SCREEN_BACKGROUND);
		imageWidget->setScaleMode(NativeUI::IMAGE_SCALE_XY);
		mMainLayout->addChild(imageWidget);
	}

	/**
	 * Create and add an HorizontalLayout to the screen's main layout.
	 * The layout will contain an back button.
	 */
	void DisclaimerScreen::createTitleBar()
	{
		// Create and add layout for title bar.
		mTitleBarHeight = gScreenHeight / TITLE_BAR_HEIGHT_PERCENTAGE;
		mTitleBarLayout = new NativeUI::HorizontalLayout();
		mTitleBarLayout->setLeftPosition(0);
		mTitleBarLayout->setTopPosition(0);
		mTitleBarLayout->setWidth(gScreenWidth);
		mTitleBarLayout->setHeight(mTitleBarHeight);
		mMainLayout->addChild(mTitleBarLayout);

		if (!isAndroid())
		{
			// For WP7 and iOS platforms add a back button that should be
			// used to navigate back to previous screen.
			// On Android platform the device's back button should be used.
			mBackButton = new NativeUI::ImageButton();
			mBackButton->setHeight(mTitleBarHeight);
			mBackButton->setImage(R_BACK_BUTTON);
			mTitleBarLayout->addChild(mBackButton);
		}
		if (!isWindowsPhone())
		{
			mTitleBarLayout->setProperty(MAW_WIDGET_BACKGROUND_COLOR, gTitleBackgroundColor);
		}

		// Create and add screen title label.
		mTitleLabel = new NativeUI::Label();
		mTitleLabel->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
		mTitleLabel->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
		mTitleLabel->fillSpaceHorizontally();
		mTitleLabel->fillSpaceVertically();
		mTitleLabel->setFontColor(COLOR_WHITE);
		mTitleLabel->setText(SCREEN_TITLE);
		mTitleBarLayout->addChild(mTitleLabel);

		// Add spacer so the title label will be centered.
		if (!isAndroid())
		{
			mTitleBarLayout->addChild(
				createSpacer(mTitleBarHeight, mBackButton->getWidth()));
		}
	}

	/**
	 * Create and add label to screen's main layout that will contain the
	 * disclaimer text.
	 */
	void DisclaimerScreen::createDisclaimerLabel()
	{
		// Calculate label position and size.
		int labelTopPosition = mTitleBarHeight;
		int labelLeftPosition = gScreenWidth * PADDING_LEFT_PERCENTAGE / 100;
		int labelHeight = gScreenHeight - labelTopPosition;
		int labelWidth = gScreenWidth - labelLeftPosition;

		// Create and add the label.
		NativeUI::Label* label = new NativeUI::Label();
		label->setTopPosition(labelTopPosition);
		label->setLeftPosition(labelLeftPosition);
		label->setWidth(labelWidth);
		label->setHeight(labelHeight);
		label->setMaxNumberOfLines(LABEL_MAX_LINES);
		label->setTextHorizontalAlignment(MAW_ALIGNMENT_LEFT);
		label->setTextVerticalAlignment(MAW_ALIGNMENT_TOP);
		label->setFontColor(COLOR_WHITE);
		label->setText(DISCLAIMER_TEXT);
		mMainLayout->addChild(label);
	}

} // end of EuropeanCountries
