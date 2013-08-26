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

#include "MainStackScreen.h"

#include <conprint.h>
#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <mastdlib.h>

#include <NativeUI/Button.h>
#include <NativeUI/VerticalLayout.h>
#include <NativeUI/HorizontalLayout.h>
#include <NativeUI/Label.h>

#include "TransitionsScreen.h"
#include "ScreenUtils.h"

#define MAIN_SCR_TITLE "1st screen"
#define PUSH_BTN_TEXT "Push next screen"

#define SCR_COLOR 0x088da5
#define DESCRIPTION_TEXT "Screen/StackScreen transition example"

#define INFO_LBL_TEXT "On this example, the transition applied to the screens pushed/popped on the StackScreen is slide. Please see MoSync documentation for transitions availability."
#define INFO_IOS_LBL_TEXT "On iOS the native transition (slide) is always applied on stacked screens."

namespace ScreenTransitionTest {
	/**
	 * Constructor.
	 */
	MainStackScreen::MainStackScreen(TransitionsScreenObserver& observer) :
		StackScreen(),
		mMainScreen(NULL),
		mMainLayout(NULL),
		mTransitionsScreen(NULL),
		mPushScreen_Button(NULL)
	{
		createUI();
		addStackScreenListener(this);

		/* Note that stack screen transitions do not apply on iOS due to
		 * the native animation. For the other two platforms (Android and WP),
		 * where the stack-screen transitions are available, will use SLIDE_LEFT/SLIDE_RIGHT
		 * since these transitions types are available on both platforms.
		 */
		setPushTransition(MAW_TRANSITION_TYPE_SLIDE_LEFT, 300);

		// Set duration of the screen transitions. Note that on WP the duration is set by the platform.
		setPopTransition(MAW_TRANSITION_TYPE_SLIDE_RIGHT, 300);

		mTransitionsScreen = new TransitionsScreen(observer);
	}

	/**
	 * Destructor.
	 */
	MainStackScreen::~MainStackScreen()
	{
		mPushScreen_Button->removeButtonListener(this);
		this->removeStackScreenListener(this);
		delete mTransitionsScreen;
		delete mMainScreen;
	}

	void MainStackScreen::createUI()
	{
		mMainScreen = new NativeUI::Screen();
		mMainScreen->setTitle(MAIN_SCR_TITLE);

		mMainLayout = new NativeUI::VerticalLayout();
		mMainLayout->setBackgroundColor(SCR_COLOR);
		if ( ScreenUtils::OS_WIN != ScreenUtils::getCurrentPlatform() )
		{
			mMainLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
		}
		mMainLayout->fillSpaceVertically();
		mMainLayout->fillSpaceHorizontally();
		mMainScreen->setMainWidget(mMainLayout);

		mMainLayout->addChild(new NativeUI::VerticalLayout());

		addTitleLabel();

		addInfoLabel();

		addFooter();

		this->push(mMainScreen);
	}

	void MainStackScreen::addTitleLabel()
	{
		NativeUI::Label* titleLbl = new NativeUI::Label();
		titleLbl->setText(DESCRIPTION_TEXT);
		titleLbl->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
		titleLbl->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
		titleLbl->fillSpaceVertically();
		titleLbl->fillSpaceHorizontally();
		titleLbl->setMaxNumberOfLines(2);
		titleLbl->setFontSize(TITLE_FONT_SIZE);
		mMainLayout->addChild(titleLbl);
		mMainLayout->addChild(new NativeUI::VerticalLayout());
	}

	void MainStackScreen::addInfoLabel()
	{
		if ( ScreenUtils::OS_IOS == ScreenUtils::getCurrentPlatform() )
		{
			NativeUI::Label* infoLbliOS = new NativeUI::Label();
			infoLbliOS->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
			infoLbliOS->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
			infoLbliOS->setMaxNumberOfLines(3);
			infoLbliOS->fillSpaceVertically();
			infoLbliOS->setText(INFO_IOS_LBL_TEXT);
			infoLbliOS->setFontSize(INFO_FONT_SIZE);
			mMainLayout->addChild(infoLbliOS);
		}
		else
		{
			NativeUI::Label* infoLbl = new NativeUI::Label();
			infoLbl->setText(INFO_LBL_TEXT);
			infoLbl->setFontSize(INFO_FONT_SIZE);
			infoLbl->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
			infoLbl->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
			infoLbl->fillSpaceHorizontally();
			infoLbl->fillSpaceVertically();
			mMainLayout->addChild(infoLbl);
		}
	}

	void MainStackScreen::addFooter()
	{
		NativeUI::HorizontalLayout* footerLayout = new NativeUI::HorizontalLayout();
		footerLayout->setBackgroundColor(BLACK_COLOR);
		footerLayout->setHeight(FOOTER_HEIGHT);
		footerLayout->fillSpaceHorizontally();

		mPushScreen_Button = new NativeUI::Button();
		mPushScreen_Button->setText(PUSH_BTN_TEXT);
		mPushScreen_Button->addButtonListener(this);

		if ( ScreenUtils::OS_WIN == ScreenUtils::getCurrentPlatform() )
		{
			footerLayout->fillSpaceHorizontally();
			footerLayout->addChild(new NativeUI::HorizontalLayout());
			footerLayout->addChild(mPushScreen_Button);
			footerLayout->addChild(new NativeUI::HorizontalLayout());
		}
		else
		{
			footerLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
			footerLayout->setChildVerticalAlignment(MAW_ALIGNMENT_CENTER);
			footerLayout->addChild(mPushScreen_Button);
		}

		mMainLayout->addChild(footerLayout);
	}

	/**
	 * This method is called if the touch-up event was inside the
	 * bounds of the button.
	 * @param button The button object that generated the event.
	 */
	void MainStackScreen::buttonClicked(Widget* button)
	{
		if (mPushScreen_Button == button)
		{
			this->push(mTransitionsScreen);
		}
	}

	/**
	 * This method is called when a screen has been popped from a stack
	 * screen.
	 * @param stackScreen The stack screen object that generated the event.
	 * @param fromScreen The screen that was popped from the stack screen.
	 * @param toScreen The screen that will be shown.
	 */
	void MainStackScreen::stackScreenScreenPopped(
		StackScreen* stackScreen,
		Screen* fromScreen,
		Screen* toScreen)
	{
		printf("stackScreenScreenPopped");
		if (this == stackScreen && (fromScreen == mTransitionsScreen))
		{
			mTransitionsScreen->giveFocusToList();
		}
	}
}
