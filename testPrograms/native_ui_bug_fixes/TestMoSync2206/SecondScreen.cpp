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
 * @file SecondScreen.cpp
 * @author emma
 */

#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <mastdlib.h>

#include "SecondScreen.h"

/**
 * Constructor.
 */
SecondScreen::SecondScreen() :
		Screen(),
		mMainLayout(NULL),
		mEditBox(NULL),
		mEventsList(NULL)

{
	createMainLayout();
	mEditBox->addEditBoxListener(this);
}

/**
 * Destructor.
 */
SecondScreen::~SecondScreen()
{
	mEditBox->addEditBoxListener(this);
}

/**
 * Creates and adds main layout to the screen.
 */
void SecondScreen::createMainLayout()
{
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	Screen::setMainWidget(mMainLayout);

	Label* mLabel = new Label("Single line edit box.Check FINISHED EDITING and BEGIN_EDITING events.");
	mMainLayout->addChild(mLabel);

	mEditBox = new EditBox();
	mEditBox->fillSpaceHorizontally();
	mMainLayout->addChild(mEditBox);

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
void SecondScreen::editBoxReturn(EditBox* editBox)
{
	if ( mEditBox == editBox )
	{
		ListViewItem* info = new ListViewItem();
		info->setText("EditBox return event" );
		mEventsList->addChild(info);
	}
}

/**
 * This method is called when an edit box gains focus.
 * The virtual keyboard is shown.
 * Only for iphone platform.
 * @param editBox The edit box object that generated the event.
 */
void SecondScreen::editBoxEditingDidBegin(EditBox* editBox)
{
	if ( mEditBox == editBox )
	{
		ListViewItem* info = new ListViewItem();
		info->setText("BEGIN EDITING  event");
		mEventsList->addChild(info);
	}
}

/**
 * This method is called when an edit box loses focus.
 * The virtual keyboard is hidden.
 * Only for iphone platform.
 * @param editBox The edit box object that generated the event.
 */
void SecondScreen::editBoxEditingDidEnd(EditBox* editBox)
{
	if ( mEditBox == editBox )
	{
		ListViewItem* info = new ListViewItem();
		info->setText("FINISHED EDITING event");
		mEventsList->addChild(info);
	}
}
