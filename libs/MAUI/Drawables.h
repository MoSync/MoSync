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
	/// Draws an image.
	class DrawableImage : public Drawable {
	public:
		/**
		* \param image The image to be drawn.
		* \param transparent Set to false if you know the image is opaque, for increased performance.
		*/
		DrawableImage(MAHandle image, bool transparent = true)
			: mImage(image), mTransparent(transparent) {}
		void draw(int x, int y, int w, int h);
		bool isTransparent() const { return mTransparent; }
	protected:
		MAHandle mImage;
		bool mTransparent;
	};

	/// Fills the area with a color.
	class DrawableColor : public Drawable {
	public:
		DrawableColor(int color)
			: mColor(color) {}
		void draw(int x, int y, int w, int h);
		bool isTransparent() const { return false; }
	protected:
		int mColor;
	};
}

#endif	//_SE_MSAB_MAUI_DRAWABLES_H_
