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
#include <maassert.h>
#include "maheaders.h"

void recursiveFail(int i) {
	if(i == 0)
		maPanic(i, "Fail!");
	else
		recursiveFail(i - 1);
}

int MAMain() {
#define SIZE 63
	char buffer[SIZE+1];
	//char ibuf[1024];

	recursiveFail(10);

	maReadData(R_UBIN, buffer, 0, maGetDataSize(R_UBIN));
	maDrawText(0, 10, buffer);
	maWriteData(R_UBIN, buffer, 42, maGetDataSize(R_UBIN) + 42);	//generates a Big Phat Error

	/*maReadData(R_UBIN_IMG, ibuf, 0, maGetDataSize(R_UBIN_IMG));
	maCreateImage(R_IMG_PLACEHOLDER, ibuf, maGetDataSize(R_UBIN_IMG));
	maDrawImage(R_IMG_PLACEHOLDER, 0, 100);*/

	maDrawImage(image1, 0, 0);
	maDrawImage(sprite1, 40, 0);

	maReadData(afile, buffer, 0, SIZE);
	buffer[SIZE] = 0;
	maDrawText(0, 20, buffer);

	maUpdateScreen();

	maDestroyObject(image1);
	maDrawText(0, 40, "Image destroyed.");
	maUpdateScreen();

	maCreateData(image1, 1024);
	maDrawText(0, 60, "Data created in its place.");
	maUpdateScreen();

	FREEZE;
}
