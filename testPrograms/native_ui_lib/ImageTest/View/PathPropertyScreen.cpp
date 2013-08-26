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
 * @file PathPropertyScreen.cpp
 * @author Bogdan Iusco.
 *
 * @brief Shows widgets used to test MAW_IMAGE_PATH property.
 */

#define SCREEN_TITLE "Image path"

#define SET_PATH_LABEL_TEXT "Path"
#define SET_IMAGE_FROM_PATH_BTN_TEXT "Select image from path"
#define SET_IMAGE_FROM_HANDLE_BTN_TEXT "Select image from handle"
#define GET_IMAGE_PATH_BTN_TEXT "Get image path"

#include <NativeUI/Button.h>
#include <NativeUI/EditBox.h>
#include <NativeUI/HorizontalLayout.h>
#include <NativeUI/Label.h>
#include <NativeUI/VerticalLayout.h>

#include "PathPropertyScreen.h"
#include "PathPropertyScreenListener.h"

namespace ImageTest
{

	/**
	 * Constructor.
	 * @param listener Listener for screen.
	 */
	PathPropertyScreen::PathPropertyScreen(PathPropertyScreenListener& listener):
		mListener(listener),
		mMainLayout(NULL),
		mPathEditBox(NULL),
		mSelectImageFromPathButton(NULL),
		mSelectImageFromHandleButton(NULL),
		mGetImagePathButton(NULL)
	{
		this->setTitle(SCREEN_TITLE);
		this->createUI();

		mPathEditBox->addEditBoxListener(this);
		mSelectImageFromHandleButton->addButtonListener(this);
		mSelectImageFromPathButton->addButtonListener(this);
		mGetImagePathButton->addButtonListener(this);

		mGetImagePathButton->setEnabled(false);
	}

	/**
	 * Destructor.
	 */
	PathPropertyScreen::~PathPropertyScreen()
	{
		mPathEditBox->removeEditBoxListener(this);
		mSelectImageFromHandleButton->removeButtonListener(this);
		mSelectImageFromPathButton->removeButtonListener(this);
		mGetImagePathButton->removeButtonListener(this);
	}

	/**
	 * Set displayed path in the edit box.
	 * @param path Text to set.
	 */
	void PathPropertyScreen::setDisplayedImagePath(const MAUtil::String& path)
	{
		mPathEditBox->setText(path);
	}

	/**
	 * Create screen's UI.
	 */
	void PathPropertyScreen::createUI()
	{
		mMainLayout = new NativeUI::VerticalLayout();
		mMainLayout->setBackgroundColor(0xFF0000);
		this->setMainWidget(mMainLayout);

		this->createEditBoxLayout();
		this->createSelectImageFromPathButton();
		this->createSelectImageFromHandleButton();
		this->createGetImagePathButton();
	}

	/**
	 * Create layout that contains:
	 * - a Label widget.
	 * - a EditBox widget that will be used to get a file path.
	 */
	void PathPropertyScreen::createEditBoxLayout()
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
	 * Create an button that will be used to select the image
	 * from a file path.
	 */
	void PathPropertyScreen::createSelectImageFromPathButton()
	{
		mSelectImageFromPathButton = new NativeUI::Button();
		mSelectImageFromPathButton->fillSpaceHorizontally();
		mSelectImageFromPathButton->setText(SET_IMAGE_FROM_PATH_BTN_TEXT);
		mMainLayout->addChild(mSelectImageFromPathButton);
	}

	/**
	 * Create an button that will be used to select the image
	 * from a handle.
	 */
	void PathPropertyScreen::createSelectImageFromHandleButton()
	{
		mSelectImageFromHandleButton = new NativeUI::Button();
		mSelectImageFromHandleButton->fillSpaceHorizontally();
		mSelectImageFromHandleButton->setText(SET_IMAGE_FROM_HANDLE_BTN_TEXT);
		mMainLayout->addChild(mSelectImageFromHandleButton);
	}

	/**
	 * Create an button that will be used to display the image path.
	 */
	void PathPropertyScreen::createGetImagePathButton()
	{
		mGetImagePathButton = new NativeUI::Button();
		mGetImagePathButton->fillSpaceHorizontally();
		mGetImagePathButton->setText(GET_IMAGE_PATH_BTN_TEXT);
		mMainLayout->addChild(mGetImagePathButton);
	}

	/**
	 * This method is called when the return button was pressed.
	 * On iOS platform the virtual keyboard is not closed after
	 * receiving this event.
	 * This method is called only if the edit box mode is
	 * #MAW_EDIT_BOX_SINGLE_LINE.
	 * @param editBox The edit box object that generated the event.
	 */
	void PathPropertyScreen::editBoxReturn(NativeUI::EditBox* editBox)
	{
		editBox->hideKeyboard();
	}

	/**
	 * This method is called if the touch-up event was inside the
	 * bounds of the button.
	 * Platform: iOS, Android, Windows Phone.
	 * @param button The button object that generated the event.
	 */
	void PathPropertyScreen::buttonClicked(NativeUI::Widget* button)
	{
		if (button == mSelectImageFromHandleButton)
		{
			mListener.selectImageFromHandle();
			mGetImagePathButton->setEnabled(false);
		}
		else if (button == mSelectImageFromPathButton)
		{
			MAUtil::String path = mPathEditBox->getText();
			mListener.selectImageFromPath(path);
			mGetImagePathButton->setEnabled(true);
		}
		else if (button == mGetImagePathButton)
		{
			mListener.showImagePath();
		}
	}

} // end of ImageTest
