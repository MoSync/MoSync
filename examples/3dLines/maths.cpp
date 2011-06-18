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

/** \file maths.cpp
*
* This file contains mathematical support functions and macros.
*
* \author Patrick Broman and Niklas Nummelin
*
*/

#include "maths.h"
#include "madmath.h"

Vector::Vector() : mX(0), mY(0), mZ(0) {
}

Vector::Vector(int x, int y, int z) : mX(x), mY(y), mZ(z) {
}

int sinTable[TRIG_LOOKUP_SIZE];
#define TRIG_LOOKUP_RANGE ((1<<TRIG_LOOKUP_BITS)-1)

void initTrigTables() {

	double ang = 0;
	double ang_delta = (2*PI)/TRIG_LOOKUP_SIZE;
	for(int i = 0; i < TRIG_LOOKUP_SIZE; i++) {
		sinTable[i] = (int)(sin(ang)*(double)TRIG_LOOKUP_RANGE);
		ang+=ang_delta;
	}
}
