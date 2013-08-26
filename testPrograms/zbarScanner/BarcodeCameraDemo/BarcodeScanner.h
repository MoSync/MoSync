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
