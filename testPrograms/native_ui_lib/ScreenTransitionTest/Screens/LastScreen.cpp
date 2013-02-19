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

#define TITLE_TEXT "2nd View. "
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
	LastScreen::LastScreen(SecondScreenObserver& observer):
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
            mObserver.hideSecondScreen();
        }
    }

    void LastScreen::resetTitleWithString(const char* appendText)
    {
        MAUtil::String currentText = TITLE_TEXT;
        currentText.append(appendText, strlen(appendText));
        mTitleLabel->setText(currentText);
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

		mTitleLabel = new NativeUI::Label();
		mTitleLabel->setText(TITLE_TEXT);
		mTitleLabel->setFontSize(TITLE_FONT_SIZE);
		mMainLayout->addChild(mTitleLabel);

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
