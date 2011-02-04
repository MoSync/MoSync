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

/** 
* \file Scaler.cpp
* \brief Utility for scaling images by nearest neighbour or bilinearly.
* \author Niklas Nummelin
*/

#include "Scaler.h"
#include <ma.h>
#include <mavsprintf.h>

#define RED(x)			(((x)&0x00ff0000)>>16)
#define GREEN(x)		(((x)&0x0000ff00)>>8)
#define BLUE(x)			(((x)&0x000000ff))
#define ALPHA(x)		(((x)&0xff000000)>>24)
#define RGBA(r,g,b,a)	 ((((a)&0xff)<<24)| \
						 (((r)&0xff)<<16)| \
						 (((g)&0xff)<<8)| \
						 (((b)&0xff)));
namespace MAUI {

	/**
	 * Locally used helper function.
	 * TODO: Document.
	 */
	static void nearestNeighbour(
		int *dst, 
		int dwidth, 
		int dheight, 
		int dpitch, 
		int *src, 
		int swidth, 
		int sheight, 
		int spitch) 
	{
		int deltax = (swidth<<16)/dwidth;
		int deltay = (sheight<<16)/dheight;

		int x = 0;
		int u, v;
		v = 0;
		int *src_scan;
		
		while (dheight) {
			x = dwidth;
			u = 0;
			src_scan = &src[(v>>16)*spitch];

			while (x > 0)
			{
				 switch (x & 0x3)
				 {
				   case 0:
					*dst++ = src_scan[(u>>16)];
					u+=deltax;					
					*dst++ = src_scan[(u>>16)];
					u+=deltax;
					*dst++ = src_scan[(u>>16)];
					u+=deltax;
					*dst++ = src_scan[(u>>16)];
					u+=deltax;
					x-=4;
					break;

				   case 3:
					*dst++ = src_scan[(u>>16)];
					u+=deltax;
					*dst++ = src_scan[(u>>16)];
					u+=deltax;
					*dst++ = src_scan[(u>>16)];
					u+=deltax;
					x-=3;
					break;

				   case 2:
					*dst++ = src_scan[(u>>16)];
					u+=deltax;
					*dst++ = src_scan[(u>>16)];
					u+=deltax;
					x-=2;
					break;

				   case 1:
					*dst++ = src_scan[(u>>16)];
					u+=deltax;
					x-=1;
					break;

				 }
			}
			dst+=-dwidth+dpitch;
			dheight--;
			v+=deltay;
		}
	}

	/**
	 * Locally used helper function.
	 * TODO: Document.
	 */
	static void bilinearScale(
		int *dst, 
		int dwidth, 
		int dheight, 
		int dpitch, 
		int *src, 
		int swidth, 
		int sheight, 
		int spitch) 
	{
		int deltax = (swidth<<16)/dwidth;
		int deltay = (sheight<<16)/dheight;

		int x = 0;
		int u, v;
		v = 0;
		int *src_scan;
		
		while (dheight) {
			x = dwidth;
			u = 0;
			src_scan = &src[(v>>16)*spitch];
			while (x) {
				// get bilinear filtered value
		//		int frac_x = (u-(u&0xffff0000));
		//		int frac_y = (v-(v&0xffff0000));
				int frac_x = 0xffff-(u&0xffff);
				int frac_y = (v&0xffff);

				int pos = (u>>16);	

				int tl_r = RED(src_scan[pos]);
				int tl_g = GREEN(src_scan[pos]);
				int tl_b = BLUE(src_scan[pos]);
				int tl_a = ALPHA(src_scan[pos]);

				int bl_r = RED(src_scan[pos+spitch]);
				int bl_g = GREEN(src_scan[pos+spitch]);
				int bl_b = BLUE(src_scan[pos+spitch]);
				int bl_a = ALPHA(src_scan[pos+spitch]);

				// TODO: This code can be clean up, some parens can be removed.
				tl_r = ((tl_r)*frac_x 
					+ (RED(src_scan[pos+1]))*(0xffff-frac_x))>>16;
				tl_g = ((tl_g)*frac_x
					+ (GREEN(src_scan[pos+1]))*(0xffff-frac_x))>>16;
				tl_b = ((tl_b)*frac_x 
					+ (BLUE(src_scan[pos+1]))*(0xffff-frac_x))>>16;
				tl_a = ((tl_a)*frac_x 
					+ (ALPHA(src_scan[pos+1]))*(0xffff-frac_x))>>16;
				
				bl_r = ((bl_r)*frac_x 
					+ (RED(src_scan[pos+spitch+1]))*(0xffff-frac_x))>>16;
				bl_g = ((bl_g)*frac_x 
					+ (GREEN(src_scan[pos+spitch+1]))*(0xffff-frac_x))>>16;
				bl_b = ((bl_b)*frac_x 
					+ (BLUE(src_scan[pos+spitch+1]))*(0xffff-frac_x))>>16;
				bl_a = ((bl_a)*frac_x 
					+ (ALPHA(src_scan[pos+spitch+1]))*(0xffff-frac_x))>>16;

				//tl_r += (((RED(src_scan[pos+1])-tl_r)*frac_x)>>16);
				//tl_g += (((GREEN(src_scan[pos+1])-tl_g)*frac_x)>>16);
				//tl_b += (((BLUE(src_scan[pos+1])-tl_b)*frac_x)>>16);
				//tl_a += (((ALPHA(src_scan[pos+1])-tl_a)*frac_x)>>16);

				//bl_r += (((RED(src_scan[pos+spitch+1])-bl_r)*frac_x)>>16);
				//bl_g += (((GREEN(src_scan[pos+spitch+1])-bl_g)*frac_x)>>16);
				//bl_b += (((BLUE(src_scan[pos+spitch+1])-bl_b)*frac_x)>>16);
				//bl_a += (((ALPHA(src_scan[pos+spitch+1])-bl_a)*frac_x)>>16);

				*dst = RGBA(
					tl_r + (((bl_r-tl_r)*frac_y)>>16),
					tl_g + (((bl_g-tl_g)*frac_y)>>16),
					tl_b + (((bl_b-tl_b)*frac_y)>>16),
					tl_a + (((bl_a-tl_a)*frac_y)>>16)
					);

				u+=deltax;

				x--;
				dst++;
			}
			dst+=-dwidth+dpitch;
			dheight--;
			v+=deltay;
		}

	}

	MAExtent Scaler::getSize(int level) {
		return maGetImageSize(mPlaceholderStart + level);
	}

	Scaler::Scaler(
		MAHandle image, 
		const MARect *srcRect, 
		double minScale, 
		double maxScale, 
		int levels, 
		eScaleType scaleType)
		: mLevels(levels)
	{
		MARect tempRect;

		MAExtent imageDims = maGetImageSize(image);
		int imageWidth = EXTENT_X(imageDims);
		int imageHeight = EXTENT_Y(imageDims);
		int *imageData = new int[(imageWidth+1)*(imageHeight+1)];

		if(!srcRect) {
			tempRect.left = tempRect.top = 0;
			tempRect.width = imageWidth;
			tempRect.height = imageHeight;
			srcRect = &tempRect;
		}
		maGetImageData(image, imageData, srcRect, imageWidth+1);

		if(scaleType == ST_BILINEAR) {
			for(int i = 0; i < imageWidth; i++) {
				imageData[i+(imageWidth+1)*(imageHeight)] = 
					imageData[i+(imageWidth+1)*(imageHeight-1)];
			}
			for(int i = 0; i < imageHeight; i++) {
				imageData[(imageWidth)+(imageWidth+1)*(i)] = 
					imageData[(imageWidth-1)+(imageWidth+1)*(i)];
			}
		}

		int scaleDelta = 
			(int)(((maxScale - minScale)*65536.0)/(double)mLevels);
		int scale = (int)(minScale*65536.0);

		for(int i = 0; i < mLevels; i++) {
			MAHandle p = maCreatePlaceholder();
			if(i == 0) mPlaceholderStart = p;

			int scaledImageWidth = ((imageWidth*scale)>>16);
			int scaledImageHeight = ((imageHeight*scale)>>16);		
			int *scaledImageData = new int[scaledImageWidth*scaledImageHeight];
			switch(scaleType) {
				case ST_BILINEAR: 
					bilinearScale(
						scaledImageData, 
						scaledImageWidth, 
						scaledImageHeight, 
						scaledImageWidth, 
						imageData, 
						imageWidth, 
						imageHeight, 
						imageWidth+1);
					break;
				case ST_NEAREST_NEIGHBOUR:
					nearestNeighbour(
						scaledImageData, 
						scaledImageWidth, 
						scaledImageHeight, 
						scaledImageWidth, 
						imageData, 
						imageWidth, 
						imageHeight, 
						imageWidth+1);
					break;
			}

			maCreateImageRaw(
				p, 
				scaledImageData, 
				EXTENT(scaledImageWidth, scaledImageHeight), 
				1);
			delete scaledImageData;
			scale += scaleDelta;
		}
		delete imageData;
	}

	void Scaler::draw(int x, int y, int level) {
	
		if(level < 0) {
			level = 0;
		}
		if(level >= mLevels) {
			level = mLevels-1;
		}

		// Get size of image for this level.
		MAExtent size = maGetImageSize(mPlaceholderStart + level);
		
		// Draw image centred.
		maDrawImage(
			mPlaceholderStart + level, 
			x - (EXTENT_X(size)>>1), 
			y - (EXTENT_Y(size)>>1));
	}
}
