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

#include <FreeImage.h>
#include <stdio.h>
#include <string.h>

static char* readFileToMem(char *filename, int *outSize) {
	FILE *file = fopen(filename, "rb");
	if(!file) {
		printf("Could not open file: %s\n.", filename);
		return NULL;
	}
	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);
	char *ret = new char[fileSize];
	int res = fread(ret, 1, fileSize, file);
	*outSize = fileSize;
	fclose(file);
	if(res != fileSize) {
		delete ret;
		return NULL;
	}
	return ret;
}

int main(int argc, char **argv) {
	if(argc<3) {
		printf("----------------------------------------------------\n");
		printf("| MAUI font generator.                             |\n");
		printf("| Tool to build a MAUI font (.mof) from an         |\n");
		printf("| Angelcode BMFont generated font (.fnt + .png).   |\n");
		printf("| usage:                                           |\n");
		printf("| -fontData <file>                                 |\n");
		printf("| -fontImage <file>                                |\n");
		printf("| -outFile <file>                                  |\n");
		printf("| -fontColor <color in hex>                        |\n");
		printf("----------------------------------------------------\n");
		return 1;
	}
	/*
	char *fontData = "font.fnt";
	char *fontImage = "font_00.png";
	char *outFile = "mofont.png";
	*/

	char *fontData = 0;
	char *fontImage = 0;
	char *outFile = 0;
	unsigned int fontColor = 0xffffffff;

	for(int i = 0; i < argc; i++) {
		if(strcmp(argv[i], "-fontData")==0) {
			i++;
			if(i>=argc) {
				printf("not enough parameters for -fontData.");			
				return 1;
			}
			fontData = argv[i];
		}
		else if(strcmp(argv[i], "-fontImage")==0) {
			i++;
			if(i>=argc) {
				printf("not enough parameters for -fontImage.");			
				return 1;
			}
			fontImage = argv[i];
		}
		else if(strcmp(argv[i], "-outFile")==0) {
			i++;
			if(i>=argc) {
				printf("not enough parameters for -outFile.");			
				return 1;
			}
			outFile = argv[i];
		}
		else if(strcmp(argv[i], "-fontColor")==0) {
			i++;
			if(i>=argc) {
				printf("not enough parameters for -fontColor.");			
				return 1;
			}
			sscanf(argv[i], "%x", &fontColor);
			if(strlen(argv[i])<=6) {
				fontColor|=0xff000000;
			}
		}
	}
	if(fontData == 0) {
		printf("you must specify -fontData <file>.\n");
		return 1;
	}
	else if(fontImage == 0) {
		printf("you must specify -fontImage <file>.\n");
		return 1;
	}
	else if(outFile == 0) {
		printf("you must specify -outFile <file>.\n");
		return 1;
	}

	/*
	SDL_Init( SDL_INIT_VIDEO);
	SDL_Surface *screen = SDL_SetVideoMode( 1024, 768, 32, SDL_SWSURFACE );
	if( screen == NULL ) { return 1; }
	*/	

	FIBITMAP *inImage = FreeImage_Load(FIF_PNG, fontImage);
	if(!inImage) {
		printf("Could not load fontImage.\n");
		return 1;
	}

	int bitToUseForAlpha = FI_RGBA_RED;
	if(FreeImage_GetBPP(inImage)!=32) {
		FIBITMAP *newImage = FreeImage_ConvertTo32Bits(inImage);
		if(newImage) {
			FreeImage_Unload(inImage);
			inImage = newImage;
		} else {
			printf("Could not convert image to 32 bits.\n");
			FreeImage_Unload(inImage);
			return 1;
		}
	} else {
		bitToUseForAlpha = FI_RGBA_ALPHA;
	}

	int width = FreeImage_GetWidth(inImage);
	int height = FreeImage_GetHeight(inImage);
	FIBITMAP *outImage = FreeImage_Allocate(width, height, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK);
	
	int dst_bytespp = FreeImage_GetLine(outImage) / FreeImage_GetWidth(outImage);
	int src_bytespp = FreeImage_GetLine(inImage) / FreeImage_GetWidth(inImage);

	for(int y = 0; y < height; y++) {
		BYTE *dstbits = FreeImage_GetScanLine(outImage, y);
		BYTE *srcbits = FreeImage_GetScanLine(inImage, y);
		for(int x = 0; x < width; x++) {
			dstbits[FI_RGBA_RED]	= ((fontColor&0xff0000)>>16);
			dstbits[FI_RGBA_GREEN]	= ((fontColor&0x00ff00)>>8);
			dstbits[FI_RGBA_BLUE]	= ((fontColor&0x0000ff));
			dstbits[FI_RGBA_ALPHA]	= (srcbits[bitToUseForAlpha]*((fontColor&0xff000000)>>24))>>8;
			dstbits+=dst_bytespp;
			srcbits+=src_bytespp;
		}
	}

	/*
	int inPitch = FreeImage_GetPitch(inImage);
	int outPitch = FreeImage_GetPitch(outImage);
	void *inPtr = FreeImage_GetBits(inImage);
	void *outPtr = FreeImage_GetBits(outImage);

	SDL_Surface *inSurface = SDL_CreateRGBSurfaceFrom(inPtr, width, height, 32, inPitch, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, ~(FI_RGBA_RED_MASK|FI_RGBA_GREEN_MASK|FI_RGBA_BLUE_MASK));
	SDL_Surface *outSurface = SDL_CreateRGBSurfaceFrom(outPtr, width, height, 32, outPitch, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, ~(FI_RGBA_RED_MASK|FI_RGBA_GREEN_MASK|FI_RGBA_BLUE_MASK));

	SDL_Rect srcRect = {0, 0, width, height};
	SDL_Rect dstRect = {0, 0, 1024, 768};

	SDL_BlitSurface(inSurface, &srcRect, screen, &dstRect);
	dstRect.y+=height;
	SDL_BlitSurface(outSurface, &srcRect, screen, &dstRect);
	SDL_Flip(screen);

	while(1) {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_KEYDOWN) {
				if(event.key.keysym.sym == SDLK_ESCAPE) {
					goto continueApp;
				}
			}
		}
	}
continueApp:
	*/

	if(!FreeImage_Save(FIF_PNG, outImage, outFile)) {
		printf("Could not save temp outfile.\n");
		FreeImage_Unload(inImage);
		FreeImage_Unload(outImage);
		return 1;
	}

	FreeImage_Unload(inImage);
	FreeImage_Unload(outImage);
	
	int fontDataSize;
	char *fontDataMem = readFileToMem(fontData, &fontDataSize);
	if(!fontDataMem) {
		printf("Could not read fontData to memory.\n");
		return 1;
	}
	
	int fontImageSize;
	char *fontImageMem = readFileToMem(outFile, &fontImageSize);
	if(!fontImageMem) {
		printf("Could not read temp image to memory.\n");
		return 1;
	}

	FILE *file = fopen(outFile, "wb");
	if(!file) {
		printf("Could not open outFile for writing.\n");
		return 1;
	}
	
	int res;
	res = fwrite(&fontDataSize, sizeof(int), 1, file);
	if(res != 1) {
		printf("Could not write outFile.\n");
		return 1;
	}
	res = fwrite(&fontImageSize, sizeof(long), 1, file);
	if(res != 1) {
		printf("Could not write outFile.\n");
		return 1;
	}
	res = fwrite(fontDataMem, sizeof(char), fontDataSize, file);
	if(res != fontDataSize) {
		printf("Could not write outFile.\n");
		return 1;
	}
	res = fwrite(fontImageMem, sizeof(char), fontImageSize, file);
	if(res != fontImageSize) {
		printf("Could not write outFile.\n");
		return 1;
	}
	
	fclose(file);
	
	delete []fontDataMem;
	delete []fontImageMem;

	//SDL_Quit();

	printf("Done.\n");

	return 0;
}
