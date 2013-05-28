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
 * @file ImagePropertiesScreen.cpp
 * @author Bogdan Iusco.
 *
 * @brief Shows widgets used to test MAW_IMAGE_BUTTON_IMAGE_PATH and
 * MAW_IMAGE_BUTTON_BACKGROUND_IMAGE_PATH properties.
 */

#define SCREEN_TITLE "Image path"

#define SET_PATH_LABEL_TEXT "Path"
#define SET_FOREGROUND_IMAGE_FROM_PATH_BTN_TEXT "Select foreground image from path"
#define SET_FOREGROUND_IMAGE_FROM_HANDLE_BTN_TEXT "Select foreground image from handle"
#define GET_FOREGROUND_IMAGE_PATH_BTN_TEXT "Get foreground image path"

#define SET_BACKGROUND_IMAGE_FROM_PATH_BTN_TEXT "Select background image from path"
#define SET_BACKGROUND_IMAGE_FROM_HANDLE_BTN_TEXT "Select background image from handle"
#define GET_BACKGROUND_IMAGE_PATH_BTN_TEXT "Get background image path"

#include <NativeUI/Button.h>
#include <NativeUI/EditBox.h>
#include <NativeUI/HorizontalLayout.h>
#include <NativeUI/Label.h>
#include <NativeUI/VerticalLayout.h>

#include "ImagePropertiesScreen.h"
#include "ImagePropertiesScreenListener.h"

namespace ImageButtonTest
{

	/**
	 * Constructor.
	 * @param listener Listener for screen.
	 */
	ImagePropertiesScreen::ImagePropertiesScreen(ImagePropertiesScreenListener& listener):
		mListener(listener),
		mMainLayout(NULL),
		mPathForegroundEditBox(NULL),
		mPathBackgroundEditBox(NULL),
		mForegroundImageFromPathButton(NULL),
		mForegroundImageFromHandleButton(NULL),
		mGetForegroundImagePathButton(NULL),
		mBackgroundImageFromPathButton(NULL),
		mBackgroundImageFromHandleButton(NULL),
		mGetBackgroundImagePathButton(NULL)
	{
		this->setTitle(SCREEN_TITLE);
		this->createUI();

		mPathForegroundEditBox->addEditBoxListener(this);
		mPathBackgroundEditBox->addEditBoxListener(this);

		mForegroundImageFromHandleButton->addButtonListener(this);
		mForegroundImageFromPathButton->addButtonListener(this);
		mGetForegroundImagePathButton->addButtonListener(this);
		mBackgroundImageFromPathButton->addButtonListener(this);
		mBackgroundImageFromHandleButton->addButtonListener(this);
		mGetBackgroundImagePathButton->addButtonListener(this);

		mGetForegroundImagePathButton->setEnabled(false);
		mGetBackgroundImagePathButton->setEnabled(false);
	}

	/**
	 * Destructor.
	 */
	ImagePropertiesScreen::~ImagePropertiesScreen()
	{
		mPathForegroundEditBox->removeEditBoxListener(this);
		mPathBackgroundEditBox->removeEditBoxListener(this);

		mForegroundImageFromHandleButton->removeButtonListener(this);
		mForegroundImageFromPathButton->removeButtonListener(this);
		mGetForegroundImagePathButton->removeButtonListener(this);
		mBackgroundImageFromPathButton->removeButtonListener(this);
		mBackgroundImageFromHandleButton->removeButtonListener(this);
		mGetBackgroundImagePathButton->removeButtonListener(this);
	}

	/**
	 * Set displayed foreground image path in the edit box.
	 * @param path Text to set.
	 */
	void ImagePropertiesScreen::setDisplayedForegoundImagePath(const MAUtil::String& path)
	{
		mPathForegroundEditBox->setText(path);
	}

	/**
	 * Set displayed background image path in the edit box.
	 * @param path Text to set.
	 */
	void ImagePropertiesScreen::setDisplayedBackgoundImagePath(const MAUtil::String& path)
	{
		mPathBackgroundEditBox->setText(path);
	}

	/**
	 * Create screen's UI.
	 */
	void ImagePropertiesScreen::createUI()
	{
		mMainLayout = new NativeUI::VerticalLayout();
		mMainLayout->setBackgroundColor(0xFF0000);
		this->setMainWidget(mMainLayout);

		this->createEditBoxLayoutForeground();
		this->createForegroundImageFromPathButton();
		this->createForegroundImageFromHandleButton();
		this->createGetForegroundImagePathButton();

		this->createEditBoxLayoutBackground();
		this->createBackgroundImageFromPathButton();
		this->createBackgroundImageFromHandleButton();
		this->createGetBackgroundImagePathButton();
	}

	/**
	 * Create layout that contains:
	 * - a Label widget.
	 * - a EditBox widget that will be used to get a file path for the
	 * foreground image.
	 */
	void ImagePropertiesScreen::createEditBoxLayoutForeground()
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

		mPathForegroundEditBox = new NativeUI::EditBox();
		mPathForegroundEditBox->wrapContentVertically();
		mPathForegroundEditBox->fillSpaceHorizontally();
		layout->addChild(mPathForegroundEditBox);
	}

	/**
	 * Create an button that will be used to select the foreground image
	 * from a file path.
	 */
	void ImagePropertiesScreen::createForegroundImageFromPathButton()
	{
		mForegroundImageFromPathButton = new NativeUI::Button();
		mForegroundImageFromPathButton->fillSpaceHorizontally();
		mForegroundImageFromPathButton->setText(SET_FOREGROUND_IMAGE_FROM_PATH_BTN_TEXT);
		mMainLayout->addChild(mForegroundImageFromPathButton);
	}

	/**
	 * Create an button that will be used to select the foreground image
	 * from a handle.
	 */
	void ImagePropertiesScreen::createForegroundImageFromHandleButton()
	{
		mForegroundImageFromHandleButton = new NativeUI::Button();
		mForegroundImageFromHandleButton->fillSpaceHorizontally();
		mForegroundImageFromHandleButton->setText(
			SET_FOREGROUND_IMAGE_FROM_HANDLE_BTN_TEXT);
		mMainLayout->addChild(mForegroundImageFromHandleButton);
	}

	/**
	 * Create an button that will be used to display the foreground image path.
	 */
	void ImagePropertiesScreen::createGetForegroundImagePathButton()
	{
		mGetForegroundImagePathButton = new NativeUI::Button();
		mGetForegroundImagePathButton->fillSpaceHorizontally();
		mGetForegroundImagePathButton->setText(GET_FOREGROUND_IMAGE_PATH_BTN_TEXT);
		mMainLayout->addChild(mGetForegroundImagePathButton);
	}

	/**
	 * Create layout that contains:
	 * - a Label widget.
	 * - a EditBox widget that will be used to get a file path for the
	 * background image.
	 */
	void ImagePropertiesScreen::createEditBoxLayoutBackground()
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

		mPathBackgroundEditBox = new NativeUI::EditBox();
		mPathBackgroundEditBox->wrapContentVertically();
		mPathBackgroundEditBox->fillSpaceHorizontally();
		layout->addChild(mPathBackgroundEditBox);
	}

	/**
	 * Create an button that will be used to select the background image
	 * from a file path.
	 */
	void ImagePropertiesScreen::createBackgroundImageFromPathButton()
	{
		mBackgroundImageFromPathButton = new NativeUI::Button();
		mBackgroundImageFromPathButton->fillSpaceHorizontally();
		mBackgroundImageFromPathButton->setText(SET_BACKGROUND_IMAGE_FROM_PATH_BTN_TEXT);
		mMainLayout->addChild(mBackgroundImageFromPathButton);
	}

	/**
	 * Create an button that will be used to select the background image
	 * from a handle.
	 */
	void ImagePropertiesScreen::createBackgroundImageFromHandleButton()
	{
		mBackgroundImageFromHandleButton = new NativeUI::Button();
		mBackgroundImageFromHandleButton->fillSpaceHorizontally();
		mBackgroundImageFromHandleButton->setText(
			SET_BACKGROUND_IMAGE_FROM_HANDLE_BTN_TEXT);
		mMainLayout->addChild(mBackgroundImageFromHandleButton);
	}

	/**
	 * Create an button that will be used to display the background image path.
	 */
	void ImagePropertiesScreen::createGetBackgroundImagePathButton()
	{
		mGetBackgroundImagePathButton = new NativeUI::Button();
		mGetBackgroundImagePathButton->fillSpaceHorizontally();
		mGetBackgroundImagePathButton->setText(GET_BACKGROUND_IMAGE_PATH_BTN_TEXT);
		mMainLayout->addChild(mGetBackgroundImagePathButton);
	}

	/**
	 * This method is called when the return button was pressed.
	 * On iOS platform the virtual keyboard is not closed after
	 * receiving this event.
	 * This method is called only if the edit box mode is
	 * #MAW_EDIT_BOX_SINGLE_LINE.
	 * @param editBox The edit box object that generated the event.
	 */
	void ImagePropertiesScreen::editBoxReturn(NativeUI::EditBox* editBox)
	{
		editBox->hideKeyboard();
	}

	/**
	 * This method is called if the touch-up event was inside the
	 * bounds of the button.
	 * Platform: iOS, Android, Windows Phone.
	 * @param button The button object that generated the event.
	 */
	void ImagePropertiesScreen::buttonClicked(NativeUI::Widget* button)
	{
		if (button == mForegroundImageFromHandleButton)
		{
			mListener.selectForegroundImageFromHandle();
			mGetForegroundImagePathButton->setEnabled(false);
		}
		else if (button == mForegroundImageFromPathButton)
		{
			MAUtil::String path = mPathForegroundEditBox->getText();
			mListener.selectForegroundImageFromPath(path);
			mGetForegroundImagePathButton->setEnabled(true);
		}
		else if (button == mGetForegroundImagePathButton)
		{
			mListener.showForegroundImagePath();
		}
		else if (button == mBackgroundImageFromHandleButton)
		{
			mListener.selectBackgroundImageFromHandle();
			mGetBackgroundImagePathButton->setEnabled(false);
		}
		else if (button == mBackgroundImageFromPathButton)
		{
			MAUtil::String path = mPathBackgroundEditBox->getText();
			mListener.selectBackgroundImageFromPath(path);
			mGetBackgroundImagePathButton->setEnabled(true);
		}
		else if (button == mGetBackgroundImagePathButton)
		{
			mListener.showBackgroundImagePath();
		}
	}

} // end of ImageButtonTest
