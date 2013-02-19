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
#include "TransitionsScreen.h"

#include <conprint.h>
#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <mastdlib.h>

namespace ScreenTransitionTest {
	/**
	 * Constructor.
	 */
	MainStackScreen::MainStackScreen(FirstScreenObserver& observer) :
		StackScreen()
	{
		this->addMainScreen();
		this->addStackScreenListener(this);

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
		mMainScreen = new Screen();
		mMainScreen->setTitle("Main stack screen");
		VerticalLayout* layout = new VerticalLayout();
		layout->setBackgroundColor(0xFF0000);
		layout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
		mMainScreen->setMainWidget(layout);

		mPushScreen_Button = new Button();
		mPushScreen_Button->setText("Push next stack screen");
		mPushScreen_Button->addButtonListener(this);
		layout->addChild(new VerticalLayout());
		layout->addChild(mPushScreen_Button);
		layout->addChild(new VerticalLayout());

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
