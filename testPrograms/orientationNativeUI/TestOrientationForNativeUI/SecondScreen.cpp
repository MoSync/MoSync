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
		mMainLayout->addChild(mLabel);

		MAUtil::Environment::getEnvironment().addPointerListener(this);
	}

	/**
	 * Destructor.
	 */
	SecondScreen::~SecondScreen()
	{
		MAUtil::Environment::getEnvironment().removePointerListener(this);
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

	/**
	 * Called just before the screen begins rotating.
	 */
	void SecondScreen::orientationWillChange()
	{
	}

	/**
	 * Called after the screen orientation has changed.
	 * Available only on iOS and Windows Phone 7.1 platforms.
	 */
	void SecondScreen::orientationDidChange()
	{

	}
} // namespace OrientationTest
