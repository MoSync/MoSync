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

/**
* \file Drawable.h
* \brief Interface for drawables.
* \author Niklas Nummelin
*/

#ifndef _SE_MSAB_MAUI_DRAWABLE_H_
#define _SE_MSAB_MAUI_DRAWABLE_H_

namespace MAUI {

	// TODO: maybe this should handle caching etc.
	// Basic implementations should be WidgetSkin, ImageDrawable, FilledDrawable
	// This function should use the Gfx_ calls.
	class Drawable {
	public:
		void setUseCaching(bool b = true);
		bool isUsingCache() const;

		virtual void draw(int x, int y, int w, int h) = 0;
		virtual bool isTransparent() const = 0;
	
	protected:
		bool mUseCaching;
	};
}

#endif // _SE_MSAB_MAUI_DRAWABLE_H_
