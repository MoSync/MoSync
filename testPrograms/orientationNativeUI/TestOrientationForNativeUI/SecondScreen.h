/*
Copyright (C) 2012 MoSync AB

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
 * @file SecondScreen.h
 * @author Bogdan Iusco
 *
 * @brief Screen used for test coordinate system after changing orientation.
 */

#ifndef ORIENTATION_SECOND_SCREEN_H_
#define ORIENTATION_SECOND_SCREEN_H_

#include <NativeUI/Screen.h>
#include <MAUtil/Environment.h>

#include <NativeUI/RelativeLayout.h>
#include <NativeUI/Label.h>


namespace OrientationTest
{
	using namespace NativeUI;
	using namespace MAUtil;

	class SecondScreen:
		public Screen,
		public PointerListener
	{
	public:
		/**
		 * Constructor.
		 */
		SecondScreen();

		/**
		 * Destructor.
		 */
		virtual ~SecondScreen();

		/**
		 * Called just before the screen begins rotating.
		 */
		virtual void orientationWillChange();

		/**
		 * Called after the screen orientation has changed.
		 * Available only on iOS and Windows Phone 7.1 platforms.
		 */
		virtual void orientationDidChange();

	private:
		/**
		 * Called when a pointer press event is fired.
		 * @param point The point where the touch down took place.
		 */
		virtual void pointerPressEvent(MAPoint2d point);

		/**
		 * Called when a pointer move event is fired.
		 * @param point The point where the touch move took place.
		 */
		virtual void pointerMoveEvent(MAPoint2d point);

		/**
		 * Called when a pointer release event is fired.
		 * @param point The point where the touch up took place.
		 */
		virtual void pointerReleaseEvent(MAPoint2d point);

	private:
		/**
		 * Main layout.
		 */
		RelativeLayout* mMainLayout;

		/**
		 * Label that will be moved on the layout.
		 */
		Label* mLabel;
	};

} // namespace OrientationTest

#endif /* ORIENTATION_SECOND_SCREEN_H_ */
