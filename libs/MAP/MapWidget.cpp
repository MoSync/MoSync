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
#include "DebugPrintf.h"

namespace MAP
{
	//
	// Appearance
	//
	//static const bool ShowPixelScale = true; // shows scale slider
	//static const bool ShowPixelScaleAsText = true; // shows meters/pixel scale (at latitude of screen center).
	//static const bool ShowHairlineCross = true;
	//static const bool ShowLatLon = true;
	//
	// Configuration
	//
	//static const int PanIntervalMs = 40;
	//static const double SmoothPanTension = 0.2;
	//static const float DefaultFriction = 0.9f;
	//static const int SmallScrollStep = 30; // pixels to scroll if not full page
	//static const int CrossSize = 4;

	////=========================================================================
	//enum MapWidgetMomentumState
	////=========================================================================
	//{
	//	MapWidgetMomentumState_None,
	//	MapWidgetMomentumState_Initializing,
	//	MapWidgetMomentumstate_Initialized,
	//	MapWidgetMomentumState_Gliding
	//};

	////=========================================================================
	//class MapWidgetPanTimerListener : public TimerListener
	////=========================================================================
	//{
	//public:
	//	//---------------------------------------------------------------------
	//	MapWidgetPanTimerListener( MapWidget* widget ) :
	//	//---------------------------------------------------------------------
	//		mWidget( widget ),
	//		mFriction( DefaultFriction ),
	//		mMomentum( MAPoint2d( ) )
	//	{
	//	}

	//	//---------------------------------------------------------------------
	//	void runTimerEvent( )
	//	//---------------------------------------------------------------------
	//	{
	//		switch( mWidget->mPanMode )
	//		{
	//		case MapWidgetPanMode_Instant:
	//			// we shouldn't be here.
	//			break;
	//		case MapWidgetPanMode_Smooth:
	//			{
	//				PixelCoordinate currentPix = mWidget->mCenterPositionPixels;
	//				PixelCoordinate targetPix = mWidget->mPanTargetPositionPixels;
	//				double offsetX = targetPix.getX( ) - currentPix.getX( );
	//				double offsetY = targetPix.getY( ) - currentPix.getY( );

	//				if( fabs( offsetX ) <= 2 && fabs( offsetY ) <= 2 )
	//				{
	//					//
	//					// Done panning, stop timer and repaint
	//					//
	//					mWidget->enterMapUpdateScope( );
	//					mWidget->exitMapUpdateScope( true );
	//					mWidget->updateMap( );
	//					mWidget->requestRepaint( );
	//					return;
	//				}
	//				PixelCoordinate newPix = PixelCoordinate(	mWidget->getMagnification( ),
	//															(int)( currentPix.getX( ) + SmoothPanTension * offsetX ),
	//															(int)( currentPix.getY( ) + SmoothPanTension * offsetY ) );
	//				mWidget->enterMapUpdateScope( );
	//				mWidget->mCenterPositionPixels = newPix;
	//				mWidget->mCenterPositionLonLat = LonLat( newPix );
	//				mWidget->exitMapUpdateScope( false );
	//				mWidget->updateMap( );
	//				mWidget->requestRepaint( );
	//			}
	//			break;
	//		case MapWidgetPanMode_Momentum:

	//			//
	//			// Calc offset to from currrent location to target location
	//			//
	//			PixelCoordinate prevCenterPix = mWidget->mCenterPositionPixels;
	//			PixelCoordinate targetPix = mWidget->mPanTargetPositionPixels;
	//			double offsetX = targetPix.getX( ) - prevCenterPix.getX( );
	//			double offsetY = targetPix.getY( ) - prevCenterPix.getY( );

	//			// TODO: shut down timer when movement has stopped
	//			if ( false ) // fabs( offsetX ) <= 2 && fabs( offsetY ) <= 2 )
	//			{
	//				//
	//				// Done panning, stop timer and repaint
	//				//
	//				mWidget->enterMapUpdateScope( );
	//				mWidget->exitMapUpdateScope( true );
	//				mWidget->updateMap( );
	//				mWidget->requestRepaint( );
	//				return;
	//			}
	//			//
	//			// Set new position based on offset to target and pan tension
	//			//
	//			mWidget->enterMapUpdateScope( );

	//			if ( mGliding )
	//			{
	//				if ( mMomentum.x == 0  && mMomentum.y == 0 )
	//				{
	//					//
	//					// Done panning, stop timer and repaint
	//					//
	//					Environment::getEnvironment( ).removeTimer( this );
	//					mWidget->mHasTimer = false;
	//					mWidget->enterMapUpdateScope( );
	//					mWidget->exitMapUpdateScope( true );
	//					mWidget->updateMap( );
	//					mWidget->requestRepaint( );
	//					return;
	//				}
	//				//
	//				// Use momentum + friction
	//				//
	//				int currentTimeMs = maGetMilliSecondCount( );
	//				int deltaMs = currentTimeMs - mPanTime;
	//				//
	//				// Set new target position
	//				//
	//				mWidget->mCenterPositionPixels = PixelCoordinate(	mWidget->getMagnification( ),
	//																	(int)( mWidget->mCenterPositionPixels.getX( ) + 0.001f * mMomentum.x * deltaMs ),
	//																	(int)( mWidget->mCenterPositionPixels.getY( ) + 0.001f * mMomentum.y * deltaMs ) );
	//				mWidget->mPanTargetPositionPixels = mWidget->mCenterPositionPixels;
	//				mWidget->mCenterPositionLonLat = LonLat( mWidget->mCenterPositionPixels );
	//				mWidget->mPanTargetPositionLonLat = mWidget->mCenterPositionLonLat;
	//				//
	//				// Reduce momentum by friction
	//				//
	//				mMomentum.x = (int)( mFriction * mMomentum.x );
	//				mMomentum.y = (int)( mFriction * mMomentum.y );
	//			}
	//			else
	//			{
	//				//
	//				// use Tension
	//				//

	//				if ( fabs( offsetX ) < 2 && fabs( offsetY ) < 2 )
	//				{
	//					//
	//					// Done panning, stop timer and repaint
	//					//
	//					Environment::getEnvironment( ).removeTimer( this );
	//					mWidget->mHasTimer = false;
	//					mWidget->enterMapUpdateScope( );
	//					mWidget->exitMapUpdateScope( true );
	//					mWidget->updateMap( );
	//					mWidget->requestRepaint( );
	//					return;
	//				}

	//				mWidget->mCenterPositionPixels = PixelCoordinate(	mWidget->getMagnification( ),
	//																	(int)( prevCenterPix.getX( ) + SmoothPanTension * offsetX ),
	//																	(int)( prevCenterPix.getY( ) + SmoothPanTension * offsetY ) );
	//				mWidget->mCenterPositionLonLat = LonLat( mWidget->mCenterPositionPixels );
	//			}
	//			
	//			mWidget->exitMapUpdateScope( false );
	//			mWidget->updateMap( );
	//			mWidget->requestRepaint( );

	//			if ( !mGliding )
	//			{
	//				//
	//				// Have to calc momentum after center position has been updated
	//				//
	//				// Calc momentum based on speed of map movement
	//				//
	//				int currentTime = maGetMilliSecondCount( );
	//				int deltaMs = currentTime - mPanTime;
	//				//
	//				// Calc momentum
	//				//
	//				if ( mMomentumState != MapWidgetMomentumState_None )
	//				{
	//					int mx = mWidget->mCenterPositionPixels.getX( ) - prevCenterPix.getX( );
	//					int my = mWidget->mCenterPositionPixels.getY( ) - prevCenterPix.getY( );
	//					mMomentum.x = deltaMs > 0 ? 1000 * mx / deltaMs : 0;
	//					mMomentum.y = deltaMs > 0 ? 1000 * my / deltaMs : 0;
	//				}
	//				//DebugPrintf( "setCenterPosition: xy=%d %d, moment=%d %d\n", mPanTargetPositionPixels.getX( ), mPanTargetPositionPixels.getY( ), mMomentum.x, mMomentum.y );
	//				//
	//				// Store point metrics
	//				//
	//				mPanTime = currentTime;

	//				switch ( mMomentumState )
	//				{
	//				case MapWidgetMomentumState_None:
	//					//
	//					// Init momentum gliding
	//					//
	//					mMomentumState = MapWidgetMomentumState_Initializing;
	//					break;
	//				case MapWidgetMomentumState_Initializing:
	//				case MapWidgetMomentumState_Gliding:
	//					//
	//					// Continue gliding state
	//					//
	//					mMomentumState = MapWidgetMomentumState_Gliding;
	//					break;
	//				}
	//			}
	//			break;
	//		}
	//	}

	//	bool mGliding;
	//	MAPoint2d mMomentum;
	//	float mFriction;
	//	MapWidgetMomentumState mMomentumState;

	//private:
	//	MapWidget* mWidget;
	//	int mPanTime;
	//};

	////=========================================================================
	//class MapWidgetStressTestListener : public TimerListener
	////=========================================================================
	//{
	//public:
	//	//---------------------------------------------------------------------
	//	MapWidgetStressTestListener( MapWidget* widget ) :
	//	//---------------------------------------------------------------------
	//		mWidget( widget ),
	//		lon( 17 )
	//	{
	//	}

	//	//---------------------------------------------------------------------
	//	void runTimerEvent( )
	//	//---------------------------------------------------------------------
	//	{
	//		lon += 0.01f;
	//		mWidget->setCenterPosition( LonLat( lon, 59 ), false, false );
	//		DebugPrintf( "StressTest: %f\n", lon );
	//	}

	//private:
	//	MapWidget* mWidget;
	//	float lon;
	//};

	//=========================================================================


	//-------------------------------------------------------------------------
	MapWidget::MapWidget( int x, int y, int width, int height, Widget* _parent )
	//-------------------------------------------------------------------------
	:	Widget( x, y, width, height, _parent )//,
		//mCenterPositionLonLat( ),
		//mCenterPositionPixels( ),
		//mPanTargetPositionLonLat( ),
		//mPanTargetPositionPixels( ),
		//mMagnification( 0 ),
		//mSource( NULL ),
		//mMapUpdateNesting( 0 ),
		//mPrevCenter( ),
		//mScreenImage( 0 ),
		//mHasScale( true ),
		//mPanTimerListener( NULL ),
		//mPanMode( MapWidgetPanMode_Smooth ),
		//mFont( NULL )
	{
		//resetScreenImage( );
		//mPanTimerListener = newobject( MapWidgetPanTimerListener, new MapWidgetPanTimerListener( this ) );
		mViewport = newobject( MapViewport, new MapViewport( ) );
		mViewport->setListener( this );
	}

	//-------------------------------------------------------------------------
	MapWidget::~MapWidget( )
	//-------------------------------------------------------------------------
	{
		//if ( mHasTimer )
		//	Environment::getEnvironment( ).removeTimer( mPanTimerListener );
		//deleteobject( mPanTimerListener );
	}

	////-------------------------------------------------------------------------
	//void MapWidget::enterMapUpdateScope( )
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
	//void MapWidget::exitMapUpdateScope( bool immediate )
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
	//void MapWidget::checkMapUpdateScope( )
	////-------------------------------------------------------------------------
	//{
	//}

	//-------------------------------------------------------------------------
	void MapWidget::setMapSource( MapSource* source )
	//-------------------------------------------------------------------------
	{
		//mSource = source;
		//updateMap( );
		mViewport->setMapSource( source );
	}

	//-------------------------------------------------------------------------
	PixelCoordinate MapWidget::getCenterPositionPixels( ) const
	//-------------------------------------------------------------------------
	{
		//switch( mPanMode)
		//{
		//case MapWidgetPanMode_Instant:
		//	return mCenterPositionPixels;
		//case MapWidgetPanMode_Smooth:
		//case MapWidgetPanMode_Momentum:
		//	return mPanTargetPositionPixels;
		//}
		//return PixelCoordinate( );
		return mViewport->getCenterPositionPixels( );
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
		//switch ( mPanMode )
		//{
		//case MapWidgetPanMode_Instant:
		//	return mCenterPositionLonLat;
		//case MapWidgetPanMode_Smooth:
		//case MapWidgetPanMode_Momentum:
		//	return mPanTargetPositionLonLat;
		//}
		////
		//// Error
		////
		//return LonLat( );
		return mViewport->getCenterPosition( );
	}

	//-------------------------------------------------------------------------
	void MapWidget::setCenterPosition( LonLat position, bool immediate, bool isPointerEvent )
	//-------------------------------------------------------------------------
	{
		//checkMapUpdateScope( );

		//if ( immediate )
		//{
		//	mCenterPositionLonLat = mPanTargetPositionLonLat = position;
		//	mCenterPositionPixels = mPanTargetPositionPixels = position.toPixels( getMagnification( ) );
		//	mPanTimerListener->mMomentumState = MapWidgetMomentumState_None;
		//	mPanTimerListener->mMomentum = MAPoint2d( );
		//	return;
		//}
		//switch( mPanMode )
		//{
		//case MapWidgetPanMode_Instant:
		//	mCenterPositionLonLat = position;
		//	mCenterPositionPixels = position.toPixels( getMagnification( ) );
		//	break;
		//case MapWidgetPanMode_Smooth:
		//case MapWidgetPanMode_Momentum:
		//	{
		//		PixelCoordinate prevTarget = mPanTargetPositionPixels;
		//		mPanTargetPositionLonLat = position;
		//		mPanTargetPositionPixels = position.toPixels( getMagnification( ) );
		//		//
		//		// Make sure current position is nearby, so we only soft scroll less than one screen.
		//		//
		//		int width = getWidth( );
		//		int height = getHeight( );
		//		if ( width > 0 && height > 0 )
		//		{
		//			int deltaX = mPanTargetPositionPixels.getX( ) - mCenterPositionPixels.getX( );
		//			int deltaY = mPanTargetPositionPixels.getY( ) - mCenterPositionPixels.getY( );
		//			//
		//			// go directly to location if delta is more than 1/6 of widget size.
		//			//
		//			double factor = 6 * fabs( Max( (double)deltaX / getWidth( ), (double)deltaY / getHeight( ) ) );
		//			if ( factor > 1 )
		//			{
		//				mCenterPositionPixels = PixelCoordinate(	getMagnification( ),
		//															mPanTargetPositionPixels.getX( ) - (int)( (double)deltaX / factor ),
		//															mPanTargetPositionPixels.getY( ) - (int)( (double)deltaY / factor ) );
		//			}
		//			if ( !isPointerEvent )
		//				mPanTimerListener->mMomentumState = MapWidgetMomentumState_None;
		//		}
		//		Environment::getEnvironment( ).addTimer( mPanTimerListener, PanIntervalMs, 0 );
		//		mHasTimer = true;
		//	}
		//	break;
		//}
		mViewport->setCenterPosition( position, immediate, isPointerEvent );
	}

	//-------------------------------------------------------------------------
	void MapWidget::setCenterPosition( LonLat position, int magnification, bool immediate, bool isPointerEvent )
	//-------------------------------------------------------------------------
	{
		mViewport->setCenterPosition( position, magnification, immediate, isPointerEvent );
	}

	//-------------------------------------------------------------------------
	void MapWidget::startGlide( )
	//-------------------------------------------------------------------------
	{
		//mPanTimerListener->mGliding = true;
		mViewport->startGlide( );
	}

	//-------------------------------------------------------------------------
	void MapWidget::stopGlide( )
	//-------------------------------------------------------------------------
	{
		//mPanTimerListener->mGliding = false;
		//mPanTimerListener->mMomentumState = MapWidgetMomentumState_None;
		//mPanTimerListener->mMomentum = MAPoint2d( );
		////
		//// make sure we lock map in place
		////
		//enterMapUpdateScope( );
		//mPanTargetPositionPixels = mCenterPositionPixels;
		//exitMapUpdateScope( true );
		mViewport->stopGlide( );
		mViewport->updateMap( );
		requestRepaint( );
	}

	////-------------------------------------------------------------------------
	//void MapWidget::testMomentumPanning( )
	////-------------------------------------------------------------------------
	//{
	//	mPanTimerListener->mMomentumState = MapWidgetMomentumState_None;
	//	setCenterPosition( LonLat( 18.07, 59.33 ), false, true );
	//	int currentTimeMs = maGetMilliSecondCount( );
	//	//TODO: mPanTimerListener->mPrevTimeMs = currentTimeMs - 100;
	//	setCenterPosition( LonLat( 18.0695, 59.33 ), false, true );
	//}

	//MapWidgetStressTestListener* stressListener;

	//-------------------------------------------------------------------------
	void MapWidget::stressTest( )
	//-------------------------------------------------------------------------
	{
		//stressListener = newobject( MapWidgetStressTestListener, new MapWidgetStressTestListener( this ) );
		//Environment::getEnvironment( ).addTimer( stressListener, 400, 0 );
		//setMagnification( 14 );
		mViewport->stressTest( );
	}

	//-------------------------------------------------------------------------
	int MapWidget::getMagnification( ) const
	//-------------------------------------------------------------------------
	{
		//return mMagnification;
		return mViewport->getMagnification( );
	}

	//-------------------------------------------------------------------------
	void MapWidget::setMagnification( int magnification )
	//-------------------------------------------------------------------------
	{
		//checkMapUpdateScope( );

		//mMagnification = magnification;

		//switch( mPanMode )
		//{
		//case MapWidgetPanMode_Instant:
		//	setCenterPosition( mCenterPositionLonLat, false, false );
		//	break;
		//case MapWidgetPanMode_Smooth:
		//case MapWidgetPanMode_Momentum:
		//	mPanTimerListener->mMomentumState = MapWidgetMomentumState_None;
		//	setCenterPosition( mPanTargetPositionLonLat, false, false );
		//	break;
		//}
		mViewport->setMagnification( magnification );
	}

	////-------------------------------------------------------------------------
	//MapWidgetPanMode MapWidget::getPanMode( ) const
	////-------------------------------------------------------------------------
	//{
	//	return mPanMode;
	//}

	//-------------------------------------------------------------------------
	void MapWidget::setPanMode( MapViewportPanMode panMode )
	//-------------------------------------------------------------------------
	{
		//mPanMode = panMode;
		mViewport->setPanMode( panMode );
	}

	//-------------------------------------------------------------------------
	float MapWidget::getFriction( ) const
	//-------------------------------------------------------------------------
	{
		//return mPanTimerListener->mFriction;
		return mViewport->getFriction( );
	}

	//-------------------------------------------------------------------------
	void MapWidget::setFriction( float friction )
	//-------------------------------------------------------------------------
	{
		///mPanTimerListener->mFriction = friction;
		mViewport->setFriction( friction );
	}

	//int tilesReceived = 0;

	////-------------------------------------------------------------------------
	//void MapWidget::tileReceived( MapCache* sender, MapTile* tile )
	////-------------------------------------------------------------------------
	//{
	//	tilesReceived++;

	//	PixelCoordinate tilePx = tile->getCenter( ).toPixels( tile->getMagnification( ) );
	//	MAPoint2d pt = worldPixelToWidget( tilePx );
	//	MAHandle old = maSetDrawTarget( mScreenImage );
	//	const int tileSize = mSource->getTileSize( );

	//	MARect rect;
	//	rect.height = tileSize;
	//	rect.width = tileSize;
	//	rect.top = 0;
	//	rect.left = 0;
	//	MAPoint2d dest;
	//	dest.x = pt.x - tileSize / 2;
	//	dest.y = pt.y - tileSize / 2;
	//	maDrawImageRegion( tile->getImage( ), &rect, &dest, TRANS_NONE );

	//	maSetDrawTarget( old );
	//	requestRepaint( );
	//}

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
		//if ( !this->enabled ) 
		//	return;

		//Point widgetPos = getPosition( );
		////
		//// Save clip
		////
		//(void)Gfx_pushClipRect( widgetPos.x, widgetPos.y, getWidth( ), getHeight( ) );
		////
		//// Draw map image
		////
		//Gfx_drawImage( mScreenImage, widgetPos.x, widgetPos.y );
		////
		//// Let subclass draw its overlay
		////
		//drawOverlay( );
		//
		////
		//// Draw scale indicator
		////
		//if ( ShowPixelScale && mHasScale )
		//{
		//	const int scaleWidth = 80;
		//	const int scaleX = widgetPos.x + getWidth( ) - scaleWidth - 5;
		//	const int scaleY = widgetPos.y + 5;
		//	int lineThickness = 3;
		//	const int crossbarHeight = 7;
		//	float scaleFrac = (float)( mMagnification - mSource->getMagnificationMin( ) ) / ( mSource->getMagnificationMax( ) - mSource->getMagnificationMin( ) );

		//	maSetColor( 0xa0a0a0 );

		//	Gfx_fillRect( scaleX, scaleY - lineThickness / 2, scaleWidth, lineThickness );
		//	Gfx_fillRect( (int)( scaleX + scaleFrac * scaleWidth - 0.5f * lineThickness ), scaleY - (crossbarHeight / 2 ), lineThickness, crossbarHeight );

		//	lineThickness = 1;

		//	maSetColor( 0x000000 );

		//	Gfx_fillRect( scaleX, scaleY, scaleWidth, lineThickness );
		//	Gfx_fillRect( (int)( scaleX + scaleFrac * scaleWidth - 0.5f * lineThickness ), scaleY - (crossbarHeight / 2 ), lineThickness, crossbarHeight );

		//	//
		//	// pixel scale
		//	//
		//	if ( ShowPixelScaleAsText )
		//	{
		//		int mag = getMagnification( );
		//		PixelCoordinate px1 = getCenterPositionPixels( );
		//		LonLat p1 = LonLat( px1 );
		//		PixelCoordinate px2 = PixelCoordinate( mag, px1.getX( ) + 1, px1.getY( ) + 1 );
		//		LonLat p2 = LonLat( px2 );
		//		double meterX1, meterY1;
		//		double meterX2, meterY2;
		//		p1.toMeters( meterX1, meterY1 );
		//		p2.toMeters( meterX2, meterY2 );
		//		double offsetX = meterX2 - meterX1;
		//		offsetX *= cos( fabs( p2.lat * PI / 180 ) );
		//		char buffer[100];
		//		sprintf( buffer, "%5.2f m/px", offsetX );

		//		if ( mFont != NULL )
		//			mFont->drawString( buffer, scaleX, scaleY + 5 );
		//		else
		//			Gfx_drawText( scaleX, scaleY + 5, buffer );
		//	}
		//}
		////
		//// Draw hairline cross
		////
		//if ( ShowHairlineCross )
		//{
		//	const int centerX = widgetPos.x + getWidth( ) / 2;
		//	const int centerY = widgetPos.y + getHeight( ) / 2;

		//	maSetColor( 0x000000 );

		//	Gfx_fillRect( centerX, centerY - CrossSize, 1, CrossSize * 2 + 1 );
		//	Gfx_fillRect( centerX - CrossSize, centerY, CrossSize * 2 + 1, 1 );
		//}
		////
		//// Draw latitude, longitude
		////
		//if ( ShowLatLon )
		//{
		//	char buffer[100];

		//	switch( mPanMode )
		//	{
		//	case MapWidgetPanMode_Instant:
		//		sprintf( buffer, "%-3.4f %-3.4f", mCenterPositionLonLat.lon, mCenterPositionLonLat.lat );
		//	case MapWidgetPanMode_Smooth:
		//	case MapWidgetPanMode_Momentum:
		//		sprintf( buffer, "%-3.4f %-3.4f", mCenterPositionLonLat.lon, mCenterPositionLonLat.lat );
		//	}
		//	maSetColor( 0x000000 );

		//	if ( mFont != NULL )
		//		mFont->drawString( buffer, widgetPos.x, widgetPos.y );
		//	else
		//		Gfx_drawText( widgetPos.x, widgetPos.y, buffer );
		//}
		////
		//// Draw debug info
		////
		//if ( ShowLatLon )
		//{
		//	char buffer[100];
		//	sprintf( buffer, "Tiles: %d Cache: %d", this->mSource->getTileCount( ), MapCache::get( )->size( ) );
		//	maSetColor( 0x000000 );

		//	if ( mFont != NULL )
		//		mFont->drawString( buffer, widgetPos.x, widgetPos.y + 20 );
		//	else
		//		Gfx_drawText( widgetPos.x, widgetPos.y + 20, buffer );
		//}
		////
		//// Restore original clip
		////
		//(void)Gfx_popClipRect( );

		mViewport->drawViewport( getPosition( ) );
	}

	////-------------------------------------------------------------------------
	//void MapWidget::drawOverlay( )
	////-------------------------------------------------------------------------
	//{
	//}

	////-------------------------------------------------------------------------
	//void MapWidget::updateMap( )
	////-------------------------------------------------------------------------
	//{
	//	if ( getWidth( ) <= 0 || getHeight( ) <= 0 ) 
	//		return;
	//	//
	//	// Clear screen image
	//	//
	//	MAHandle old = maSetDrawTarget( mScreenImage );
	//	maSetColor( 0xc0c0c0 );
	//	maFillRect( 0, 0, getWidth( ), getHeight( ) );
	//	maSetDrawTarget( old );
	//	//
	//	// Request tiles
	//	//
	//	// We want to use currently displayed center position here, so we bypass getCenterPosition( ).
	//	//
	//	MapCache::get( )->requestTiles( this, mSource, LonLat( mCenterPositionPixels ), mMagnification, getWidth( ), getHeight( ) );
	//}

	////-------------------------------------------------------------------------
	//MAPoint2d MapWidget::worldPixelToWidget( PixelCoordinate wpx )
	////-------------------------------------------------------------------------
	//{
	//	MAPoint2d pt;
	//	PixelCoordinate screenPx = mCenterPositionPixels;

	//	pt.x =    wpx.getX( ) - screenPx.getX( )   + ( getWidth( ) >> 1 );
	//	pt.y = -( wpx.getY( ) - screenPx.getY( ) ) + ( getHeight( ) >> 1 );

	//	return pt;
	//}

	////-------------------------------------------------------------------------
	//PixelCoordinate MapWidget::widgetToWorldPixel( MAPoint2d pt )
	////-------------------------------------------------------------------------
	//{
	//	PixelCoordinate screenPx = mCenterPositionPixels;
	//	return PixelCoordinate( mMagnification,
	//							(int)( pt.x + 0.5 - 0.5 * getWidth( ) + screenPx.getX( ) ),
	//							(int)( -( pt.y + 0.5 - 0.5 * getHeight( ) - screenPx.getY( ) ) ) );
	//}

	////-------------------------------------------------------------------------
	//static double clamp( double x, double min, double max )
	////-------------------------------------------------------------------------
	//{
	//	return x < min ? min : x > max ? max : x;
	//}

	//-------------------------------------------------------------------------
	void MapWidget::zoomIn( )
	//-------------------------------------------------------------------------
	{
		//if ( mMagnification < mSource->getMagnificationMax( ) )
		//{
		//	mMagnification++;

		//	switch( mPanMode )
		//	{
		//	case MapWidgetPanMode_Instant:
		//		mCenterPositionPixels = mCenterPositionLonLat.toPixels( mMagnification );
		//		break;
		//	case MapWidgetPanMode_Smooth:
		//	case MapWidgetPanMode_Momentum:
		//		mPanTimerListener->mMomentumState = MapWidgetMomentumState_None;
		//		mCenterPositionPixels = mCenterPositionLonLat.toPixels( mMagnification );
		//		mPanTargetPositionPixels = mPanTargetPositionLonLat.toPixels( mMagnification );
		//		break;
		//	}
		//}

		mViewport->zoomIn( );
	}

	//-------------------------------------------------------------------------
	void MapWidget::zoomOut( )
	//-------------------------------------------------------------------------
	{
		//if ( mMagnification > mSource->getMagnificationMin( ) )
		//{
		//	mMagnification--;

		//	switch( mPanMode )
		//	{
		//	case MapWidgetPanMode_Instant:
		//		mCenterPositionPixels = mCenterPositionLonLat.toPixels( mMagnification );
		//		break;
		//	case MapWidgetPanMode_Smooth:
		//	case MapWidgetPanMode_Momentum:
		//		mPanTimerListener->mMomentumState = MapWidgetMomentumState_None;
		//		mCenterPositionPixels = mCenterPositionLonLat.toPixels( mMagnification );
		//		mPanTargetPositionPixels = mPanTargetPositionLonLat.toPixels( mMagnification );
		//		break;
		//	}
		//}

		mViewport->zoomOut( );
	}

	////-------------------------------------------------------------------------
	//void MapWidget::scroll( MapWidgetScrollDirection direction, bool largeStep )
	////-------------------------------------------------------------------------
	//{
	//	switch( mPanMode )
	//	{
	//	case MapWidgetPanMode_Instant:
	//	case MapWidgetPanMode_Smooth:
	//	case MapWidgetPanMode_Momentum:
	//		{
	//			PixelCoordinate px = getCenterPositionPixels( );
	//			const int hStep = largeStep ? getWidth( ) : SmallScrollStep;
	//			const int vStep = largeStep ? getHeight( ) : SmallScrollStep;

	//			switch( direction )
	//			{
	//			case SCROLLDIRECTION_NORTH: px = PixelCoordinate( px.getMagnification( ), px.getX( ), px.getY( ) + vStep ); break;
	//			case SCROLLDIRECTION_SOUTH: px = PixelCoordinate( px.getMagnification( ), px.getX( ), px.getY( ) - vStep ); break;
	//			case SCROLLDIRECTION_EAST:  px = PixelCoordinate( px.getMagnification( ), px.getX( ) + hStep, px.getY( ) ); break;
	//			case SCROLLDIRECTION_WEST:  px = PixelCoordinate( px.getMagnification( ), px.getX( ) - hStep, px.getY( ) ); break;
	//			}

	//			LonLat newPos = LonLat( px );
	//			newPos = LonLat( clamp( newPos.lon, -180, 180 ), clamp( newPos.lat, -85, 85 ) );
	//			setCenterPosition( newPos, false, false );
	//		}
	//		break;
	//	}
	//}

	//-------------------------------------------------------------------------
	bool MapWidget::handleKeyPress( int keyCode )
	//-------------------------------------------------------------------------
	{
		//enterMapUpdateScope( );
		//bool ret = false;
		//switch( keyCode )
		//{
		//case MAK_LEFT:		
		//	scroll( SCROLLDIRECTION_WEST, false ); 
		//	ret = true;
		//	break;
		//case MAK_RIGHT:		
		//	scroll( SCROLLDIRECTION_EAST, false ); 
		//	ret = true;
		//	break;
		//case MAK_UP:		
		//	scroll( SCROLLDIRECTION_NORTH, false ); 
		//	ret = true;
		//	break;
		//case MAK_DOWN:		
		//	scroll( SCROLLDIRECTION_SOUTH, false ); 
		//	ret = true;
		//	break;
		//case MAK_1:			
		//	zoomOut( ); 
		//	ret = true;
		//	break;
		//case MAK_3:			
		//	zoomIn( ); 
		//	ret = true;
		//	break;
		//default: 
		//	ret = false;
		//	break;
		//}
		//exitMapUpdateScope( false );
		//return ret;

		return mViewport->handleKeyPress( keyCode );
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
		mViewport->setWidth( width );
		//resetScreenImage( );
	}

	//-------------------------------------------------------------------------
	void MapWidget::setHeight( int height )
	//-------------------------------------------------------------------------
	{
		Widget::setHeight( height );
		mViewport->setHeight( height );
		//resetScreenImage( );
	}

	////-------------------------------------------------------------------------
	//void MapWidget::resetScreenImage( )
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

	//-------------------------------------------------------------------------
	void MapWidget::viewportUpdated( MapViewport* viewport )
	//-------------------------------------------------------------------------
	{
		requestRepaint( );
	}
}
