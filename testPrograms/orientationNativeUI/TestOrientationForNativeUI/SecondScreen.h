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
#include <NativeUI/ScreenListener.h>


namespace OrientationTest
{
	using namespace NativeUI;
	using namespace MAUtil;

	class SecondScreen:
		public Screen,
		public PointerListener,
		public ScreenListener
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
		 * Called after the screen has finished rotating.
		 * Subclasses may override this method to perform additional actions
		 * after the rotation.
		 * @param screenOrientation The new screen orientation.
		 */
		virtual void orientationChanged(Screen* screen, int screenOrientation);		/**
		 * @deprecated: Use addScreenListener(ScreenListener*) instead.
		 * Called after the screen has finished rotating.
		 * Subclasses may override this method to perform additional actions
		 * after the rotation.
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
