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
