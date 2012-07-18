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
	mEditBox(NULL)
{
	createMainLayout();
	mEditBox->addEditBoxListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
	mEditBox->addEditBoxListener(this);
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout()
{
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	Screen::setMainWidget(mMainLayout);

	mLabel = new Label("Edit Box. Check DID_BEGIN, DID_END, TEXT_CHANGED events");
	mMainLayout->addChild(mLabel);

	mEditBox = new EditBox();
	mEditBox->fillSpaceHorizontally();
	mMainLayout->addChild(mEditBox);

	mEventsList = new ListView();
	mEventsList->fillSpaceVertically();
	mMainLayout->addChild(mEventsList);
	ListViewItem* info = new ListViewItem();
	info->setText("Events list:");
	mEventsList->addChild(info);
}

/**
 * This method is called when an edit box gains focus.
 * The virtual keyboard is shown.
 * Only for iphone platform.
 * @param editBox The edit box object that generated the event.
 */
void MainScreen::editBoxEditingDidBegin(EditBox* editBox)
{
	ListViewItem* info = new ListViewItem();
	info->setText("Editing Did Begin");
	mEventsList->addChild(info);
}

/**
 * This method is called when an edit box loses focus.
 * The virtual keyboard is hidden.
 * Only for iphone platform.
 * @param editBox The edit box object that generated the event.
 */
void MainScreen::editBoxEditingDidEnd(EditBox* editBox)
{
	ListViewItem* info = new ListViewItem();
	info->setText("Editing Did End ");
	mEventsList->addChild(info);
}

/**
 * This method is called when the edit box text was changed.
 * @param editBox The edit box object that generated the event.
 * @param text The new text.
 */
void MainScreen::editBoxTextChanged(
    EditBox* editBox,
    const MAUtil::String& text)
{
	ListViewItem* info = new ListViewItem();
	info->setText("Text Changed: " + text);
	mEventsList->addChild(info);
}
