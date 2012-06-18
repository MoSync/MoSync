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
		void SetBackgroundImage(const MAHandle imageHandle);

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
	};

} // namespace NativeUI

#endif

/*! @} */
