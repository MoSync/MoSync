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
* \brief Class for defining the visual appearance of a widget.
* \author Patrick Broman and Niklas Nummelin
*/

#ifndef _SE_MSAB_MAUI_WIDGET_SKIN_H_
#define _SE_MSAB_MAUI_WIDGET_SKIN_H_

#include <ma.h>
#include <MAUtil/HashMap.h>
#include "Drawable.h"

namespace MAUI {

	using namespace MAUtil;

	/** 
	 * \brief A customizable skin for widgets.
	 * 
	 * A WidgetSkin is a class used to define the visual appearance
	 * of a widget. It is instantiated by passing an image , and two
	 * coordinates telling how to divide the image into 9 segments.
	 * These segments are then used as information of how to tile the
	 * skin to different widget sizes. The constructor also takes parameters
	 * telling if the image is transparent or not, used to
	 * optimize MAUI drawing.
	 **/
	class WidgetSkin : public Drawable {
	public:
		/**
		 * The default constructor.
		 */
		WidgetSkin();

		/**
		 * The WidgetSkin constructor takes one image
		 * and 2 coordinate pairs, defining the 9 segments of the image. 
		 * Finally it takes one bool that specifies if the image is 
		 * transparent or not (default true).
		 *
		 * \param image The nine-patch image.
		 * \param x1 X coordinate of top left patch corner.
		 * \param x2 X coordinate of bottom right patch corner.
		 * \param y1 Y coordinate of top left patch corner.
		 * \param y2 Y coordinate of bottom right patch corner.
		 * \param transparent Specifies if the image is 
		 * transparent or not (default is true).
		 */
		WidgetSkin(
			MAHandle image, 
			int x1, 
			int x2, 
			int y1, 
			int y2, 
			bool transparent=true);

		/** 
		 * Set the start x coordinate of the WidgetSkin rect. 
		 * \param x X coordinate of top left patch corner.
		 */
		void setStartX(int x);

		/** 
		 * Set the end x coordinate of the WidgetSkin rect. 
		 * \param x X coordinate of bottom right patch corner.
		 */
		void setEndX(int x);

		/** 
		 * Set the start y coordinate of the WidgetSkin rect. 
		 * \param y1 Y coordinate of top left patch corner.
		 */
		void setStartY(int y);

		/** 
		 * Set the end y coordinate of the WidgetSkin rect. 
		 * \param y2 Y coordinate of bottom right patch corner.
		 */
		void setEndY(int y);

		/** 
		 * Get the start x coordinate of the WidgetSkin rect. 
		 * \return X coordinate of top left patch corner.
		 */
		int	getStartX() const;

		/** 
		 * Get the start y coordinate of the WidgetSkin rect. 
		 * \return X coordinate of bottom right patch corner.
		 */
		int getStartY() const;

		/** 
		 * Get the end x coordinate of the WidgetSkin rect. 
		 * \return X coordinate of bottom right patch corner.
		 */
		int getEndX() const;

		/** 
		 * Get the end y coordinate of the WidgetSkin rect. 
		 * \return Y coordinate of bottom right patch corner.
		 */
		int getEndY() const;

		/** 
		 * Get the image used to display an unselected state.
		 * \return The nine-patch skin image.
		 */
		MAHandle getImage() const;

		/** 
		 * Set the image of the skin.
		 * \param image The nine-patch skin image.
		 */
		void setImage(MAHandle image);
		
		/**
		 * Draw the WidgetSkin. The upper-left corner
		 * is placed at 'x' and 'y' and the skin is automatically
		 * resized to 'width' and 'height. 'type' specifies which
		 * drawing state should be used when drawing the widget skin.
		 **/
		
		/**
		 * Draw the WidgetSkin. 
		 *
		 * The upper-left corner is placed at 'x' and 'y' and the skin 
		 * is automatically resized to 'width' and 'height.
		 *
		 * If room exists in the cache, put the image in the cache. 
		 * Next time the same WidgetSkin with the same dimensions is drawn, 
		 * the cached bitmap is used.
		 *
		 * \param x Left coordinate of skin.
		 * \param y Top coordinate of skin.
		 * \param width Width of skin.
		 * \param height Height of skin.
		 */
		void draw(int x, int y, int width, int height);
		
		/**
		 * Draws the widget skin, not taking the cache in acount.
		 *
		 * The upper-left corner is placed at 'x' and 'y' and the skin 
		 * is automatically resized to 'width' and 'height.
		 *
		 * \param x Left coordinate of skin.
		 * \param y Top coordinate of skin.
		 * \param width Width of skin.
		 * \param height Height of skin.
		 */
		void drawDirect(int x, int y, int width, int height);

		/**
		 * Draws the widget skin to a 32-bit argb buffer.
		 *
		 * The upper-left corner is placed at 'x' and 'y' and the skin 
		 * is automatically resized to 'width' and 'height.
		 *
		 * \param data 32-bit argb buffer where the skin is rendered.
		 * \param x Left coordinate of skin.
		 * \param y Top coordinate of skin.
		 * \param width Width of skin.
		 * \param height Height of skin.
		 */
		void drawToData(int* data, int x, int y, int width, int height);

		/**
		 * Calculate the number of tiles needed for the widget 
		 * skin rendered with the given width and height.
		 *
		 * \param width Width of rendered skin.
		 * \param height Height of rendered skin.
		 * \return Number of tiles needed to draw the skin.
		 */
		int calculateNumTiles(int width, int height);		

		/**
		 * Helper function that draws a region of image data to an 
		 * image resource.
		 *
		 * \param image Destination image.
		 * \param data Source image data.
		 * \param scanLength Length of a scan line.
		 * \param srcRect Source rectangle.
		 * \param dstPoint Destination coordinate.
		 */
		void drawRegion(
			MAHandle image, 
			int* data, 
			int scanLength, 
			const MARect* srcRect, 
			const MAPoint2d* dstPoint);
			
		/** 
		 * Get the image height.
		 * \return Height of skin image.
		 */
		int getImageHeight() const;

		/** 
		 * Get the image width.
		 * \return Width of skin image.
		 */
		int getImageWidth() const;

		/** 
		 * Returns whether the image is transparent or not.
		 * \return Transparency flag.
		 */
		bool isTransparent() const;

		/** 
		* \brief Key to cache element for skins.
		*/
		struct CacheKey {
		
			/**
			 * Default constructor.
			 */
			CacheKey() {
			}
			
			/**
			 * Constructor.
			 * \param widgetSkin The skin object.
			 * \param width Width of skin.
			 * \param height Height of skin.
			 */
			CacheKey(WidgetSkin* widgetSkin, int width, int height) 
				: skin(widgetSkin), w(width), h(height) {
			}
			
			/**
			 * Equivalence operator.
			 */
			bool operator==(const CacheKey& c) const {
				return (skin==c.skin && w==c.w && h==c.h);
			}
			
			/**
			 * Less than operator.
			 */
			bool operator<(const CacheKey& c) const {
				return (skin<c.skin && w<c.w && h<c.h);
			}
				
			/**
			 * The skin object.
			 */	
			WidgetSkin *skin;
			
			/**
			 * Width of skin.
			 */	
			int w;
			
			/**
			 * Height of skin.
			 */	
			int h;
		};		
	
		/** 
		* \brief Cache element for skins.
		*/
		struct CacheElement {
			/*
			// key
			int w, h;
			eType type;
			*/
			
			/**
			 * Cached image.
			 */
			MAHandle image;
			
			/**
			 * Timestamp.
			 */
			int lastUsed;
		};		
		
		/**
		 * Set the max cache size.
		 * \param c TODO: Old comment says "in pixels", what is this param?
		 */
		static void setMaxCacheSize(int c);
		
		/**
		 * Enable or disable the cache.
		 * \param enable Enable flag (true or false).
		 */
		static void setCacheEnabled(bool enable=true);

		/**
		 * Empty the cache.
		 */
		static void flushCache();
		
		/**
		 * TODO: Check that this comment is corrent.
		 * Make room in the cache by flusing elements until the
		 * requested number of pixels is available.
		 * \param numPixels The number of pixels requested.
		 */
		static void flushCacheUntilNewImageFits(int numPixels); 
		
		/**
		 * Store an object in the cache.
		 * \param key The key for the cached object.
		 * \param elem The object to be stored in the cache.
		 */
		static void addToCache(const CacheKey& key, const CacheElement& elem);
		
		/**
		 * TODO: Check that this comment is corrent.
		 * Get an image from the cache.
		 * \param key The key for the cached object.
		 * \return The cached image.
		 */
		static MAHandle getFromCache(const CacheKey& key);
		
	private:
		/**
		 * Recalculate the rectangles used by the skin.
		 */
		void rebuildRects();

		/**
		 * Max size of the cache. 
		 * TODO: Which unit is this?
		 */
		static int sMaxCacheSize;
		
		/**
		 * Flag that tells if the cache should be used.
		 */
		static bool sUseCache;
		
		// TODO: Remove this commented out line if it is not used.
		//Vector<CacheElement> cache;
		
		/**
		 * TODO: Document.
		 */
		static HashMap<CacheKey, CacheElement> sCache;

		/**
		 * TODO: Document.
		 */
		int mImageWidth;
		
		/**
		 * TODO: Document.
		 */
		int mImageHeight;
		
		/**
		 * TODO: Document.
		 */
		MAHandle mImage;
		
		/**
		 * TODO: Document.
		 */
		MARect mTopLeft;
		
		/**
		 * TODO: Document.
		 */
		MARect mTop;
		
		/**
		 * TODO: Document.
		 */
		MARect mTopRight;
		
		/**
		 * TODO: Document.
		 */
		MARect mLeft;
		
		/**
		 * TODO: Document.
		 */
		MARect mCenter;
		
		/**
		 * TODO: Document.
		 */
		MARect mRight;
		
		/**
		 * TODO: Document.
		 */
		MARect mBottomLeft;
		
		/**
		 * TODO: Document.
		 */
		MARect mBottom;
		
		/**
		 * TODO: Document.
		 */
		MARect mBottomRight;
		
		/**
		 * TODO: Document.
		 */
		int mStartX;
		
		/**
		 * TODO: Document.
		 */
		int mEndX;
		
		/**
		 * TODO: Document.
		 */
		int mStartY;
		
		/**
		 * TODO: Document.
		 */
		int mEndY;
		
		/**
		 * TODO: Document.
		 */
		bool mTransparent;

	};	
}

#endif
