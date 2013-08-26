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
 * @author emma.
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
	mMainLayout(NULL)
{
	createMainLayout();

	mRemoveLabel->addButtonListener(this);
	mRemoveLayout->addButtonListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
	mRemoveLabel->removeButtonListener(this);
	mRemoveLayout->removeButtonListener(this);
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonClicked(Widget* button)
{
	if ( mRemoveLabel == button )
	{
		if ( strcmp(mRemoveLabel->getText().c_str(), "Remove label") == 0 )
		{
			mLayout->removeChild(mLabel);
			mRemoveLabel->setText("Add label");
		}
		else
		{
			mLayout->insertChild(mLabel,0);
			mRemoveLabel->setText("Remove label");
		}
	}
	else if ( mRemoveLayout == button )
	{
		if ( strcmp(mRemoveLayout->getText().c_str(), "Remove bottom layout") == 0 )
		{
			mMainLayout->removeChild(mTestLayout);
			mRemoveLayout->setText("Add bottom layout");
		}
		else
		{
			mMainLayout->addChild(mTestLayout);
			mRemoveLayout->setText("Remove bottom layout");
		}

	}

}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout() {
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(0x0000FF);
	Screen::setMainWidget(mMainLayout);

	mLayout = new VerticalLayout();
	mLayout->setBackgroundColor(0xAA0088);
	mLabel = new Label();
	mLabel->setText("Label");
	mLayout->addChild(mLabel);
	mMainLayout->addChild(mLayout);

	HorizontalLayout* buttonsLayout = new HorizontalLayout();
	mMainLayout->addChild(buttonsLayout);
	mRemoveLabel = new Button();
	mRemoveLabel->setText("Remove label");
	buttonsLayout->addChild(mRemoveLabel);
	mRemoveLayout = new Button();
	mRemoveLayout->setText("Remove bottom layout");
	buttonsLayout->addChild(mRemoveLayout);

	mTestLayout = new HorizontalLayout();
	mTestLayout->setBackgroundColor(0x000066);//0xBB0000);
//	mSecondParent->setHeight(50);
	mMainLayout->addChild(mTestLayout);

	one = new Button();
	one->setText("The first button");
	one->fillSpaceHorizontally();
	mTestLayout->addChild(one);
	two = new Button();
	two->setText("second");
	two->fillSpaceHorizontally();
	mTestLayout->addChild(two);

}
