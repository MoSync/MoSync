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
 * @file ViewUtils.h
 * @author Bogdan Iusco
 *
 * @brief UI utility functions.
 */

#include <mastdlib.h>
#include <conprint.h>

#ifndef CPT_VIEW_UTILS_H_
#define CPT_VIEW_UTILS_H_

namespace CustomPickerTest
{

	// Buffer size constant.
	const int kBufferSize =  256;

	/**
	 * Get a random color.
	 * @return Color in hex representation.
	 */
	static int getRandomColor()
	{
		int red = rand() / 255;
		int green = rand() / 255;
		int blue = rand() / 255;
		int hexColor = (red << 16) + (green << 8) + blue;
		return hexColor;
	}

} // end of CustomPickerTest

#endif /* CPT_VIEW_UTILS_H_ */
