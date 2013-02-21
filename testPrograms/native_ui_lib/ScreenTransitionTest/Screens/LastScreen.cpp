/*
 Copyright (C) 2013 MoSync AB

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
 * @file SecondScreen.cpp
 * @author Bogdan Iusco & Mircea Vasiliniuc
 * @date 20 Nov 2012
 *
 * @brief Last screen.
 */

#define SCREEN_INFO_LABEL_TXT "This screen was not pushed to the StackScreen. It was displayed using Screen's showWithTransition method."
#define INFO_LABEL_TXT "The symmetric transition used for going back is:"
#define BACK_BUTTON_TEXT "Go back"

#define SCREEN_COLOR 0x4d5870

#include <NativeUI/Button.h>
#include <NativeUI/HorizontalLayout.h>
#include <NativeUI/VerticalLayout.h>
#include <NativeUI/Label.h>

#include "LastScreen.h"
#include "ScreenUtils.h"

namespace ScreenTransitionTest
{
	/**
	 * Constructor.
	 * @param observer Observer for this screen.
	 */
	LastScreen::LastScreen(LastScreenObserver& observer):
		mObserver(observer),
		mMainLayout(NULL),
		mHideScreenButton(NULL),
		mFooterLayout(NULL)
	{
		this->createUI();
		mHideScreenButton->addButtonListener(this);
	}

	/**
	 * Destructor.
	 */
	LastScreen::~LastScreen()
	{
		mHideScreenButton->removeButtonListener(this);
	}

    /**
     * This method is called if the touch-up event was inside the
     * bounds of the button.
     * Platform: iOS, Android, Windows Phone.
     * @param button The button object that generated the event.
     */
    void LastScreen::buttonClicked(NativeUI::Widget* button)
    {
        if (button == mHideScreenButton)
        {
            mObserver.hideLastScreen();
        }
    }

    void LastScreen::resetTitleWithString(const char* aString)
    {
        mTransitionNameLabel->setText(aString);
    }

	/**
	 * Create screen's UI.
	 */
	void LastScreen::createUI()
	{
		mMainLayout = new NativeUI::VerticalLayout();
		mMainLayout->setBackgroundColor(SCREEN_COLOR);
		mMainLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
		mMainLayout->fillSpaceVertically();
		mMainLayout->fillSpaceHorizontally();
		this->setMainWidget(mMainLayout);

		ScreenUtils::addVerticalSpacerToLayout(mMainLayout, SPACER_HEIGHT);

		NativeUI::Label* screenInfoLabel = new NativeUI::Label();
		screenInfoLabel->setText(SCREEN_INFO_LABEL_TXT);
		screenInfoLabel->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
		screenInfoLabel->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
		screenInfoLabel->setMaxNumberOfLines(3);
		screenInfoLabel->fillSpaceVertically();
		screenInfoLabel->setFontSize(INFO_FONT_SIZE);

		NativeUI::VerticalLayout* paddingLayout = new NativeUI::VerticalLayout();
		paddingLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
		paddingLayout->setPaddingLeft(5);
		paddingLayout->setPaddingRight(5);
		paddingLayout->addChild(screenInfoLabel);
		mMainLayout->addChild(paddingLayout);

		ScreenUtils::addVerticalSpacerToLayout(mMainLayout, SPACER_HEIGHT);

		NativeUI::Label* infoLabel = new NativeUI::Label();
		infoLabel->setText(INFO_LABEL_TXT);
		infoLabel->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
		infoLabel->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
		infoLabel->setFontSize(INFO_FONT_SIZE);
		mMainLayout->addChild(infoLabel);

		mTransitionNameLabel = new NativeUI::Label();
		mTransitionNameLabel->setFontSize(TITLE_FONT_SIZE);
		mMainLayout->addChild(mTransitionNameLabel);

		ScreenUtils::addVerticalSpacerToLayout(mMainLayout, SPACER_HEIGHT);

		NativeUI::VerticalLayout* spacer = new NativeUI::VerticalLayout();
		spacer->fillSpaceVertically();
		mMainLayout->addChild(spacer);

		mFooterLayout = new NativeUI::HorizontalLayout();
		mFooterLayout->setBackgroundColor(BLACK_COLOR);
		mFooterLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
		mFooterLayout->setChildVerticalAlignment(MAW_ALIGNMENT_CENTER);
		mFooterLayout->setHeight(FOOTER_HEIGHT);

		mHideScreenButton = new NativeUI::Button();
		mHideScreenButton->setText(BACK_BUTTON_TEXT);
		mFooterLayout->addChild(mHideScreenButton);

		mMainLayout->addChild(mFooterLayout);
	}
}
