/* Copyright (C) 2010 Mobile Sorcery AB

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
* \file LayerMapViewport.h
* \brief Viewport displaying a slippy map with layers overlaid
* \author Lars-Åke Vinberg
*/

#ifndef LAYERMAPVIEWPORT_H_
#define LAYERMAPVIEWPORT_H_

//#include <ma.h>
//#include <MAUI/Font.h>

//#include "LonLat.h"
//#include "MapCache.h"
#include "MapViewport.h"
#include "GeoPoint.h"
#include "Broadcaster.h"
#include "Layer.h"

using namespace MAUI;

namespace MAP
{
	//class MapTile;
	//class MapSource;
	//class MapCache;
	//class MapViewport;
	//class MapViewportPanTimerListener;

	////=========================================================================
	///**
	// * \brief Scroll direction for use in MapViewport::scroll( )
	// */
	//enum MapViewportScrollDirection
	////=========================================================================
	//{
	//	SCROLLDIRECTION_NORTH,
	//	SCROLLDIRECTION_SOUTH,
	//	SCROLLDIRECTION_EAST,
	//	SCROLLDIRECTION_WEST
	//};

	////=========================================================================
	///**
	// * \brief Pan mode for use in MapViewport::scroll( )
	// */
	//enum MapViewportPanMode
	////=========================================================================
	//{
	//	MapViewportPanMode_Instant,
	//	MapViewportPanMode_Smooth,
	//	MapViewportPanMode_Momentum
	//};

	////=========================================================================
	///**
	// * \brief Listener class, for listening client to implement.
	// */
	//class IMapViewportListener
	////=========================================================================
	//{
	//public:
	//	/**
	//	 * Called when a requested tile has been received into cache from map source.
	//	 */
	//	virtual void viewportUpdated( MapViewport* viewport ) = 0;
	//};

	class LayerMapViewport;
	class Layer;

	//=========================================================================
	class ILayerMapViewportListener
	//=========================================================================
	{
	public:
		virtual void layerItemActivated( LayerMapViewport* sender, Layer* layer, GeoPoint* item ) = 0;
		virtual void layerItemDeactivated( LayerMapViewport* sender, Layer* layer, GeoPoint* item ) = 0;
	};

	//=========================================================================
	/**
	 * \brief Simple slippy map viewport.
	 */
	class LayerMapViewport : 
		public MapViewport,
		public Broadcaster<ILayerMapViewportListener>,
		ILayerListener
	//=========================================================================
	{
		//friend class MapViewportPanTimerListener;

	public:
		LayerMapViewport( );
		
		virtual ~LayerMapViewport( );
		//
		// Layer handling
		//
		Vector<Layer*>& getLayers( ) { return mLayers; }

		void addLayer( Layer* layer );
		
		void removeLayer( Layer* layer );
		
		void clearLayers( );
		
		void selectNextLayer( );
		void selectLayer( int layer );
		//
		// Layer item selection and activation
		//
		void selectItem( int index );
		void selectNextItem( );
		void selectPreviousItem( );
		void activateSelectedItem( );
		void centerAndScaleToLayer( );
		//
		// MapWidget overrides
		//
		virtual bool handleKeyPress( int keyCode );
		virtual bool handleKeyRelease( int keyCode );
		//
		// ILayerListener implementation
		//
		virtual void contentChanged( Layer* sender );
		/**
		 * Map update scope
		 */
		//void updateMap( );
		//void setListener( IMapViewportListener* listener ) { mListener = listener; }
		/**
		 * Map source property
		 */
		//MapSource* getMapSource( ) const { return mSource; }
		//void setMapSource( MapSource* source );
		/**
		 * Center position property
		 */
		//LonLat getCenterPosition( ) const;
		//void setCenterPosition( LonLat position, bool immediate, bool isPointerEvent );
		//void setCenterPosition( LonLat position, int magnification, bool immediate, bool isPointerEvent );
		//PixelCoordinate getCenterPositionPixels( ) const;
		/**
		 * Magnification property
		 */
		//int getMagnification( ) const;
		//void setMagnification( int magnification );
		/**
		 * Magnification scale display property
		 */
		//bool getHasScale( ) const { return mHasScale; }
		//void setHasScale( bool hasScale ) { mHasScale = hasScale; }
		/**
		 * Sets panning mode
		 */
		//MapViewportPanMode getPanMode( ) const;
		//void setPanMode( MapViewportPanMode panMode );
		/**
		 * sets friction for momentum-based pan mode
		 */
		//float getFriction( ) const;
		//void setFriction( float friction );

		//void startGlide( );
		//void stopGlide( );

		/**
		 * Returns currently used font.
		 */
		//Font* getFont( ) const { return mFont; }
		/**
		 * Sets font property.
		 */ 
		//void setFont( Font* font ) { mFont = font; }
		/**
		 * Scrolls the map in the specified direction.
		 * 
		 */
		//void scroll( MapViewportScrollDirection direction, bool largeStep);
		/**
		 * Increases magnification by 1 step, i.e. a factor of two.
		 */
		//void zoomIn( );
		//void zoomOut( );
		//
		// Viewport size
		//
		//void setWidth( int width );
		//void setHeight( int height );
		/**
		 * Handles key press.
		 * Returns true if handled.
		 */
		//virtual bool handleKeyPress( int keyCode );
		/**
		 * Handles key release.
		 * Returns true if handled.
		 */
		//virtual bool handleKeyRelease( int keyCode );
		//
		// IMapCacheListener implementation
		//
		//virtual void tileReceived( MapCache* sender, MapTile* tile );
		/**
		 * Converts from global map pixels to viewport pixels.
		 */
		//MAPoint2d worldPixelToViewport( PixelCoordinate wpx );
		/**
		 * Converts from viewport pixels to global map pixels.
		 */
		//PixelCoordinate viewportToWorldPixel( MAPoint2d pt );

		//
		// For debugging, remove when done
		//
		//void testMomentumPanning( );
		//void stressTest( );
		//int getWidth( ) const { return mWidth; }
		//int getHeight( ) const { return mHeight; }
		//virtual void drawViewport( Point origin );

	protected:
		//
		// Redraw
		//
		//virtual void drawOverlay( );
	protected:
		//
		// Redraw
		//
		//void drawItem( Layer* layer, GeoPoint* item, bool selected, bool drawText );
		virtual void drawOverlay( Rect& bounds, MagnificationType magnification );
		void panSelectionIntoView( );

	private:
		//bool scrolltoNearest( int keyCode );

		//int mWidth;
		//int mHeight;
		//LonLat mCenterPositionLonLat;
		//PixelCoordinate mCenterPositionPixels;
		//LonLat mPanTargetPositionLonLat;
		//PixelCoordinate mPanTargetPositionPixels;
		//int mMagnification;
		//MapSource* mSource;
		//bool mHasScale;
		//MapViewportPanTimerListener* mPanTimerListener;
		//Font* mFont;
		//MapViewportPanMode mPanMode;
		//bool mHasTimer;
		//IMapViewportListener* mListener;

		Vector<Layer*> mLayers;
		int mSelectedLayer;
	};
}
#endif // MAPVIEWPORT_H_
