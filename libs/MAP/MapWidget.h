/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
