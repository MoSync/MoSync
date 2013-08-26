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

#ifndef _IMAGE_H_
#define _IMAGE_H_

struct Rect {
	int x, y, width, height;
};

/* should make it refcounted. */
class Image {
private:
	bool clipPolygon();
	void clipPolygonTop(int src, int dst);
	void clipPolygonLeft(int src, int dst);
	void clipPolygonRight(int src, int dst);
	void clipPolygonBottom(int src, int dst);
	void drawTriangleWithoutClipping(int x1, int y1, int x2, int y2, int x3, int y3, int color);

public:
	enum PixelFormat {
		PIXELFORMAT_RGB555,
		PIXELFORMAT_RGB444,
		PIXELFORMAT_RGB565,
		PIXELFORMAT_RGB888,
		PIXELFORMAT_ARGB8888,
	};

	Image(int width, int height, int pitch, PixelFormat pixelFormat);	
	Image(unsigned char *data, unsigned char *alpha, int width, int height, int pitch, PixelFormat pixelFormat, bool makeCopy=false, bool shouldFreeData=true);
	~Image();

	void drawPoint(int x, int y, int color);
	void drawLine(int x1, int y1, int x2, int y2, int color);
	void drawFilledRect(int x, int y, int w, int h, int color);
	void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int color);
	void drawImageRegion(int left, int top, Rect *srcRect, Image *src, int transformMode);
	void drawImage(int left, int top, Image *src);

	Rect			clipRect;

	PixelFormat		pixelFormat;
	unsigned char	*data;
	unsigned char	*alpha;
	int				width;
	int				height;
	int				pitch;
	int				bytesPerPixel;
	int				bitsPerPixel;

	unsigned int	alphaMask;
	unsigned int	alphaShift;
	unsigned int	alphaBits;
	unsigned int	redMask;
	unsigned int	redShift;
	unsigned int	redBits;
	unsigned int	greenMask;
	unsigned int	greenShift;
	unsigned int	greenBits;
	unsigned int	blueMask;
	unsigned int	blueShift;
	unsigned int	blueBits;

	bool shouldFreeData;
};

void initMulTable();
void initRecipLut();

#endif /* _IMAGE_ */
