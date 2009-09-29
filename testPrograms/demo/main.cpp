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

#include <MAUtil/Moblet.h>
#include <madmath.h>
#include <conprint.h>

#include "Demo.h"

#include "Part1.h"
#include "Part2.h"

using namespace MAUtil;
using namespace MegaDemoEngine;

extern "C" int MAMain() {

	Demo demo;
	
	demo.addPart(new Part1(), 0, 100000);
	
	// add parts here
	
	int startTime = maGetMilliSecondCount();
	
	while(1) {
		int t = maGetMilliSecondCount();
		demo.render(t-startTime);
		EVENT e;
		while(maGetEvent(&e)) {
			if(e.type == EVENT_TYPE_CLOSE) {
				maExit(0);
			}
			else if(e.type == EVENT_TYPE_KEY_PRESSED) {
				if(e.key == MAK_0) {
					maExit(0);
				}
			}
		}
		maUpdateScreen();
		maResetBacklight();
	}
};
