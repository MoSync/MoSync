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
 * @file TestScreen.cpp
 * @author Bogdan Iusco.
 *
 * @brief Shows the tested ImageButton widget.
 */

#define SCREEN_TITLE "Test screen"

#include <conprint.h>
#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <mastdlib.h>

#include "TestScreen.h"

#include "MAHeaders.h"

namespace ImageButtonTest
{
	/**
	 * Constructor.
	 */
	TestScreen::TestScreen() :
		Screen(),
		mMainLayout(NULL),
		mImageButton(NULL),
		mEvents(NULL)
	{
		this->setTitle(SCREEN_TITLE);

		createMainLayout();
		mImageButton->addButtonListener(this);
	}

	/**
	 * Destructor.
	 */
	TestScreen::~TestScreen()
	{
		mImageButton->removeButtonListener(this);

	}

	/**
	 * Set the foreground image from a given file path.
	 * @param path Image file path.
	 */
	void TestScreen::setImageButtonForeground(const MAUtil::String& path)
	{
		mImageButton->setImagePath(path);
	}

	/**
	 * Set the foreground image from resources.
	 * @param handle Handle to a image.
	 */
	void TestScreen::setImageButtonForeground(const MAHandle handle)
	{
		mImageButton->setImage(handle);
	}

	/**
	 * Get the foreground image path.
	 * @return The foreground image path.
	 */
	MAUtil::String TestScreen::getImageButtonForegroundPath()
	{
		return mImageButton->getImagePath();
	}

	/**
	 * Set the background image from a given file path.
	 * @param path Image file path.
	 */
	void TestScreen::setImageButtonBackground(const MAUtil::String& path)
	{
		mImageButton->setBackgroundImagePath(path);
	}

	/**
	 * Set the background image from resources.
	 * @param handle Handle to a image.
	 */
	void TestScreen::setImageButtonBackground(const MAHandle handle)
	{
		mImageButton->setBackgroundImage(handle);
	}

	/**
	 * Get the background image path.
	 * @return The foreground image path.
	 */
	MAUtil::String TestScreen::getImageButtonBackgroundPath()
	{
		return mImageButton->getBackgroundImagePath();
	}

	/**
	 * Set the pressed image from a given file path.
	 * @param path Image file path.
	 */
	void TestScreen::setImageButtonPressed(const MAUtil::String& path)
	{
		mImageButton->setPressedImagePath(path);
	}

	/**
	 * Set the pressed image from resources.
	 * @param handle Handle to a image.
	 */
	void TestScreen::setImageButtonPressed(const MAHandle handle)
	{
		mImageButton->setPressedImage(handle);
	}

	/**
	 * Get the pressed image handle.
	 * @return The pressed image handle.
	 */
	MAHandle TestScreen::getImageButtonPressedHandle()
	{
		return mImageButton->getPressedImage();
	}

	/**
	 * Get the pressed image path.
	 * @return The pressed image path.
	 */
	MAUtil::String TestScreen::getImageButtonPressedPath()
	{
		return mImageButton->getPressedImagePath();
	}

	/**
	 * Creates and adds main layout to the screen.
	 */
	void TestScreen::createMainLayout() {
		// Create and add the main layout to the screen.
		mMainLayout = new VerticalLayout();
		mMainLayout->setBackgroundColor(0xFF0000);
		Screen::setMainWidget(mMainLayout);

		mImageButton = new ImageButton();
		mImageButton->wrapContentHorizontally();
		mMainLayout->addChild(mImageButton);

		int result = mImageButton->setBackgroundImage(RES_IMAGE_BACKGROUND);
		mImageButton->setText("click me");
		printf("result setImage = %d", result);

		mEvents = new ListView();
		mEvents->fillSpaceHorizontally();
		mEvents->fillSpaceVertically();
		mMainLayout->addChild(mEvents);
	}

	/**
	 * This method is called when there is an touch-down event for
	 * a button.
	 * Platform: iOS and Android.
	 * @param button The button object that generated the event.
	 */
	void TestScreen::buttonPressed(Widget* button)
	{
		if (button == mImageButton)
		{
			printf("button pressed event for mImageButton");
			ListViewItem* item = new ListViewItem();
			item->setText("buttonPressed");
			mEvents->addChild(item);
		}
	}

	/**
	 * This method is called when there is an touch-up event for
	 * a button.
	 * Platform: iOS and Android.
	 * @param button The button object that generated the event.
	 */
	void TestScreen::buttonReleased(Widget* button)
	{
		if (button == mImageButton)
		{
			printf("button released event for mImageButton");
			ListViewItem* item = new ListViewItem();
			item->setText("buttonReleased");
			mEvents->addChild(item);
		}
	}

	/**
	 * This method is called if the touch-up event was inside the
	 * bounds of the button.
	 * Platform: iOS and Android.
	 * @param button The button object that generated the event.
	 */
	void TestScreen::buttonClicked(Widget* button)
	{
		if (button == mImageButton)
		{
			printf("button clicked event for mImageButton");
			ListViewItem* item = new ListViewItem();
			item->setText("buttonClicked");
			mEvents->addChild(item);
		}
	}

} // end of ImageButtonTest
