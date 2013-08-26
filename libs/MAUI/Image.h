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
* \file Image.h 
* \brief Image widget, with position, size, and autosizing control
* \author Patrick Broman and Niklas Nummelin
*/

#ifndef _SE_MSAB_MAUI_IMAGE_H_
#define _SE_MSAB_MAUI_IMAGE_H_

#include "Widget.h"
#include <ma.h>

namespace MAUI {

	/** 
     * \brief Image widget, with position, size, and autosizing control
	 *
	 * Image is a widget used to put plain images in a graphical user
	 * interface. Pass the resource handle to the widget and it will display
	 * that image.
	 * The widget doesn't draw its background by default, use setDrawBackground(true) to enable it.
	 */
	class Image : public Widget {
	public:

		/** Constructor.
		  * \param x the horizontal position of the Image relative to its parent's top left padded corner.
		  * \param y the vertical position of the Image relative to its parent's top left padded corner
		  * \param width the width of the Image.
		  * \param height the height of the Image.
		  * \param parent pointer to the parent widget. Passing anything else than NULL causes the Image to be added to the parent's children.
		  * \param autoSizeX If true, the widget will be resized in the x-axis to the image size.
		  * \param autoSizeY If true, the widget will be resized in the y-axis to the image size.
		  * \param res A handle to the image resource containing the image data.
		  **/
		Image(int x, int y, int width, int height, Widget* parent=NULL, bool autoSizeX=true, bool autoSizeY=true, MAHandle res=0);

		/**
		 * Set the image resource of the image widget.
		 */
		void setResource(MAHandle res);

		/**
		 * Get the image resource of the image widget.
		 */
		MAHandle getResource() const;

	protected:
		void drawWidget();

		MAHandle resource;
		bool autoSizeX, autoSizeY;
		int imageWidth, imageHeight;
	};

}

#endif /* _SE_MSAB_MAUI_IMAGE_H_ */
