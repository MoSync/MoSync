/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
