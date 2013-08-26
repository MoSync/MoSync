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
* \brief Utility for scaling images by nearest neighbour or bilinearly.
* \author Niklas Nummelin
*/

#ifndef _SE_MSAB_MAUI_SCALER_H_
#define _SE_MSAB_MAUI_SCALER_H_

#include <ma.h>

namespace MAUI {

	/** 
	 * \brief Utility for scaling images by nearest neighbour or bilinearly.
	 *
	 * TODO: This class is overly complicated to use. There should be a way to
	 * just get a scaled image, either by specifying scale factor, or by 
	 * specifying a target rectangle (if the algorithm used supports this). 
	 * Both ways are useful.
	 *
	 * TODO: There is no descructor! Scaled images will not be deallocated.
	 * Make a way to deallocate the scaled images.
	 *
	 * TODO: The implementation relies on maCreatePlaceholder returning
	 * consecutive placeholder ids. This might not be safe.
	 */
	class Scaler {
	public:
		/**
		 * Scaling types.
		 */
		enum eScaleType {
			ST_NEAREST_NEIGHBOUR,
			ST_BILINEAR
		};

		/**
		 * Constructor.
		 *
		 * The scaler creates images within a range of scaling range.
		 *
		 * TODO: The following comment is difficult to understand, rewrite.
		 * Be aware that you need placeholderStart to placeholderStart+levels 
		 * amount of placeholders ordered after each other.
		 *
		 * \param image The image to scale.
		 * \param srcRect The part of the image to scale.
		 * \param minScale The scale for the beginning of the scaling sequence.
		 * \param maxScale The scale for the end of the scaling sequence.
		 * \param levels The number of images (scaling steps) to be generated.
		 * \param scaleType The scaling type.
		 *
		 * TODO: Create a constrcutor that is easier to use that 
		 * just scales the image to the desired scale and/or size 
		 * (preferably both options).
		 */
		Scaler(
			MAHandle image, 
			const MARect* srcRect, 
			double minScale, 
			double maxScale,
			int levels,
			eScaleType scaleType);
			
		/**
		 * TODO: Destructor is missing.
		 */
		
		/**
		 * Returns the size for a given scaling step (level).
		 * \param level The level for which the size is wanted.
		 * \return The size of the requested scaling step.
		 */
		MAExtent getSize(int level);
		
		/**
		 * Draw the scaled image identified by param "level"
		 * the the current drawing surface.
		 *
		 * \param centerX The center x coordinate of the image.
		 * \param centerY The center y coordinate of the image.
		 * \param level The scaling step to draw.
		 *
		 * TODO: It is a bit unexpected that this method draws the image
		 * centred when left/top is common in the rest of the MoSync API. 
		 * Suggesting new method that draws at left/top.
		 */
		void draw(int centerX, int centerY, int level);

	private:
		/**
		 * Number of scaling steps.
		 */
		int mLevels;
		
		/**
		 * Id of first placeholder for scaled images.
		 */
		MAHandle mPlaceholderStart;
	};

}

#endif /*_SE_MSAB_MAUI_SCALER_H_*/
