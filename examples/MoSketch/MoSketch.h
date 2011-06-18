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

#include <MAUtil/Vector.h>

using namespace MAUtil;

//Shorts are slower than ints, but we don't use them quickly enough here that it matters.
//In any case, minimizing the space used in the permanent store takes priority.
struct ShortPoint {
	short x, y;
};

typedef Vector<ShortPoint> PolyLine;

/*struct PSEUDO_FILE_FORMAT {
	int nPolyLines;
	struct PSEUDO_POLYLINE {
		int length;
		ShortPoint points[length];
	} polyLines[nPolyLines];
	bool penIsDown;
	ShortPoint pointer;
};*/
