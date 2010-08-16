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
* \file WidgetSkin.h 
* \brief Class for defining the visual appearance of a widget
* \author Patrick Broman and Niklas Nummelin
*/

#ifndef _SE_MSAB_MAUI_WIDGET_SKIN_H_
#define _SE_MSAB_MAUI_WIDGET_SKIN_H_

#include <ma.h>

//#include <MAUtil/Vector.h>
#include <MAUtil/HashMap.h>

using namespace MAUtil;

namespace MAUI {

	/** \brief A customizable skin for widgets.
	 * 
	 * A WidgetSkin is a class used to define the visual appearance
	 * of a widget. It is instantiated by passing an image for the
	 * selected and unselected mode, and two coordinates telling how 
	 * to divide the image into 9 segments. These segments are then 
	 * used as information of how to tile the skin to different widget 
	 * sizes. The constructor also takes parameters telling if the 
	 * selected or unselected images are transparent or not, used to 
	 * optimize MAUI drawing.
	 **/
	class WidgetSkin {
	public:

		/**
		 * Enumerator for the different drawing states of a WidgetSkin.
		 **/
		enum eType {
			SELECTED = 0,
			UNSELECTED = 1
		};

		/**
		 * The default constructor.
		 */
		WidgetSkin();

		/**
		 * The WidgetSkin constructor takes two images (selected and unselected) 
		 * and 2 coordinate pairs, defining the 9 segments of the images. Finally
		 * it takes two bools that specifies if the images are transparent or not
		 * (default true).
		 **/
		WidgetSkin(MAHandle selectedImage, MAHandle unselectedImage, int x1, int x2, int y1, int y2, bool selectedTransparent=true, bool unselectedTransparent=true);

		/** 
		 * Set the start x coordinate of the WidgetSkin rect. 
		 **/
		void setStartX(int x);

		/** 
		 * Set the end x coordinate of the WidgetSkin rect. 
		 **/
		void setEndX(int x);

		/** 
		 * Set the start y coordinate of the WidgetSkin rect. 
		 **/
		void setStartY(int y);

		/** 
		 * Set the end y coordinate of the WidgetSkin rect. 
		 **/
		void setEndY(int y);

		/** 
		 * Get the start x coordinate of the WidgetSkin rect. 
		 **/
		int	getStartX() const;

		/** 
		 * Get the start y coordinate of the WidgetSkin rect. 
		 **/
		int getStartY() const;

		/** 
		 * Get the end x coordinate of the WidgetSkin rect. 
		 **/
		int getEndX() const;

		/** 
		 * Get the end y coordinate of the WidgetSkin rect. 
		 **/
		int getEndY() const;

		/** 
		 * Get the image used to display an unselected state. 
		 **/
		MAHandle getUnselectedImage() const;

		/** 
		 * Get the image used to display a selected state. 
		 **/
		MAHandle getSelectedImage() const;

		/** 
		 * Set the selected image, returns false if there is an 
		 * unselected image and the dimensions of the selected image
		 * does not equal the unselected image.
		 **/
		void setSelectedImage(MAHandle image);

		/** 
		 * Set the unselected image, returns false if there is a
		 * selected image and the dimensions of the unselected image
		 * does not equal the selected image.
		 **/
		void setUnselectedImage(MAHandle image);

		/**
		 * Use this to draw the WidgetSkin. The upper-left corner
		 * is placed at 'x' and 'y' and the skin is automatically
		 * resized to 'width' and 'height. 'type' specifies which
		 * drawing state should be used when drawing the widget skin.
		 **/
		
		// uses a cache.
		void draw(int x, int y, int width, int height, eType type);
		
		void drawDirect(int x, int y, int width, int height, eType type);
		void drawToData(int *data, int x, int y, int width, int height, eType type);
		int calculateNumTiles(int width, int height);		
		void drawRegion(MAHandle image, int* data, int scanLength, const MARect* srcRect, const MAPoint2d *dstPoint);
			
		/** 
		 * Get the image height.
		 **/
		int getImageHeight() const;

		/** 
		 * Get the image width.
		 **/
		int getImageWidth() const;

		/** 
		 * Returns whether the selected image is transparent or not.
		 **/
		bool isSelectedTransparent() const;

		/** 
		 * Returns whether the unselected image is transparent or not.
		 **/
		bool isUnselectedTransparent() const;
/** 
* \brief Key to cache element for skins
*/
		struct CacheKey {
			CacheKey() {
			}
			CacheKey(WidgetSkin* s, int width, int height, eType atype) : skin(s), w(width), h(height), type(atype) {
			}
			
			bool operator==(const CacheKey& c) const {
				return (skin==c.skin && w==c.w && h==c.h && type==c.type);
			}
			
			bool operator<(const CacheKey& c) const {
				return (skin<c.skin && w<c.w && h<c.h && type<c.type);
			}
					
			WidgetSkin *skin;
			int w, h;
			eType type;	
		};		
	
/** 
* \brief Cache element for skins
*/

		struct CacheElement {
			/*
			// key
			int w, h;
			eType type;
			*/
			
			// value
			MAHandle image;
			
			// timestamp
			int lastUsed;
		};		
		
		// in pixels.
		static void setMaxCacheSize(int c);
		static void setCacheEnabled(bool e=true);

		static void flushCache();		
		static void flushCacheUntilNewImageFits(int numPixels); 
		static void addToCache(const CacheKey& key, const CacheElement& elem);
		static MAHandle getFromCache(const CacheKey& key);
		
	private:
		static int maxCacheSize;
		static bool useCache;
		
		//Vector<CacheElement> cache;
		static HashMap<CacheKey, CacheElement> sCache;


			
		void rebuildRects();

		int selectedImageWidth;
		int selectedImageHeight;
		int unselectedImageWidth;
		int unselectedImageHeight;
		MAHandle selectedImage;
		MAHandle unselectedImage;

		MARect topLeft;
		MARect top;
		MARect topRight;
		MARect left;
		MARect center;
		MARect right;
		MARect bottomLeft;
		MARect bottom;
		MARect bottomRight;
		int startX, endX;
		int startY, endY;
		int imageWidth, imageHeight;

		bool selectedTransparent, 
			unselectedTransparent;

	};	
}

#endif
