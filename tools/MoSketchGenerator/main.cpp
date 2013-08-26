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

#include <stdio.h>
#include <stdlib.h>
#define FREEIMAGE_LIB
#include <FreeImage.h>
#include "..\..\examples\MoSketch\MoSketch.h"

#define MYASSERT(a) if(!(a)) { printf("Error on line %i\n", __LINE__); exit(-1); }
#define MY_FWRITE(ptr, size, file) MYASSERT(fwrite(ptr, 1, size, file) == size)

void generate(Vector<PolyLine>& shorts, const char* input_file);
void eat(const Vector<PolyLine>& shorts);

int main() {
	FreeImage_Initialise();
	atexit(FreeImage_DeInitialise);

	Vector<PolyLine> my_shorts;
	generate(my_shorts, "pixellogo.png");
	eat(my_shorts);
	return 0;
}

void eat(const Vector<PolyLine>& shorts) {
	FILE* output = fopen("MoSync.sketch", "wb");

	int nShorts = shorts.size();
	MY_FWRITE(&nShorts, sizeof(int), output);
	for(int i=0; i<nShorts; i++) {
		const PolyLine& pl(shorts[i]);
		int length = pl.size();
		MY_FWRITE(&length, sizeof(length), output);
		MY_FWRITE(pl.pointer(), length * sizeof(ShortPoint), output);
	}
	bool penDown = false;
	MY_FWRITE(&penDown, sizeof(bool), output);
	//figure this one out, why don'tcha ;)
	ShortPoint p(shorts[shorts.size() - 1][shorts[shorts.size() - 1].size() - 1]);
	MY_FWRITE(&p, sizeof(ShortPoint), output);

	fclose(output);
}

FIBITMAP* monoChrome;

bool getPixel(int x, int y) {
	BYTE b;
	MYASSERT(FreeImage_GetPixelIndex(monoChrome, x, y, &b));
	return !b;
}

void generate(Vector<PolyLine>& shorts, const char* input_file) {
	//read png file into 2d bool vector
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(input_file, 2510);
	FIBITMAP *originalBitmap = FreeImage_Load(format, input_file, 0);
	MYASSERT(originalBitmap);
	monoChrome = FreeImage_Threshold(originalBitmap, 10);
	MYASSERT(monoChrome);
	FreeImage_Unload(originalBitmap);

	//generate shorts by working downwards
	for(unsigned y=FreeImage_GetHeight(monoChrome) - 1; y<FreeImage_GetHeight(monoChrome); y--) {
		for(unsigned x=0; x<FreeImage_GetWidth(monoChrome); x++) {
			if(getPixel(x, y)) {
				int left = x;
				while(x < FreeImage_GetWidth(monoChrome)) {
					if(!getPixel(x, y))	//must only be called while (x < width)
						break;
					x++;
				}
				int right = x - 1;
				ShortPoint pLeft = { left, FreeImage_GetHeight(monoChrome) - y - 1 };
				ShortPoint pRight = { right, FreeImage_GetHeight(monoChrome) - y - 1 };
				PolyLine pl;
				pl.add(pLeft);
				pl.add(pRight);
				shorts.add(pl);
			}
		}
	}

	//cleanup
	FreeImage_Unload(monoChrome);
}
