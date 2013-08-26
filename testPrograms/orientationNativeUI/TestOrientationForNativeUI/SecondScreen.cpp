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
 * @file SecondScreen.cpp
 * @author Bogdan Iusco
 *
 * @brief Screen used for test coordinate system after changing orientation.
 */

#define SCREEN_TITLE "Coordinate system"

#define LABEL_WIDTH 80
#define LABEL_HEIGHT 80
#define LABEL_BG_COLOR 0x123456
#define LAYOUT_BG_COLOR 0xC0C0C0

#include <conprint.h>

#include "SecondScreen.h"

namespace OrientationTest
{

	/**
	 * Constructor.
	 */
	SecondScreen::SecondScreen():
		Screen(),
		mMainLayout(NULL),
		mLabel(NULL)
	{
		this->setTitle(SCREEN_TITLE);

		mMainLayout = new RelativeLayout();
		mMainLayout->fillSpaceHorizontally();
		mMainLayout->fillSpaceVertically();
		mMainLayout->setBackgroundColor(LAYOUT_BG_COLOR);
		this->setMainWidget(mMainLayout);

		mLabel = new Label();
		mLabel->setWidth(LABEL_WIDTH);
		mLabel->setHeight(LABEL_HEIGHT);
		mLabel->setBackgroundColor(LABEL_BG_COLOR);
		mLabel->setText("test");
		mMainLayout->addChild(mLabel);

		addScreenListener(this);
		MAUtil::Environment::getEnvironment().addPointerListener(this);
	}

	/**
	 * Destructor.
	 */
	SecondScreen::~SecondScreen()
	{
		removeScreenListener(this);
		MAUtil::Environment::getEnvironment().removePointerListener(this);
	}

	/**
	 * Called after the screen has finished rotating.
	 * Subclasses may override this method to perform additional actions
	 * after the rotation.
	 * @param screenOrientation The new screen orientation.
	 */
	void SecondScreen::orientationChanged(Screen* screen, int screenOrientation)
	{
		printf("SECOND Screen: orientationChanged to: %d", screenOrientation);
	}

	/**
	 * @deprecated: Use addScreenListener(ScreenListener*) instead.
	 * Called after the screen has finished rotating.
	 * Subclasses may override this method to perform additional actions
	 * after the rotation.
	 */
	void SecondScreen::orientationDidChange()
	{
	}

	/**
	 * Called when a pointer press event is fired.
	 * @param point The point where the touch down took place.
	 */
	void SecondScreen::pointerPressEvent(MAPoint2d point)
	{
		mLabel->setTopPosition(point.y);
		mLabel->setLeftPosition(point.x);
	}

	/**
	 * Called when a pointer move event is fired.
	 * @param point The point where the touch move took place.
	 */
	void SecondScreen::pointerMoveEvent(MAPoint2d point)
	{
		mLabel->setTopPosition(point.y);
		mLabel->setLeftPosition(point.x);
	}

	/**
	 * Called when a pointer release event is fired.
	 * @param point The point where the touch up took place.
	 */
	void SecondScreen::pointerReleaseEvent(MAPoint2d point)
	{
	}
} // namespace OrientationTest
