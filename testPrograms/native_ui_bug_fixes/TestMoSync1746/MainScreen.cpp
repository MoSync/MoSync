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
 * @author emma
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
	mLayout(NULL),
	mLabel(NULL)
{
	createMainLayout();

}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout() {
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(0x0000FF);
	Screen::setMainWidget(mMainLayout);

	mLabel = new Label();
	mLabel->setText("MainLayout is blue");
	mMainLayout->addChild(mLabel);

	mLayout = new VerticalLayout();
	mLayout->setBackgroundColor(0xEE0000);

	Label* info = new Label();
	info->setText("The child layout is scrollable with multiple labels.");
	mLayout->addChild(info);
	for (int i=0; i < 30; i++)
	{
		Label* label = new Label();
		label->setText("Label " + MAUtil::integerToString(i));
		mLayout->addChild(label);
	}

	mLayout->setScrollable(true);
	mMainLayout->addChild(mLayout);
//	mLayout->setScrollable(true);
}
