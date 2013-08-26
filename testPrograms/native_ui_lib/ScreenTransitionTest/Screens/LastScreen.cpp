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
		if ( ScreenUtils::OS_WIN != ScreenUtils::getCurrentPlatform() )
		{
			mMainLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
		}
		mMainLayout->fillSpaceVertically();
		mMainLayout->fillSpaceHorizontally();
		setMainWidget(mMainLayout);

		ScreenUtils::addVerticalSpacerToLayout(mMainLayout, SPACER_HEIGHT);

		addInfoLabel();

		ScreenUtils::addVerticalSpacerToLayout(mMainLayout, SPACER_HEIGHT);

		addTransitionInfoLabels();

		ScreenUtils::addVerticalSpacerToLayout(mMainLayout, SPACER_HEIGHT);

		addFooter();
	}

	void LastScreen::addInfoLabel()
	{
		NativeUI::Label* screenInfoLabel = new NativeUI::Label();
		screenInfoLabel->setText(SCREEN_INFO_LABEL_TXT);
		screenInfoLabel->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
		screenInfoLabel->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
		screenInfoLabel->setMaxNumberOfLines(3);
		screenInfoLabel->fillSpaceHorizontally();
		screenInfoLabel->fillSpaceVertically();
		screenInfoLabel->setFontSize(INFO_FONT_SIZE);

		NativeUI::VerticalLayout* paddingLayout = new NativeUI::VerticalLayout();
		paddingLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
		paddingLayout->setPaddingLeft(5);
		paddingLayout->setPaddingRight(5);
		paddingLayout->addChild(screenInfoLabel);
		mMainLayout->addChild(paddingLayout);
	}

	void LastScreen::addTransitionInfoLabels()
	{
		NativeUI::Label* infoLabel = new NativeUI::Label();
		infoLabel->setText(INFO_LABEL_TXT);
		infoLabel->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
		infoLabel->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
		infoLabel->setFontSize(INFO_FONT_SIZE);
		infoLabel->fillSpaceVertically();
		infoLabel->fillSpaceHorizontally();
		infoLabel->setMaxNumberOfLines(3);
		mMainLayout->addChild(infoLabel);

		mTransitionNameLabel = new NativeUI::Label();
		mTransitionNameLabel->setFontSize(TITLE_FONT_SIZE);
		mTransitionNameLabel->fillSpaceHorizontally();
		mTransitionNameLabel->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
		mTransitionNameLabel->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
		mMainLayout->addChild(mTransitionNameLabel);
	}

	void LastScreen::addFooter()
	{
		mFooterLayout = new NativeUI::HorizontalLayout();
		mFooterLayout->setBackgroundColor(BLACK_COLOR);
		mFooterLayout->setHeight(FOOTER_HEIGHT);

		mHideScreenButton = new NativeUI::Button();
		mHideScreenButton->setText(BACK_BUTTON_TEXT);
		mHideScreenButton->addButtonListener(this);

		if ( ScreenUtils::OS_WIN == ScreenUtils::getCurrentPlatform() )
		{
			mFooterLayout->fillSpaceHorizontally();
			mFooterLayout->addChild(new NativeUI::HorizontalLayout());
			mFooterLayout->addChild(mHideScreenButton);
			mFooterLayout->addChild(new NativeUI::HorizontalLayout());
		}
		else
		{
			mFooterLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
			mFooterLayout->setChildVerticalAlignment(MAW_ALIGNMENT_CENTER);
			mFooterLayout->addChild(mHideScreenButton);
		}

		mMainLayout->addChild(mFooterLayout);
	}
}
