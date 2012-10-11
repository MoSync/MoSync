/*
 * BarcodeScanner.h
 *
 *  Created on: Feb 28, 2012
 *      Author: Anders
 */

#ifndef BARCODESCANNER_H_
#define BARCODESCANNER_H_

#include <ma.h>
#include <string>
#include <zbar.h>

class BarcodeScanner
{
public:
	BarcodeScanner();
	virtual ~BarcodeScanner();

	bool initiate();
	bool shutdown();

	bool uploadHandle(MAHandle image);
	bool uploadRGB888(int* img, int width, int height);

	bool getBarcode(char* barcodeType, char* barcode);

private:

	bool upload(int* img, int width, int height);
	const char* getBarcodeType(int type);

	zbar::zbar_image_t *mImage;

	unsigned char* mImgData;

	zbar::ImageScanner mImageScanner;

};


#endif /* BARCODESCANNER_H_ */
