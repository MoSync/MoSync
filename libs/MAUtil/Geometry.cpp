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

#include "Geometry.h"

namespace MAUtil {
	Rect::Rect() : x(0), y(0), width(0), height(0) {
	}

	Rect::Rect(int ax, int ay, int w, int h) {
		this->x = ax;
		this->y = ay;
		this->width = w;
		this->height = h;
	}


	void Rect::set(int ax, int ay, int w, int h) {
		this->x = ax;
		this->y = ay;
		this->width = w;
		this->height = h;
	}

	void Rect::setPosition(int ax, int ay) {
		set(ax, ay, this->width, this->height);
	}


	bool Rect::contains(const Point &p) const
	{
		return contains(p.x, p.y);
	}

	bool Rect::overlaps(const Rect &r) const {
			bool xInside = false;
			bool yInside = false;

			if(x <= r.x && r.x <= x+width) xInside = true;
			if(x <= r.x+r.width && r.x+r.width <= x+width) xInside = true;

			if(r.x <= x && x <= r.x+r.width) xInside = true;
			if(r.x <= x+width && x+width <= r.x+r.width) xInside = true;


			if(y <= r.y && r.y <= y+height) yInside = true;
			if(y <= r.y+r.height && r.y+r.height <= y+height) yInside = true;

			if(r.y <= y && y <= r.y+r.height) yInside = true;
			if(r.y <= y+height && y+height <= r.y+r.height) yInside = true;

			return (xInside && yInside);
	}

	bool Rect::contains(int xx, int yy) const
	{
		if(xx < x)	return false;
		if(yy < y)	return false;
		if(xx > x + width)	return false;
		if(yy > y + height) return false;
		return true;
	}
}
