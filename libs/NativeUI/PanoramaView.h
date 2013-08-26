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

/*! \addtogroup NativeUILib
 *  @{
 */

/**
 *  @defgroup NativeUILib Native UI Library
 *  @{
 */

/**
 * @file PanoramaView.h
 * @author Ciprian Filipas
 *
 * \brief Class that represents a visible panorama view. Note that
 * this is a Windows Phone 7 specific widget.
 */

#ifndef NATIVEUI_PANORAMA_VIEW_H_
#define NATIVEUI_PANORAMA_VIEW_H_

#include "Screen.h"

namespace NativeUI
{

	/**
	 * \brief Class that represents a visible panorama view. Note that
	 * this is available only on Windows Phone 7.
	 * A panorama control is a Windows Phone 7 specific control. The paroramaView is a screen container
	 * which may contain more then one screen. A screen can spann over the hardware screen width.
	 * In order to enable this you will have to set the width property of a screen to a certain value.
	 * For more information regarding this control please check the following link:
	 * http://msdn.microsoft.com/en-us/library/ff941104(v=vs.92).aspx
	 */
	class PanoramaView : public Screen
	{
	public:
		/**
		 * Constructor.
		 */
		PanoramaView();

		/**
		 * Destructor.
		 */
		~PanoramaView();

		/**
		 * Set the background image of the panorama view. The background image
		 * is displayed behind the child screens.
		 * @param imageHandle Handle to an image.
		 */
		void setBackgroundImage(const MAHandle imageHandle);

		/**
		 * Get the background image of the panorama view.
		 * @return A value greater than zero representing a MoSync handle to an
		 * uncompressed image resource, or zero if the property was not set.
		 */
		MAHandle getBackgroundImage();

		/**
		 * Add a new screen to the panorama view.
		 * @param screen The screen to be add at the panorama view.
		 */
		void addScreen(Screen* screen);

		/**
		 * Show a given screen.
		 * @param index The index of the screen to show.
		 * Index starts at zero.
		 */
		void setActiveScreen(const int& index);

		/**
		 * Returns the index of the current screen.
		 * Index starts at zero.
		 * @return The index of the current screen.
		 */
		int getActiveScreen();

		/**
		 * Check if panorama view is shown.
		 * @return true if the panorama view is visible, false otherwise.
		 */
		virtual bool isShown();
	};

} // namespace NativeUI

#endif

/*! @} */
