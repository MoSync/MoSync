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
