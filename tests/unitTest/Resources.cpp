/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

#include "common.h"
#include <mastdlib.h>
#include <mastring.h>
#include "MAHeaders.h"
#include <conprint.h>

static const unsigned int sColors[] = {
	0xff000000,
	0xff0000ff,
	0xff00ff00,
	0xff00ffff,
	0xffff0000,
	0xffff00ff,
	0xffffff00,
	0xffffffff,
	0x00000000,
	0x000000ff,
	0x0000ff00,
	0x0000ffff,
	0x00ff0000,
	0x00ff00ff,
	0x00ffff00,
	0x00ffffff,
};

class ResTestCase : public TestCase {

public:
	ResTestCase() : TestCase("Resource test") {
	}

	void testDrawableImages() {
		const unsigned int colors[] = {
			0xff000000,
			0xff0000ff,
			0xff00ff00,
			0xff00ffff,
			0xffff0000,
			0xffff00ff,
			0xffffff00,
			0xffffffff,
		};

		const unsigned int NUMCOLORS = sizeof(colors)/sizeof(int);

		unsigned int colors2[NUMCOLORS];

		MAHandle testImg = maCreatePlaceholder();

		printf("testing resources\n");

		int res = maCreateDrawableImage(testImg, NUMCOLORS, 1);
		assert("maCreateImageRaw", res == RES_OK);

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

	void testGetImageData() {
		printf("testing maGetImageData\n");

		const unsigned int NUMCOLORS = sizeof(sColors)/sizeof(int);
		unsigned int colors2[NUMCOLORS];
		MAExtent e1 = maGetImageSize(ARGB_PNG);

		assert("maGetImageSize", e1 == EXTENT(NUMCOLORS,1));

		MARect rect = {0, 0, NUMCOLORS, 1};
		maGetImageData(ARGB_PNG, colors2, &rect, NUMCOLORS);

		assert("image: getData from PNG",
			(memcmp(sColors, colors2, sizeof(sColors)) == 0)
		);
		maDrawImage(ARGB_PNG, 0, 0);
		maUpdateScreen();
		//FREEZE;
	}

	void testImageRawData() {
		const unsigned int NUMCOLORS = sizeof(sColors)/sizeof(int);

		unsigned int colors2[NUMCOLORS];

		MAHandle testImg = maCreatePlaceholder();

		printf("imageRawData\n");

		int res = maCreateImageRaw(testImg, sColors, EXTENT(NUMCOLORS,1), 1);
		assert("maCreateImageRaw", res == RES_OK);

		MAExtent e1 = maGetImageSize(testImg);

		assert("maGetImageSize", e1 == EXTENT(NUMCOLORS,1));

		MARect rect = {0, 0, NUMCOLORS, 1};

		maGetImageData(testImg, colors2, &rect, NUMCOLORS);

		assert("image: createRaw then getData",
			(memcmp(sColors, colors2, sizeof(sColors)) == 0)
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

	void testCreateImageFromDataBug()
	{
		MAHandle hData, hImage;
		int iRet;
		char buf[32]={0};
		int cbData=sizeof(buf);

		hData = maCreatePlaceholder();
		if (maCreateData(hData, cbData) != RES_OK)
		{
			assert("maCreateData", false);
			return;
		}
		maWriteData (hData, buf, 0, cbData);
		hImage = maCreatePlaceholder();
		if ((iRet = maCreateImageFromData (hImage, hData, 0, cbData)) == RES_BAD_INPUT)
		{
			printf ("Image creation failed, as expected.\n");
			return;
		}
		printf ("Incorrect return value: %i\n", iRet);
		assert("testCreateImageFromDataBug", false);
	}

	void start() {
		testCreateImageFromDataBug();
		testDrawableImages();
		testGetImageData();
		testImageRawData();
		testStaticResources();

		suite->runNextCase();
	}

};

void addResTests(TestSuite* suite);
void addResTests(TestSuite* suite) {
	suite->addTestCase(new ResTestCase());
}
