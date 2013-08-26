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
