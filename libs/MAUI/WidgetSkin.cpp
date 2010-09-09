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
		return THashFunction<int>(data.w | (data.h<<12)) - THashFunction<int>((int)data.skin);
	}	
}

namespace MAUI {

	WidgetSkin::WidgetSkin() :
		mImage(0),
		mStartX(16),
		mEndX(32),
		mStartY(16),
		mEndY(32),
		mTransparent(true)
		{
			rebuildRects();
	}

	WidgetSkin::WidgetSkin(MAHandle image, int startX, int endX, int startY, int endY, bool transparent) :
		mImage(0),
		mStartX(startX),
		mEndX(endX),
		mStartY(startY),
		mEndY(endY),
		mTransparent(transparent)
		{
			setImage(image);
			rebuildRects();
	}

	MAHandle WidgetSkin::getImage() const {
		return mImage;
	}

	void WidgetSkin::setImage(MAHandle image) {
		this->mImage = image;
		if(!mImage) return;
		MAExtent imgSize = maGetImageSize(image);
		
		mImageWidth = EXTENT_X(imgSize);
		mImageHeight = EXTENT_Y(imgSize);

		rebuildRects();
	}

	void WidgetSkin::setStartX(int x) {
		mStartX = x;
		rebuildRects();
	}

	void WidgetSkin::setEndX(int x) {
		mEndX = x;
		rebuildRects();
	}

	void WidgetSkin::setStartY(int y) {
		mStartY = y;
		rebuildRects();
	}

	void WidgetSkin::setEndY(int y) {
		mEndY = y;
		rebuildRects();
	}

	void WidgetSkin::drawRegion(MAHandle image, int* data, int scanLength, const MARect* srcRect, const MAPoint2d *dstPoint) {
		maGetImageData(image, &data[dstPoint->x+dstPoint->y*scanLength], srcRect, scanLength);
	}

	
	int WidgetSkin::calculateNumTiles(int width, int height) {
		int numTilesX = 0;
		int numTilesY = 0;
		if(mTop.width)
			numTilesX = (width-(mRight.width+mLeft.width))/mTop.width;
		
		if(mLeft.height)
			numTilesY =  (height-(mTop.height+mBottom.height))/mLeft.height;
		
		int numTiles = numTilesX*numTilesY;
		if(mLeft.width)
			numTiles += numTilesY;
		if(mRight.width)
			numTiles += numTilesY;
		if(mTop.height)
			numTiles += numTilesX;
		if(mBottom.height)
			numTiles += numTilesX;	
		return numTiles;
	}
	
	#define DEFAULT_CACHE_THRESHOLD (16*1024*1024)

	HashMap<WidgetSkin::CacheKey, WidgetSkin::CacheElement> WidgetSkin::sCache;
	int WidgetSkin::mMaxCacheSize = 	DEFAULT_CACHE_THRESHOLD;
	bool WidgetSkin::mUseCache = false;
	
	void WidgetSkin::setMaxCacheSize(int c) {
		mMaxCacheSize = c;
	}
	
	void WidgetSkin::setCacheEnabled(bool e) {
		mUseCache = e;
	}
	
		
	void WidgetSkin::flushCacheUntilNewImageFits(int numPixels) {
		int totalPixelsInCache = numPixels;
		
		HashMap<CacheKey, CacheElement>::Iterator iter = sCache.begin();
		while(iter != sCache.end()) {
			totalPixelsInCache += iter->first.w*iter->first.h;
			iter++;
		}

		int currentTime = maGetMilliSecondCount();
		
		while(totalPixelsInCache>mMaxCacheSize) {
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
		
	void WidgetSkin::draw(int x, int y, int width, int height) {
		MAHandle cached = 0;

		// Calculate numTiles needed to be drawn, if they are many, we need to cache, otherwise draw directly...
		int numTiles = calculateNumTiles(width, height);
		if(!mUseCache || numTiles<100) {
			drawDirect(x, y, width, height);
			return;
		}
		
		CacheKey newKey = CacheKey(this, width, height);
		cached =  getFromCache(newKey);
		
		// If we didn't find a cached widgetskin, let's generate one and save it in the cache.
		if(!cached) {
			// set malloc handler to null so that we can catch if we're out of heap and write directly to the screen then.
			malloc_handler mh = set_malloc_handler(NULL);
			int *data = new int[width*height];
			if(!data) {
				drawDirect(x, y, width, height);
				return;		
			}
			set_malloc_handler(mh);
			drawToData(data, 0, 0, width, height);
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

	void WidgetSkin::drawToData(int *data, int x, int y, int width, int height) {
		MAPoint2d dst;
		MAPoint2d dst2;
		MAHandle image = mImage;

		if(image == 0) return;

		// draw corners
		dst.x = x; dst.y = y;
		//maDrawImageRegion(image, &mTopLeft, &dst, TRANS_NONE);
		drawRegion(image, data, width, &mTopLeft, &dst);

		dst.x = x; dst.y = y+height-mBottomLeft.height;
		//maDrawImageRegion(image, &mBottomLeft, &dst, TRANS_NONE);
		drawRegion(image, data, width, &mBottomLeft, &dst);

		dst.x = x+width-mTopRight.width; dst.y = y;
		//maDrawImageRegion(image, &mTopRight, &dst, TRANS_NONE);
		drawRegion(image, data, width, &mTopRight, &dst);

		dst.x = x+width-mBottomRight.width; dst.y = y+height-mBottomRight.height;
		//maDrawImageRegion(image, &mBottomRight, &dst, TRANS_NONE);
		drawRegion(image, data, width, &mBottomRight, &dst);

		// draw middle
		if(mCenter.height && mCenter.width) {
		for(int j = y+mTop.height; j < y+height-mBottom.height; j+=mCenter.height) {
			int h = mCenter.height;
			if(j+mCenter.height>y+height-mBottom.height) {
				mCenter.height -= (j+mCenter.height)-(y+height-mBottom.height);
			} 
			for(int i = x+mLeft.width; i < x+width-mRight.width; i+=mCenter.width) {
				dst.x = i; dst.y = j;
				int w = mCenter.width;
				if(i+mCenter.width>x+width-mRight.width) {
					mCenter.width -= (i+mCenter.width)-(x+width-mRight.width);
				}
				//maDrawImageRegion(image, &mCenter, &dst, TRANS_NONE);
				drawRegion(image, data, width, &mCenter, &dst);

				mCenter.width = w;
			}
			mCenter.height = h;
		}
		}

		// draw borders
		if(mTop.width) {
		for(int i = x+mLeft.width; i < x+width-mRight.width; i+=mTop.width) {
				dst.x = i; dst.y = y;
				dst2.x = i; dst2.y = y+height-mBottom.height;

				int w1 = mTop.width;
				int w2 = mBottom.width;
				if(i+mTop.width>x+width-mRight.width) {
					mTop.width -= (i+w1)-(x+width-mRight.width);
					mBottom.width -= (i+w1)-(x+width-mRight.width);
				} 
				//maDrawImageRegion(image, &mTop, &dst, TRANS_NONE);
				//maDrawImageRegion(image, &mBottom, &dst2, TRANS_NONE);
				drawRegion(image, data, width, &mTop, &dst);
				drawRegion(image, data, width, &mBottom, &dst2);

				mTop.width = w1;
				mBottom.width = w2;
		}
		}

		if(mLeft.height) {
		for(int i = y+mTop.height; i < y+height-mBottom.height; i+=mLeft.height) {
				dst.x = x; dst.y = i;
				dst2.x = x+width-mRight.width; dst2.y = i;

				int w1 = mLeft.height;
				int w2 = mRight.height;
				if(i+mLeft.height>y+height-mBottom.height) {
					mLeft.height -= (i+w1)-(y+height-mBottom.height);
					mRight.height -= (i+w1)-(y+height-mBottom.height);
				} 
				//maDrawImageRegion(image, &mLeft, &dst, TRANS_NONE);
				//maDrawImageRegion(image, &mRight, &dst2, TRANS_NONE);
				drawRegion(image, data, width, &mLeft, &dst);
				drawRegion(image, data, width, &mRight, &dst2);

				mLeft.height = w1;
				mRight.height = w2;
		}
		}
	}

	void WidgetSkin::drawDirect(int x, int y, int width, int height) {
		MAPoint2d dst;
		MAPoint2d dst2;
		MAHandle image = mImage;
		if(image == 0) return;

		// draw corners
		dst.x = x; dst.y = y;
		//maDrawImageRegion(image, &mTopLeft, &dst, TRANS_NONE);
		Gfx_drawImageRegion(image, &mTopLeft, &dst, TRANS_NONE);

		dst.x = x; dst.y = y+height-mBottomLeft.height;
		//maDrawImageRegion(image, &mBottomLeft, &dst, TRANS_NONE);
		Gfx_drawImageRegion(image, &mBottomLeft, &dst, TRANS_NONE);

		dst.x = x+width-mTopRight.width; dst.y = y;
		//maDrawImageRegion(image, &mTopRight, &dst, TRANS_NONE);
		Gfx_drawImageRegion(image, &mTopRight, &dst, TRANS_NONE);

		dst.x = x+width-mBottomRight.width; dst.y = y+height-mBottomRight.height;
		//maDrawImageRegion(image, &mBottomRight, &dst, TRANS_NONE);
		Gfx_drawImageRegion(image, &mBottomRight, &dst, TRANS_NONE);

		// draw middle
		if(mCenter.height && mCenter.width) {
		for(int j = y+mTop.height; j < y+height-mBottom.height; j+=mCenter.height) {
			int h = mCenter.height;
			if(j+mCenter.height>y+height-mBottom.height) {
				mCenter.height -= (j+mCenter.height)-(y+height-mBottom.height);
			} 
			for(int i = x+mLeft.width; i < x+width-mRight.width; i+=mCenter.width) {
				dst.x = i; dst.y = j;
				int w = mCenter.width;
				if(i+mCenter.width>x+width-mRight.width) {
					mCenter.width -= (i+mCenter.width)-(x+width-mRight.width);
				}
				//maDrawImageRegion(image, &mCenter, &dst, TRANS_NONE);
				Gfx_drawImageRegion(image, &mCenter, &dst, TRANS_NONE);

				mCenter.width = w;
			}
			mCenter.height = h;
		}
		}

		// draw borders
		if(mTop.width) {
		for(int i = x+mLeft.width; i < x+width-mRight.width; i+=mTop.width) {
				dst.x = i; dst.y = y;
				dst2.x = i; dst2.y = y+height-mBottom.height;

				int w1 = mTop.width;
				int w2 = mBottom.width;
				if(i+mTop.width>x+width-mRight.width) {
					mTop.width -= (i+w1)-(x+width-mRight.width);
					mBottom.width -= (i+w1)-(x+width-mRight.width);
				} 
				//maDrawImageRegion(image, &mTop, &dst, TRANS_NONE);
				//maDrawImageRegion(image, &mBottom, &dst2, TRANS_NONE);
				Gfx_drawImageRegion(image, &mTop, &dst, TRANS_NONE);
				Gfx_drawImageRegion(image, &mBottom, &dst2, TRANS_NONE);

				mTop.width = w1;
				mBottom.width = w2;
		}
		}

		if(mLeft.height) {
		for(int i = y+mTop.height; i < y+height-mBottom.height; i+=mLeft.height) {
				dst.x = x; dst.y = i;
				dst2.x = x+width-mRight.width; dst2.y = i;

				int w1 = mLeft.height;
				int w2 = mRight.height;
				if(i+mLeft.height>y+height-mBottom.height) {
					mLeft.height -= (i+w1)-(y+height-mBottom.height);
					mRight.height -= (i+w1)-(y+height-mBottom.height);
				} 
				//maDrawImageRegion(image, &mLeft, &dst, TRANS_NONE);
				//maDrawImageRegion(image, &mRight, &dst2, TRANS_NONE);
				Gfx_drawImageRegion(image, &mLeft, &dst, TRANS_NONE);
				Gfx_drawImageRegion(image, &mRight, &dst2, TRANS_NONE);

				mLeft.height = w1;
				mRight.height = w2;
		}
		}
	}

	int	WidgetSkin::getStartX() const {
		return mStartX;
	}

	int WidgetSkin::getStartY() const {
		return mStartY;
	}

	int WidgetSkin::getEndX() const {
		return mEndX;
	}

	int WidgetSkin::getEndY() const {
		return mEndY;
	}

	bool WidgetSkin::isTransparent() const {
		return mTransparent;
	}

	int WidgetSkin::getImageHeight() const {
		return mImageHeight;
	}

	int WidgetSkin::getImageWidth() const {
		return mImageWidth;
	}


#define SET_RECT(rect, x1, y1, x2, y2) {rect.left = (x1); rect.top = (y1); rect.width = (x2)-(x1); rect.height = (y2)-(y1);}

	void WidgetSkin::rebuildRects() {
#ifdef MOBILEAUTHOR
		int temp;
		if(mStartX>mEndX) {
			temp = mStartX;
			mStartX = mEndX;
			mEndX = temp;
		}
		if(mStartY>mEndY) {
			temp = mStartY;
			mStartY = mEndY;
			mEndY = temp;
		}
		if(mStartX<0) mStartX = 0;
		if(mStartY<0) mStartY = 0;
		if(mEndX<0) mEndX = 0;
		if(mEndY<0) mEndY = 0;
		if(mStartX>mImageWidth) mStartX = mImageWidth;
		if(mEndX>mImageWidth) mEndX = mImageWidth;
		if(mStartY>mImageHeight) mStartY = mImageHeight;
		if(mEndY>mImageHeight) mEndY = mImageHeight;
#else
		if(mStartX>mEndX)			maPanic(0, "WidgetSkin::rebuildRects mStartX>mEndX");
		if(mStartY>mEndY)			maPanic(0, "WidgetSkin::rebuildRects mStartY>mEndY");
		if(mStartX<0)			maPanic(0, "WidgetSkin::rebuildRects mStartX<0");
		if(mStartY<0)			maPanic(0, "WidgetSkin::rebuildRects mStartY<0");
		if(mEndX<0)				maPanic(0, "WidgetSkin::rebuildRects mEndX<0");
		if(mEndY<0)				maPanic(0, "WidgetSkin::rebuildRects mEndY<0");
		if(mStartX>mImageWidth)	maPanic(0, "WidgetSkin::rebuildRects mStartX>mImageWidth");
		if(mEndX>mImageWidth)		maPanic(0, "WidgetSkin::rebuildRects mEndX>mImageWidth");
		if(mStartY>mImageHeight)	maPanic(0, "WidgetSkin::rebuildRects mStartY>mImageHeight");
		if(mEndY>mImageHeight)	maPanic(0, "WidgetSkin::rebuildRects mEndY>mImageHeight");
#endif		

		SET_RECT(mTopLeft,     0,      0,      mStartX,     mStartY);
		SET_RECT(mTop,         mStartX, 0,      mEndX,       mStartY);
		SET_RECT(mTopRight,    mEndX,   0,      mImageWidth, mStartY);
		SET_RECT(mLeft,	      0,      mStartY, mStartX,     mEndY);
		SET_RECT(mCenter,      mStartX, mStartY, mEndX,       mEndY);
		SET_RECT(mRight,       mEndX,   mStartY, mImageWidth, mEndY);
		SET_RECT(mBottomLeft,  0,      mEndY,   mStartX,     mImageHeight);
		SET_RECT(mBottom,      mStartX, mEndY,   mEndX,       mImageHeight);
		SET_RECT(mBottomRight, mEndX,   mEndY,  mImageWidth,  mImageHeight);
	}
}
