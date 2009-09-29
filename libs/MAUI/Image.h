/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

/** \file Image.h Copyright (c) Mobile Sorcery AB 2005-2006 **/

#ifndef _SE_MSAB_MAUI_IMAGE_H_
#define _SE_MSAB_MAUI_IMAGE_H_

#include "Widget.h"
#include <ma.h>

namespace MAUI {

	/**
	 * Image is a widget used to put plain images in a graphical user
	 * interface. Pass the resource handle to the widget and it will display
	 * that image.
	 */
	class Image : public Widget {
	public:

		/** Constructor.
		  * \param x the horizontal position of the Image relative to its parent's top left padded corner.
		  * \param y the vertical position of the Image relative to its parent's top left padded corner
		  * \param width the width of the Image.
		  * \param height the height of the Image.
		  * \param parent pointer to the parent widget. Passing anything else than NULL causes the Image to be added to the parent's children.
		  **/
		Image(int x, int y, int width, int height, Widget *parent=NULL);

		/**
		 * Constructor of the image widget. Pass the image resource of the image to display.
		 */
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
