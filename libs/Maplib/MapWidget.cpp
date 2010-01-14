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

#include "MemoryMgr.h"
#include <mastdlib.h>
#include <MAUtil/Graphics.h>
#include <MAUI/Image.h>
#include <MapWidget.h>
#include "MapTile.h"
#include "MapCache.h"
#include "MapSource.h"
//#include "DebugPrintf.h"
#include "OpenStreetMapSource.h"
#include "CloudMadeMapSource.h"
//#include "VirtualEarthMapSource.h"
#include "GoogleMapSource.h"
//#include "XmlHttpConnection.h"
#include "PixelCoordinate.h"
//#include "TraceScope.h"
//#include "MapCacheMgr.h"
//#include "MessageMgr.h"
//#include "SelectMapSourceActionGroup.h"
#include "MapSourceMgr.h"

namespace MAP
{
	//
	// Debug
	//
	static const bool Trace = false;
	//
	// Appearance
	//
	static const bool ShowPixelScale = true; // shows scale slider
	static const bool ShowPixelScaleAsText = true; // shows meters/pixel scale (at latitude of screen center).
	static const bool ShowHairlineCross = true;
	static const bool ShowLatLon = true;
	//
	// Configuration
	//
	static const int PanIntervalMs = 80;
	static const double PanTension = 0.5;
	static const int SmallScrollStep = 30; // pixels to scroll if not full page
	static const int CrossSize = 4;

	//=========================================================================
	class MapWidgetPanTimerListener : public TimerListener
	//=========================================================================
	{
	public:
		//---------------------------------------------------------------------
		MapWidgetPanTimerListener( MapWidget* widget ) :
		//---------------------------------------------------------------------
			m_widget( widget )
		{
		}

		//---------------------------------------------------------------------
		void runTimerEvent( )
		//---------------------------------------------------------------------
		{
			PixelCoordinate currentPix = m_widget->m_centerPositionPixels;
			PixelCoordinate targetPix = m_widget->m_panTargetPositionPixels;
			double offsetX = targetPix.getX( ) - currentPix.getX( );
			double offsetY = targetPix.getY( ) - currentPix.getY( );

			if( fabs( offsetX ) <= 2 && fabs( offsetY ) <= 2 )
			{
				Environment::getEnvironment( ).removeTimer( this );
				m_widget->enterMapUpdateScope( );
				// performed below m_widget->m_centerPositionLonLat = m_widget->m_panTargetPositionLonLat;
				// performed below m_widget->m_centerPositionPixels = m_widget->m_panTargetPositionPixels;
				m_widget->exitMapUpdateScope( true );
				m_widget->updateMap( );
				m_widget->requestRepaint( );
				return;
			}
			m_widget->enterMapUpdateScope( );
			PixelCoordinate newPix = PixelCoordinate(	m_widget->getMagnification( ),
														(int)( currentPix.getX( ) + PanTension * offsetX ),
														(int)( currentPix.getY( ) + PanTension * offsetY ) );
			m_widget->m_centerPositionPixels = newPix;
			m_widget->exitMapUpdateScope( false );
			m_widget->updateMap( );
			m_widget->requestRepaint( );
		}

	private:
		MapWidget*		m_widget;
	};

	//=========================================================================


	//-------------------------------------------------------------------------
	MapWidget::MapWidget( int x, int y, int width, int height, Widget* parent)
	//-------------------------------------------------------------------------
	:	Widget( x, y, width, height, parent ),
		m_centerPositionLonLat( ),
		m_centerPositionPixels( ),
		m_panTargetPositionLonLat( ),
		m_panTargetPositionPixels( ),
		m_magnification( 0 ),
		m_sourceKind( MapSourceKind_OpenStreetMap ),
		//m_cache( NULL ),
		m_mapUpdateNesting( 0 ),
		m_prevCenter( ),
		m_screenImage( NULL ),
		m_hasScale( true ),
		m_hasSmoothPanning( true ),
		m_font( NULL ),
		m_timerRunning( false ),
		m_panTimerListener( NULL )
	{
		//if ( Trace ) trace( );

		resetScreenImage( );
		m_panTimerListener = newobject( MapWidgetPanTimerListener, new MapWidgetPanTimerListener( this ) );
	}

	//-------------------------------------------------------------------------
	MapWidget::~MapWidget( )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		//m_cache = NULL;
		//m_source = NULL;
		deleteobject( m_panTimerListener );
	}

	//-------------------------------------------------------------------------
	void MapWidget::enterMapUpdateScope( )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		if ( m_mapUpdateNesting == 0 )
		{
			m_prevCenter = getCenterPositionPixels( );
			m_prevMagnification = getMagnification( );
		}
		m_mapUpdateNesting ++;
	}

	//-------------------------------------------------------------------------
	void MapWidget::exitMapUpdateScope( bool immediate )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		//DebugAssert( m_mapUpdateNesting > 0 ); 

		m_mapUpdateNesting--;
		if ( m_mapUpdateNesting == 0 )
		{
			if ( immediate )
			{
				m_centerPositionLonLat = m_panTargetPositionLonLat;
				m_centerPositionPixels = m_panTargetPositionPixels;
			}
			if ( getCenterPositionPixels( ) != m_prevCenter || getMagnification( ) != m_prevMagnification )
			{
				updateMap( );
			}
		}
	}

	//-------------------------------------------------------------------------
	void MapWidget::checkMapUpdateScope( )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		//DebugAssert( m_mapUpdateNesting > 0 );
	}

	//-------------------------------------------------------------------------
	PixelCoordinate MapWidget::getCenterPositionPixels( ) const
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		return m_hasSmoothPanning ? m_panTargetPositionPixels : m_centerPositionPixels;
	}

	//-------------------------------------------------------------------------
	inline double Max( double x, double y )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		return x > y ? x : y;
	}

	//-------------------------------------------------------------------------
	LonLat MapWidget::getCenterPosition( ) const
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		return m_hasSmoothPanning ? m_panTargetPositionLonLat : m_centerPositionLonLat;
	}

	//-------------------------------------------------------------------------
	void MapWidget::setCenterPosition( LonLat position )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		checkMapUpdateScope( );

		if ( m_hasSmoothPanning )
		{
			m_panTargetPositionLonLat = position;
			m_panTargetPositionPixels = position.toPixels( getMagnification( ) );
			//
			// Make sure current position is nearby, so we only soft scroll less than one screen.
			//
			int deltaX = m_panTargetPositionPixels.getX( ) - m_centerPositionPixels.getX( );
			int deltaY = m_panTargetPositionPixels.getY( ) - m_centerPositionPixels.getY( );
			//
			// go directly to location if delta is more than 1/6 of widget size.
			//
			int width = getWidth( );
			int height = getHeight( );
			if ( width > 0 && height > 0 )
			{
				double factor = 6 * fabs( Max( (double)deltaX / getWidth( ), (double)deltaY / getHeight( ) ) );
				if ( factor > 1 )
				{
					m_centerPositionPixels = PixelCoordinate(	getMagnification( ),
																m_panTargetPositionPixels.getX( ) - (int)( (double)deltaX / factor ),
																m_panTargetPositionPixels.getY( ) - (int)( (double)deltaY / factor ) );
				}
				Environment::getEnvironment( ).addTimer( m_panTimerListener, PanIntervalMs, 0 );
			}
		}
		else
		{
			m_centerPositionLonLat = position;
			m_centerPositionPixels = position.toPixels( getMagnification( ) );
		}
	}

	//-------------------------------------------------------------------------
	int MapWidget::getMagnification( ) const
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		return m_magnification;
	}

	//-------------------------------------------------------------------------
	void MapWidget::setMagnification( int magnification )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		checkMapUpdateScope( );

		m_magnification = magnification;
		if ( m_hasSmoothPanning )
			setCenterPosition( m_panTargetPositionLonLat );
		else
			setCenterPosition( m_centerPositionLonLat );
	}

	//-------------------------------------------------------------------------
	void MapWidget::setMapSourceKind( MapSourceKind sourceKind )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		m_sourceKind = sourceKind;
		//m_cache = MapCacheMgr::get( )->getMapCache( m_source );
		updateMap( );
	}

	//-------------------------------------------------------------------------
	bool MapWidget::getHasSmoothPanning( ) const
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		return m_hasSmoothPanning;
	}

	//-------------------------------------------------------------------------
	void MapWidget::setHasSmoothPanning( bool hasSmoothPanning )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		m_hasSmoothPanning = hasSmoothPanning;
	}

	//-------------------------------------------------------------------------
	void MapWidget::tileReceived( MapCache* sender, MapTile* tile )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		//if ( Trace ) DebugPrintf( "MapTile received %d %d %d\n", tile->getMagnification( ), tile->getGridX( ), tile->getGridY( ) );

		//DebugAssert( tile->getSourceKind( ) == m_sourceKind );

		PixelCoordinate tilePx = tile->getCenter( ).toPixels( tile->getMagnification( ) );
		MAPoint2d pt = worldPixelToWidget( tilePx );
		MAHandle old = maSetDrawTarget( m_screenImage );
		MapSource* source = MapSourceMgr::get( )->getMapSource( m_sourceKind );
		const int tileSize = source->getTileSize( );

		//maDrawImage( tile->getImage( ), pt.x - tileSize / 2, pt.y - tileSize / 2 );

		//void maDrawImageRegion(Handle image, const MARect* srcRect, const MAPoint2d* dstPoint, int transformMode);
		MARect rect;
		rect.height = tileSize;
		rect.width = tileSize;
		rect.top = 0;
		rect.left = 0;
		MAPoint2d dest;
		dest.x = pt.x - tileSize / 2;
		dest.y = pt.y - tileSize / 2;
		maDrawImageRegion( tile->getImage( ), &rect, &dest, TRANS_NONE );

		maSetDrawTarget( old );
		requestRepaint( );
	}

	//-------------------------------------------------------------------------
	Point MapWidget::getActualPosition( )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		Widget* parent = this->getParent( );
		Point pos = getPosition( );

		while( parent != 0 )
		{
			pos.x += parent->getPosition( ).x;
			pos.y += parent->getPosition( ).y;
			parent = parent->getParent( );
		}
		return pos;
	}

	//-------------------------------------------------------------------------
	void MapWidget::drawWidget( )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		if ( !this->enabled ) 
			return;

		Point widgetPos = getPosition( );
		//
		// Save clip
		//
		/*BOOL ret =*/ Gfx_pushClipRect( widgetPos.x, widgetPos.y, getWidth( ), getHeight( ) );
		//
		// Draw map image
		//
		Gfx_drawImage( m_screenImage, widgetPos.x, widgetPos.y );
		//
		// Let subclass draw its overlay
		//
		drawOverlay( );
		
		MapSource* source = MapSourceMgr::get( )->getMapSource( m_sourceKind );
		//
		// Draw scale indicator
		//
		if ( ShowPixelScale && m_hasScale )
		{
			const int scaleWidth = 80;
			const int scaleX = widgetPos.x + getWidth( ) - scaleWidth - 5;
			const int scaleY = widgetPos.y + 5;
			int lineThickness = 3;
			const int crossbarHeight = 7;
			float scaleFrac = (float)( m_magnification - source->getMagnificationMin( ) ) / ( source->getMagnificationMax( ) - source->getMagnificationMin( ) );

			maSetColor( 0xa0a0a0 );

			Gfx_fillRect( scaleX, scaleY - lineThickness / 2, scaleWidth, lineThickness );
			Gfx_fillRect( (int)( scaleX + scaleFrac * scaleWidth - 0.5f * lineThickness ), scaleY - (crossbarHeight / 2 ), lineThickness, crossbarHeight );

			lineThickness = 1;

			maSetColor( 0x000000 );

			Gfx_fillRect( scaleX, scaleY, scaleWidth, lineThickness );
			Gfx_fillRect( (int)( scaleX + scaleFrac * scaleWidth - 0.5f * lineThickness ), scaleY - (crossbarHeight / 2 ), lineThickness, crossbarHeight );

			//
			// pixel scale
			//
			if ( ShowPixelScaleAsText )
			{
				int mag = getMagnification( );
				PixelCoordinate px1 = getCenterPositionPixels( );
				LonLat p1 = LonLat( px1 );
				PixelCoordinate px2 = PixelCoordinate( mag, px1.getX( ) + 1, px1.getY( ) + 1 );
				LonLat p2 = LonLat( px2 );
				double meterX1, meterY1;
				double meterX2, meterY2;
				p1.toMeters( mag, meterX1, meterY1 );
				p2.toMeters( mag, meterX2, meterY2 );
				double offsetX = meterX2 - meterX1;
				offsetX *= cos( fabs( p2.lat * PI / 180 ) );
				char buffer[100];
				sprintf( buffer, "%5.2f m/px", offsetX );

				if ( m_font != NULL )
					m_font->drawString( buffer, scaleX, scaleY + 5 );
				else
					Gfx_drawText( scaleX, scaleY + 5, buffer );
			}
		}
		//
		// Draw hairline cross
		//
		if ( ShowHairlineCross )
		{
			const int centerX = widgetPos.x + getWidth( ) / 2;
			const int centerY = widgetPos.y + getHeight( ) / 2;

			maSetColor( 0x000000 );

			Gfx_fillRect( centerX, centerY - CrossSize, 1, CrossSize * 2 + 1 );
			Gfx_fillRect( centerX - CrossSize, centerY, CrossSize * 2 + 1, 1 );
		}
		//
		// Draw latitude, longitude
		//
		if ( ShowLatLon )
		{
			static const int textWidth = 100;
			static const int textHeight = 12;
			char buffer[100];
			if ( m_hasSmoothPanning )
				sprintf( buffer, "%-3.3f %-3.3f", m_panTargetPositionLonLat.lon, m_panTargetPositionLonLat.lat );
			else
				sprintf( buffer, "%-3.3f %-3.3f", m_centerPositionLonLat.lon, m_centerPositionLonLat.lat );
			maSetColor( 0x000000 );

			if ( m_font != NULL )
				m_font->drawString( buffer, widgetPos.x, widgetPos.y );
			else
				Gfx_drawText( widgetPos.x, widgetPos.y, buffer );
		}
		//
		// Restore original clip
		//
		/*BOOL ret2 =*/ Gfx_popClipRect( );
	}

	//-------------------------------------------------------------------------
	void MapWidget::drawOverlay( )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

	}

	//-------------------------------------------------------------------------
	void MapWidget::updateMap( )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		//DebugAssert( m_mapUpdateNesting == 0 );

		if ( getWidth( ) <= 0 || getHeight( ) <= 0 ) 
			return;

		//
		// Clear screen image
		//
		MAHandle old = maSetDrawTarget( m_screenImage );
		maSetColor( 0xc0c0c0 );
		maFillRect( 0, 0, getWidth( ), getHeight( ) );
		maSetDrawTarget( old );
		//
		// Request tiles
		//
		// We want to use currently displayed center position here, so we bypass getCenterPosition( ).
		//if ( m_cache != NULL )
		MapCache::get( )->requestTiles( this, m_sourceKind, LonLat( m_centerPositionPixels ), m_magnification, getWidth( ), getHeight( ) );
	}

	//-------------------------------------------------------------------------
	MAPoint2d MapWidget::worldPixelToWidget( PixelCoordinate wpx )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		MAPoint2d pt;
		PixelCoordinate screenPx = m_centerPositionPixels;

		//pt.x = (int)( 0.499 + wpx.getX( ) - screenPx.getX( ) + ( getWidth( ) >> 1 ) );
		//pt.y = (int)( 0.499 -( wpx.getY( ) - screenPx.getY( ) ) + ( getHeight( ) >> 1 ) );
		pt.x =    wpx.getX( ) - screenPx.getX( )   + ( getWidth( ) >> 1 );
		pt.y = -( wpx.getY( ) - screenPx.getY( ) ) + ( getHeight( ) >> 1 );

		return pt;
	}

	//-------------------------------------------------------------------------
	PixelCoordinate MapWidget::widgetToWorldPixel( MAPoint2d pt )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		PixelCoordinate screenPx = m_centerPositionPixels;
		return PixelCoordinate( m_magnification,
								(int)( pt.x + 0.5 - 0.5 * getWidth( ) + screenPx.getX( ) ),
								(int)( -( pt.y + 0.5 - 0.5 * getHeight( ) - screenPx.getY( ) ) ) );
	}

	//-------------------------------------------------------------------------
	static double clamp( double x, double min, double max )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		return x < min ? min : x > max ? max : x;
	}

	//-------------------------------------------------------------------------
	void MapWidget::zoomIn( )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		MapSource* source = MapSourceMgr::get( )->getMapSource( m_sourceKind );

		if ( m_magnification < source->getMagnificationMax( ) )
		{
			m_magnification++;
			if ( m_hasSmoothPanning )
			{
				m_centerPositionPixels = m_centerPositionLonLat.toPixels( m_magnification );
				m_panTargetPositionPixels = m_panTargetPositionLonLat.toPixels( m_magnification );
			}
			else
				m_centerPositionPixels = m_centerPositionLonLat.toPixels( m_magnification );
		}
	}

	//-------------------------------------------------------------------------
	void MapWidget::zoomOut( )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		//if ( m_source == NULL )
		//	return;

		MapSource* source = MapSourceMgr::get( )->getMapSource( m_sourceKind );

		if ( m_magnification > source->getMagnificationMin( ) )
		{
			m_magnification--;
			if ( m_hasSmoothPanning )
			{
				m_centerPositionPixels = m_centerPositionLonLat.toPixels( m_magnification );
				m_panTargetPositionPixels = m_panTargetPositionLonLat.toPixels( m_magnification );
			}
			else
				m_centerPositionPixels = m_centerPositionLonLat.toPixels( m_magnification );
		}
	}

	//-------------------------------------------------------------------------
	void MapWidget::scroll( MapWidgetScrollDirection direction, bool largeStep)
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		PixelCoordinate px = getCenterPositionPixels( );
		const int hStep = largeStep ? getWidth( ) : SmallScrollStep;
		const int vStep = largeStep ? getHeight( ) : SmallScrollStep;

		switch( direction )
		{
		case SCROLLDIRECTION_NORTH: px = PixelCoordinate( px.getMagnification( ), px.getX( ), px.getY( ) + vStep ); break;
		case SCROLLDIRECTION_SOUTH: px = PixelCoordinate( px.getMagnification( ), px.getX( ), px.getY( ) - vStep ); break;
		case SCROLLDIRECTION_EAST:  px = PixelCoordinate( px.getMagnification( ), px.getX( ) + hStep, px.getY( ) ); break;
		case SCROLLDIRECTION_WEST:  px = PixelCoordinate( px.getMagnification( ), px.getX( ) - hStep, px.getY( ) ); break;
		}

		LonLat newPos = LonLat( px );
		newPos = LonLat( clamp( newPos.lon, -180, 180 ), clamp( newPos.lat, -85, 85 ) );
		setCenterPosition( newPos );
	}

	//-------------------------------------------------------------------------
	bool MapWidget::handleKeyPress( int keyCode )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		enterMapUpdateScope( );
		bool ret = false;
		switch( keyCode )
		{
		case MAK_LEFT:		
			scroll( SCROLLDIRECTION_WEST, false ); 
			ret = true;
			break;
		case MAK_RIGHT:		
			scroll( SCROLLDIRECTION_EAST, false ); 
			ret = true;
			break;
		case MAK_UP:		
			scroll( SCROLLDIRECTION_NORTH, false ); 
			ret = true;
			break;
		case MAK_DOWN:		
			scroll( SCROLLDIRECTION_SOUTH, false ); 
			ret = true;
			break;
		case MAK_1:			
			zoomOut( ); 
			ret = true;
			break;
		case MAK_3:			
			zoomIn( ); 
			ret = true;
			break;
		default: 
			ret = false;
			break;
		}
		exitMapUpdateScope( false );
		return ret;
	}

	//-------------------------------------------------------------------------
	bool MapWidget::handleKeyRelease( int keyCode )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		return true;
	}

	//-------------------------------------------------------------------------
	void MapWidget::setWidth( int width )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		Widget::setWidth( width );
		resetScreenImage( );
	}

	//-------------------------------------------------------------------------
	void MapWidget::setHeight( int height )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		Widget::setHeight( height );
		resetScreenImage( );
	}

	//-------------------------------------------------------------------------
	void MapWidget::resetScreenImage( )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		if ( m_screenImage != NULL )
			maDestroyObject( m_screenImage );

		if ( getWidth( ) > 0 && getHeight( ) > 0 )
		{
			m_screenImage = maCreatePlaceholder( );
			maCreateDrawableImage( m_screenImage, getWidth( ), getHeight( ) );
		}
	}
}
