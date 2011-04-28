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
* \file MapWidget.h
* \brief Widget displaying a slippy map
* \author Lars-Åke Vinberg
*/

#ifndef MAPWIDGET_H_
#define MAPWIDGET_H_

#include <ma.h>
#include <MAUI/Widget.h>
#include "MapViewport.h"

/**
 * \brief MoSync slippy maps.
 */
namespace MAP
{
	using namespace MAUI;
	
	class MapSource;

	//=========================================================================
	/**
	 * \brief Simple slippy map widget.
	 */
	class MapWidget : public Widget,
		IMapViewportListener
	//=========================================================================
	{
	public:
		MapWidget( int x, int y, int width, int height, Widget* parent );
		
		virtual ~MapWidget( );
		/**
		 * Map source property
		 */
		MapSource* getMapSource( ) const { return mViewport->getMapSource( ); }
		void setMapSource( MapSource* source );
		/**
		 * Map viewport property
		 */
		void setViewport( MapViewport* viewport );
		MapViewport* getViewport( ) const { return mViewport; }
		/**
		 * Center position property
		 */
		LonLat getCenterPosition( ) const;
		
		/**
		 * Note: setCenterPosition depends on a mapSource and it's magnification levels, if not
		 *       mapSource is set prior to this call, the behaviour is undefined.
		 */
		void setCenterPosition( LonLat position, bool immediate, bool isPointerEvent );
		void setCenterPosition( LonLat position, int magnification, bool immediate, bool isPointerEvent );
		PixelCoordinate getCenterPositionPixels( ) const;
		/**
		 * Magnification property
		 */
		int getMagnification( ) const;
		void setMagnification( int magnification );
		/**
		 * Scale property
		 */		
		void setScale( double scale );
		/*
		 *  Positions and scales map to show specificed rectangle
		 */
		void centerAndScaleToRectangle( LonLat lowerLeft, LonLat upperRight );

		void startGlide( );
		void stopGlide( );

		/**
		 * Returns currently used font.
		 */
		Font* getFont( ) const { return mViewport->getFont( ); }
		/**
		 * Sets font property.
		 */ 
		void setFont( Font* font ) { mViewport->setFont( font ); }
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
		// Widget overrides
		//
		virtual void setWidth( int width );
		virtual void setHeight( int height );
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
		// IMapViewportListener implementation
		//
		virtual void viewportUpdated( MapViewport* viewport );
		virtual void error( MapViewport* viewport, int code );

	protected:
		//
		// Redraw
		//
		virtual void drawWidget( );
		virtual void drawOverlay( );
		Point getActualPosition( );
		MapViewport* mViewport;

	private:
	};
}

#endif // MAPWIDGET_H_
