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

/**
* \brief Point geometric helper structure/function
*/

	class Point : public MAPoint2d {
	public:
		Point() { x = y = 0; }
		Point(int ax, int ay) { x = ax; y = ay; }
		void set(int ax, int ay) { this->x = ax; this->y = ay; }
	};

/**
* \brief Rect geometric helper structure/function
*/

	class Rect {
	public:
		Rect();
		Rect(int x, int y, int width, int height);

		bool contains(const Point &p) const;
		bool contains(int x, int y) const;
		
		/**
		* Returns true if \a this and \a r share any surface point.
		*/
		bool overlaps(const Rect &r) const;

		/*
		* Returns true if any edge of \a this crosses any edge of \a r.
		*/
		//virtual bool intersects(const Rect &r) const;

		void setPosition(int x, int y);
		virtual void set(int x, int y, int w, int h);

		int x, y, width, height;
	};
}

#endif
