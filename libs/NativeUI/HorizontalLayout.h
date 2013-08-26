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
 * @file HorizontalLayout.h
 * @author Mikael Kindborg
 *
 * \brief Class for horizontal layout of widgets.
 */

#ifndef NATIVEUI_HORIZONTAL_LAYOUT_H_
#define NATIVEUI_HORIZONTAL_LAYOUT_H_

#include "Widget.h"
#include "Layout.h"

namespace NativeUI
{
	/**
	 * \brief A horizontal layout is a layout that stacks widgets in the
	 * horizontal axis.
	 */
	class HorizontalLayout : public Layout
	{
	public:
		/**
		 * Constructor.
		 * By default, when a horizontal layout is created it fills the
		 * available space in the parent widget horizontally and vertically.
		 */
		HorizontalLayout();

		/**
		 * Destructor.
		 */
		virtual ~HorizontalLayout();

		/**
		 * Sets how the children in the layout should be aligned in the
		 * vertical axis.
		 * @param alignment One of the constants #MAW_ALIGNMENT_TOP,
		 * #MAW_ALIGNMENT_CENTER or #MAW_ALIGNMENT_BOTTOM.
		 */
		virtual void setChildVerticalAlignment(const MAUtil::String alignment);

		/**
		 * Sets how the children in the layout should be aligned in the
		 * horizontal axis.
		 * @param alignment One of the constants #MAW_ALIGNMENT_LEFT,
		 * #MAW_ALIGNMENT_CENTER or #MAW_ALIGNMENT_RIGHT.
		 */
		virtual void setChildHorizontalAlignment(const MAUtil::String alignment);

	};

} // namespace NativeUI

#endif

/*! @} */
