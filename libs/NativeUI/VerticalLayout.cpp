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
 * @file VerticalLayout.cpp
 * @author Mikael Kindborg
 *
 * Class for vertical layout of widgets.
 */

#include "VerticalLayout.h"

namespace NativeUI
{

	/**
	 * Constructor.
	 */
	VerticalLayout::VerticalLayout() :
		Layout(MAW_VERTICAL_LAYOUT)
	{
		// Set some common default values.
		fillSpaceVertically();
		fillSpaceHorizontally();
	}

	/**
	 * Destructor.
	 */
	VerticalLayout::~VerticalLayout()
	{
	}
	/**
	 * Set how the children in the layout should be aligned in the horizontal axis.
	 * @param alignment One of the constants #MAW_ALIGNMENT_LEFT,
	 * #MAW_ALIGNMENT_CENTER or #MAW_ALIGNMENT_RIGHT.
	 */
	void VerticalLayout::setChildHorizontalAlignment
	(const MAUtil::String alignment)
	{
		setProperty(
				MAW_VERTICAL_LAYOUT_CHILD_HORIZONTAL_ALIGNMENT, alignment);
	}

	/**
	 * Set how the children in the layout should be aligned in the vertical axis.
	 * @param alignment One of the constants #MAW_ALIGNMENT_TOP,
	 * #MAW_ALIGNMENT_CENTER or #MAW_ALIGNMENT_BOTTOM.
	 */
	void VerticalLayout::setChildVerticalAlignment
	(const MAUtil::String alignment)
	{
		setProperty(
				MAW_VERTICAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT, alignment);
	}

} // namespace NativeUI
