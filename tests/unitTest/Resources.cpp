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

#include "common.h"
#include <mastdlib.h>
#include <mastring.h>
#include "MAHeaders.h"
#include <conprint.h>

class ResTestCase : public TestCase {

public:
	ResTestCase() : TestCase("Resource test") {
	}

	void testDrawableImages() {
		unsigned int colors[] = {
				0xff000000,
				0xff0000ff,
				0xff00ff00,
				0xff00ffff,
				0xffff0000,
				0xffff00ff,
				0xffffff00,
				0xffffffff,
		};

#define NUMCOLORS (sizeof(colors)/sizeof(int))

		unsigned int colors2[NUMCOLORS];

		MAHandle testImg = maCreatePlaceholder();

		printf("testing resources\n");

		maCreateDrawableImage(testImg, NUMCOLORS, 1);

		MAExtent e1 = maGetImageSize(testImg);

		assert("maGetImageSize", e1 == EXTENT(NUMCOLORS,1));

		maSetDrawTarget(testImg);

		for(unsigned int i = 0; i < NUMCOLORS; i++) {
			maSetColor(colors[i]);
			maPlot(i, 0);
		}

		MARect rect = {0, 0, NUMCOLORS, 1};

		maSetDrawTarget(0);

		maGetImageData(testImg, colors2, &rect, NUMCOLORS);

		assert(
				"testing drawable image res",
				(memcmp(colors, colors2, sizeof(colors)) == 0)
		);

	}

	void testStaticResources() {
		int numbers[16];

		maReadData(RES_BINTEST, numbers, 0, 16*sizeof(int));

		bool failed = false;

		for(int i = 0; i < 16; i++)
			if(numbers[i] != i + 1) {
				printf("%d != %d", numbers[i], i +1);
				failed = true;
				break;
			}
		assert("Static res test", !failed);
	}

	void testPlaceHolder() {
		maCreateData(RES_PLACEHOLDER, 16);

		assert("maGetDataSize", (maGetDataSize(RES_PLACEHOLDER) == 16));

		for(int i = 0; i < 4; i++)
			maWriteData(RES_PLACEHOLDER, &i, i*4, 4);

		bool failed = false;

		for(int i = 0; i < 4; i++) {
			int dest;
			maReadData(RES_PLACEHOLDER, &dest, i*4, 4);
			if(dest != i) {
				failed = true;
				break;
			}
		}
		assert("Placeholder test", !failed);

	}

	void start() {



		testDrawableImages();
		testStaticResources();

		suite->runNextCase();
	}

};

void addResTests(TestSuite* suite);
void addResTests(TestSuite* suite) {
	suite->addTestCase(new ResTestCase());
}
