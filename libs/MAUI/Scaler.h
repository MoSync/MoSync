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
* \file Scaler.h 
* \brief Utility class for scaling images by nearest neighbour or bilinearly
* \author Patrick Broman and Niklas Nummelin
*/

#ifndef _SE_MSAB_MAUI_SCALER_H_
#define _SE_MSAB_MAUI_SCALER_H_

#include <ma.h>

namespace MAUI {

	class Scaler {
	public:

		enum eScaleType {
			ST_NEAREST_NEIGHBOUR,
			ST_BILINEAR
		};

		// be aware that you need placeholderStart to placeholderStart+levels amount of placeholders ordered
		// after eachother.
		Scaler(MAHandle image, const MARect *srcRect, double minScale, double maxScale,
			int levels, eScaleType scaleType);
		MAExtent getSize(int scale);
		void draw(int x, int y, int level);

	private:
		int levels;
		MAHandle placeholderStart;
	};

}

#endif /*_SE_MSAB_MAUI_SCALER_H_*/
