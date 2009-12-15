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

#include <ma.h>
#include <maprofile.h>

enum Orientation {
	Landscape, Portrait
};

static Orientation defaultOrientation() {
	if(MA_PROF_CONST_SCREENSIZE_X > MA_PROF_CONST_SCREENSIZE_Y)
		return Landscape;
	else
		return Portrait;
}

Orientation currentOrientation() {
	MAExtent s = maGetScrSize();
	if(EXTENT_X(s) > EXTENT_Y(s))
		return Landscape;
	else
		return Portrait;
}


enum SoftkeyLocation {
	Bottom, Right
};

SoftkeyLocation currentSoftkeyLocation() {
	if(defaultOrientation() == currentOrientation())
		return Bottom;
	else
		return Right;
}
