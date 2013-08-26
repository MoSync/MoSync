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
//	this->enableNavigationBar(true);
    this->createFirstScreen();
    this->push(mFirstScreen);

    this->createSecondScreen();
    this->createThirdScreen();
    this->addStackScreenListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
    mPushButton->removeButtonListener(this);
    mSecondPushButton->removeButtonListener(this);
    this->removeStackScreenListener(this);
    delete mFirstScreen;
    delete mSecondScreen;
    delete mThirdScreen;
}

void MainScreen::createFirstScreen()
{
    mFirstScreen = new Screen();
    mFirstScreen->setTitle("1st screen");
    VerticalLayout* layout = new VerticalLayout();
    layout->setBackgroundColor(0xFF0000);
    mFirstScreen->setMainWidget(layout);

    mPushButton = new Button();
    mPushButton->setText("Push 2nd screen");
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

    mSecondPushButton = new Button();
    mSecondPushButton->setText("Push 3rd screen");
    mSecondPushButton->addButtonListener(this);
    layout->addChild(mSecondPushButton);
}

void MainScreen::createThirdScreen()
{
    mThirdScreen = new Screen();
    mThirdScreen->setTitle("3rd screen");
    VerticalLayout* layout = new VerticalLayout();
    layout->setBackgroundColor(0x654456);
    mThirdScreen->setMainWidget(layout);
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
    if (mSecondPushButton == button)
    {
        printf("mPushButtonClicked - second push screen");
        this->push(mThirdScreen);
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
        /*if(mFirstScreen == toScreen && mSecondScreen == fromScreen)
        {
            printf("screen test param is ok!");
        }
        else
        {
            maAlert("Error", "Wrong callback parameter(s)", "OK", NULL, NULL);
        }*/
    }
}
