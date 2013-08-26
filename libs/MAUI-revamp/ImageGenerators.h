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
* \file ImageGenerators.h
* \brief Utility class that generates linear and circular gradients.
* \author Patrick Broman and Niklas Nummelin
*/

#ifndef _SE_MSAB_MAUI_IMAGE_GENERATORS_H_
#define _SE_MSAB_MAUI_IMAGE_GENERATORS_H_

#include <ma.h>
#include <MAUtil/Geometry.h>

namespace MAUI {
	using namespace MAUtil;

/** 
 * \brief Utility class that generates linear and circular gradients.
 */
class ImageGenerators {
public:
	/**
	 * Available alpha modes.
	 */
	enum AlphaMode {
		AM_NOALPHA,
		AM_WRITEALPHA,
		AM_USEALPHA
	};

	/**
	 * TODO: Document method.
	 * \param image TODO: Document param.
	 * \param start TODO: Document param.
	 * \param end TODO: Document param.
	 * \param startColor TODO: Document param.
	 * \param endColor TODO: Document param.
	 * \param alphaMode TODO: Document param.
	 */
	static void linearGradient(
		MAHandle image, 
		Point start, 
		Point end, 
		int startColor, 
		int endColor, 
		ImageGenerators::AlphaMode alphaMode=AM_WRITEALPHA);
		
	/**
	 * TODO: Document method.
	 * \param image TODO: Document param.
	 * \param origo TODO: Document param.
	 * \param radius TODO: Document param.
	 * \param origoColor TODO: Document param.
	 * \param circleColor TODO: Document param.
	 * \param alphaMode TODO: Document param.
	 */
	static void circularGradient(
		MAHandle image, 
		Point origo, 
		int radius, 
		int origoColor, 
		int circleColor, 
		ImageGenerators::AlphaMode alphaMode=AM_WRITEALPHA);
};

} // namespace MAUI

#endif /* _SE_MSAB_MAUI_IMAGE_GENERATORS_H_ */
