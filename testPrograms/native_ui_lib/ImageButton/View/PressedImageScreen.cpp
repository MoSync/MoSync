/*
 Copyright (C) 2013 MoSync AB

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
 * @file PressedImageScreen.cpp
 * @author Bogdan Iusco.
 *
 * @brief Shows widgets used to test MAW_IMAGE_BUTTON_PRESSED_IMAGE and
 * MAW_IMAGE_BUTTON_PRESSED_IMAGE_PATH properties.
 */


#define SCREEN_TITLE "Pressed image"

#define SET_PATH_LABEL_TEXT "Path"
#define SET_PRESSED_IMAGE_FROM_PATH_BTN_TEXT "Select pressed image from path"
#define SET_PRESSED_IMAGE_FROM_HANDLE_BTN_TEXT "Select pressed image from handle"
#define GET_IMAGE_HANDLE_BTN_TEXT "Get image handle"
#define GET_PRESSED_IMAGE_PATH_BTN_TEXT "Get pressed image path"

#include <NativeUI/Button.h>
#include <NativeUI/EditBox.h>
#include <NativeUI/HorizontalLayout.h>
#include <NativeUI/Label.h>
#include <NativeUI/VerticalLayout.h>

#include "PressedImageScreen.h"
#include "PressedImageScreenListener.h"

namespace ImageButtonTest
{

	/**
	 * Constructor.
	 * @param listener Listener for screen.
	 */
	PressedImageScreen::PressedImageScreen(PressedImageScreenListener& listener):
		mListener(listener),
		mMainLayout(NULL),
		mPathEditBox(NULL),
		mPressedImageFromPathButton(NULL),
		mPressedImageFromHandleButton(NULL),
		mGetPressedImageHandleButton(NULL),
		mGetPressedImagePathButton(NULL)
	{
		this->setTitle(SCREEN_TITLE);
		this->createUI();

		mPathEditBox->addEditBoxListener(this);

		mPressedImageFromHandleButton->addButtonListener(this);
		mPressedImageFromPathButton->addButtonListener(this);
		mGetPressedImageHandleButton->addButtonListener(this);
		mGetPressedImagePathButton->addButtonListener(this);

		mGetPressedImageHandleButton->setEnabled(false);
		mGetPressedImagePathButton->setEnabled(false);
	}

	/**
	 * Destructor.
	 */
	PressedImageScreen::~PressedImageScreen()
	{
		mPathEditBox->removeEditBoxListener(this);

		mPressedImageFromHandleButton->removeButtonListener(this);
		mPressedImageFromPathButton->removeButtonListener(this);
		mGetPressedImageHandleButton->removeButtonListener(this);
		mGetPressedImagePathButton->removeButtonListener(this);
	}

	/**
	 * Set displayed pressed image path in the edit box.
	 * @param path Text to set.
	 */
	void PressedImageScreen::setDisplayedPressedImagePath(const MAUtil::String& path)
	{
		mPathEditBox->setText(path);
	}

	/**
	 * Create screen's UI.
	 */
	void PressedImageScreen::createUI()
	{
		mMainLayout = new NativeUI::VerticalLayout();
		mMainLayout->setBackgroundColor(0xFF0000);
		this->setMainWidget(mMainLayout);

		this->createEditBoxLayout();
		this->createPressedImageFromPathButton();
		this->createPressedImageFromHandleButton();
		this->createGetImageHandleButton();
		this->createGetPressedImagePathButton();
	}

	/**
	 * Create layout that contains:
	 * - a Label widget.
	 * - a EditBox widget that will be used to get a file path for the
	 * pressed image.
	 */
	void PressedImageScreen::createEditBoxLayout()
	{
		NativeUI::HorizontalLayout* layout = new NativeUI::HorizontalLayout();
		layout->fillSpaceHorizontally();
		layout->wrapContentVertically();
		mMainLayout->addChild(layout);

		NativeUI::Label* label = new NativeUI::Label();
		label->wrapContentHorizontally();
		label->wrapContentVertically();
		label->setText(SET_PATH_LABEL_TEXT);
		layout->addChild(label);

		mPathEditBox = new NativeUI::EditBox();
		mPathEditBox->wrapContentVertically();
		mPathEditBox->fillSpaceHorizontally();
		layout->addChild(mPathEditBox);
	}

	/**
	 * Create an button that will be used to select the pressed image
	 * from a file path.
	 */
	void PressedImageScreen::createPressedImageFromPathButton()
	{
		mPressedImageFromPathButton = new NativeUI::Button();
		mPressedImageFromPathButton->fillSpaceHorizontally();
		mPressedImageFromPathButton->setText(SET_PRESSED_IMAGE_FROM_PATH_BTN_TEXT);
		mMainLayout->addChild(mPressedImageFromPathButton);
	}

	/**
	 * Create an button that will be used to select the pressed image
	 * from a handle.
	 */
	void PressedImageScreen::createPressedImageFromHandleButton()
	{
		mPressedImageFromHandleButton = new NativeUI::Button();
		mPressedImageFromHandleButton->fillSpaceHorizontally();
		mPressedImageFromHandleButton->setText(
			SET_PRESSED_IMAGE_FROM_HANDLE_BTN_TEXT);
		mMainLayout->addChild(mPressedImageFromHandleButton);
	}

	/**
	 * Create an button that will be used to display the pressed image
	 * handle.
	 */
	void PressedImageScreen::createGetImageHandleButton()
	{
		mGetPressedImageHandleButton = new NativeUI::Button();
		mGetPressedImageHandleButton->fillSpaceHorizontally();
		mGetPressedImageHandleButton->setText(GET_IMAGE_HANDLE_BTN_TEXT);
		mMainLayout->addChild(mGetPressedImageHandleButton);
	}

	/**
	 * Create an button that will be used to display the pressed image path.
	 */
	void PressedImageScreen::createGetPressedImagePathButton()
	{
		mGetPressedImagePathButton = new NativeUI::Button();
		mGetPressedImagePathButton->fillSpaceHorizontally();
		mGetPressedImagePathButton->setText(GET_PRESSED_IMAGE_PATH_BTN_TEXT);
		mMainLayout->addChild(mGetPressedImagePathButton);
	}

	/**
	 * This method is called when the return button was pressed.
	 * On iOS platform the virtual keyboard is not closed after
	 * receiving this event.
	 * This method is called only if the edit box mode is
	 * #MAW_EDIT_BOX_SINGLE_LINE.
	 * @param editBox The edit box object that generated the event.
	 */
	void PressedImageScreen::editBoxReturn(NativeUI::EditBox* editBox)
	{
		editBox->hideKeyboard();
	}

	/**
	 * This method is called if the touch-up event was inside the
	 * bounds of the button.
	 * Platform: iOS, Android, Windows Phone.
	 * @param button The button object that generated the event.
	 */
	void PressedImageScreen::buttonClicked(NativeUI::Widget* button)
	{
		if (button == mPressedImageFromHandleButton)
		{
			mListener.selectPressedImageFromHandle();
			mGetPressedImageHandleButton->setEnabled(true);
			mGetPressedImagePathButton->setEnabled(false);
		}
		else if (button == mPressedImageFromPathButton)
		{
			MAUtil::String path = mPathEditBox->getText();
			mListener.selectPressedImageFromPath(path);
			mGetPressedImageHandleButton->setEnabled(false);
			mGetPressedImagePathButton->setEnabled(true);
		}
		else if (button == mGetPressedImageHandleButton)
		{
			mListener.showPressedImageHandle();
		}
		else if (button == mGetPressedImagePathButton)
		{
			mListener.showPressedImagePath();
		}
	}

} // end of ImageButtonTest
