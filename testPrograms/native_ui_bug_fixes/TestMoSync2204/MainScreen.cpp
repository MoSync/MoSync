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
 * @author Emma Tresanszki.
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
	mLabel(NULL),
	mEventsList(NULL),
	mEditBox1(NULL),
	mEditBox2(NULL),
	mEditBox3(NULL)
{
	createMainLayout();
	mEditBox1->addEditBoxListener(this);
	mEditBox2->addEditBoxListener(this);
	mEditBox3->addEditBoxListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
	mEditBox1->addEditBoxListener(this);
	mEditBox2->addEditBoxListener(this);
	mEditBox3->addEditBoxListener(this);
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout()
{
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	Screen::setMainWidget(mMainLayout);

	mLabel = new Label("Multi line edit boxes. ");//Check RETURN events for each one.");
	mMainLayout->addChild(mLabel);

	mEditBox1 = new EditBox();
	mEditBox2= new EditBox();
	mEditBox3 = new EditBox();
	mEditBox1->setInputMode(EDIT_BOX_INPUT_MODE_ANY);
	mEditBox2->setInputMode(EDIT_BOX_INPUT_MODE_ANY);
	mEditBox3->setInputMode(EDIT_BOX_INPUT_MODE_ANY);
	mEditBox1->fillSpaceHorizontally();
	mEditBox2->fillSpaceHorizontally();
	mEditBox2->setLinesNumber(5);
	mEditBox3->fillSpaceHorizontally();

	mMainLayout->addChild(mEditBox1);
	mMainLayout->addChild(mEditBox2);
	mMainLayout->addChild(mEditBox3);

	mEventsList = new ListView();
	mEventsList->fillSpaceVertically();
	mMainLayout->addChild(mEventsList);
//	ListViewItem* info = new ListViewItem();
//	info->setText("Events list:");
//	mEventsList->addChild(info);
}

/**
 * This method is called when the return button was pressed.
 * On iphone platform the virtual keyboard is not hidden after
 * receiving this event.
 * @param editBox The edit box object that generated the event.
 */
void MainScreen::editBoxReturn(EditBox* editBox)
{
//	ListViewItem* info = new ListViewItem();
//	info->setText("Editing Did End for: EditBox " + MAUtil::integerToString(getEditBoxIndex(*editBox)));
//	mEventsList->addChild(info);
}

/**
 * This method is called when an edit box gains focus.
 * The virtual keyboard is shown.
 * Only for iphone platform.
 * @param editBox The edit box object that generated the event.
 */
void MainScreen::editBoxEditingDidBegin(EditBox* editBox)
{
//	ListViewItem* info = new ListViewItem();
//	info->setText("Editing Did Begin for: EditBox " + MAUtil::integerToString(getEditBoxIndex(*editBox)));
//	mEventsList->addChild(info);
}

int MainScreen::getEditBoxIndex(EditBox& editBox)
{
	if ( editBox.getWidgetHandle() == mEditBox1->getWidgetHandle() )
		return 1;
	if ( editBox.getWidgetHandle() == mEditBox2->getWidgetHandle() )
		return 2;
	if ( editBox.getWidgetHandle() == mEditBox3->getWidgetHandle() )
		return 3;
}

/**
 * This method is called when an edit box loses focus.
 * The virtual keyboard is hidden.
 * Only for iphone platform.
 * @param editBox The edit box object that generated the event.
 */
void MainScreen::editBoxEditingDidEnd(EditBox* editBox)
{
//	ListViewItem* info = new ListViewItem();
//	//info->setText("Editing Did End for: EditBox " + MAUtil::integerToString(getEditBoxIndex(*editBox)));
//	info->setText("DiD END");
//	mEventsList->addChild(info);
}
