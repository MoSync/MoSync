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
 * @file FirstScreen.cpp
 * @author emma
 */

#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <mastdlib.h>

#include "FirstScreen.h"

/**
 * Constructor.
 */
FirstScreen::FirstScreen() :
		Screen(), mMainLayout(NULL), mEventsList(NULL)

{
	createMainLayout();
	mEditBox1->addEditBoxListener(this);
	mEditBox2->addEditBoxListener(this);
	mEditBox3->addEditBoxListener(this);
}

/**
 * Destructor.
 */
FirstScreen::~FirstScreen()
{
	mEditBox1->addEditBoxListener(this);
	mEditBox2->addEditBoxListener(this);
	mEditBox3->addEditBoxListener(this);
}

/**
 * Creates and adds main layout to the screen.
 */
void FirstScreen::createMainLayout()
{
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	Screen::setMainWidget(mMainLayout);

	Button* topLabel = new Button();
	topLabel->setText("Focus this Button if you want the edit box to loose focus");
	mMainLayout->addChild(topLabel);

	mLabel = new Label("Edit boxes. Check FINISHED EDITING and BEGIN_EDITING events for each one.");
	mMainLayout->addChild(mLabel);

	mEditBox1 = new EditBox();
	mEditBox2= new EditBox();
	mEditBox3 = new EditBox();
	mEditBox1->fillSpaceHorizontally();
	mEditBox2->fillSpaceHorizontally();
	mEditBox3->fillSpaceHorizontally();

	mMainLayout->addChild(mEditBox1);
	mMainLayout->addChild(mEditBox2);
	mMainLayout->addChild(mEditBox3);

	mEventsList = new ListView();
	mMainLayout->addChild(mEventsList);
	ListViewItem* info = new ListViewItem();
	info->setText("Events list:");
	mEventsList->addChild(info);
}

/**
 * This method is called when the return button was pressed.
 * On iphone platform the virtual keyboard is not hidden after
 * receiving this event.
 * @param editBox The edit box object that generated the event.
 */
void FirstScreen::editBoxReturn(EditBox* editBox)
{
	ListViewItem* info = new ListViewItem();
	info->setText("EditBox return for: EditBox " + MAUtil::integerToString(getEditBoxIndex(*editBox)));
	mEventsList->addChild(info);
}

/**
 * This method is called when an edit box gains focus.
 * The virtual keyboard is shown.
 * Only for iphone platform.
 * @param editBox The edit box object that generated the event.
 */
void FirstScreen::editBoxEditingDidBegin(EditBox* editBox)
{
	ListViewItem* info = new ListViewItem();
	info->setText("BEGIN EDITING for: EditBox " + MAUtil::integerToString(getEditBoxIndex(*editBox)));
	mEventsList->addChild(info);
}

int FirstScreen::getEditBoxIndex(EditBox& editBox)
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
void FirstScreen::editBoxEditingDidEnd(EditBox* editBox)
{
	ListViewItem* info = new ListViewItem();
	info->setText("FINISHED EDITING for: EditBox " + MAUtil::integerToString(getEditBoxIndex(*editBox)));
	mEventsList->addChild(info);
}
