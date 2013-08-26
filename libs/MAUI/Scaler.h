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
* \file Scaler.h 
* \brief Utility for scaling images by nearest neighbour or bilinearly
* \author Patrick Broman and Niklas Nummelin
*/

#ifndef _SE_MSAB_MAUI_SCALER_H_
#define _SE_MSAB_MAUI_SCALER_H_

#include <ma.h>

namespace MAUI {

/** 
* \brief Utility for scaling images by nearest neighbour or bilinearly
*/
	class Scaler {
	public:

		enum eScaleType {
			ST_NEAREST_NEIGHBOUR,
			ST_BILINEAR
		};

		// be aware that you need placeholderStart to placeholderStart+levels amount of placeholders ordered
		// after each other.
		Scaler(MAHandle image, const MARect *srcRect, double minScale, double maxScale,
			int levels, eScaleType scaleType);
		MAExtent getSize(int scale);
		void draw(int x, int y, int level);

	private:
		int levels;
		MAHandle placeholderStart;
	};

}

#endif /*_SE_MSAB_MAUI_SCALER_H_*/
