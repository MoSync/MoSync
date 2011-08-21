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
 * @author Bogdan Iusco.
 */

#include <conprint.h>
#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <mastdlib.h>

#include "MainScreen.h"

/**
 * Constructor.
 */
MainScreen::MainScreen() :
	StackScreen()

{
//    this->enableNavigationBar(true);
    this->createFirstScreen();
    this->push(mFirstScreen);

    this->createSecondScreen();
    this->addStackScreenListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
    mPushButton->removeButtonListener(this);
    this->removeStackScreenListener(this);
    delete mFirstScreen;
    delete mSecondScreen;
}

void MainScreen::createFirstScreen()
{
    mFirstScreen = new Screen();
    mFirstScreen->setTitle("1st screen");
    VerticalLayout* layout = new VerticalLayout();
    layout->setBackgroundColor(0xFF0000);
    mFirstScreen->setMainWidget(layout);

    mPushButton = new Button();
    mPushButton->setText("Push screen");
    mPushButton->addButtonListener(this);
    layout->addChild(mPushButton);
}

void MainScreen::createSecondScreen()
{
    mSecondScreen = new Screen();
    mSecondScreen->setTitle("2nd screen");
    VerticalLayout* layout = new VerticalLayout();
    layout->setBackgroundColor(0x123456);
    mSecondScreen->setMainWidget(layout);
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonClicked(Widget* button)
{
    if (mPushButton == button)
    {
        printf("mPushButtonClicked - push screen");
        this->push(mSecondScreen);
    }
}

/**
 * This method is called when a screen has been popped from a stack
 * screen.
 * @param stackScreen The stack screen object that generated the event.
 * @param fromScreen The screen that was popped from the stack screen.
 * @param toScreen The screen that will be shown.
 */
void MainScreen::stackScreenScreenPopped(
    StackScreen* stackScreen,
    Screen* fromScreen,
    Screen* toScreen)
{
    if (this == stackScreen)
    {
        printf("stack screen event");
        if(mFirstScreen == toScreen && mSecondScreen == fromScreen)
        {
            printf("screen test param is ok!");
        }
    }
}
