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
#include <MAUtil/PlaceholderPool.h>

namespace MAUtil {
	template<> hash_val_t THashFunction<MAUI::WidgetSkin::CacheKey>(const MAUI::WidgetSkin::CacheKey& data) {
		return THashFunction<int>(data.w | (data.h<<12) | (((int)data.type)<<24)) - THashFunction<int>((int)data.skin);
	}	
}

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
		if(!selectedImage) return;
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
		if(!unselectedImage) return;
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

	void WidgetSkin::drawRegion(MAHandle image, int* data, int scanLength, const MARect* srcRect, const MAPoint2d *dstPoint) {
		maGetImageData(image, &data[dstPoint->x+dstPoint->y*scanLength], srcRect, scanLength);
	}

	
	int WidgetSkin::calculateNumTiles(int width, int height) {
		int numTilesX = 0;
		int numTilesY = 0;
		if(top.width)
			numTilesX = (width-(right.width+left.width))/top.width; 
		
		if(left.height)
			numTilesY =  (height-(top.height+bottom.height))/left.height;
		
		int numTiles = numTilesX*numTilesY;
		if(left.width)
			numTiles += numTilesY;
		if(right.width)
			numTiles += numTilesY;
		if(top.height)
			numTiles += numTilesX;
		if(bottom.height)
			numTiles += numTilesX;	
		return numTiles;
	}
	
	#define DEFAULT_CACHE_THRESHOLD (16*1024*1024)

	HashMap<WidgetSkin::CacheKey, WidgetSkin::CacheElement> WidgetSkin::sCache;
	int WidgetSkin::maxCacheSize = 	DEFAULT_CACHE_THRESHOLD;
	bool WidgetSkin::useCache = false;
	
	void WidgetSkin::setMaxCacheSize(int c) {
		maxCacheSize = c;
	}
	
	void WidgetSkin::setCacheEnabled(bool e) {
		useCache = e;
	}
	
		
	void WidgetSkin::flushCacheUntilNewImageFits(int numPixels) {
		int totalPixelsInCache = numPixels;
		
		HashMap<CacheKey, CacheElement>::Iterator iter = sCache.begin();
		while(iter != sCache.end()) {
			totalPixelsInCache += iter->first.w*iter->first.h;
			iter++;
		}

		int currentTime = maGetMilliSecondCount();
		
		while(totalPixelsInCache>maxCacheSize) {
			int oldest = currentTime;
			iter = sCache.begin();	
			HashMap<CacheKey, CacheElement>::Iterator best = sCache.end();
			while(iter != sCache.end()) {
				if(iter->second.lastUsed<oldest) {
					oldest = iter->second.lastUsed;
					best = iter;
				}
				iter++;
			}
			if(best == sCache.end()) break;
			maDestroyObject(best->second.image);
			PlaceholderPool::put(best->second.image);
			sCache.erase(best);
			totalPixelsInCache-=iter->first.w*iter->first.h;
		}
	}
	
	void WidgetSkin::flushCache() {
		HashMap<CacheKey, CacheElement>::Iterator iter = sCache.begin();
		while(iter != sCache.end()) {
			maDestroyObject(iter->second.image);
			PlaceholderPool::put(iter->second.image);
			iter++;
		}
		sCache.clear();
	}
			
	void WidgetSkin::addToCache(const CacheKey& key, const CacheElement& elem) {
		sCache.insert(key, elem);
	}
	
	MAHandle WidgetSkin::getFromCache(const CacheKey& key) {
		HashMap<CacheKey, CacheElement>::Iterator s = sCache.find(key);
		if(s == sCache.end()) return 0;
		else return s->second.image;
		
	}
		
	void WidgetSkin::draw(int x, int y, int width, int height, eType type) {
		MAHandle cached = 0;

		// Calculate numTiles needed to be drawn, if they are many, we need to cache, otherwise draw directly...
		int numTiles = calculateNumTiles(width, height);
		if(!useCache || numTiles<100) {
			drawDirect(x, y, width, height, type);
			return;
		}
		
		CacheKey newKey = CacheKey(this, width, height, type);
		cached =  getFromCache(newKey);
		
		// If we didn't find a cached widgetskin, let's generate one and save it in the cache.
		if(!cached) {
			// set malloc handler to null so that we can catch if we're out of heap and write directly to the screen then.
			malloc_handler mh = set_malloc_handler(NULL);
			int *data = new int[width*height];
			if(!data) {
				drawDirect(x, y, width, height, type);
				return;		
			}
			set_malloc_handler(mh);
			drawToData(data, 0, 0, width, height, type);
			CacheElement cacheElem;

			flushCacheUntilNewImageFits(width*height);	
			
			cacheElem.image = PlaceholderPool::alloc();
			if(maCreateImageRaw(cacheElem.image,data,EXTENT(width,height),1)!=RES_OK) {
				maPanic(1, "Could not create raw image");
			}

			delete data;
			cacheElem.lastUsed = maGetMilliSecondCount();
			cached = cacheElem.image;
			addToCache(newKey, cacheElem);
		}
		
		// Draw the cached widgetskin.
		Gfx_drawImage(cached, x, y);
	}

	void WidgetSkin::drawToData(int *data, int x, int y, int width, int height, eType type) {
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
		drawRegion(image, data, width, &topLeft, &dst);

		dst.x = x; dst.y = y+height-bottomLeft.height;
		//maDrawImageRegion(image, &bottomLeft, &dst, TRANS_NONE);
		drawRegion(image, data, width, &bottomLeft, &dst);

		dst.x = x+width-topRight.width; dst.y = y;
		//maDrawImageRegion(image, &topRight, &dst, TRANS_NONE);
		drawRegion(image, data, width, &topRight, &dst);

		dst.x = x+width-bottomRight.width; dst.y = y+height-bottomRight.height;
		//maDrawImageRegion(image, &bottomRight, &dst, TRANS_NONE);
		drawRegion(image, data, width, &bottomRight, &dst);

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
				drawRegion(image, data, width, &center, &dst);

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
				drawRegion(image, data, width, &top, &dst);
				drawRegion(image, data, width, &bottom, &dst2);

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
				drawRegion(image, data, width, &left, &dst);
				drawRegion(image, data, width, &right, &dst2);

				left.height = w1;			
				right.height = w2;
		}
		}
	}

	void WidgetSkin::drawDirect(int x, int y, int width, int height, eType type) {
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
