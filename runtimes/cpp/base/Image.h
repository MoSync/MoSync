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

#include "helpers/types.h"
typedef u32 uint32_t;
typedef s32 int32_t;

struct ClipRect {
	int x, y, width, height;
};

inline uint32_t rgb16_to_rgb32(int a, int redMask, int greenMask, int blueMask, int redShift, int greenShift, int blueShift, int redBits, int greenBits, int blueBits)
{
	uint32_t r = (a & redMask) >> redShift;
	uint32_t g = (a & greenMask) >> greenShift;
	uint32_t b = (a & blueMask) >> blueShift;
	r = r * 255 / ((1<<redBits)-1);
	g = g * 255 / ((1<<greenBits)-1);
	b = b * 255 / ((1<<blueBits)-1);
	return (r << 16) | (g << 8) | (b);
}


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
		PIXELFORMAT_ARGB8888
	};

	struct ImageInitParams {
		int width;
		int height;
		int pitch;
		int redMask;
		int greenMask;
		int blueMask;
		int alphaMask;
		int redShift;
		int greenShift;
		int blueShift;
		int alphaShift;
		int redBits;
		int greenBits;
		int blueBits;
		int alphaBits;
		int bytesPerPixel;
		int bitsPerPixel;
	};

	Image();
	Image(unsigned char *data, unsigned char *alpha, int width, int height, int pitch,
		PixelFormat pixelFormat, bool makeCopy=false, bool shouldFreeData=true, int alphaPitch=-1);

	Image(unsigned char *data, unsigned char *alpha, const ImageInitParams &params,
		bool makeCopy=false, bool shouldFreeData=true, int alphaPitch=-1);
	Image(int width, int height, int pitch, PixelFormat pixelFormat);

	~Image();

	bool hasData();
	bool hasAlpha();

	void init(unsigned char *data, unsigned char *alpha, bool makeCopy, int alphaPitch=-1);
	void calculateConstants();

	void drawPoint(int x, int y, int color);
	void drawLine(int x1, int y1, int x2, int y2, int color);
	void drawFilledRect(int x, int y, int w, int h, int color);
	void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int color);
	void drawImageRegion(int left, int top, ClipRect *srcRect, Image *src, int transformMode);
	void drawImage(int left, int top, Image *src);

	ClipRect		clipRect;

#ifdef SYMBIAN
	// must be set before use.
	unsigned char* mulTable;
#endif

	PixelFormat		pixelFormat;
	unsigned char	*data;
	unsigned char	*alpha;
	int				width;
	int				height;
	int				pitch;
	int alphaPitch;
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

#ifdef SYMBIAN
unsigned char* initMulTable();
#else
void initMulTable();
void initRecipLut();
#endif

#endif /* _IMAGE_ */
