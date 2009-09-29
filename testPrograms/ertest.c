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

#include "maapi.h"
#include "shared/print.h"
#include "shared/misc.h"

int MAMain() {
	const char* purl = "wap.mopix.se/test/test.comb";
	println("ER HTTP GET test");
	println(purl);
	maHttpGetToObject(purl, 1);
	maWait(WAIT_CONN);
	{
		int res = maConnState();
		printlnf("res %i", res);
		while(!(maGetEvent() == MAK_DOWN)) maWait(WAIT_EVENT);
		if(res <= 0)
			maExit(0);
	}
	maLoadProgram(1);  
	println("It didn't work.");
	while(1) maWait(WAIT_EVENT);
}

#include "shared/print.c"
#include "shared/misc.c"

