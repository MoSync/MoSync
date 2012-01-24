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

#define MAX_TEXT_LENGTH_LABEL "Max text length"


/**
 * Constructor.
 */
MainScreen::MainScreen() :
	Screen(),
	mMainLayout(NULL),
	mEditBox(NULL),
	mSetTextButton(NULL),
	mGetTextButton(NULL),
	mGetTextLabel(NULL),
	mKeyboardButton(NULL),
	mMaxTextLengthEditBox(NULL),
	mKeyboard(false)
{
	createMainLayout();

	mSetTextButton->addButtonListener(this);
	mGetTextButton->addButtonListener(this);
	mKeyboardButton->addButtonListener(this);
	mMaxTextLengthEditBox->addEditBoxListener(this);
	mEditBox->addEditBoxListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
	mSetTextButton->removeButtonListener(this);
	mGetTextButton->removeButtonListener(this);
	mKeyboardButton->removeButtonListener(this);
	mEditBox->removeEditBoxListener(this);
	mMaxTextLengthEditBox->removeEditBoxListener(this);
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonClicked(Widget* button)
{
	if (button == mSetTextButton)
	{
		mEditBox->setText("DEFAULT");
	}
	else if (button == mGetTextButton)
	{
		mGetTextLabel->setText(mEditBox->getText());

		MAUtil::String text = mEditBox->getText();
		printf("get text = %s", text.c_str());
	}
	else if (button == mKeyboardButton)
	{
		mKeyboard = !mKeyboard;
		if (mKeyboard)
		{
			mEditBox->showKeyboard();
		}
		else
		{
			mEditBox->hideKeyboard();
		}
	}
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout()
{
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
//	mMainLayout->setBackgroundColor(0xFF0000);
	Screen::setMainWidget(mMainLayout);

	Label* info1 = new Label();
	info1->setText("First edit box with Capitalize all characters");
	mMainLayout->addChild(info1);

	mEditBox = new EditBox();
	mEditBox->setPlaceholder("Enter text...");
	mEditBox->setInputFlag(EDIT_BOX_INPUT_GLAG_INITIAL_CAPS_ALL_CHARACTERS);
	mEditBox->fillSpaceHorizontally();
	mMainLayout->addChild(mEditBox);

	mSetTextButton = new Button();
	mSetTextButton->setText("Reset text to DEFAULT ");
	mMainLayout->addChild(mSetTextButton);

	mGetTextButton = new Button();
	mGetTextButton->setText("Get text");
	mMainLayout->addChild(mGetTextButton);

	mGetTextLabel = new Label();
//	mGetTextLabel->setText("No text, just placeholder");
	mMainLayout->addChild(mGetTextLabel);

	mKeyboardButton = new Button();
	mKeyboardButton->setText("Show/hide keyboard");
	mMainLayout->addChild(mKeyboardButton);

	this->createMaxTextLengthWidgets(mMainLayout);

	mEditBoxEmail = new EditBox();
	mEditBoxEmail->setPlaceholder("Enter email address...");
	mEditBoxEmail->setInputMode(EDIT_BOX_INPUT_MODE_EMAILADDR);
	mEditBoxEmail->fillSpaceHorizontally();
	mMainLayout->addChild(mEditBoxEmail);

	mEditBoxDecimal = new EditBox();
	mEditBoxDecimal->setPlaceholder("Enter a real number...");
	mEditBoxDecimal->setInputMode(EDIT_BOX_INPUT_MODE_DECIMAL);
	mEditBoxDecimal->fillSpaceHorizontally();
	mMainLayout->addChild(mEditBoxDecimal);

	mEditBoxUrl = new EditBox();
	mEditBoxUrl->setPlaceholder("Enter an URL ..");
	mEditBoxUrl->setInputMode(EDIT_BOX_INPUT_MODE_URL);
	mEditBoxUrl->fillSpaceHorizontally();
	mMainLayout->addChild(mEditBoxUrl);

	maSetColor(0x8A2BE2);
}

/**
 * Create and add widgets for testing the max text length property.
 * @param mainLayout Widgets will be added to it.
 */
void MainScreen::createMaxTextLengthWidgets(VerticalLayout* mainLayout)
{
	// Create layout for widgets.
	HorizontalLayout* layout = new HorizontalLayout();
	mainLayout->addChild(layout);

	// Add label with info.
	Label* label = new Label();
	label->setText(MAX_TEXT_LENGTH_LABEL);
	layout->addChild(label);

	// Create the edit box.
	mMaxTextLengthEditBox = new EditBox();
	mMaxTextLengthEditBox->setInputMode(EDIT_BOX_INPUT_MODE_DECIMAL);
	mMaxTextLengthEditBox->setWidth(this->getWidth() / 2);
	layout->addChild(mMaxTextLengthEditBox);
}

/**
 * This method is called when an edit box gains focus.
 * The virtual keyboard is shown.
 * Only for iphone platform.
 * @param editBox The edit box object that generated the event.
 */
void MainScreen::editBoxEditingDidBegin(EditBox* editBox)
{
	if (editBox == mEditBox)
	{
		printf("editBoxEditingDidBegin for mEditBox");
	}
}

/**
 * This method is called when an edit box loses focus.
 * The virtual keyboard is hidden.
 * Only for iphone platform.
 * @param editBox The edit box object that generated the event.
 */
void MainScreen::editBoxEditingDidEnd(EditBox* editBox)
{
	if (editBox == mEditBox)
	{
		printf("editBoxEditingDidEnd for mEditBox");
		mEditBox->hideKeyboard();
	}
}

/**
 * This method is called when the edit box text was changed.
 * Only for iphone platform.
 * @param editBox The edit box object that generated the event.
 * @param text The new text.
 */
void MainScreen::editBoxTextChanged(
	EditBox* editBox,
	const MAUtil::String& text)
{
	if (editBox == mEditBox)
	{
		printf("editBoxTextChanged for mEditBox text = %s", text.c_str());
	}
	else if (editBox == mMaxTextLengthEditBox)
	{
		int maxTextLength = MAUtil::stringToInteger(
			mMaxTextLengthEditBox->getText());
		if (maxTextLength != 0)
		{
			mEditBox->setMaxLength(maxTextLength);
		}
	}
}

/**
 * This method is called when the return button was pressed.
 * On iphone platform the virtual keyboard is not hidden after
 * receiving this event.
 * Only for iphone platform.
 * @param editBox The edit box object that generated the event.
 */
void MainScreen::editBoxReturn(EditBox* editBox)
{
	 mEditBox->hideKeyboard();
}
