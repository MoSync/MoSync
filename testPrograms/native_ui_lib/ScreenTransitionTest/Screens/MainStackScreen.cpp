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
#define INFO_LBL_TEXT "On this example, the transition applied to stacked screens is Slide. Please see MoSync documentation for transitions availability."
#define INFO_IOS_LBL_TEXT "On iOS the native transition (slide) is always applied on stacked screens."

namespace ScreenTransitionTest {
	/**
	 * Constructor.
	 */
	MainStackScreen::MainStackScreen(TransitionsScreenObserver& observer) :
		StackScreen()
	{
		this->addMainScreen();
		this->addStackScreenListener(this);

		/* Note that stack screen transitions do not apply on iOS due to
		 * the native animation. For the other two platforms (Android and WP),
		 * where the stack-screen transitions are available, will use SLIDE_LEFT/SLIDE_RIGHT
		 * since these transitions types are available on both platforms.
		 */
		char valueBuffer[32];
		sprintf(valueBuffer, "%d", MAW_TRANSITION_TYPE_SLIDE_LEFT);
		this->setProperty(MAW_STACK_SCREEN_PUSH_TRANSITION_TYPE, valueBuffer);
		sprintf(valueBuffer, "%d", MAW_TRANSITION_TYPE_SLIDE_RIGHT);
		this->setProperty(MAW_STACK_SCREEN_POP_TRANSITION_TYPE, valueBuffer );

		// Set duration of the screen transitions. Note that on WP the duration is constrained.
		sprintf(valueBuffer, "%d", 300);
		this->setProperty(MAW_STACK_SCREEN_PUSH_TRANSITION_DURATION, valueBuffer);
		this->setProperty(MAW_STACK_SCREEN_POP_TRANSITION_DURATION, valueBuffer);

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

	void MainStackScreen::addMainScreen()
	{
		mMainScreen = new NativeUI::Screen();
		mMainScreen->setTitle(MAIN_SCR_TITLE);
		NativeUI::VerticalLayout* layout = new NativeUI::VerticalLayout();
		layout->setBackgroundColor(SCR_COLOR);
		layout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
		layout->fillSpaceVertically();
		layout->fillSpaceHorizontally();
		mMainScreen->setMainWidget(layout);

		mPushScreen_Button = new NativeUI::Button();
		mPushScreen_Button->setText(PUSH_BTN_TEXT);
		mPushScreen_Button->addButtonListener(this);
		layout->addChild(new NativeUI::VerticalLayout());

		if ( ScreenUtils::OS_IOS == ScreenUtils::getCurrentPlatform() )
		{
			NativeUI::Label* infoLbliOS = new NativeUI::Label();
			infoLbliOS->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
			infoLbliOS->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
			infoLbliOS->setMaxNumberOfLines(3);
			infoLbliOS->fillSpaceVertically();
			infoLbliOS->setText(INFO_IOS_LBL_TEXT);
			infoLbliOS->setFontSize(INFO_FONT_SIZE);
			layout->addChild(infoLbliOS);
		}
		else
		{
			NativeUI::Label* infoLbl = new NativeUI::Label();
			infoLbl->setText(INFO_LBL_TEXT);
			infoLbl->setFontSize(INFO_FONT_SIZE);
			layout->addChild(infoLbl);
		}

		NativeUI::HorizontalLayout* footerLayout = new NativeUI::HorizontalLayout();
		footerLayout->setBackgroundColor(BLACK_COLOR);
		footerLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
		footerLayout->setChildVerticalAlignment(MAW_ALIGNMENT_CENTER);
		footerLayout->setHeight(FOOTER_HEIGHT);

		footerLayout->addChild(mPushScreen_Button);
		layout->addChild(footerLayout);

		this->push(mMainScreen);
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
		if (this == stackScreen)
		{
			printf("stack screen event");
		}
	}
}
