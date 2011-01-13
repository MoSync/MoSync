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
//#include <MAUI/Image.h>
#include "MapViewport.h"
#include "MapCache.h"
#include "MapSource.h"
#include "DebugPrintf.h"

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
	static const int PanIntervalMs = 40;
	static const double SmoothPanTension = 0.2;
	static const float DefaultFriction = 0.9f;
	static const int SmallScrollStep = 30; // pixels to scroll if not full page
	static const int CrossSize = 4;

	//=========================================================================
	enum MapViewportMomentumState
	//=========================================================================
	{
		MapViewportMomentumState_None,
		MapViewportMomentumState_Initializing,
		MapViewportMomentumstate_Initialized,
		MapViewportMomentumState_Gliding
	};

	//=========================================================================
	class MapViewportPanTimerListener : public TimerListener
	//=========================================================================
	{
	public:
		//---------------------------------------------------------------------
		MapViewportPanTimerListener( MapViewport* viewport ) :
		//---------------------------------------------------------------------
			mViewport( viewport ),
			mFriction( DefaultFriction ),
			mMomentum( MAPoint2d( ) )
		{
		}

		//---------------------------------------------------------------------
		void runTimerEvent( )
		//---------------------------------------------------------------------
		{
			switch( mViewport->mPanMode )
			{
			case MapViewportPanMode_Instant:
				// we shouldn't be here.
				break;
			case MapViewportPanMode_Smooth:
				{
					PixelCoordinate currentPix = mViewport->mCenterPositionPixels;
					PixelCoordinate targetPix = mViewport->mPanTargetPositionPixels;
					double offsetX = targetPix.getX( ) - currentPix.getX( );
					double offsetY = targetPix.getY( ) - currentPix.getY( );

					if( fabs( offsetX ) <= 2 && fabs( offsetY ) <= 2 )
					{
						//
						// Done panning, stop timer and repaint
						//
						//mViewport->enterMapUpdateScope( );
						//mViewport->exitMapUpdateScope( true );

						mViewport->updateMap( );
						//mViewport->requestRepaint( );
						//mViewport->mListener->viewportUpdated( mViewport );
						return;
					}
					PixelCoordinate newPix = PixelCoordinate(	mViewport->getMagnification( ),
																(int)( currentPix.getX( ) + SmoothPanTension * offsetX ),
																(int)( currentPix.getY( ) + SmoothPanTension * offsetY ) );

					//mViewport->enterMapUpdateScope( );
					mViewport->mCenterPositionPixels = newPix;
					mViewport->mCenterPositionLonLat = LonLat( newPix );
					//mViewport->exitMapUpdateScope( false );
					mViewport->updateMap( );

					//mViewport->requestRepaint( );
					//mViewport->mListener->viewportUpdated( mViewport );
				}
				break;
			case MapViewportPanMode_Momentum:

				//
				// Calc offset to from currrent location to target location
				//
				PixelCoordinate prevCenterPix = mViewport->mCenterPositionPixels;
				PixelCoordinate targetPix = mViewport->mPanTargetPositionPixels;
				double offsetX = targetPix.getX( ) - prevCenterPix.getX( );
				double offsetY = targetPix.getY( ) - prevCenterPix.getY( );

				// TODO: shut down timer when movement has stopped
				if ( false ) // fabs( offsetX ) <= 2 && fabs( offsetY ) <= 2 )
				{
					//
					// Done panning, stop timer and repaint
					//
					//mViewport->enterMapUpdateScope( );
					//mViewport->exitMapUpdateScope( true );
					mViewport->updateMap( );

					//mViewport->requestRepaint( );
					//mViewport->mListener->viewportUpdated( mViewport );
					return;
				}
				//
				// Set new position based on offset to target and pan tension
				//
				//mViewport->enterMapUpdateScope( );

				if ( mGliding )
				{
					if ( mMomentum.x == 0  && mMomentum.y == 0 )
					{
						//
						// Done panning, stop timer and repaint
						//
						Environment::getEnvironment( ).removeTimer( this );
						mViewport->mHasTimer = false;
						//mViewport->enterMapUpdateScope( );
						//mViewport->exitMapUpdateScope( true );
						mViewport->updateMap( );
						//mViewport->requestRepaint( );
						//mViewport->mListener->viewportUpdated( mViewport );
						return;
					}
					//
					// Use momentum + friction
					//
					int currentTimeMs = maGetMilliSecondCount( );
					int deltaMs = currentTimeMs - mPanTime;
					//
					// Set new target position
					//
					mViewport->mCenterPositionPixels = PixelCoordinate(	mViewport->getMagnification( ),
																		(int)( mViewport->mCenterPositionPixels.getX( ) + 0.001f * mMomentum.x * deltaMs ),
																		(int)( mViewport->mCenterPositionPixels.getY( ) + 0.001f * mMomentum.y * deltaMs ) );
					mViewport->mPanTargetPositionPixels = mViewport->mCenterPositionPixels;
					mViewport->mCenterPositionLonLat = LonLat( mViewport->mCenterPositionPixels );
					mViewport->mPanTargetPositionLonLat = mViewport->mCenterPositionLonLat;
					//
					// Reduce momentum by friction
					//
					mMomentum.x = (int)( mFriction * mMomentum.x );
					mMomentum.y = (int)( mFriction * mMomentum.y );
				}
				else
				{
					//
					// use Tension
					//

					if ( fabs( offsetX ) < 2 && fabs( offsetY ) < 2 )
					{
						//
						// Done panning, stop timer and repaint
						//
						Environment::getEnvironment( ).removeTimer( this );
						mViewport->mHasTimer = false;
						//mViewport->enterMapUpdateScope( );
						//mViewport->exitMapUpdateScope( true );
						mViewport->updateMap( );
						//mViewport->requestRepaint( );
						//mViewport->mListener->viewportUpdated( mViewport );
						return;
					}

					mViewport->mCenterPositionPixels = PixelCoordinate(	mViewport->getMagnification( ),
																		(int)( prevCenterPix.getX( ) + SmoothPanTension * offsetX ),
																		(int)( prevCenterPix.getY( ) + SmoothPanTension * offsetY ) );
					mViewport->mCenterPositionLonLat = LonLat( mViewport->mCenterPositionPixels );
				}
				
				//mViewport->exitMapUpdateScope( false );
				mViewport->updateMap( );
				//mViewport->requestRepaint( );
				//mViewport->mListener->viewportUpdated( mViewport );

				if ( !mGliding )
				{
					//
					// Have to calc momentum after center position has been updated
					//
					// Calc momentum based on speed of map movement
					//
					int currentTime = maGetMilliSecondCount( );
					int deltaMs = currentTime - mPanTime;
					//
					// Calc momentum
					//
					if ( mMomentumState != MapViewportMomentumState_None )
					{
						int mx = mViewport->mCenterPositionPixels.getX( ) - prevCenterPix.getX( );
						int my = mViewport->mCenterPositionPixels.getY( ) - prevCenterPix.getY( );
						mMomentum.x = deltaMs > 0 ? 1000 * mx / deltaMs : 0;
						mMomentum.y = deltaMs > 0 ? 1000 * my / deltaMs : 0;
					}
					//DebugPrintf( "setCenterPosition: xy=%d %d, moment=%d %d\n", mPanTargetPositionPixels.getX( ), mPanTargetPositionPixels.getY( ), mMomentum.x, mMomentum.y );
					//
					// Store point metrics
					//
					mPanTime = currentTime;

					switch ( mMomentumState )
					{
					case MapViewportMomentumState_None:
						//
						// Init momentum gliding
						//
						mMomentumState = MapViewportMomentumState_Initializing;
						break;
					case MapViewportMomentumState_Initializing:
					case MapViewportMomentumState_Gliding:
						//
						// Continue gliding state
						//
						mMomentumState = MapViewportMomentumState_Gliding;
						break;
					}
				}
				break;
			}
		}

		bool mGliding;
		MAPoint2d mMomentum;
		float mFriction;
		MapViewportMomentumState mMomentumState;

	private:
		MapViewport* mViewport;
		int mPanTime;
	};

	//=========================================================================
	class MapViewportStressTestListener : public TimerListener
	//=========================================================================
	{
	public:
		//---------------------------------------------------------------------
		MapViewportStressTestListener( MapViewport* viewport ) :
		//---------------------------------------------------------------------
			mViewport( viewport ),
			lon( 17 )
		{
		}

		//---------------------------------------------------------------------
		void runTimerEvent( )
		//---------------------------------------------------------------------
		{
			lon += 0.01f;
			mViewport->setCenterPosition( LonLat( lon, 59 ), false, false );
			DebugPrintf( "StressTest: %f\n", lon );
		}

	private:
		MapViewport* mViewport;
		float lon;
	};

	//=========================================================================


	//-------------------------------------------------------------------------
	MapViewport::MapViewport( )
	//-------------------------------------------------------------------------
	:	//Viewport( x, y, width, height, _parent ),
		mCenterPositionLonLat( ),
		mCenterPositionPixels( ),
		mPanTargetPositionLonLat( ),
		mPanTargetPositionPixels( ),
		mMagnification( 0 ),
		mSource( NULL ),
		mMapUpdateNesting( 0 ),
		mPrevCenter( ),
		//mScreenImage( 0 ),
		mHasScale( true ),
		mPanTimerListener( NULL ),
		mPanMode( MapViewportPanMode_Smooth ),
		mFont( NULL )
	{
		//resetScreenImage( );
		mPanTimerListener = newobject( MapViewportPanTimerListener, new MapViewportPanTimerListener( this ) );
	}

	//-------------------------------------------------------------------------
	MapViewport::~MapViewport( )
	//-------------------------------------------------------------------------
	{
		if ( mHasTimer )
			Environment::getEnvironment( ).removeTimer( mPanTimerListener );
		deleteobject( mPanTimerListener );
	}

	////-------------------------------------------------------------------------
	//void MapViewport::enterMapUpdateScope( )
	////-------------------------------------------------------------------------
	//{
	//	if ( mMapUpdateNesting == 0 )
	//	{
	//		mPrevCenter = getCenterPositionPixels( );
	//		mPrevMagnification = getMagnification( );
	//	}
	//	mMapUpdateNesting ++;
	//}

	////-------------------------------------------------------------------------
	//void MapViewport::exitMapUpdateScope( bool immediate )
	////-------------------------------------------------------------------------
	//{
	//	mMapUpdateNesting--;
	//	if ( mMapUpdateNesting == 0 )
	//	{
	//		if ( immediate )
	//		{
	//			mCenterPositionLonLat = mPanTargetPositionLonLat;
	//			mCenterPositionPixels = mPanTargetPositionPixels;
	//		}
	//		if ( getCenterPositionPixels( ) != mPrevCenter || getMagnification( ) != mPrevMagnification )
	//		{
	//			updateMap( );
	//		}
	//	}
	//}

	////-------------------------------------------------------------------------
	//void MapViewport::checkMapUpdateScope( )
	////-------------------------------------------------------------------------
	//{
	//}

	//-------------------------------------------------------------------------
	void MapViewport::setMapSource( MapSource* source )
	//-------------------------------------------------------------------------
	{
		mSource = source;
		updateMap( );
	}

	//-------------------------------------------------------------------------
	PixelCoordinate MapViewport::getCenterPositionPixels( ) const
	//-------------------------------------------------------------------------
	{
		switch( mPanMode)
		{
		case MapViewportPanMode_Instant:
			return mCenterPositionPixels;
		case MapViewportPanMode_Smooth:
		case MapViewportPanMode_Momentum:
			return mPanTargetPositionPixels;
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
	LonLat MapViewport::getCenterPosition( ) const
	//-------------------------------------------------------------------------
	{
		switch ( mPanMode )
		{
		case MapViewportPanMode_Instant:
			return mCenterPositionLonLat;
		case MapViewportPanMode_Smooth:
		case MapViewportPanMode_Momentum:
			return mPanTargetPositionLonLat;
		}
		//
		// Error
		//
		return LonLat( );
	}

	//-------------------------------------------------------------------------
	void MapViewport::setCenterPosition( LonLat position, int magnification, bool immediate, bool isPointerEvent )
	//-------------------------------------------------------------------------
	{
		//checkMapUpdateScope( );

		if ( immediate )
		{
			mMagnification = magnification;
			mCenterPositionLonLat = mPanTargetPositionLonLat = position;
			mCenterPositionPixels = mPanTargetPositionPixels = position.toPixels( getMagnification( ) );

			mPanTimerListener->mMomentumState = MapViewportMomentumState_None;
			mPanTimerListener->mMomentum = MAPoint2d( );
			return;
		}

		switch( mPanMode )
		{
		case MapViewportPanMode_Instant:
			mMagnification = magnification;
			mCenterPositionLonLat = position;
			mCenterPositionPixels = position.toPixels( getMagnification( ) );
			break;
		case MapViewportPanMode_Smooth:
		case MapViewportPanMode_Momentum:
			{
				mMagnification = magnification;
				PixelCoordinate prevTarget = mPanTargetPositionPixels;
				mPanTargetPositionLonLat = position;
				mPanTargetPositionPixels = position.toPixels( getMagnification( ) );
				//
				// Make sure current position is nearby, so we only soft scroll less than one screen.
				//
				int width = getWidth( );
				int height = getHeight( );
				if ( width > 0 && height > 0 )
				{
					int deltaX = mPanTargetPositionPixels.getX( ) - mCenterPositionPixels.getX( );
					int deltaY = mPanTargetPositionPixels.getY( ) - mCenterPositionPixels.getY( );
					//
					// go directly to location if delta is more than 1/6 of viewport size.
					//
					double factor = 6 * fabs( Max( (double)deltaX / getWidth( ), (double)deltaY / getHeight( ) ) );
					if ( factor > 1 )
					{
						mCenterPositionPixels = PixelCoordinate(	getMagnification( ),
																	mPanTargetPositionPixels.getX( ) - (int)( (double)deltaX / factor ),
																	mPanTargetPositionPixels.getY( ) - (int)( (double)deltaY / factor ) );
					}
					if ( !isPointerEvent )
						mPanTimerListener->mMomentumState = MapViewportMomentumState_None;
				}
				Environment::getEnvironment( ).addTimer( mPanTimerListener, PanIntervalMs, 0 );
				mHasTimer = true;
			}
			break;
		}
	}

	//-------------------------------------------------------------------------
	void MapViewport::setCenterPosition( LonLat position, bool immediate, bool isPointerEvent )
	//-------------------------------------------------------------------------
	{
		setCenterPosition( position, mMagnification, immediate, isPointerEvent );
	}

	//-------------------------------------------------------------------------
	void MapViewport::startGlide( )
	//-------------------------------------------------------------------------
	{
		mPanTimerListener->mGliding = true;
	}

	//-------------------------------------------------------------------------
	void MapViewport::stopGlide( )
	//-------------------------------------------------------------------------
	{
		mPanTimerListener->mGliding = false;
		mPanTimerListener->mMomentumState = MapViewportMomentumState_None;
		mPanTimerListener->mMomentum = MAPoint2d( );
		//
		// make sure we lock map in place
		//
		//enterMapUpdateScope( );
		mPanTargetPositionPixels = mCenterPositionPixels;
		//exitMapUpdateScope( true );
		updateMap( );
		//requestRepaint( );
		//mListener->viewportUpdated( this );
	}

	//-------------------------------------------------------------------------
	void MapViewport::testMomentumPanning( )
	//-------------------------------------------------------------------------
	{
		mPanTimerListener->mMomentumState = MapViewportMomentumState_None;
		setCenterPosition( LonLat( 18.07, 59.33 ), false, true );
		int currentTimeMs = maGetMilliSecondCount( );
		//TODO: mPanTimerListener->mPrevTimeMs = currentTimeMs - 100;
		setCenterPosition( LonLat( 18.0695, 59.33 ), false, true );
	}

	MapViewportStressTestListener* stressListener;

	//-------------------------------------------------------------------------
	void MapViewport::stressTest( )
	//-------------------------------------------------------------------------
	{
		stressListener = newobject( MapViewportStressTestListener, new MapViewportStressTestListener( this ) );
		Environment::getEnvironment( ).addTimer( stressListener, 400, 0 );
		setMagnification( 14 );
	}

	//-------------------------------------------------------------------------
	int MapViewport::getMagnification( ) const
	//-------------------------------------------------------------------------
	{
		return mMagnification;
	}

	//-------------------------------------------------------------------------
	void MapViewport::setMagnification( int magnification )
	//-------------------------------------------------------------------------
	{
		//checkMapUpdateScope( );

		mMagnification = magnification;

		switch( mPanMode )
		{
		case MapViewportPanMode_Instant:
			setCenterPosition( mCenterPositionLonLat, false, false );
			break;
		case MapViewportPanMode_Smooth:
		case MapViewportPanMode_Momentum:
			mPanTimerListener->mMomentumState = MapViewportMomentumState_None;
			setCenterPosition( mPanTargetPositionLonLat, false, false );
			break;
		}
	}

	//-------------------------------------------------------------------------
	MapViewportPanMode MapViewport::getPanMode( ) const
	//-------------------------------------------------------------------------
	{
		return mPanMode;
	}

	//-------------------------------------------------------------------------
	void MapViewport::setPanMode( MapViewportPanMode panMode )
	//-------------------------------------------------------------------------
	{
		mPanMode = panMode;
	}

	//-------------------------------------------------------------------------
	float MapViewport::getFriction( ) const
	//-------------------------------------------------------------------------
	{
		return mPanTimerListener->mFriction;
	}

	//-------------------------------------------------------------------------
	void MapViewport::setFriction( float friction )
	//-------------------------------------------------------------------------
	{
		mPanTimerListener->mFriction = friction;
	}

	bool inDraw = false;

	//-------------------------------------------------------------------------
	void MapViewport::tileReceived( MapCache* sender, MapTile* tile )
	//-------------------------------------------------------------------------
	{
		//PixelCoordinate tilePx = tile->getCenter( ).toPixels( tile->getMagnification( ) );
		//MAPoint2d pt = worldPixelToViewport( tilePx );
		//MAHandle old = maSetDrawTarget( mScreenImage );
		//const int tileSize = mSource->getTileSize( );
		//MARect rect;
		//rect.height = tileSize;
		//rect.width = tileSize;
		//rect.top = 0;
		//rect.left = 0;
		//MAPoint2d dest;
		//dest.x = pt.x - tileSize / 2;
		//dest.y = pt.y - tileSize / 2;
		//maDrawImageRegion( tile->getImage( ), &rect, &dest, TRANS_NONE );
		//maSetDrawTarget( old );
		//requestRepaint( );

		// TODO!!!

		// draw to current gfx

		if ( inDraw )
		{
			//
			// draw context is set up, draw tile to viewport
			//
			PixelCoordinate tilePx = tile->getCenter( ).toPixels( tile->getMagnification( ) );
			MAPoint2d pt = worldPixelToViewport( tilePx );
			const int tileSize = mSource->getTileSize( );
			Gfx_drawImage( tile->getImage( ),  pt.x - tileSize / 2, pt.y - tileSize / 2 );
		}
		else
		{
			// notify client that update is needed
			mListener->viewportUpdated( this );
		}

	}

	////-------------------------------------------------------------------------
	//Point MapViewport::getActualPosition( )
	////-------------------------------------------------------------------------
	//{
	//	Viewport* p = this->getParent( );
	//	Point pos = getPosition( );

	//	while( p != 0 )
	//	{
	//		pos.x += p->getPosition( ).x;
	//		pos.y += p->getPosition( ).y;
	//		p = p->getParent( );
	//	}
	//	return pos;
	//}

	//-------------------------------------------------------------------------
	void MapViewport::drawViewport( Point origin )
	//-------------------------------------------------------------------------
	{
		inDraw = true;
		//
		// Save clip
		//
		(void)Gfx_pushClipRect( origin.x, origin.y, getWidth( ), getHeight( ) );
		//
		// Draw available tiles
		//
		MapCache::get( )->requestTiles( this, mSource, LonLat( mCenterPositionPixels ), mMagnification, getWidth( ), getHeight( ) );
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
			const int scaleX = origin.x + getWidth( ) - scaleWidth - 5;
			const int scaleY = origin.y + 5;
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
			const int centerX = origin.x + getWidth( ) / 2;
			const int centerY = origin.y + getHeight( ) / 2;

			maSetColor( 0x000000 );

			Gfx_fillRect( centerX, centerY - CrossSize, 1, CrossSize * 2 + 1 );
			Gfx_fillRect( centerX - CrossSize, centerY, CrossSize * 2 + 1, 1 );
		}
		//
		// Draw latitude, longitude
		//
		if ( ShowLatLon )
		{
			char buffer[100];

			switch( mPanMode )
			{
			case MapViewportPanMode_Instant:
				sprintf( buffer, "%-3.4f %-3.4f", mCenterPositionLonLat.lon, mCenterPositionLonLat.lat );
			case MapViewportPanMode_Smooth:
			case MapViewportPanMode_Momentum:
				sprintf( buffer, "%-3.4f %-3.4f", mCenterPositionLonLat.lon, mCenterPositionLonLat.lat );
			}
			maSetColor( 0x000000 );

			if ( mFont != NULL )
				mFont->drawString( buffer, origin.x, origin.y );
			else
				Gfx_drawText( origin.x, origin.y, buffer );
		}
		//
		// Draw debug info
		//
		if ( ShowLatLon )
		{
			char buffer[100];
			sprintf( buffer, "Tiles: %d Cache: %d", this->mSource->getTileCount( ), MapCache::get( )->size( ) );
			maSetColor( 0x000000 );

			if ( mFont != NULL )
				mFont->drawString( buffer, origin.x, origin.y + 20 );
			else
				Gfx_drawText( origin.x, origin.y + 20, buffer );
		}
		//
		// Restore original clip
		//
		(void)Gfx_popClipRect( );

		inDraw = false;
	}

	//-------------------------------------------------------------------------
	void MapViewport::drawOverlay( )
	//-------------------------------------------------------------------------
	{
	}

	//-------------------------------------------------------------------------
	void MapViewport::updateMap( )
	//-------------------------------------------------------------------------
	{
		if ( getWidth( ) <= 0 || getHeight( ) <= 0 ) 
			return;
		//
		// Clear screen image
		//
		//MAHandle old = maSetDrawTarget( mScreenImage );
		//maSetColor( 0xc0c0c0 );
		//maFillRect( 0, 0, getWidth( ), getHeight( ) );
		//maSetDrawTarget( old );
		//
		// Request tiles
		//
		// We want to use currently displayed center position here, so we bypass getCenterPosition( ).
		//
		MapCache::get( )->requestTiles( this, mSource, LonLat( mCenterPositionPixels ), mMagnification, getWidth( ), getHeight( ) );
	}

	//-------------------------------------------------------------------------
	MAPoint2d MapViewport::worldPixelToViewport( PixelCoordinate wpx )
	//-------------------------------------------------------------------------
	{
		MAPoint2d pt;
		PixelCoordinate screenPx = mCenterPositionPixels;

		pt.x =    wpx.getX( ) - screenPx.getX( )   + ( getWidth( ) >> 1 );
		pt.y = -( wpx.getY( ) - screenPx.getY( ) ) + ( getHeight( ) >> 1 );

		return pt;
	}

	//-------------------------------------------------------------------------
	PixelCoordinate MapViewport::viewportToWorldPixel( MAPoint2d pt )
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
	void MapViewport::zoomIn( )
	//-------------------------------------------------------------------------
	{
		if ( mMagnification < mSource->getMagnificationMax( ) )
		{
			mMagnification++;

			switch( mPanMode )
			{
			case MapViewportPanMode_Instant:
				mCenterPositionPixels = mCenterPositionLonLat.toPixels( mMagnification );
				break;
			case MapViewportPanMode_Smooth:
			case MapViewportPanMode_Momentum:
				mPanTimerListener->mMomentumState = MapViewportMomentumState_None;
				mCenterPositionPixels = mCenterPositionLonLat.toPixels( mMagnification );
				mPanTargetPositionPixels = mPanTargetPositionLonLat.toPixels( mMagnification );
				break;
			}
		}
	}

	//-------------------------------------------------------------------------
	void MapViewport::zoomOut( )
	//-------------------------------------------------------------------------
	{
		if ( mMagnification > mSource->getMagnificationMin( ) )
		{
			mMagnification--;

			switch( mPanMode )
			{
			case MapViewportPanMode_Instant:
				mCenterPositionPixels = mCenterPositionLonLat.toPixels( mMagnification );
				break;
			case MapViewportPanMode_Smooth:
			case MapViewportPanMode_Momentum:
				mPanTimerListener->mMomentumState = MapViewportMomentumState_None;
				mCenterPositionPixels = mCenterPositionLonLat.toPixels( mMagnification );
				mPanTargetPositionPixels = mPanTargetPositionLonLat.toPixels( mMagnification );
				break;
			}
		}
	}

	//-------------------------------------------------------------------------
	void MapViewport::scroll( MapViewportScrollDirection direction, bool largeStep )
	//-------------------------------------------------------------------------
	{
		switch( mPanMode )
		{
		case MapViewportPanMode_Instant:
		case MapViewportPanMode_Smooth:
		case MapViewportPanMode_Momentum:
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
				setCenterPosition( newPos, false, false );
			}
			break;
		}
	}

	//-------------------------------------------------------------------------
	bool MapViewport::handleKeyPress( int keyCode )
	//-------------------------------------------------------------------------
	{
		//enterMapUpdateScope( );
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
		//exitMapUpdateScope( false );
		return ret;
	}

	//-------------------------------------------------------------------------
	bool MapViewport::handleKeyRelease( int keyCode )
	//-------------------------------------------------------------------------
	{
		return true;
	}

	//-------------------------------------------------------------------------
	void MapViewport::setWidth( int width )
	//-------------------------------------------------------------------------
	{
		//Viewport::setWidth( width );
		mWidth = width;
		//resetScreenImage( );
	}

	//-------------------------------------------------------------------------
	void MapViewport::setHeight( int height )
	//-------------------------------------------------------------------------
	{
		//Viewport::setHeight( height );
		mHeight = height;
		//resetScreenImage( );
	}

	////-------------------------------------------------------------------------
	//void MapViewport::resetScreenImage( )
	////-------------------------------------------------------------------------
	//{
	//	if ( mScreenImage != 0 )
	//		maDestroyObject( mScreenImage );

	//	if ( getWidth( ) > 0 && getHeight( ) > 0 )
	//	{
	//		mScreenImage = maCreatePlaceholder( );
	//		maCreateDrawableImage( mScreenImage, getWidth( ), getHeight( ) );
	//	}
	//}
}
