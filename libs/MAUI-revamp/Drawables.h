/* Copyright (C) 2010 MoSync AB

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

/** 
* \file Drawables.h 
* \brief Common types of Drawable.
* \author Fredrik Eldh
*/

#ifndef _SE_MSAB_MAUI_DRAWABLES_H_
#define _SE_MSAB_MAUI_DRAWABLES_H_

#include "Drawable.h"
#include <ma.h>

namespace MAUI {
	/**
	 * \brief Class that draws an image.
	 */
	class ImageDrawable : public Drawable {
	public:
		/**
		 * Constructor.
		 * \param image The image to be drawn.
		 * \param transparent Transparency of the image. 
		 * Set to false if you know the image is opaque, 
		 * for increased performance.
		 */
		ImageDrawable(MAHandle image, bool transparent = true)
			: mImage(image), mTransparent(transparent) {}
		
		/**
		 * Draw this image.
		 * \param left Left coordinate of image.
		 * \param top Right coordinate of image.
		 * \param width TODO: Unused param, implement of remove.
		 * \param height TODO: Unused param, implement of remove.
		 */
		void draw(int left, int top, int width, int height);
		
		/**
		 * Get the transparency of this image.
		 * \return true if transparent, false if not.
		 */
		bool isTransparent() const { return mTransparent; }
		
	protected:
		/**
		 * The color of this image.
		 */
		MAHandle mImage;
		
		/**
		 * The transparency of this image.
		 * true if transparent, false if opaque.
		 */
		bool mTransparent;
	};

	/**
	 * \brief Class that fills a rectangle with a color.
	 */
	class ColorDrawable : public Drawable {
	public:
		/**
		 * Constructor.
		 * \param color The color of this drawable.
		 */
		ColorDrawable(int color)
			: mColor(color) {}
			
		/**
		 * Draw this drawable. Draws a filled rectangle.
		 * \param left Left coordinate of rectangle.
		 * \param top Right coordinate of rectangle.
		 * \param width Width of rectangle.
		 * \param height Height of rectangle.
		 */
		void draw(int left, int top, int width, int height);
		
		/**
		 * Get the transparency of this drawable.
		 * \return true if transparent, false if opaque.
		 * Currently always returns false.
		 */
		bool isTransparent() const { return false; }
		
	protected:
		/**
		 * The color of this drawable.
		 */
		int mColor;
	};
}

#endif	//_SE_MSAB_MAUI_DRAWABLES_H_
