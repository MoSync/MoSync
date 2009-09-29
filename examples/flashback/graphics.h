/* REminiscence - Flashback interpreter
 * Copyright (C) 2005-2007 Gregory Montoir
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "intern.h"

struct Graphics {
	uint8 *_layer;
	int16 _areaPoints[0x200];
	int16 _crx, _cry, _crw, _crh;

	void setClippingRect(int16 vx, int16 vy, int16 vw, int16 vh);
	void drawPoint(uint8 color, const Point *pt);
	void drawLine(uint8 color, const Point *pt1, const Point *pt2);
	void addEllipseRadius(int16 y, int16 x1, int16 x2);
	void drawEllipse(uint8 color, bool hasAlpha, const Point *pt, int16 rx, int16 ry);
	void fillArea(uint8 color, bool hasAlpha);
	void drawSegment(uint8 color, bool hasAlpha, int16 ys, const Point *pts, uint8 numPts);
	void drawPolygonOutline(uint8 color, const Point *pts, uint8 numPts);
	void drawPolygon(uint8 color, bool hasAlpha, const Point *pts, uint8 numPts);
};

#endif // __GRAPHICS_H__
