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
#include <MAUtil/PlaceholderPool.h>
#include <conprint.h>
#include <IX_SEGMENTED_DATA.h>

using namespace MAUtil;

#define ALLOC_MUL 4
#define ALLOC_SIZE (ALLOC_MUL*1024)

static char sBuf[ALLOC_SIZE];

class MyMoblet : public Moblet {
public:
	MyMoblet() {
		int total;
		printf("allocating...\n");
		int res;
		do {
			Handle h = PlaceholderPool::alloc();
			res = maCreateData(h, ALLOC_SIZE);
			if(res != RES_OK)
				break;
			maWriteData(h, sBuf, 0, ALLOC_SIZE);
			maReadData(h, sBuf, 0, ALLOC_SIZE);
			total += 4;
		} while(res == RES_OK);
		printf("%i KiB\n", total);
	}

	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_0)
			close();
	}
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};
