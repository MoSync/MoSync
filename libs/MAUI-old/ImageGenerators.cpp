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

#include "ImageGenerators.h"

#include <conprint.h>

namespace MAUI {

#define EPSILON 1
	void ImageGenerators::circularGradient(MAHandle image, Point origo, int radius, int origoColor, int circleColor, ImageGenerators::AlphaMode alphaMode) {
		MAExtent size = maGetImageSize(image);
		int imgWidth = EXTENT_X(size);
		int imgHeight = EXTENT_Y(size);
		int *tempSurface = new int[imgWidth*imgHeight];
		if(!tempSurface) {
			maPanic(0, "ImageGenerators::circularGradient, OH NO! NO MEMORY!!!!!!!!!!!!!!!!!!!!!!!!");
		}

		int *dst = tempSurface;

		int startAlpha =	(origoColor&0xff000000)>>24;
		int startRed =		(origoColor&0x00ff0000)>>16;
		int startGreen =	(origoColor&0x0000ff00)>>8;
		int startBlue =		(origoColor&0x000000ff);

		int endAlpha =		(circleColor&0xff000000)>>24;
		int endRed =		(circleColor&0x00ff0000)>>16;
		int endGreen =		(circleColor&0x0000ff00)>>8;
		int endBlue =		(circleColor&0x000000ff);

		double radiusRecip = 1.0/(double)radius;

		MAHandle lastDrawTarget = maSetDrawTarget(image);

		for(int j = 0; j < imgHeight; j++) {
			for(int i = 0; i < imgWidth; i++) {
				double len = sqrt((double)(origo.x-i)*(origo.x-i)+(origo.y-j)*(origo.y-j));
				if(len < radius) {
					int r = (int)(((len*(double)endRed) + ((radius-len)*(double)startRed))*radiusRecip);
					int g = (int)(((len*(double)endGreen) + ((radius-len)*(double)startGreen))*radiusRecip);
					int b = (int)(((len*(double)endBlue) + ((radius-len)*(double)startBlue))*radiusRecip);
					int a = (int)(((len*(double)endAlpha) + ((radius-len)*(double)startAlpha))*radiusRecip);
					if(r<0)		r = 0;
					if(r>255)	r = 255;
					if(g<0)		g = 0;
					if(g>255)	g = 255;
					if(b<0)		b = 0;
					if(b>255)	b = 255;
					if(a<0)		a = 0;
					if(a>255)	a = 255;
					*dst++ = (a<<24)|(r<<16)|(g<<8)|(b);
				} else {
					*dst++ = circleColor;
				}
			}
		}

		if(alphaMode == AM_WRITEALPHA) {
			MAPoint2d dstPoint = {0, 0};
			MARect srcRect = {0, 0, imgWidth, imgHeight};

			maDrawRGB(&dstPoint, tempSurface, &srcRect, imgWidth);
		} else if(alphaMode == AM_USEALPHA) {

		} else if(alphaMode == AM_NOALPHA) {

		}
		
		maSetDrawTarget(lastDrawTarget);
	}

	void ImageGenerators::linearGradient(MAHandle image, Point start, Point end, int startColor,
		int endColor, ImageGenerators::AlphaMode alphaMode)
	{

		MAExtent size = maGetImageSize(image);
		int imgWidth = EXTENT_X(size);
		int imgHeight = EXTENT_Y(size);

		int gradVecX, gradVecY, 
			gradOrthoVecX, gradOrthoVecY;

		gradVecX = end.x - start.x;
		gradVecY = end.y - start.y;
		gradOrthoVecX = gradVecY;
		gradOrthoVecY = -gradVecX;

		int startAlpha =	(startColor&0xff000000)>>24;
		int startRed =		(startColor&0x00ff0000)>>16;
		int startGreen =	(startColor&0x0000ff00)>>8;
		int startBlue =		(startColor&0x000000ff);

		int endAlpha =		(endColor&0xff000000)>>24;
		int endRed =		(endColor&0x00ff0000)>>16;
		int endGreen =		(endColor&0x0000ff00)>>8;
		int endBlue =		(endColor&0x000000ff);

		int *tempSurface = new int[imgWidth*imgHeight];

		if(!tempSurface) {
			maPanic(0, "ImageGenerators::linearGradient, OH NO! NO MEMORY!!!!!!!!!!!!!!!!!!!!!!!!");
		}

		int *dst = tempSurface;

		MAHandle lastDrawTarget = maSetDrawTarget(image);

		int sqrLen = gradOrthoVecX*gradOrthoVecX + gradOrthoVecY*gradOrthoVecY;
		double sqrtLen = (sqrt((double)sqrLen));
		double sqrtLenRecip = 1.0/sqrtLen;
		//lprintfln("imgWidth: %d, imgHeight: %d\n", imgWidth, imgHeight);
		for(int j = 0; j < imgHeight; j++) {
			for(int i = 0; i < imgWidth; i++) {

				int u = (((i - start.x)*gradOrthoVecX + 
					(j - start.y)*gradOrthoVecY)<<16) / sqrLen;

				int projectedPointX = ((u*gradOrthoVecX)>>16);
				int projectedPointY = ((u*gradOrthoVecY)>>16);

				//lprintfln("we come here! %d\n", i + j*imgWidth);

				double distToStart = 
					(sqrt((double)
					(((start.x+projectedPointX)-i)*((start.x+projectedPointX)-i) +
					((start.y+projectedPointY)-j)*((start.y+projectedPointY)-j))
					));

				double distToEnd = (sqrt((double)
					(((end.x+projectedPointX)-i)*((end.x+projectedPointX)-i) +
					((end.y+projectedPointY)-j)*((end.y+projectedPointY)-j))
					)); 
				//double distToStart = 0.0;
				//double distToEnd = 0.0;

				//	double distToEnd = sqrtLen - distToStart;

				if(distToEnd >= sqrtLen) {
					*dst++  = startColor;
				} else if(distToStart >= sqrtLen) {
					*dst++ = endColor;
				} else {
					int r = (int)(((distToStart*(double)endRed) + (distToEnd*(double)startRed))*sqrtLenRecip);
					int g = (int)(((distToStart*(double)endGreen) + (distToEnd*(double)startGreen))*sqrtLenRecip);
					int b = (int)(((distToStart*(double)endBlue) + (distToEnd*(double)startBlue))*sqrtLenRecip);
					int a = (int)(((distToStart*(double)endAlpha) + (distToEnd*(double)startAlpha))*sqrtLenRecip);
					if(r<0)		r = 0;
					if(r>255)	r = 255;
					if(g<0)		g = 0;
					if(g>255)	g = 255;
					if(b<0)		b = 0;
					if(b>255)	b = 255;
					if(a<0)		a = 0;
					if(a>255)	a = 255;

					*dst++ = (a<<24)|(r<<16)|(g<<8)|(b);
				}
			} 
		}

		if(alphaMode == AM_WRITEALPHA) {
			MAPoint2d dstPoint = {0, 0};
			MARect srcRect = {0, 0, imgWidth, imgHeight};
			maDrawRGB(&dstPoint, tempSurface, &srcRect, imgWidth);
			maSetDrawTarget(lastDrawTarget);
		} else if(alphaMode == AM_USEALPHA) {

		} else if(alphaMode == AM_NOALPHA) {

		}

		delete []tempSurface;
	}

}
