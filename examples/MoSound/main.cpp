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

/** \file main.cpp
*
* This file contains the main example program source.
*
* Shows how to use the sound api. This example shows
* how to play and loop a sound once.
*
* Copyright (c) Mobile Sorcery AB 2005-2007
*
* \author Niklas Nummelin
*
*/
#include <ma.h>
#include <mastdlib.h>
#include <maassert.h>
#include "MAHeaders.h"

extern "C" {
/**
 * \brief The entry point.
 */
int MAMain() GCCATTRIB(noreturn);
int MAMain()
{
	MAExtent e = maGetScrSize();

	/// play R_MOSO sound
	maSoundPlay(R_MOSO, 0, maGetDataSize(R_MOSO));

	/// clear the screen to black
	maSetColor(0x0);
	maFillRect(0, 0, EXTENT_X(e), EXTENT_Y(e));
	maUpdateScreen();

	/// wait for key press or close event
	FREEZE;
}
}
