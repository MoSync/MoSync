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

#include "MAHeaders.h"
#include <MAUI/WidgetSkin.h>
#include <MAUtil/Graphics.h>
#include <MAUtil/Vector.h>
#include "HelloLayerMapScreen.h"
#include "HelloLayerMapMoblet.h"
#include <MAP/GeoPoint.h>
#include <MAP/GeoPointLayerRenderer.h>
#include <MAP/Color.h>
#include <MAP/OpenStreetMapSource.h>
#include <MAP/GoogleMapSource.h>
#include <MAP/GeoPointLayer.h>
#include <MAP/LayerMapViewport.h>

using namespace MAUI;
using namespace MAPUtil;

namespace HelloLayerMap
{
	static const int DoubleClickTimeMs = 200;
	static const int TapPanAcceleration = 1;
	static const int TapPanIntervalMs = 100;

	//=========================================================================
	//
	// The test data source
	//
	class TestDataSource : public GeoPointDataSource
	//=========================================================================
	{
	public:
		//-------------------------------------------------------------------------
		TestDataSource( )
		//-------------------------------------------------------------------------
		{
		}

		//-------------------------------------------------------------------------
		virtual ~TestDataSource( )
		//-------------------------------------------------------------------------
		{
		}

		//
		// LayerDataSource overrides
		//
		//-------------------------------------------------------------------------
		virtual void refresh( ) 
		//-------------------------------------------------------------------------
		{ 
			/* nothing to refresh */ 
		}

		//-------------------------------------------------------------------------
		virtual int size( ) 
		//-------------------------------------------------------------------------
		{ 
			return mItems.size( ); 
		}

		//-------------------------------------------------------------------------
		virtual GeoPoint* getItem( int index ) 
		//-------------------------------------------------------------------------
		{ 
			return index >= mItems.size( ) ? NULL : mItems[index];
		}

		//-------------------------------------------------------------------------
		void addPoint( LonLat lonlat )
		//-------------------------------------------------------------------------
		{
			mItems.add( newobject( GeoPoint, new GeoPoint( lonlat, "Point", "Point", 3 , DateTime( 109, 05, 18, 16, 50, 00 ) ) ) );
			onDataChanged( );
		}

	private:
		//-------------------------------------------------------------------------
		void onDataChanged( )
		//-------------------------------------------------------------------------
		{
			Vector<IGeoPointDataSourceListener*>* listeners = getBroadcasterListeners<IGeoPointDataSourceListener>( *this );
			for ( int i = 0; i < listeners->size( ); i++ )
				(*listeners)[i]->dataChanged( this );
		}

		Vector<GeoPoint*> mItems;
	};

	static const char* weekdays[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

	//=========================================================================
	//
	// The layer renderer
	//
	class TestLayerRenderer : public GeoPointLayerRenderer
	//=========================================================================
	{
		static const int PointTextGap = 3;
		//
		// true: draws items as colored squares
		// false: draws items as images
		//
		static const bool DrawAsSquares = true;
		//
		// true: draws item text at item
		// false: draws item text in overlaid rectangle at bottom
		//
		static const bool DrawTextAtItem = false;
	public:
		//-------------------------------------------------------------------------
		TestLayerRenderer( ) 
		//-------------------------------------------------------------------------
		{
			mLabelFont = newobject( Font, new Font( RES_FONT_VERDANA13WHITE ) );
			mMsgSkin = newobject( WidgetSkin, new WidgetSkin( RES_SMOKESKIN, RES_SMOKESKIN, 8, 57, 8, 56, true, true ) );
		}

		//-------------------------------------------------------------------------
		virtual ~TestLayerRenderer( )
		//-------------------------------------------------------------------------
		{
			deleteobject( mLabelFont );
			deleteobject( mMsgSkin );
		}

		//-------------------------------------------------------------------------
		void renderItem( Layer* layer, GeoPoint* item, int x, int y, bool selected )
		//-------------------------------------------------------------------------
		{
			if ( DrawAsSquares )
			{
				//
				// Draw as squares
				//
				int pointRadius = item->getMarkerSize( );
				//
				// Fill rect
				//
				maSetColor( ( selected ? Color::blue : Color::red ).val( ) );

				Gfx_fillRect( x - pointRadius, y - pointRadius, 2 * pointRadius, 2 * pointRadius );
				//
				// Draw rect border
				//
				maSetColor( Color::black.val( ) );
				DrawRectangle( x - pointRadius, y - pointRadius, 2 * pointRadius, 2 * pointRadius );
			}
			else
			{
				//
				// Draw as pins
				//
				MAHandle handle = selected ? RES_PIN16BLUE : RES_PIN16RED;
				MAExtent ext = maGetImageSize( handle );
				int imageWidth = EXTENT_X( ext );
				int imageHeight = EXTENT_Y( ext );
				Gfx_drawImage( handle, x - imageWidth / 2, y - imageHeight );
			}
		}

		//-------------------------------------------------------------------------
		// Helper function
		//
		static void DrawRectangle( int x, int y, int width, int height )
		//-------------------------------------------------------------------------
		{
			Gfx_line( x, y, x + width, y );
			Gfx_line( x + width, y, x + width, y + height );
			Gfx_line( x + width, y + height, x, y + height );
			Gfx_line( x, y + height, x, y );
		}

		//-------------------------------------------------------------------------
		void renderItemText( Layer* layer, GeoPoint* item, int x, int y, const Rect& bounds )
		//-------------------------------------------------------------------------
		{
			if ( DrawTextAtItem )
			{
				//
				// Draw text next to point
				//
				int pointRadius = item->getMarkerSize( );
				const int textWidth = 100;
				const int textHeight = 12;
				maSetColor( 0xa0a0a0 );
				Gfx_fillRect( x - textWidth / 2, y + pointRadius / 2 + PointTextGap, textWidth, textHeight );
				maSetColor( Color::black.val( ) );
				Gfx_drawText( x - textWidth / 2 + 1, y + pointRadius / 2 + 1, item->getDescription( ) );
			}
			else
			{
				//
				// Draw text in box at bottom of map
				//
				const int margin = 3;
				const int width = bounds.width - 2 * margin;
				const int height = 40;
				const int left = bounds.x + margin;
				const int top = bounds.y + bounds.height - height - margin;

				mMsgSkin->draw( left, top, width, height, WidgetSkin::SELECTED );

				if ( mLabelFont == NULL )
				{
					maSetColor( Color::black.val( ) );
					DrawRectangle( left, top, width, height );
				}

				const char* desc = item->getTitle( );
				// strip leading spaces
				while ( *desc == ' ' )
					desc++;

				if ( mLabelFont != NULL )
					mLabelFont->drawString( desc, left + 4, top + 2 );
				else
					Gfx_drawText( left + 4, top + 2, desc );

				DateTime pubDate = item->getTimestamp( );
				if ( pubDate != DateTime::minValue( ) )
				{
					char buf[30];
					sprintf( buf, "%s %02d:%02d", weekdays[pubDate.getWeekday( )], pubDate.getHour( ), pubDate.getMinute( ) );

					if ( mLabelFont != NULL )
						mLabelFont->drawString( buf, left + 4, top + 14 );
					else
						Gfx_drawText( left + 4, top + 14, buf );
				}
			}
		}
	private:
		Font* mLabelFont;
		WidgetSkin* mMsgSkin;
	};

	//=========================================================================

	//-------------------------------------------------------------------------
	HelloLayerMapScreen::HelloLayerMapScreen( )
	//-------------------------------------------------------------------------
	{
		//
		// Screen size
		//
		MAExtent screenSize = maGetScrSize( );
		int width = EXTENT_X( screenSize );
		int height = EXTENT_Y( screenSize );
		//
		// Layer data source
		//
		mDataSource = newobject( TestDataSource, new TestDataSource( ) );
		GeoPointLayer* layer = newobject( GeoPointLayer, new GeoPointLayer( ) );
		layer->setDataSource( mDataSource );
		mRenderer = newobject( TestLayerRenderer, new TestLayerRenderer( ) );
		layer->setRenderer( mRenderer );
		//
		// Viewport
		//
		LayerMapViewport* viewport = newobject( LayerMapViewport, new LayerMapViewport( ) );
		viewport->addLayer( layer );
		//
		// Layer map widget
		//
		mMap = newobject( MapWidget, new MapWidget( 0, 0, width, height, NULL ) );
		mMap->setViewport( viewport );
		//
		// barcelona  2.15058 41.3740 
		// http://local.google.com/?ll=41.373781,2.150016
		//
		mMap->setCenterPosition( LonLat( 18.07, 59.33 ), 10, true, false );

		mBlackFont = newobject( Font, new Font( RES_FONT_VERDANA13BLACK ) );
		mMap->setFont( mBlackFont );
		//
		// Create map sources
		//
		mOpenStreetMapSource = newobject( OpenStreetMapSource, new OpenStreetMapSource( ) );
		mGoogleStreetMapSource = newobject( GoogleMapSource, new GoogleMapSource( GoogleMapKind_StreetMap  ) );
		mGoogleAerialMapSource = newobject( GoogleMapSource, new GoogleMapSource( GoogleMapKind_Aerial ) );
		mGoogleHybridMapSource = newobject( GoogleMapSource, new GoogleMapSource( GoogleMapKind_Hybrid ) );

		setMain( mMap );
		//
		// Have to wait until we have proper width and height
		//
		mMap->setMapSource( mOpenStreetMapSource );
		//
		// Map cache size:
		// add one to width and height for case where viewport is not aligned to tile grid
		// Multiply by two to get some margin
		//
		int tileSize = mOpenStreetMapSource->getTileSize( );
		int tiles = 2 * ( ( ( width - 1 ) / tileSize ) + 2 ) * ( ( ( height - 1 ) / tileSize ) + 2 );
		DebugPrintf( "Cache tiles: %d\n", tiles );
		MapCache::get( )->setCapacity( tiles );
	}

	//-------------------------------------------------------------------------
	HelloLayerMapScreen::~HelloLayerMapScreen( )
	//-------------------------------------------------------------------------
	{
		deleteobject( mBlackFont );
		deleteobject( mMap );
		deleteobject( mRenderer );
		deleteobject( mDataSource );
		deleteobject( mOpenStreetMapSource );
		deleteobject( mGoogleStreetMapSource );
		deleteobject( mGoogleAerialMapSource );
		deleteobject( mGoogleHybridMapSource );
	}

	//-------------------------------------------------------------------------
	void HelloLayerMapScreen::keyPressEvent( int keyCode )
	//-------------------------------------------------------------------------
	{
		switch( keyCode )
		{
		case MAK_2:
			nextMapSource( );
			return;
		case MAK_FIRE:
			addPoint( );
			return;
		case MAK_SOFTRIGHT:
			HelloLayerMapMoblet& moblet = (HelloLayerMapMoblet&)Environment::getEnvironment( );
			moblet.Terminate( );
			return;
		}
		
		(void)mMap->handleKeyPress( keyCode );
	}

	//-------------------------------------------------------------------------
	void HelloLayerMapScreen::keyReleaseEvent( int keyCode )
	//-------------------------------------------------------------------------
	{
		(void)mMap->handleKeyRelease( keyCode );
	}

	bool scrolling = false;
	int pointerPressX;
	int pointerPressY;
	PixelCoordinate pointerPressCenter;
	int pointerPressTimeMs = -1;

	//-------------------------------------------------------------------------
	void HelloLayerMapScreen::pointerPressEvent( MAPoint2d p )
	//-------------------------------------------------------------------------
	{
		//
		// Set up tap panning
		//
		pointerPressX = p.x;
		pointerPressY = p.y;
		pointerPressCenter = mMap->getCenterPositionPixels( );
		//
		// set position to stop any movement
		//
		mMap->stopGlide( );
		//
		// Handle doubletap for app exit
		//
		int curTimeMs = maGetMilliSecondCount( );
		int deltaTimeMs = curTimeMs - pointerPressTimeMs;
		if ( deltaTimeMs < DoubleClickTimeMs ) 
		{
			HelloLayerMapMoblet& moblet = (HelloLayerMapMoblet&)Environment::getEnvironment( );
			moblet.Terminate( );
			//mMap->stressTest( );
		}
		else
		{
			pointerPressTimeMs = curTimeMs;
		}
	}

    //-------------------------------------------------------------------------
    static double clamp( double x, double min, double max )
    //-------------------------------------------------------------------------
    {
            return x < min ? min : x > max ? max : x;
    }

	int lastPointerMoveMs = -1;

	//-------------------------------------------------------------------------
	void HelloLayerMapScreen::setPosition( MAPoint2d p )
	//-------------------------------------------------------------------------
	{
		int curTimeMs = maGetMilliSecondCount( );
		int deltaMs = curTimeMs - lastPointerMoveMs;
		lastPointerMoveMs = curTimeMs;
		int dx = ( p.x - pointerPressX ) * TapPanAcceleration;
		int dy = ( p.y - pointerPressY ) * TapPanAcceleration;
		PixelCoordinate px = PixelCoordinate(	pointerPressCenter.getMagnification( ), 
												pointerPressCenter.getX( ) - dx,
												pointerPressCenter.getY( ) + dy );
        LonLat newPos = LonLat( px );
        newPos = LonLat( clamp( newPos.lon, -180, 180 ), clamp( newPos.lat, -85, 85 ) );
        mMap->setCenterPosition( newPos, false, true );
	}

	//-------------------------------------------------------------------------
	void HelloLayerMapScreen::pointerMoveEvent( MAPoint2d p )
	//-------------------------------------------------------------------------
	{
		if( scrolling ) 
			return;
		int curTimeMs = maGetMilliSecondCount( );
		if ( curTimeMs < lastPointerMoveMs + TapPanIntervalMs )
			return;
		setPosition( p );
	}

	//-------------------------------------------------------------------------
	void HelloLayerMapScreen::pointerReleaseEvent( MAPoint2d p )
	//-------------------------------------------------------------------------
	{
		scrolling = false;
		setPosition( p );
		mMap->startGlide( );
	}

	//-------------------------------------------------------------------------
	void HelloLayerMapScreen::addPoint( )
	//-------------------------------------------------------------------------
	{
		mDataSource->addPoint( mMap->getCenterPosition( ) );
	}

	//-------------------------------------------------------------------------
	void HelloLayerMapScreen::nextMapSource( )
	//-------------------------------------------------------------------------
	{
		MapSource* current = mMap->getMapSource( );
		if ( current == mOpenStreetMapSource ) 
			mMap->setMapSource( mGoogleStreetMapSource );
		else if ( current == mGoogleStreetMapSource )
			mMap->setMapSource( mGoogleAerialMapSource );
		else if ( current == mGoogleAerialMapSource )
			mMap->setMapSource( mGoogleHybridMapSource );
		else if ( current == mGoogleHybridMapSource )
			mMap->setMapSource( mOpenStreetMapSource );
	}
}
