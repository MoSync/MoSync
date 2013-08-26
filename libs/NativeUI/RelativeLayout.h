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
 * @file RelativeLayout.h
 * @author Emma Tresanszki
 *
 * \brief Class for relative layout of widgets.
 */

#ifndef NATIVEUI_RELATIVE_LAYOUT_H_
#define NATIVEUI_RELATIVE_LAYOUT_H_

#include "Widget.h"
#include "Layout.h"

namespace NativeUI
{
	/**
	 * \brief A relative layout is a layout that layouts widgets relative
	 * to its coordinate system.
	 */
	class RelativeLayout : public Layout
	{
	public:
		/**
		 * Constructor.
		 */
		RelativeLayout();

		/**
		 * Destructor.
		 */
		virtual ~RelativeLayout();

		/**
		 * Sets the content offset.
		 * The layout will be scrolled to the given coordinate so its
		 * left-top visible corner will match the content offset point.
		 *
		 * Platform: iOS
		 * @param xCoord The left coordinate of the content offset point.
		 * @param yCoord The top coordinate of the content offset point.
		 */
		void setContentOffset(const int xCoord, const int yCoord);

		/**
		 * Get the x coordinate of the content offset point.
		 * The content offset point repesents the visible top-left point
		 * of a layout.
		 * Platform: iOS.
		 * @return The left coordinate.
		 */
		int getContentOffsetCoordX();

		/**
		 * Get the y coordinate of the content offset point.
		 * The content offset point repesents the visible top-left point
		 * of a layout.
		 * Platform: iOS.
		 * @return The top coordinate.
		 */
		int getContentOffsetCoordY();
	};

} // namespace NativeUI

#endif

/*! @} */
