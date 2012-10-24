/*
 * This project is set up to use STL (Standard Template Library) and
 * the Newlib standard C library (Newlib is required for STL).
 *
 * The program below makes a simple demonstration of STL. Numbers are
 * added to a vector and printed to the screen when the display is
 * touched or a key on the keypad is pressed.
 */

#include <ma.h>
#include <stdio.h>
//#include <vector>

//#include "MAHeaders.h"
#define RES_BARCODE_IMAGE 1

#include <zbar.h>

void createLuminosity(int* src, unsigned char* dst, int size)
{
	//0.21 R + 0.71 G + 0.07 B
	//0xaarrggbb

/*
	long long rf = (long long)(0.21f * 65536.0f);
	long long gf = (long long)(0.71f * 65536.0f);
	long long bf = (long long)(0.07f * 65536.0f);

	for(int i = 0; i < size; i++)
	{
		long long rb = (((long long)(src[i] & 0x00ff0000) * rf) >> 32);
		long long gb = (((((long long)(src[i] & 0x0000ff00)) << 8) * gf) >> 32);
		long long bb = (((((long long)(src[i] & 0x000000ff)) << 16) * bf) >> 32);

		int result = (int)(rb + gb + bb);
		if(result > 255)
			result = 255;

		dst[i] = (unsigned char)result;
	}
*/
	unsigned int rf = (unsigned int)(0.21f * 255.0f);
	unsigned int gf = (unsigned int)(0.71f * 255.0f);
	unsigned int bf = (unsigned int)(0.07f * 255.0f);

	for(int i = 0; i < size; i++)
	{
		unsigned int rb = (((unsigned int)(src[i] & 0x00ff0000 >> 8) * rf) >> 16);
		unsigned int gb = (((((unsigned int)(src[i] & 0x0000ff00))) * gf) >> 16);
		unsigned int bb = (((((unsigned int)(src[i] & 0x000000ff)) << 8) * bf) >> 16);

		int luminosity = (int)(rb + gb + bb);
		if(luminosity > 255)
			luminosity = 255;

		dst[i] = (unsigned char)luminosity;
	}
}

/**
 * Entry point of the program. The MAMain function
 * needs to be declared as extern "C".
 */
extern "C" int MAMain()
{
	MAEvent event;

	int imgSize = maGetImageSize(RES_BARCODE_IMAGE);

	int imgW = EXTENT_X(imgSize);
	int imgH = EXTENT_Y(imgSize);

	int imgDataSize = imgW * imgH;
	int* imgData = (int*) malloc(imgDataSize * 4);

	MARect imgRect;
	imgRect.left = 0;
	imgRect.top = 0;
	imgRect.width = imgW;
	imgRect.height = imgH;

	maGetImageData(RES_BARCODE_IMAGE, imgData, &imgRect, imgW);

	unsigned char* fixedImg = (unsigned char*) malloc(imgDataSize);

	printf("Converting image\n");

	createLuminosity(imgData, fixedImg, imgDataSize);

	printf("Scanning for barcodes\n");

	// create a reader
	zbar::ImageScanner scanner = zbar::zbar_image_scanner_create();

	// configure the reader
	zbar::zbar_image_scanner_set_config(scanner, zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);

	// wrap image data
	zbar::zbar_image_t *image = zbar::zbar_image_create();
	zbar::zbar_image_set_format(image, 0x30303859);// "Y800" = 0x30303859

	zbar::zbar_image_set_size(image, imgW, imgH);

	zbar::zbar_image_set_data(image, fixedImg, imgW * imgH, NULL);//zbar_image_free_data);

	// scan the image for barcodes
	zbar_scan_image(scanner, image);

	// extract results
	bool result = false;
	const zbar::zbar_symbol_t *symbol = zbar_image_first_symbol(image);
	for(; symbol; symbol = zbar_symbol_next(symbol)) {
		// do something useful with results
		zbar::zbar_symbol_type_t typ = zbar_symbol_get_type(symbol);
		const char *data = zbar_symbol_get_data(symbol);
		printf("decoded %s symbol \"%s\"\n",
			   zbar_get_symbol_name(typ), data);
		result = true;
	}

	// clean up
	zbar_image_destroy(image);

	if(!result)
		printf("No symbols found.\n");

	printf("Press zero, back or touch screen to exit\n");

	while (TRUE)
	{
		maWait(0);
		maGetEvent(&event);

		if (EVENT_TYPE_CLOSE == event.type)
		{
			// Exit while loop.
			break;
		}
		else if (EVENT_TYPE_KEY_PRESSED == event.type)
		{
			if (MAK_BACK == event.key || MAK_0 == event.key)
			{
				// Exit while loop.
				break;
			}
		}
		else if (EVENT_TYPE_POINTER_PRESSED == event.type)
		{
			break;
		}
	}

	return 0;
}
