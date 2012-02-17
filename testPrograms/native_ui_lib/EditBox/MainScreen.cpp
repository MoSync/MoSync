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
#include <mastring.h>
#include <MAUtil/String.h>

#include "MainScreen.h"

using namespace MAUtil;

#define MAX_TEXT_LENGTH_LABEL_TEXT "Max text length"
#define MAX_LINES_LABEL_TEXT "Max lines"
#define MIN_LINES_LABEL_TEXT "Min lines"
#define PLACEHOLDER_COLOR_LABEL_TEXT "Placeholder color"
#define LINES_NUMBER_LABEL_TEXT "Lines number"
#define INPUT_MODES_COUNT 7
#define INPUT_FLAGS_COUNT 5

const String inputModes[] = {
		"MAW_EDIT_BOX_TYPE_ANY",
		"MAW_EDIT_BOX_TYPE_EMAILADDR",
		"MAW_EDIT_BOX_TYPE_NUMERIC",
		"MAW_EDIT_BOX_TYPE_PHONENUMBER",
		"MAW_EDIT_BOX_TYPE_URL",
		"MAW_EDIT_BOX_TYPE_DECIMAL",
		"MAW_EDIT_BOX_TYPE_SINGLELINE"
};

const String inputFlags[] = {
		"MAW_EDIT_BOX_FLAG_PASSWORD",
		"MAW_EDIT_BOX_FLAG_SENSITIVE",
		"MAW_EDIT_BOX_FLAG_INITIAL_CAPS_ALL_CHARACTERS",
		"MAW_EDIT_BOX_FLAG_INITIAL_CAPS_WORD",
		"MAW_EDIT_BOX_FLAG_INITIAL_CAPS_SENTENCE"
};

enum InputModeType {
	ANY,
	EMAILADDR,
	NUMERIC,
	PHONENUMBER,
	URL,
	DECIMAL,
	SINGLELINE
};

enum InputFlagType {
	PASSWORD,
	SENSITIVE,
	CAPS_ALL_CHARACTERS,
	CAPS_WORD,
	CAPS_SENTENCE
};

enum FontSize {
	FONT_SMALL        = 12 ,
	FONT_LARGE        = 15 ,
	FONT_EXTRA_LARGE  = 25 ,
};

/**
 * Constructor.
 */
MainScreen::MainScreen() :
	Screen(),
	mMainListView(NULL),
	mEditBox(NULL),
	mSetTextButton(NULL),
	mGetTextButton(NULL),
	mGetTextLabel(NULL),
	mKeyboardButton(NULL),
	mMaxTextLengthEditBox(NULL),
	mInputFlagListView(NULL),
	mInputModeListView(NULL),
	mMaxLinesEditBox(NULL),
	mMaxLinesLabel(NULL),
	mMinLinesEditBox(NULL),
	mMinLinesLabel(NULL),
	mPlaceholderColorEditBox(NULL),
	mPlaceholderColorLabel(NULL),
	mLinesNumberEditBox(NULL),
	mLinesNumberLabel(NULL),
	mKeyboard(false)
{
	// Set the screen size, available for each screen.
	MAExtent screenSize = maGetScrSize();
	mScreenWidth = EXTENT_X(screenSize);
	mScreenHeight = EXTENT_Y(screenSize);

	// For small screens, use small fonts and padding.
	if ( mScreenHeight < 600 )
	{
		mFontSize = FONT_SMALL;
	}
	else if( mScreenHeight < 800 )
	{
		mFontSize = FONT_LARGE;
	}
	else
	{
		mFontSize = FONT_EXTRA_LARGE;
	}

	createMainLayout();

	mSetTextButton->addButtonListener(this);
	mGetTextButton->addButtonListener(this);
	mKeyboardButton->addButtonListener(this);
	mMaxTextLengthEditBox->addEditBoxListener(this);
	mEditBox->addEditBoxListener(this);
	mInputModeListView->addListViewListener(this);
	mInputFlagListView->addListViewListener(this);
	mMaxLinesEditBox->addEditBoxListener(this);
	mMinLinesEditBox->addEditBoxListener(this);
	mPlaceholderColorEditBox->addEditBoxListener(this);
	mLinesNumberEditBox->addEditBoxListener(this);
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
	mInputModeListView->removeListViewListener(this);
	mInputFlagListView->removeListViewListener(this);
	mMaxLinesEditBox->removeEditBoxListener(this);
	mMinLinesEditBox->removeEditBoxListener(this);
	mPlaceholderColorEditBox->removeEditBoxListener(this);
	mLinesNumberEditBox->removeEditBoxListener(this);
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
 * This method is called when the user selects an item from
 * the list view
 * @param listView The list view object that generated the event
 * @param listViewItem The ListViewItem object that was clicked.
 */
void MainScreen::listViewItemClicked(ListView* listView, ListViewItem* listViewItem)
{
	int resultCode;
	int listViewItemIndex;
	String listViewItemText;
	if (listView == mInputModeListView)
	{
		listViewItemText = listViewItem->getPropertyString("text", resultCode);

		if (resultCode == MAW_RES_OK)
		{
			listViewItemIndex = this->getIndexForString(inputModes, listViewItemText);
			this->setInputMode(listViewItemIndex);
		}
	}
	else if (listView == mInputFlagListView)
	{
		listViewItemText = listViewItem->getPropertyString("text", resultCode);
		if (resultCode == MAW_RES_OK)
		{
			listViewItemIndex = this->getIndexForString(inputFlags, listViewItemText);
			this->setInputFlag(listViewItemIndex);
		}
	}
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout()
{
	// Create and add the main layout to the screen.
	mMainListView = new ListView();
	Screen::setMainWidget(mMainListView);

	VerticalLayout* editBoxLayout = new VerticalLayout();
	mEditBox = new EditBox();
	mEditBox->setPlaceholder("Enter text...");
	mEditBox->setHeight(100);
	mEditBox->fillSpaceHorizontally();
	editBoxLayout->addChild(mEditBox);
	mMainListView->addChild(editBoxLayout);

	HorizontalLayout* layout = new HorizontalLayout();
	layout->setHeight(75);
	mMainListView->addChild(layout);

	mSetTextButton = new Button();
	mSetTextButton->setText("Reset text to DEFAULT ");
	layout->addChild(mSetTextButton);

	mGetTextButton = new Button();
	mGetTextButton->setText("Get text");
	layout->addChild(mGetTextButton);

	mGetTextLabel = new Label();
	mMainListView->addChild(mGetTextLabel);

	mKeyboardButton = new Button();
	mKeyboardButton->setText("Show/hide keyboard");
	mKeyboardButton->fillSpaceHorizontally();
	mMainListView->addChild(mKeyboardButton);

	// Create layout for widgets.
	this->createDecimalEditBoxView(mMaxTextLengthEditBox, mMainListView, MAX_TEXT_LENGTH_LABEL_TEXT);
	this->createDecimalEditBoxView(mMaxLinesEditBox, mMainListView, MAX_LINES_LABEL_TEXT);
	this->createDecimalEditBoxView(mMinLinesEditBox, mMainListView, MIN_LINES_LABEL_TEXT);
	this->createDecimalEditBoxView(mLinesNumberEditBox, mMainListView, LINES_NUMBER_LABEL_TEXT);
	this->createDecimalEditBoxView(mPlaceholderColorEditBox, mMainListView, PLACEHOLDER_COLOR_LABEL_TEXT);
	this->createInputModeListView(mMainListView);
	this->createInputFlagListView(mMainListView);

	maSetColor(0x8A2BE2);
}

/**
 * Creates a horizontal layout, adds it to the main layout, initializes the
 * edit box and adds it to the horizontal layout.
 * @param editBox The editbox to be created and added on the screen.
 * @param mainLayout Widgets will be added to it.
 */
void MainScreen::createDecimalEditBoxView(EditBox* &editBox, ListView* mainListView, String text)
{
	// Create layout for widgets.
	HorizontalLayout* layout = new HorizontalLayout();
	layout->setHeight(65);

	mainListView->addChild(layout);

	// Add label with info.
	Label* label = new Label();
	label->setText(text);
	layout->addChild(label);

	// Create the edit box.
	editBox = new EditBox();
	editBox->setInputMode(EDIT_BOX_INPUT_MODE_DECIMAL);
	editBox->fillSpaceHorizontally();
	layout->addChild(editBox);
}

/**
 * Creates the input mode list view
 * @param mainLayout Widgets will be added to it.
 */
void MainScreen::createInputModeListView(ListView* mainListView)
{
	mInputModeListView = new ListView();
	mInputModeListView->setHeight(200);

	for(int i = 0; i < INPUT_MODES_COUNT; i++)
	{
		Label* inputModeLabel = new Label();
		inputModeLabel->setText(inputModes[i]);
		inputModeLabel->setFontSize(mFontSize);
		mInputModeListView->addChild(inputModeLabel);
	}

	mInputModeListView->fillSpaceHorizontally();
	mainListView->addChild(mInputModeListView);
}

/**
 * Creates the input flag list view
 * @param mainLayout Widgets will be added to it.
 */
void MainScreen::createInputFlagListView(ListView* mainListView)
{
	mInputFlagListView = new ListView();
	mInputFlagListView->setHeight(200);

	for(int i = 0; i < INPUT_FLAGS_COUNT; i++)
	{
		Label* inputFlagLabel = new Label();
		inputFlagLabel->setText(inputFlags[i]);
		inputFlagLabel->setFontSize(mFontSize);
		mInputFlagListView->addChild(inputFlagLabel);
	}

	mInputFlagListView->fillSpaceHorizontally();
	mainListView->addChild(mInputFlagListView);
}

/**
 * Gets the index of a string. Returns -1 if the string is not inside the array
 * @param array The String array in which to search
 * @param text The list view item text
 */
int MainScreen::getIndexForString(const String* array, String text)
{
	for (int i = 0; i < array->length(); i++)
	{
		if (array[i] == text)
		{
			return i;
		}
	}

	return -1;
}

/**
 * Set the input mode of the edit box
 * @param mode The input mode code
 */
void MainScreen::setInputMode(int mode)
{
	mEditBox->setText("");
	switch(mode)
	{
		case ANY:
			mGetTextLabel->setText("ANY");
			mEditBox->setInputMode(EDIT_BOX_INPUT_MODE_ANY);
			break;
		case EMAILADDR:
			mGetTextLabel->setText("EMAIL");
			mEditBox->setInputMode(EDIT_BOX_INPUT_MODE_EMAILADDR);
			break;
		case NUMERIC:
			mGetTextLabel->setText("NUMERIC");
			mEditBox->setInputMode(EDIT_BOX_INPUT_MODE_NUMERIC);
			break;
		case PHONENUMBER:
			mGetTextLabel->setText("PHONENUMBER");
			mEditBox->setInputMode(EDIT_BOX_INPUT_MODE_PHONENUMBER);
			break;
		case URL:
			mGetTextLabel->setText("URL");
			mEditBox->setInputMode(EDIT_BOX_INPUT_MODE_URL);
			break;
		case DECIMAL:
			mGetTextLabel->setText("DECIMAL");
			mEditBox->setInputMode(EDIT_BOX_INPUT_MODE_DECIMAL);
			break;
		case SINGLELINE:
			mGetTextLabel->setText("SINGLELINE");
			mEditBox->setInputMode(EDIT_BOX_INPUT_MODE_SINGLE_LINE);
			break;
		default:
			mGetTextLabel->setText("Not a valid mode selection");
			break;
	}
}

/**
 * Set the input flag of the edit box
 * @param flag The input flag code
 */
void MainScreen::setInputFlag(int flag)
{
	mEditBox->setText("");
	switch(flag)
	{
		case PASSWORD:
			mGetTextLabel->setText("PASSWORD");
			mEditBox->setInputFlag(EDIT_BOX_INPUT_FLAG_PASSWORD);
			break;
		case SENSITIVE:
			mGetTextLabel->setText("SENSITIVE");
			mEditBox->setInputFlag(EDIT_BOX_INPUT_FLAG_SENSITIVE);
			break;
		case CAPS_ALL_CHARACTERS:
			mGetTextLabel->setText("CAPS_ALL_CHARACTERS");
			mEditBox->setInputFlag(EDIT_BOX_INPUT_GLAG_INITIAL_CAPS_ALL_CHARACTERS);
			break;
		case CAPS_WORD:
			mGetTextLabel->setText("CAPS_WORD");
			mEditBox->setInputFlag(EDIT_BOX_INPUT_FLAG_INITIAL_CAPS_WORD);
			break;
		case CAPS_SENTENCE:
			mGetTextLabel->setText("CAPS_SENTENCE");
			mEditBox->setInputFlag(EDIT_BOX_INPUT_FLAG_INITIAL_CAPS_SENTENCE);
			break;
		default:
			mGetTextLabel->setText("Not a valid flag selection!");
			break;
	}
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
			mGetTextLabel->setText("Max text length changed!");
		}
	}
	else if (editBox == mMaxLinesEditBox)
	{
		int maxLines = MAUtil::stringToInteger(
			mMaxLinesEditBox->getText());
		mEditBox->setMaxLines(maxLines);
		mGetTextLabel->setText("Max lines changed!");
	}
	else if (editBox == mMinLinesEditBox)
	{
		int minLines = MAUtil::stringToInteger(
			mMinLinesEditBox->getText());
		mEditBox->setMinLines(minLines);
		mGetTextLabel->setText("Min lines changed!");
	}
	else if (editBox == mPlaceholderColorEditBox)
	{
		int placeHolderColor = MAUtil::stringToInteger(
			mPlaceholderColorEditBox->getText());
		mEditBox->setPlaceholderFontColor(placeHolderColor);
		mGetTextLabel->setText("Placeholder color changed!");
	}
	else if (editBox == mLinesNumberEditBox)
	{
		int linesNumber = MAUtil::stringToInteger(
				mLinesNumberEditBox->getText());
		mEditBox->setLinesNumber(linesNumber);
		mGetTextLabel->setText("Lines number changed!");
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
