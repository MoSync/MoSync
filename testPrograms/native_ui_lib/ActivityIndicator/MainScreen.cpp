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
	Screen(),
	mMainLayout(NULL),
	mActivityIndicator(NULL),
	mShow(NULL),
	mHide(NULL)
{
	createMainLayout();

	mShow->addButtonListener(this);
    mHide->addButtonListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
    mShow->removeButtonListener(this);
    mHide->removeButtonListener(this);
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonClicked(Widget* button)
{
    if (button == mShow)
    {
        mActivityIndicator->show();
    }
    else if (button == mHide)
    {
        mActivityIndicator->hide();
    }
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout() {
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	Screen::setMainWidget(mMainLayout);

	Label* label = new Label();
	label->setText("Show/hide Activity Indicator");
	mMainLayout->addChild(label);

	mShow = new Button();
	mShow->setText("show ActivityIndicator");
	mMainLayout->addChild(mShow);

	mActivityIndicator = new ActivityIndicator();
	mMainLayout->addChild(mActivityIndicator);

	mHide = new Button();
	mHide->setText("hide ActivityIndicator");
	mMainLayout->addChild(mHide);

}
