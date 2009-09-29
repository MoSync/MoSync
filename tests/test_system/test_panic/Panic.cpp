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

/** \file Panic.cpp
* TEST PANIC
*
* TEST PANIC causes a MoSync Panic by trying to write to an invalid data object.
* Only for internal test purposes.
*
* \author Ann-Sofie Lindblom
*/
#include "MAHeaders.h"
#include <MAUtil/Moblet.h>
#include <conprint.h>
#include "../examples/common/helpers.h"
#include <maassert.h>

using namespace MAUtil;

class PanicMoblet : public Moblet {
public:
	PanicMoblet() {
		// Open and read storage
		printf("Don't panic!\n");
		/*Handle test_panic = maOpenStore("TestPanic.sav", MAS_CREATE_IF_NECESSARY);
		maReadStore(test_panic, RES_STORE);
		maCloseStore(test_panic, true);*/

		// Destroy data object and then try to write to it
		//maDestroyObject(TMP_STORE);
		//maCreateData(TMP_STORE, 0);
		int tmp = 17;
		//if(maGetDataSize(TMP_STORE) <= 0) {
			//PANIC_MESSAGE("Data object empty.");
			//FREEZE;
		//}
		maReadData(TMP_STORE, &tmp, 0, sizeof(int));
	}

	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_0)
			maExit(0);
	}

	void keyReleaseEvent(int keyCode) {}

private:
};

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;
	Moblet::run(new PanicMoblet());
	return 0;
};
