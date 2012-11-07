/*
 * BarcodeScanner.cpp
 *
 *  Created on: Feb 28, 2012
 *      Author: Anders
 */

#include "BarcodeScanner.h"

#include <mavsprintf.h>

BarcodeScanner::BarcodeScanner()
{
	mImgData = NULL;
}

BarcodeScanner::~BarcodeScanner()
{

}

bool BarcodeScanner::initiate()
{
	// create a reader
	zbar::ImageScanner mImageScanner = zbar::zbar_image_scanner_create();

	// configure the reader
	zbar::zbar_image_scanner_set_config(mImageScanner, zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
}

bool BarcodeScanner::shutdown()
{

}

bool BarcodeScanner::uploadHandle(MAHandle image)
{
	if(mImageScanner == NULL)
		maPanic(0, "No Image Scanner");

	if(mImgData != NULL)
		maPanic(0, "Image already uploaded!");

	int imgSize = maGetImageSize(image);

	int imgW = EXTENT_X(imgSize);
	int imgH = EXTENT_Y(imgSize);

	int imgDataSize = imgW * imgH;
	int* imgData = (int*) malloc(imgDataSize * 4);

	MARect imgRect = {0, 0, imgW, imgH};

	maGetImageData(image, imgData, &imgRect, imgW);

	mImgData = (unsigned char*) malloc(imgDataSize);

	return upload(imgData, imgW, imgH);
}

bool BarcodeScanner::uploadRGB888(int* img, int width, int height)
{
	lprintfln("uploadRGB888 called\n");

	if(mImageScanner == NULL)
			maPanic(0, "No Image Scanner");

	lprintfln("We have an image scanner\n");

	if(mImgData != NULL)
			maPanic(0, "Image already uploaded!");

	lprintfln("We have an image\n");

	int imgDataSize = width * height;

	if(mImgData != NULL)
		maPanic(0, "WTF!");

	mImgData = (unsigned char*) malloc(imgDataSize);

	lprintfln("Allocated the internal image buffer ( w:%u h:%u s:%u )\n", width, height, imgDataSize);

	return upload(img, width, height);
}

bool BarcodeScanner::upload(int* img, int width, int height)
{
	lprintfln("upload called\n");

	unsigned int rf = (unsigned int)(0.21f * 255.0f);
	unsigned int gf = (unsigned int)(0.71f * 255.0f);
	unsigned int bf = (unsigned int)(0.07f * 255.0f);

	lprintfln("Converting from RGB888 to luminosity\n");

	int size = width*height;

	lprintfln("Width:%u Height:%u Size:%u", width, height, size);

	for(int i = 0; i < size; i++)
	{
		unsigned int rb = (((unsigned int)(img[i] & 0x00ff0000 >> 8) * rf) >> 16);
		unsigned int gb = (((((unsigned int)(img[i] & 0x0000ff00))) * gf) >> 16);
		unsigned int bb = (((((unsigned int)(img[i] & 0x000000ff)) << 8) * bf) >> 16);

		int luminosity = (int)(rb + gb + bb);
		if(luminosity > 255)
			luminosity = 255;

		mImgData[i] = (unsigned char)luminosity;
	}

	lprintfln("conversion done!\n");

	lprintfln("Creates the zbar Image\n");

	// wrap image data
	mImage = zbar::zbar_image_create();

	if(mImage == NULL)
		maPanic(0, "zbar image wasn't created!");

	lprintfln("Set zbar image format\n");

	zbar::zbar_image_set_format(mImage, 0x30303859);// "Y800" = 0x30303859

	lprintfln("Set zbar size\n");

	zbar::zbar_image_set_size(mImage, width, height);

	lprintfln("upload data to zbar\n");

	zbar::zbar_image_set_data(mImage, mImgData, width * height, NULL);//zbar_image_free_data);

}

bool BarcodeScanner::getBarcode(char* barcodeType, char* barcode)
{
	if(mImageScanner == NULL)
		maPanic(0, "No Image Scanner");
	if(mImage == NULL)
		maPanic(0, "No Image Data!");

	// scan the image for barcodes
	int n = zbar_scan_image(mImageScanner, mImage);

	// extract results
	const zbar::zbar_symbol_t *symbol = zbar_image_first_symbol(mImage);

	bool result = false;
	for(; symbol; symbol = zbar_symbol_next(symbol))
	{
		// do something useful with results
		zbar::zbar_symbol_type_t typ = zbar_symbol_get_type(symbol);

		//sprintf(barcode, "%s\n%s",
		//		getBarcodeType((int)zbar_symbol_get_type(symbol)),
		//		zbar_symbol_get_data(symbol) );

		const char *type = getBarcodeType((int)typ);
		const char *data = zbar_symbol_get_data(symbol);

		if((type == NULL) || (data == NULL))
			break;

		result = true;

		sprintf(barcodeType, "%s", type);
		sprintf(barcode, "%s", data);

		//printf("decoded %s symbol \"%s\"\n",
		//	   zbar_get_symbol_name(typ), data);

	}

	// clean up
	zbar_image_destroy(mImage);
	free(mImgData);

	mImage = NULL;
	mImgData = NULL;

	return result;
}

const char* BarcodeScanner::getBarcodeType(int type)
{
	switch(type)
	{
	case zbar::ZBAR_PARTIAL:
		return "Partial barcode";
	case zbar::ZBAR_EAN8:
		return "EAN-8";
	case zbar::ZBAR_UPCE:
		return "UPC-E";
	case zbar::ZBAR_ISBN10:
		return "ISBN-10 (EAN-13)";
	case zbar::ZBAR_UPCA:
		return "UPC-A";
	case zbar::ZBAR_EAN13:
		return "EAN-13";
	case zbar::ZBAR_ISBN13:
		return "ISBN-13 (EAN-13)";
	case zbar::ZBAR_I25:
		return "Interleaved 2 of 5";
	case zbar::ZBAR_CODE39:
			return "Code 39";
	case zbar::ZBAR_PDF417:
			return "PDF417";
	case zbar::ZBAR_QRCODE:
			return "QR Code";
	}
	return "Unknown";

}
