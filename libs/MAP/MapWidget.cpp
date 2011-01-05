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

#include <matime.h>
#include "MemoryMgr.h"
#include <mastdlib.h>
#include <MAUtil/Graphics.h>
#include <MAUI/Image.h>
#include "MapWidget.h"
#include "MapCache.h"
#include "MapSource.h"

namespace MAP
{
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
	static const int PanIntervalMs = 60;
	static const double PanTension = 0.3;
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
			mWidget( widget )
		{
		}

		//---------------------------------------------------------------------
		void runTimerEvent( )
		//---------------------------------------------------------------------
		{
			switch( mWidget->mPanMode )
			{
			case MapWidgetPanMode_Instant:
				// we shouldn't be here.
				break;
			case MapWidgetPanMode_Smooth:
				{
					PixelCoordinate currentPix = mWidget->mCenterPositionPixels;
					PixelCoordinate targetPix = mWidget->mPanTargetPositionPixels;
					double offsetX = targetPix.getX( ) - currentPix.getX( );
					double offsetY = targetPix.getY( ) - currentPix.getY( );

					if( fabs( offsetX ) <= 2 && fabs( offsetY ) <= 2 )
					{
						Environment::getEnvironment( ).removeTimer( this );
						mWidget->enterMapUpdateScope( );
						mWidget->exitMapUpdateScope( true );
						mWidget->updateMap( );
						mWidget->requestRepaint( );
						//mWidget->draw( false );
						return;
					}
					PixelCoordinate newPix = PixelCoordinate(	mWidget->getMagnification( ),
																(int)( currentPix.getX( ) + PanTension * offsetX ),
																(int)( currentPix.getY( ) + PanTension * offsetY ) );
					mWidget->enterMapUpdateScope( );
					mWidget->mCenterPositionPixels = newPix;
					mWidget->exitMapUpdateScope( false );
					mWidget->updateMap( );
					mWidget->requestRepaint( );
					//mWidget->draw( false );
				}
				break;
			case MapWidgetPanMode_Momentum:
					PixelCoordinate currentPix = mWidget->mCenterPositionPixels;
					int currentTimeMs = maGetMilliSecondCount( );
					int deltaMs = currentTimeMs - mWidget->mPanStartTimeMs;
					PixelCoordinate newPix = PixelCoordinate(	mWidget->getMagnification( ),
																(int)( currentPix.getX( ) - mWidget->mPanMomentumX * deltaMs * 0.001f ),
																(int)( currentPix.getY( ) + mWidget->mPanMomentumY * deltaMs * 0.001f ) );
					mWidget->mPanMomentumX = (int)( mWidget->mPanMomentumX * mWidget->mPanFriction );
					mWidget->mPanMomentumY = (int)( mWidget->mPanMomentumY * mWidget->mPanFriction );

					mWidget->enterMapUpdateScope( );
					mWidget->mCenterPositionPixels = newPix;
					mWidget->exitMapUpdateScope( false );
					mWidget->updateMap( );
					mWidget->requestRepaint( );
					//mWidget->draw( false );
				break;
			}
		}

	private:
		MapWidget* mWidget;
	};

	//=========================================================================


	//-------------------------------------------------------------------------
	MapWidget::MapWidget( /*MapSource* source,*/ int x, int y, int width, int height, Widget* _parent)
	//-------------------------------------------------------------------------
	:	Widget( x, y, width, height, _parent ),
		mCenterPositionLonLat( ),
		mCenterPositionPixels( ),
		mPanTargetPositionLonLat( ),
		mPanTargetPositionPixels( ),
		mMagnification( 0 ),
		mSource( /*source*/NULL ),
		//mCache( NULL ),
		mMapUpdateNesting( 0 ),
		mPrevCenter( ),
		mScreenImage( 0 ),
		mHasScale( true ),
		mPanTimerListener( NULL ),
		//mHasSmoothPanning( true ),
		mPanMode( MapWidgetPanMode_Smooth ),
		mFont( NULL ),
		mTimerRunning( false ),
		mPanMomentumX( 0 ),
		mPanMomentumY( 0 ),
		mPanFriction( 0.9f )
	{
		resetScreenImage( );
		mPanTimerListener = newobject( MapWidgetPanTimerListener, new MapWidgetPanTimerListener( this ) );
	}

	//-------------------------------------------------------------------------
	MapWidget::~MapWidget( )
	//-------------------------------------------------------------------------
	{
		deleteobject( mPanTimerListener );
	}

	//-------------------------------------------------------------------------
	void MapWidget::enterMapUpdateScope( )
	//-------------------------------------------------------------------------
	{
		if ( mMapUpdateNesting == 0 )
		{
			mPrevCenter = getCenterPositionPixels( );
			mPrevMagnification = getMagnification( );
		}
		mMapUpdateNesting ++;
	}

	//-------------------------------------------------------------------------
	void MapWidget::exitMapUpdateScope( bool immediate )
	//-------------------------------------------------------------------------
	{
		mMapUpdateNesting--;
		if ( mMapUpdateNesting == 0 )
		{
			if ( immediate )
			{
				mCenterPositionLonLat = mPanTargetPositionLonLat;
				mCenterPositionPixels = mPanTargetPositionPixels;
			}
			if ( getCenterPositionPixels( ) != mPrevCenter || getMagnification( ) != mPrevMagnification )
			{
				updateMap( );
			}
		}
	}

	//-------------------------------------------------------------------------
	void MapWidget::checkMapUpdateScope( )
	//-------------------------------------------------------------------------
	{
	}

	//-------------------------------------------------------------------------
	void MapWidget::setMapSource( MapSource* source )
	//-------------------------------------------------------------------------
	{
		mSource = source;
		updateMap( );
	}

	//-------------------------------------------------------------------------
	PixelCoordinate MapWidget::getCenterPositionPixels( ) const
	//-------------------------------------------------------------------------
	{
		//return mHasSmoothPanning ? mPanTargetPositionPixels : mCenterPositionPixels;

		switch( mPanMode)
		{
		case MapWidgetPanMode_Instant:
			return mCenterPositionPixels;
		case MapWidgetPanMode_Smooth:
			return mPanTargetPositionPixels;
		case MapWidgetPanMode_Momentum:
			return mCenterPositionPixels;
		}
		return PixelCoordinate( );
	}

	//-------------------------------------------------------------------------
	inline double Max( double x, double y )
	//-------------------------------------------------------------------------
	{
		return x > y ? x : y;
	}

	//-------------------------------------------------------------------------
	LonLat MapWidget::getCenterPosition( ) const
	//-------------------------------------------------------------------------
	{
		//return mHasSmoothPanning ? mPanTargetPositionLonLat : mCenterPositionLonLat;
		switch ( mPanMode )
		{
		case MapWidgetPanMode_Instant:
			return mCenterPositionLonLat;
		case MapWidgetPanMode_Smooth:
			return mPanTargetPositionLonLat;
		case MapWidgetPanMode_Momentum:
			return mCenterPositionLonLat;
		}
		//
		// Error
		//
		return LonLat( );
	}

	//-------------------------------------------------------------------------
	void MapWidget::setCenterPosition( LonLat position )
	//-------------------------------------------------------------------------
	{
		checkMapUpdateScope( );

		switch( mPanMode )
		{
		case MapWidgetPanMode_Instant:
			mCenterPositionLonLat = position;
			mCenterPositionPixels = position.toPixels( getMagnification( ) );
			break;
		case MapWidgetPanMode_Smooth:
			{
				mPanTargetPositionLonLat = position;
				mPanTargetPositionPixels = position.toPixels( getMagnification( ) );
				//
				// Make sure current position is nearby, so we only soft scroll less than one screen.
				//
				int deltaX = mPanTargetPositionPixels.getX( ) - mCenterPositionPixels.getX( );
				int deltaY = mPanTargetPositionPixels.getY( ) - mCenterPositionPixels.getY( );
				int width = getWidth( );
				int height = getHeight( );
				if ( width > 0 && height > 0 )
				{
					//
					// go directly to location if delta is more than 1/6 of widget size.
					//
					double factor = 6 * fabs( Max( (double)deltaX / getWidth( ), (double)deltaY / getHeight( ) ) );
					if ( factor > 1 )
					{
						mCenterPositionPixels = PixelCoordinate(	getMagnification( ),
																	mPanTargetPositionPixels.getX( ) - (int)( (double)deltaX / factor ),
																	mPanTargetPositionPixels.getY( ) - (int)( (double)deltaY / factor ) );
					}
					Environment::getEnvironment( ).addTimer( mPanTimerListener, PanIntervalMs, 0 );
				}
			}
			break;
		case MapWidgetPanMode_Momentum:
			mCenterPositionLonLat = position;
			mCenterPositionPixels = position.toPixels( getMagnification( ) );
			mPanMomentumX = 0;
			mPanMomentumY = 0;
			break;
		}
	}

	//-------------------------------------------------------------------------
	int MapWidget::getMagnification( ) const
	//-------------------------------------------------------------------------
	{
		return mMagnification;
	}

	//-------------------------------------------------------------------------
	void MapWidget::setMagnification( int magnification )
	//-------------------------------------------------------------------------
	{
		checkMapUpdateScope( );

		mMagnification = magnification;

		switch( mPanMode )
		{
		case MapWidgetPanMode_Instant:
			setCenterPosition( mCenterPositionLonLat );
			break;
		case MapWidgetPanMode_Smooth:
			setCenterPosition( mPanTargetPositionLonLat );
			break;
		case MapWidgetPanMode_Momentum:
			setCenterPosition( mCenterPositionLonLat );
			break;
		}
	}

	////-------------------------------------------------------------------------
	//bool MapWidget::getHasSmoothPanning( ) const
	////-------------------------------------------------------------------------
	//{
	//	return mHasSmoothPanning;
	//}

	////-------------------------------------------------------------------------
	//void MapWidget::setHasSmoothPanning( bool hasSmoothPanning )
	////-------------------------------------------------------------------------
	//{
	//	mHasSmoothPanning = hasSmoothPanning;
	//}

	//-------------------------------------------------------------------------
	MapWidgetPanMode MapWidget::getPanMode( ) const
	//-------------------------------------------------------------------------
	{
		return mPanMode;
	}

	//-------------------------------------------------------------------------
	void MapWidget::setPanMode( MapWidgetPanMode panMode )
	//-------------------------------------------------------------------------
	{
		mPanMode = panMode;
	}

	//-------------------------------------------------------------------------
	void MapWidget::setPanMomentum( int momentumX, int momentumY )
	//-------------------------------------------------------------------------
	{
		mPanMomentumX = momentumX;
		mPanMomentumY = momentumY;
		mPanStartTimeMs = maGetMilliSecondCount( );
	}
	
	//-------------------------------------------------------------------------
	float MapWidget::getPanFriction( ) const
	//-------------------------------------------------------------------------
	{
		return mPanFriction;
	}

	//-------------------------------------------------------------------------
	void MapWidget::setPanFriction( float friction )
	//-------------------------------------------------------------------------
	{
		mPanFriction = friction;
	}

	//-------------------------------------------------------------------------
	void MapWidget::tileReceived( MapCache* sender, MapTile* tile )
	//-------------------------------------------------------------------------
	{
		PixelCoordinate tilePx = tile->getCenter( ).toPixels( tile->getMagnification( ) );
		MAPoint2d pt = worldPixelToWidget( tilePx );
		MAHandle old = maSetDrawTarget( mScreenImage );
		const int tileSize = mSource->getTileSize( );

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
		//draw( false );
	}

	//-------------------------------------------------------------------------
	Point MapWidget::getActualPosition( )
	//-------------------------------------------------------------------------
	{
		Widget* p = this->getParent( );
		Point pos = getPosition( );

		while( p != 0 )
		{
			pos.x += p->getPosition( ).x;
			pos.y += p->getPosition( ).y;
			p = p->getParent( );
		}
		return pos;
	}

	//-------------------------------------------------------------------------
	void MapWidget::drawWidget( )
	//-------------------------------------------------------------------------
	{
		if ( !this->enabled ) 
			return;

		Point widgetPos = getPosition( );
		//
		// Save clip
		//
		(void)Gfx_pushClipRect( widgetPos.x, widgetPos.y, getWidth( ), getHeight( ) );
		//
		// Draw map image
		//
		Gfx_drawImage( mScreenImage, widgetPos.x, widgetPos.y );
		//
		// Let subclass draw its overlay
		//
		drawOverlay( );
		
		//
		// Draw scale indicator
		//
		if ( ShowPixelScale && mHasScale )
		{
			const int scaleWidth = 80;
			const int scaleX = widgetPos.x + getWidth( ) - scaleWidth - 5;
			const int scaleY = widgetPos.y + 5;
			int lineThickness = 3;
			const int crossbarHeight = 7;
			float scaleFrac = (float)( mMagnification - mSource->getMagnificationMin( ) ) / ( mSource->getMagnificationMax( ) - mSource->getMagnificationMin( ) );

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
				p1.toMeters( meterX1, meterY1 );
				p2.toMeters( meterX2, meterY2 );
				double offsetX = meterX2 - meterX1;
				offsetX *= cos( fabs( p2.lat * PI / 180 ) );
				char buffer[100];
				sprintf( buffer, "%5.2f m/px", offsetX );

				if ( mFont != NULL )
					mFont->drawString( buffer, scaleX, scaleY + 5 );
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
			//static const int textWidth = 100;
			//static const int textHeight = 12;
			char buffer[100];

			switch( mPanMode )
			{
			case MapWidgetPanMode_Instant:
				sprintf( buffer, "%-3.4f %-3.4f", mCenterPositionLonLat.lon, mCenterPositionLonLat.lat );
			case MapWidgetPanMode_Smooth:
				sprintf( buffer, "%-3.4f %-3.4f", mPanTargetPositionLonLat.lon, mPanTargetPositionLonLat.lat );
			case MapWidgetPanMode_Momentum:
				sprintf( buffer, "%-3.4f %-3.4f", mCenterPositionLonLat.lon, mCenterPositionLonLat.lat );
			}
			maSetColor( 0x000000 );

			if ( mFont != NULL )
				mFont->drawString( buffer, widgetPos.x, widgetPos.y );
			else
				Gfx_drawText( widgetPos.x, widgetPos.y, buffer );
		}
		//
		// Restore original clip
		//
		(void)Gfx_popClipRect( );
	}

	//-------------------------------------------------------------------------
	void MapWidget::drawOverlay( )
	//-------------------------------------------------------------------------
	{
	}

	//-------------------------------------------------------------------------
	void MapWidget::updateMap( )
	//-------------------------------------------------------------------------
	{
		if ( getWidth( ) <= 0 || getHeight( ) <= 0 ) 
			return;
		//
		// Clear screen image
		//
		MAHandle old = maSetDrawTarget( mScreenImage );
		maSetColor( 0xc0c0c0 );
		maFillRect( 0, 0, getWidth( ), getHeight( ) );
		maSetDrawTarget( old );
		//
		// Request tiles
		//
		// We want to use currently displayed center position here, so we bypass getCenterPosition( ).
		MapCache::get( )->requestTiles( this, mSource, LonLat( mCenterPositionPixels ), mMagnification, getWidth( ), getHeight( ) );
	}

	//-------------------------------------------------------------------------
	MAPoint2d MapWidget::worldPixelToWidget( PixelCoordinate wpx )
	//-------------------------------------------------------------------------
	{
		MAPoint2d pt;
		PixelCoordinate screenPx = mCenterPositionPixels;

		pt.x =    wpx.getX( ) - screenPx.getX( )   + ( getWidth( ) >> 1 );
		pt.y = -( wpx.getY( ) - screenPx.getY( ) ) + ( getHeight( ) >> 1 );

		return pt;
	}

	//-------------------------------------------------------------------------
	PixelCoordinate MapWidget::widgetToWorldPixel( MAPoint2d pt )
	//-------------------------------------------------------------------------
	{
		PixelCoordinate screenPx = mCenterPositionPixels;
		return PixelCoordinate( mMagnification,
								(int)( pt.x + 0.5 - 0.5 * getWidth( ) + screenPx.getX( ) ),
								(int)( -( pt.y + 0.5 - 0.5 * getHeight( ) - screenPx.getY( ) ) ) );
	}

	//-------------------------------------------------------------------------
	static double clamp( double x, double min, double max )
	//-------------------------------------------------------------------------
	{
		return x < min ? min : x > max ? max : x;
	}

	//-------------------------------------------------------------------------
	void MapWidget::zoomIn( )
	//-------------------------------------------------------------------------
	{
		if ( mMagnification < mSource->getMagnificationMax( ) )
		{
			mMagnification++;

			switch( mPanMode )
			{
			case MapWidgetPanMode_Instant:
				mCenterPositionPixels = mCenterPositionLonLat.toPixels( mMagnification );
				break;
			case MapWidgetPanMode_Smooth:
				mCenterPositionPixels = mCenterPositionLonLat.toPixels( mMagnification );
				mPanTargetPositionPixels = mPanTargetPositionLonLat.toPixels( mMagnification );
				break;
			case MapWidgetPanMode_Momentum:
				mCenterPositionPixels = mCenterPositionLonLat.toPixels( mMagnification );
				break;
			}
		}
	}

	//-------------------------------------------------------------------------
	void MapWidget::zoomOut( )
	//-------------------------------------------------------------------------
	{
		if ( mMagnification > mSource->getMagnificationMin( ) )
		{
			mMagnification--;

			switch( mPanMode )
			{
			case MapWidgetPanMode_Instant:
				mCenterPositionPixels = mCenterPositionLonLat.toPixels( mMagnification );
				break;
			case MapWidgetPanMode_Smooth:
				mCenterPositionPixels = mCenterPositionLonLat.toPixels( mMagnification );
				mPanTargetPositionPixels = mPanTargetPositionLonLat.toPixels( mMagnification );
				break;
			case MapWidgetPanMode_Momentum:
				mCenterPositionPixels = mCenterPositionLonLat.toPixels( mMagnification );
				break;
			}
		}
	}

	//-------------------------------------------------------------------------
	void MapWidget::scroll( MapWidgetScrollDirection direction, bool largeStep)
	//-------------------------------------------------------------------------
	{
		switch( mPanMode )
		{
		case MapWidgetPanMode_Instant:
		case MapWidgetPanMode_Smooth:
			{
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
			break;
		case MapWidgetPanMode_Momentum:
			{
				int momentumX = mPanMomentumX;
				int momentumY = mPanMomentumY;
				const int momentum = SmallScrollStep;
				switch( direction )
				{
				case SCROLLDIRECTION_NORTH:  momentumY += momentum;break;
				case SCROLLDIRECTION_SOUTH: momentumY -= momentum; break;
				case SCROLLDIRECTION_EAST: momentumX -= momentum; break;
				case SCROLLDIRECTION_WEST: momentumX += momentum; break;
				}
				setPanMomentum( momentumX, momentumY );
				break;
			}
		}
	}

	//-------------------------------------------------------------------------
	bool MapWidget::handleKeyPress( int keyCode )
	//-------------------------------------------------------------------------
	{
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
		return true;
	}

	//-------------------------------------------------------------------------
	void MapWidget::setWidth( int width )
	//-------------------------------------------------------------------------
	{
		Widget::setWidth( width );
		resetScreenImage( );
	}

	//-------------------------------------------------------------------------
	void MapWidget::setHeight( int height )
	//-------------------------------------------------------------------------
	{
		Widget::setHeight( height );
		resetScreenImage( );
	}

	//-------------------------------------------------------------------------
	void MapWidget::resetScreenImage( )
	//-------------------------------------------------------------------------
	{
		if ( mScreenImage != 0 )
			maDestroyObject( mScreenImage );

		if ( getWidth( ) > 0 && getHeight( ) > 0 )
		{
			mScreenImage = maCreatePlaceholder( );
			maCreateDrawableImage( mScreenImage, getWidth( ), getHeight( ) );
		}
	}
}
