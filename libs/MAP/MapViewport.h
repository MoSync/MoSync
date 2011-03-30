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
* \file MapViewport.h
* \brief Viewport displaying a slippy map
* \author Lars-�ke Vinberg
*/

#ifndef MAPVIEWPORT_H_
#define MAPVIEWPORT_H_

#include <ma.h>
#include <MAUI/Font.h>

#include "LonLat.h"
#include "MapCache.h"
#include "Broadcaster.h"



namespace MAP
{
	using namespace MAUI;
	
	class MapTile;
	class MapSource;
	class MapCache;
	class MapViewport;
	class MapViewportIdleListener;

	//=========================================================================
	/**
	 * \brief Scroll direction for use in MapViewport::scroll( )
	 */
	enum MapViewportScrollDirection
	//=========================================================================
	{
		SCROLLDIRECTION_NORTH,
		SCROLLDIRECTION_SOUTH,
		SCROLLDIRECTION_EAST,
		SCROLLDIRECTION_WEST
	};

	//=========================================================================
	/**
	 * \brief Pan mode for use in MapViewport::scroll( )
	 */
	enum MapViewportPanMode
	//=========================================================================
	{
		MapViewportPanMode_Instant,
		MapViewportPanMode_Smooth,
		MapViewportPanMode_Momentum
	};

	//=========================================================================
	/**
	 * \brief Listener class, for listening client to implement.
	 */
	class IMapViewportListener
	//=========================================================================
	{
	public:
		/**
		 * Called when a requested tile has been received into cache from map source.
		 */
		virtual void viewportUpdated( MapViewport* viewport ) = 0;
		virtual void error( MapViewport* viewport, int code ) = 0;
	};

	//=========================================================================
	/**
	 * \brief Simple slippy map viewport.
	 */
	class MapViewport : 
		public IMapCacheListener,
		public Broadcaster<IMapViewportListener>
	//=========================================================================
	{
		friend class MapViewportIdleListener;

	public:
		MapViewport( );
		
		virtual ~MapViewport( );
		/**
		 * Map update scope
		 */
		void updateMap( );
		/**
		 * Map source property
		 */
		MapSource* getMapSource( ) const { return mSource; }
		void setMapSource( MapSource* source );
		/**
		 * Center position property
		 */
		LonLat getCenterPosition( ) const;
		void setCenterPosition( LonLat position, bool immediate, bool isPointerEvent );
		void setCenterPosition( LonLat position, MagnificationType magnification, bool immediate, bool isPointerEvent );
		PixelCoordinate getCenterPositionPixels( ) const;
		/**
		 * Magnification property
		 */
		MagnificationType getMagnification( ) const;
		void setMagnification(MagnificationType magnification );
		
		/*
		 * Show latitude/longitude
		 */
		bool showLonLat( ) const { return mShowLonLat; }
		void setShowLonLat( bool enabled );
		/*
		 * Show hairline cross at center of viewport.
		 */
		bool showHairlineCross( ) const { return mShowHairlineCross; }
		void setShowHairlineCross( bool enabled );
		/*
		 * Show pixel scale.
		 */
		bool showPixelScale( ) const { return mShowPixelScale; }
		void setShowPixelScale( bool enabled );

		void startGlide( );
		void stopGlide( );

		/**
		 * Returns currently used font.
		 */
		Font* getFont( ) const { return mFont; }
		/**
		 * Sets font property.
		 */ 
		void setFont( Font* font ) { mFont = font; }
		/**
		 * Scrolls the map in the specified direction.
		 * 
		 */
		void scroll( MapViewportScrollDirection direction, bool largeStep);
		/**
		 * Increases magnification by 1 step, i.e. a factor of two.
		 */
		void zoomIn( );
		void zoomOut( );
		//
		// Viewport size
		//
		void setWidth( int width );
		void setHeight( int height );
		/**
		 * Handles key press.
		 * Returns true if handled.
		 */
		virtual bool handleKeyPress( int keyCode );
		/**
		 * Handles key release.
		 * Returns true if handled.
		 */
		virtual bool handleKeyRelease( int keyCode );
		//
		// IMapCacheListener implementation
		//
		virtual void tileReceived( MapCache* sender, MapTile* tile, bool foundInCache );
		virtual void jobComplete( MapCache* sender );
		virtual void error( MapCache* sender, int error );
		/**
		 * Converts from global map pixels to viewport pixels.
		 */
		MAPoint2d worldPixelToViewport( PixelCoordinate wpx );
		/**
		 * Converts from viewport pixels to global map pixels.
		 */
		PixelCoordinate viewportToWorldPixel( MAPoint2d pt );
		/*
		 *  Positions and scales map to show specificed rectangle
		 */
		void centerAndScaleToRectangle( LonLat lowerLeft, LonLat upperRight );

		int getWidth() {
			return mWidth;
		}
		
		int getHeight( ) const {
			return mHeight; 	
		}

		int getScaledWidth( ) const { 
			int viewportWidth = (int)floor(((double)mWidth / mScale));
			return viewportWidth; 
		}
		int getScaledHeight( ) const { 
			int viewportHeight = (int)floor(((double)mHeight / mScale));
			return viewportHeight;
		}
		virtual void drawViewport( Point origin );

		void beginZooming(const MAPoint2d& p1, const MAPoint2d& p2);
		void updateZooming(const MAPoint2d& p1, const MAPoint2d& p2);
		void endZooming();
		bool isZooming();
		
		void beginPanning(const MAPoint2d& p);
		void updatePanning(const MAPoint2d& p);
		void endPanning();

		void moveCenterPositionInPixels(int xdelta, int ydelta);

		// updates magnification
		void setScale(double scale);
		double getScale();

	protected:
	
		//
		// events
		//
		virtual void onViewportUpdated( );
		virtual void onError( int code );
		//
		// Redraw
		//
		virtual void drawOverlay( Rect& bounds, MagnificationType magnification );

	private:
	
		//-------------------------------------------------------------------------
		// zooming
		//-------------------------------------------------------------------------
		double mOldDistance;
		MAPoint2d mOldCenter;
		MAPoint2d mNewCenter;
		double mMagnificationD;
		double mLinearMagnificationStart;
		int mZoomTime;
		bool mZooming;
		double mScale;
		//-------------------------------------------------------------------------
	
		//-------------------------------------------------------------------------
		// panning
		//-------------------------------------------------------------------------
		int mPanTime;
		MAPoint2d mTouchScreenCoordinate;
		PixelCoordinate mTouchPixelCoordinate;
		void setPositionInScreenCoordinatesSinceTouch(const MAPoint2d& p );	
		//-------------------------------------------------------------------------
			
		int mWidth;
		int mHeight;
		LonLat mCenterPositionLonLat;
		PixelCoordinate mCenterPositionPixels;
		LonLat mPanTargetPositionLonLat;
		PixelCoordinate mPanTargetPositionPixels;
		int mMagnification;
		MapSource* mSource;
		MapViewportIdleListener* mIdleListener;
		Font* mFont;
		bool mHasTimer;
		bool mInDraw;
		bool mShowPixelScale;
		bool mShowHairlineCross;
		bool mShowLonLat;
	};
}
#endif // MAPVIEWPORT_H_
