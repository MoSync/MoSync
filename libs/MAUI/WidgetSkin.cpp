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

#include "WidgetSkin.h"
#include <MAUtil/Graphics.h>

namespace MAUI {

	WidgetSkin::WidgetSkin() :
		selectedImage(0),
		unselectedImage(0),
		startX(16),
		endX(32),
		startY(16),
		endY(32),
		selectedTransparent(true), 
		unselectedTransparent(true)
		{
			rebuildRects();
	}

	WidgetSkin::WidgetSkin(MAHandle selectedImage, MAHandle unselectedImage, int startX, int endX, int startY, int endY, bool selectedTransparent, bool unselectedTransparent) :
		selectedImage(0),
		unselectedImage(0),
		startX(startX),
		endX(endX),
		startY(startY),
		endY(endY),
		selectedTransparent(selectedTransparent), 
		unselectedTransparent(unselectedTransparent)
		{
			setSelectedImage(selectedImage);
			setUnselectedImage(unselectedImage);
			rebuildRects();
	}

	MAHandle WidgetSkin::getUnselectedImage() const {
		return unselectedImage;
	}

	MAHandle WidgetSkin::getSelectedImage() const {
		return selectedImage;
	}

	void WidgetSkin::setSelectedImage(MAHandle image) {
		this->selectedImage = image;
		if(!selectedImage) return;//maPanic(0, "WidgetSkin::setSelectedImage argument image==null");
		MAExtent imgSize = maGetImageSize(image);
		
		selectedImageWidth = EXTENT_X(imgSize);
		selectedImageHeight = EXTENT_Y(imgSize);

		if(unselectedImage) {
			if(unselectedImageHeight!=selectedImageHeight ||
				unselectedImageWidth!=selectedImageWidth) {
				selectedImageWidth = 0;
				selectedImageHeight = 0;
				selectedImage = 0;
				maPanic(0, "Not the same dimension on WidgetSkin selectedImage as unselectedImage");
			}
		} else {
			imageWidth = selectedImageWidth;
			imageHeight = selectedImageHeight;
		}

		rebuildRects();
	}

	void WidgetSkin::setUnselectedImage(MAHandle image) {
		this->unselectedImage = image;
		if(!unselectedImage) return;//maPanic(0, "WidgetSkin::setUnselectedImage argument image==null");
		MAExtent imgSize = maGetImageSize(image);
		
		unselectedImageWidth = EXTENT_X(imgSize);
		unselectedImageHeight = EXTENT_Y(imgSize);

		if(selectedImage) {
			if(unselectedImageHeight!=selectedImageHeight ||
				unselectedImageWidth!=selectedImageWidth) {
				unselectedImageWidth = 0;
				unselectedImageHeight = 0;
				unselectedImage = 0;
				maPanic(0, "Not the same dimension on WidgetSkin unselectedImage as selectedImage");
			}
		} else {
			imageWidth = unselectedImageWidth;
			imageHeight = unselectedImageHeight;
		}

		rebuildRects();	
	}

	void WidgetSkin::setStartX(int x) {
		startX = x;
		rebuildRects();
	}

	void WidgetSkin::setEndX(int x) {
		endX = x;
		rebuildRects();
	}

	void WidgetSkin::setStartY(int y) {
		startY = y;
		rebuildRects();
	}

	void WidgetSkin::setEndY(int y) {
		endY = y;
		rebuildRects();
	}

	void WidgetSkin::draw(int x, int y, int width, int height, eType type) {
		MAPoint2d dst;
		MAPoint2d dst2;
		MAHandle image;
		switch(type) {
			case SELECTED:
				image = selectedImage;
				break;
			case UNSELECTED:
				image = unselectedImage;
				break;
			default:
				maPanic(0, "WidgetSkin::draw undefined drawing type");
		}

		if(image == 0) return;

		// draw corners
		dst.x = x; dst.y = y;
		//maDrawImageRegion(image, &topLeft, &dst, TRANS_NONE);
		Gfx_drawImageRegion(image, &topLeft, &dst, TRANS_NONE);

		dst.x = x; dst.y = y+height-bottomLeft.height;
		//maDrawImageRegion(image, &bottomLeft, &dst, TRANS_NONE);
		Gfx_drawImageRegion(image, &bottomLeft, &dst, TRANS_NONE);

		dst.x = x+width-topRight.width; dst.y = y;
		//maDrawImageRegion(image, &topRight, &dst, TRANS_NONE);
		Gfx_drawImageRegion(image, &topRight, &dst, TRANS_NONE);

		dst.x = x+width-bottomRight.width; dst.y = y+height-bottomRight.height;
		//maDrawImageRegion(image, &bottomRight, &dst, TRANS_NONE);
		Gfx_drawImageRegion(image, &bottomRight, &dst, TRANS_NONE);

		// draw middle
		if(center.height && center.width) {
		for(int j = y+top.height; j < y+height-bottom.height; j+=center.height) {
			int h = center.height;
			if(j+center.height>y+height-bottom.height) {
				center.height -= (j+center.height)-(y+height-bottom.height);
			} 
			for(int i = x+left.width; i < x+width-right.width; i+=center.width) {
				dst.x = i; dst.y = j;
				int w = center.width;
				if(i+center.width>x+width-right.width) {
					center.width -= (i+center.width)-(x+width-right.width);
				}
				//maDrawImageRegion(image, &center, &dst, TRANS_NONE);
				Gfx_drawImageRegion(image, &center, &dst, TRANS_NONE);

				center.width = w;
			}
			center.height = h;
		}
		}

		// draw borders
		if(top.width) {
		for(int i = x+left.width; i < x+width-right.width; i+=top.width) {
				dst.x = i; dst.y = y;
				dst2.x = i; dst2.y = y+height-bottom.height;

				int w1 = top.width;
				int w2 = bottom.width;
				if(i+top.width>x+width-right.width) {
					top.width -= (i+w1)-(x+width-right.width);
					bottom.width -= (i+w1)-(x+width-right.width);
				} 
				//maDrawImageRegion(image, &top, &dst, TRANS_NONE);
				//maDrawImageRegion(image, &bottom, &dst2, TRANS_NONE);
				Gfx_drawImageRegion(image, &top, &dst, TRANS_NONE);
				Gfx_drawImageRegion(image, &bottom, &dst2, TRANS_NONE);

				top.width = w1;			
				bottom.width = w2;
		}
		}

		if(left.height) {
		for(int i = y+top.height; i < y+height-bottom.height; i+=left.height) {
				dst.x = x; dst.y = i;
				dst2.x = x+width-right.width; dst2.y = i;

				int w1 = left.height;
				int w2 = right.height;
				if(i+left.height>y+height-bottom.height) {
					left.height -= (i+w1)-(y+height-bottom.height);
					right.height -= (i+w1)-(y+height-bottom.height);
				} 
				//maDrawImageRegion(image, &left, &dst, TRANS_NONE);
				//maDrawImageRegion(image, &right, &dst2, TRANS_NONE);
				Gfx_drawImageRegion(image, &left, &dst, TRANS_NONE);
				Gfx_drawImageRegion(image, &right, &dst2, TRANS_NONE);

				left.height = w1;			
				right.height = w2;
		}
		}
	}

	int	WidgetSkin::getStartX() const {
		return startX;
	}

	int WidgetSkin::getStartY() const {
		return startY;
	}

	int WidgetSkin::getEndX() const {
		return endX;
	}

	int WidgetSkin::getEndY() const {
		return endY;
	}

	bool WidgetSkin::isSelectedTransparent() const {
		return selectedTransparent;
	}

	bool WidgetSkin::isUnselectedTransparent() const {
		return unselectedTransparent;
	}

	int WidgetSkin::getImageHeight() const {
		return imageHeight;
	}

	int WidgetSkin::getImageWidth() const {
		return imageWidth;
	}


#define SET_RECT(rect, x1, y1, x2, y2) {rect.left = (x1); rect.top = (y1); rect.width = (x2)-(x1); rect.height = (y2)-(y1);}

	void WidgetSkin::rebuildRects() {
#ifdef MOBILEAUTHOR
		int temp;
		if(startX>endX) {
			temp = startX;
			startX = endX;
			endX = temp;
		}
		if(startY>endY) {
			temp = startY;
			startY = endY;
			endY = temp;
		}
		if(startX<0) startX = 0;
		if(startY<0) startY = 0;
		if(endX<0) endX = 0;
		if(endY<0) endY = 0;
		if(startX>imageWidth) startX = imageWidth;
		if(endX>imageWidth) endX = imageWidth;
		if(startY>imageHeight) startY = imageHeight;
		if(endY>imageHeight) endY = imageHeight;
#else
		if(startX>endX)			maPanic(0, "WidgetSkin::rebuildRects startX>endX");
		if(startY>endY)			maPanic(0, "WidgetSkin::rebuildRects startY>endY");
		if(startX<0)			maPanic(0, "WidgetSkin::rebuildRects startX<0");
		if(startY<0)			maPanic(0, "WidgetSkin::rebuildRects startY<0");
		if(endX<0)				maPanic(0, "WidgetSkin::rebuildRects endX<0");
		if(endY<0)				maPanic(0, "WidgetSkin::rebuildRects endY<0");
		if(startX>imageWidth)	maPanic(0, "WidgetSkin::rebuildRects startX>imageWidth");
		if(endX>imageWidth)		maPanic(0, "WidgetSkin::rebuildRects endX>imageWidth");
		if(startY>imageHeight)	maPanic(0, "WidgetSkin::rebuildRects startY>imageHeight");
		if(endY>imageHeight)	maPanic(0, "WidgetSkin::rebuildRects endY>imageHeight");		
#endif		

		SET_RECT(topLeft,     0,      0,      startX,     startY);
		SET_RECT(top,         startX, 0,      endX,       startY);
		SET_RECT(topRight,    endX,   0,      imageWidth, startY);
		SET_RECT(left,	      0,      startY, startX,     endY);
		SET_RECT(center,      startX, startY, endX,       endY);
		SET_RECT(right,       endX,   startY, imageWidth, endY);
		SET_RECT(bottomLeft,  0,      endY,   startX,     imageHeight);
		SET_RECT(bottom,      startX, endY,   endX,       imageHeight);
		SET_RECT(bottomRight, endX,   endY,  imageWidth,  imageHeight);
	}
}
