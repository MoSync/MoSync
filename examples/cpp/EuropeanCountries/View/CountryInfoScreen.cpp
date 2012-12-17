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
 * @file CountryInfoScreen.cpp
 * @author Bogdan Iusco
 *
 * @brief NativeUI Screen used to show country info.
 */

#define PADDING_LEFT_PERCENTAGE 2
#define BACK_BUTTON_WIDTH 64
#define TITLE_BAR_HEIGHT_PERCENTAGE 11
#define MAX_TITLE_BAR_HEIGHT 70

#define POPULATION_LABEL_TEXT "Population"
#define AREA_LABEL_TEXT "Area"
#define LANGUAGES_LABEL_TEXT "Languages"
#define GOVERNMENT_LABEL_TEXT "Government"
#define CAPITAL_LABEL_TEXT "Capital"

#define DISCLAIMER_BUTTON_TEXT "Disclaimer"

#include <NativeUI/Button.h>
#include <NativeUI/HorizontalLayout.h>
#include <NativeUI/Image.h>
#include <NativeUI/ImageButton.h>
#include <NativeUI/Label.h>
#include <NativeUI/RelativeLayout.h>
#include <NativeUI/VerticalLayout.h>

#include "CountryInfoScreen.h"
#include "CountryInfoScreenObserver.h"
#include "MAHeaders.h"
#include "ViewUtils.h"
#include "../Model/Country.h"

namespace EuropeanCountries
{
	/**
	 * Constructor.
	 * @param observer Will be notified when user taps the back button.
	 */
	CountryInfoScreen::CountryInfoScreen(CountryInfoScreenObserver& observer):
		mObserver(observer),
		mMainLayout(NULL),
		mDataLayout(NULL),
		mTitleBarLayout(NULL),
		mInfoLayout(NULL),
		mNameLabel(NULL),
		mBackButton(NULL),
		mDisclaimerTitleBarButton(NULL),
		mPopulationLabel(NULL),
		mAreaLabel(NULL),
		mLanguagesLabel(NULL),
		mGovernmentLabel(NULL),
		mCapitalLabel(NULL),
		mDisclaimerScrollAreaButton(NULL),
		mInfoLayoutWidth(0)
	{
		if (isIOS())
		{
			initScreenSizeConstants(this->getWidth(), this->getHeight());
		}

		this->createUI();

		if (isAndroid())
		{
			mDisclaimerScrollAreaButton->addButtonListener(this);
		}
		else
		{
			mBackButton->addButtonListener(this);
			mDisclaimerTitleBarButton->addButtonListener(this);
		}
	}

	/**
	 * Destructor.
	 */
	CountryInfoScreen::~CountryInfoScreen()
	{
		if (isAndroid())
		{
			mDisclaimerScrollAreaButton->removeButtonListener(this);
		}
		else
		{
			mBackButton->removeButtonListener(this);
			mDisclaimerTitleBarButton->removeButtonListener(this);
		}
	}

	/**
	 * Show a screen.
	 */
	void CountryInfoScreen::show()
	{
		if (isIOS())
		{
			// Scroll to the top of the screen.
			mInfoLayoutRelative->setContentOffset(0, 0);
			mBackButton->setImage(R_BACK_BUTTON);
		}
		NativeUI::Screen::show();
	}

	/**
	 * Set displayed country.
	 * Labels will be filled with data from a given country.
	 * @param country Country to display.
	 */
	void CountryInfoScreen::setDisplayedCountry(const Country& country)
	{
		mNameLabel->setText(country.getName());
		mPopulationLabel->setText(country.getPopulation());
		mAreaLabel->setText(country.getArea());
		mLanguagesLabel->setText(country.getLanguages());
		mGovernmentLabel->setText(country.getGovernment());
		mCapitalLabel->setText(country.getCapital());
	}

	/**
	 * This method is called if the touch-up event was inside the
	 * bounds of the button.
	 * From ButtonListener.
	 * @param button The button object that generated the event.
	 */
	void CountryInfoScreen::buttonClicked(NativeUI::Widget* button)
	{
		if (mBackButton == button)
		{
			mObserver.showCountriesListScreen();
		}
		else if (mDisclaimerTitleBarButton == button ||
				 mDisclaimerScrollAreaButton == button)
		{
			mObserver.showDisclaimerScreen();
		}
	}

	/**
	 * Create screen UI.
	 */
	void CountryInfoScreen::createUI()
	{
		// Create and add layout to the screen.
		mMainLayout = new NativeUI::RelativeLayout();
		this->setMainWidget(mMainLayout);

		this->createImageWidget();

		if (isAndroid())
		{
			// For Android platform create a layout that will hold widgets.
			mDataLayout = new NativeUI::VerticalLayout();
			mDataLayout->setHeight(gScreenHeight);
			mDataLayout->setWidth(gScreenWidth);
			mDataLayout->setTopPosition(0);
			mDataLayout->setLeftPosition(0);
			mMainLayout->addChild(mDataLayout);
		}
		else
		{
			mDataLayout = mMainLayout;
		}

		this->createTitleBar();
		this->createInfoLayout();

		this->createPopulationLabel();
		this->createAreaLabel();
		this->createLanguagesLabel();
		this->createGovernmentLabel();
		this->createCapitalLabel();

		if (isAndroid())
		{
			this->createDisclaimerButton();
		}

		mInfoLayout->addChild(createSpacer());
	}

	/**
	 * Create and add an Image widget to the screen's main layout.
	 */
	void CountryInfoScreen::createImageWidget()
	{
		NativeUI::Image* image = new NativeUI::Image();
		image->setHeight(gScreenHeight);
		image->setWidth(gScreenWidth);
		image->setTopPosition(0);
		image->setLeftPosition(0);
		image->setImage(R_SCREEN_BACKGROUND);
		image->setScaleMode(NativeUI::IMAGE_SCALE_XY);
		mMainLayout->addChild(image);
	}

	/**
	 * Create and add an HorizontalLayout to the screen's main layout.
	 * The layout will contain an back button.
	 */
	void CountryInfoScreen::createTitleBar()
	{
		// Create and add layout that will hold all title bar related widgets.
		int barHeight = gScreenHeight / TITLE_BAR_HEIGHT_PERCENTAGE;
		if (barHeight > MAX_TITLE_BAR_HEIGHT)
		{
			barHeight = MAX_TITLE_BAR_HEIGHT;
		}
		mTitleBarLayout = new NativeUI::HorizontalLayout();
		mTitleBarLayout->setLeftPosition(0);
		mTitleBarLayout->setTopPosition(0);
		mTitleBarLayout->setWidth(gScreenWidth);
		mTitleBarLayout->setHeight(barHeight);
		mDataLayout->addChild(mTitleBarLayout);

		if (!isAndroid())
		{
			// Add back button only for iOS and WP7 platforms.
			// On Android platform the device's back button will be used.
			mBackButton = new NativeUI::ImageButton();
			mBackButton->setHeight(barHeight);
			mBackButton->setImage(R_BACK_BUTTON);
			mTitleBarLayout->addChild(mBackButton);
		}
		if (!isWindowsPhone())
		{
			mTitleBarLayout->setProperty(MAW_WIDGET_BACKGROUND_COLOR, gTitleBackgroundColor);
		}

		// Create and add label that will display the country name.
		mNameLabel = new NativeUI::Label();
		mNameLabel->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
		mNameLabel->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
		mNameLabel->fillSpaceHorizontally();
		mNameLabel->fillSpaceVertically();
		mNameLabel->setFontColor(COLOR_WHITE);
		mTitleBarLayout->addChild(mNameLabel);

		if (!isAndroid())
		{
			// Add disclaimer button only on iOS and WP7 platforms.
			// On Android the button will be added at the bottom of the screen.
			mDisclaimerTitleBarButton = new NativeUI::ImageButton();
			mDisclaimerTitleBarButton->setHeight(barHeight);
			mDisclaimerTitleBarButton->setImage(R_DISCLAIMER_BUTTON);

			if (isIOS())
			{
				// Add spacer.
				int spacerWidth = mBackButton->getWidth() - barHeight;
				mTitleBarLayout->addChild(createSpacer(mNameLabel->getHeight(), spacerWidth));
			}

			mTitleBarLayout->addChild(mDisclaimerTitleBarButton);
		}
	}

	/**
	 * Create and add an VerticalLayout widget to the screen's main layout.
	 * The widget will be transparent.
	 */
	void CountryInfoScreen::createInfoLayout()
	{
		int titleBarHeight = mTitleBarLayout->getHeight();
		int height = gScreenHeight - titleBarHeight;
		if (isIOS())
		{
			mInfoLayoutRelative = new NativeUI::RelativeLayout();
			mInfoLayoutRelative->setTopPosition(titleBarHeight);
			mInfoLayoutRelative->setLeftPosition(0);
			mInfoLayoutRelative->setHeight(height);
			mInfoLayoutRelative->setWidth(gScreenWidth);
			mInfoLayoutRelative->setScrollable(true);
			mDataLayout->addChild(mInfoLayoutRelative);
		}
		else if (isWindowsPhone())
		{
			mInfoLayoutVertical = new NativeUI::VerticalLayout();
			mInfoLayoutVertical->setTopPosition(titleBarHeight);
			mInfoLayoutVertical->setLeftPosition(0);
			mInfoLayoutVertical->setHeight(height);
			mInfoLayoutVertical->setWidth(gScreenWidth);
			mDataLayout->addChild(mInfoLayoutVertical);
		}

		int paddingLeft = gScreenWidth * PADDING_LEFT_PERCENTAGE / 100;
		mInfoLayoutWidth = gScreenWidth - (2 * paddingLeft);

		mInfoLayout = new NativeUI::VerticalLayout();
		mInfoLayout->setWidth(mInfoLayoutWidth);
		mInfoLayout->setTopPosition(0);
		mInfoLayout->setScrollable(true);
		mInfoLayout->setLeftPosition(paddingLeft);

		if (isAndroid())
		{
			mInfoLayout->setPaddingLeft(paddingLeft);
			mInfoLayout->wrapContentVertically();
			mInfoLayout->setProperty(MAW_WIDGET_BACKGROUND_COLOR, gLayoutBackgroundColor);
			mDataLayout->addChild(mInfoLayout);
		}
		else if (isIOS())
		{
			mInfoLayout->wrapContentVertically();
			mInfoLayoutRelative->addChild(mInfoLayout);
			mInfoLayout->setProperty(MAW_WIDGET_BACKGROUND_COLOR, gLayoutBackgroundColor);
		}
		else if (isWindowsPhone())
		{
			mInfoLayoutVertical->addChild(mInfoLayout);
		}
	}

	/**
	 * Create and add two labels: first with "Population" text and second
	 * with country's population.
	 */
	void CountryInfoScreen::createPopulationLabel()
	{
		mInfoLayout->addChild(createSpacer());
		NativeUI::Label* countryTextLabel = createLabel(
			POPULATION_LABEL_TEXT, COLOR_LABEL_INFO, mInfoLayoutWidth);
		mInfoLayout->addChild(countryTextLabel);
		mInfoLayout->addChild(createSpacer());

		mPopulationLabel = createLabel("", COLOR_LABEL_DATA, mInfoLayoutWidth);
		mInfoLayout->addChild(mPopulationLabel);
	}

	/**
	 * Create and add two labels: first with "Area" text and second
	 * with country's area.
	 */
	void CountryInfoScreen::createAreaLabel()
	{
		mInfoLayout->addChild(createSpacer());
		NativeUI::Label* areaTextLabel =
			createLabel(AREA_LABEL_TEXT, COLOR_LABEL_INFO, mInfoLayoutWidth);
		mInfoLayout->addChild(areaTextLabel);
		mInfoLayout->addChild(createSpacer());

		mAreaLabel = createLabel("", COLOR_LABEL_DATA, mInfoLayoutWidth);
		mInfoLayout->addChild(mAreaLabel);
	}

	/**
	 * Create and add two labels: first with "Languages" text and second
	 * with country's languages.
	 */
	void CountryInfoScreen::createLanguagesLabel()
	{
		mInfoLayout->addChild(createSpacer());
		NativeUI::Label* languagesTextLabel = createLabel(
			LANGUAGES_LABEL_TEXT, COLOR_LABEL_INFO, mInfoLayoutWidth);
		mInfoLayout->addChild(languagesTextLabel);
		mInfoLayout->addChild(createSpacer());

		mLanguagesLabel = createLabel("", COLOR_LABEL_DATA, mInfoLayoutWidth);
		mInfoLayout->addChild(mLanguagesLabel);
	}

	/**
	 * Create and add two labels: first with "Government" text and second
	 * with country's government.
	 */
	void CountryInfoScreen::createGovernmentLabel()
	{
		mInfoLayout->addChild(createSpacer());
		NativeUI::Label* governmentTextLabel = createLabel(
			GOVERNMENT_LABEL_TEXT, COLOR_LABEL_INFO, mInfoLayoutWidth);
		mInfoLayout->addChild(governmentTextLabel);
		mInfoLayout->addChild(createSpacer());

		mGovernmentLabel = createLabel("", COLOR_LABEL_DATA, mInfoLayoutWidth);
		mInfoLayout->addChild(mGovernmentLabel);
	}

	/**
	 * Create and add two labels: first with "Capital" text and second
	 * with country's capital.
	 */
	void CountryInfoScreen::createCapitalLabel()
	{
		mInfoLayout->addChild(createSpacer());
		NativeUI::Label* capitalTextLabel = createLabel(
			CAPITAL_LABEL_TEXT, COLOR_LABEL_INFO, mInfoLayoutWidth);
		mInfoLayout->addChild(capitalTextLabel);
		mInfoLayout->addChild(createSpacer());

		mCapitalLabel = createLabel("", COLOR_LABEL_DATA, mInfoLayoutWidth);
		mInfoLayout->addChild(mCapitalLabel);
	}

	/**
	 * Create and add button used to show the disclaimer screen.
	 * Only for Android platform.
	 */
	void CountryInfoScreen::createDisclaimerButton()
	{
		mInfoLayout->addChild(createSpacer());
		mDisclaimerScrollAreaButton = new NativeUI::Button();
		mDisclaimerScrollAreaButton->setText(DISCLAIMER_BUTTON_TEXT);
		mDisclaimerScrollAreaButton->setWidth(mInfoLayoutWidth);
		mDisclaimerScrollAreaButton->wrapContentVertically();
		mInfoLayout->addChild(mDisclaimerScrollAreaButton);
	}

} // end of EuropeanCountries
