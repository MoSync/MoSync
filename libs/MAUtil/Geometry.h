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

/** \file Geometry.h
* \brief Geometric helper structures/functions (Rect, Point)
*
*
* \author Niklas Nummelin
*
*/

#ifndef _SE_MSAB_MAUTIL_GEOMETRY_H_
#define _SE_MSAB_MAUTIL_GEOMETRY_H_

#include <ma.h>

namespace MAUtil {
	class Point : public MAPoint2d {
	public:
		Point() { x = y = 0; }
		Point(int ax, int ay) { x = ax; y = ay; }
		void set(int ax, int ay) { this->x = ax; this->y = ay; }
	};

	class Rect {
	public:
		Rect();
		Rect(int x, int y, int width, int height);

		bool contains(const Point &p) const;
		bool contains(int x, int y) const;
		
		/**
		* Returns true iff \a this and \a r share any surface point.
		*/
		bool overlaps(const Rect &r) const;

		/*
		* Returns true iff any edge of \a this crosses any edge of \a r.
		*/
		//virtual bool intersects(const Rect &r) const;

		void setPosition(int x, int y);
		virtual void set(int x, int y, int w, int h);

		int x, y, width, height;
	};
}

#endif
