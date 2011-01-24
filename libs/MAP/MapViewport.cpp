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

#include "MapConfig.h"
#include <matime.h>
#include "MemoryMgr.h"
#include <mastdlib.h>
#include <MAUtil/Graphics.h>
#include "MapViewport.h"
#include "MapCache.h"
#include "MapSource.h"
#include "DebugPrintf.h"
#include <madmath.h>

//#define OnlyUpdateWhenJobComplete
//#define USE_ALPHAFADES

#define SCALE_TO_MAG(scale) log(scale)/log(2.0)
#define MAG_TO_SCALE(mag) pow(2.0, mag)
	
MAPoint2d calculateVector(const MAPoint2d& p1, const MAPoint2d& p2) {
	MAPoint2d vec;
	vec.x = p2.x - p1.x;
	vec.y = p2.y - p1.y;
	return vec;
}
	
double calculateDistance(MAPoint2d touch1, MAPoint2d touch2) {		
	MAPoint2d vector = calculateVector(touch1, touch2);
	double distance = sqrt((double)(vector.x*vector.x + vector.y*vector.y));
	return distance;
}	
	

namespace MAP
{
	//
	// Appearance
	//
	static const bool ShowPixelScaleAsText = true; // shows meters/pixel scale (at latitude of screen center).
	//
	// Configuration
	//
	static const bool UseAggregatedTime = true;
	//static const bool OnlyUpdateWhenJobComplete = false;
	//
	// Pan smoothing time = PanInterval * PanAveragePoints
	//
	static const int PanInterval = 30;
	static const int PanAveragePoints = 3;
	static const double Tension = 4.0;
	static const double PanFriction = 0.60;
	static const double GlideFriction = 0.02;
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
	class MapViewportIdleListener : public IdleListener
	//=========================================================================
	{
	public:
		//---------------------------------------------------------------------
		MapViewportIdleListener( MapViewport* viewport ) :
		//---------------------------------------------------------------------
			mMomentumX( 0 ),
			mMomentumY( 0 ),
			mViewport( viewport ),
			mPanTime( 0 ),
			points( 0 ),
			pointPtr( 0 )
		{
		}
		
		virtual ~MapViewportIdleListener( )  
		{	
		}
		
		void removeIdleListenerIfEverythingIsDone() 
		{		
			Environment::getEnvironment( ).removeIdleListener( this );
			mViewport->mHasTimer = false;
			mViewport->mCenterPositionPixels = mViewport->mPanTargetPositionPixels;
			mViewport->mCenterPositionLonLat = mViewport->mPanTargetPositionLonLat;		
		}
		

		//---------------------------------------------------------------------
		void idle( )
		//---------------------------------------------------------------------
		{
			int currentTime = maGetMilliSecondCount( );
			//
			// Delta is time since previous idle event
			//
			int delta = currentTime - mPanTime;
			if ( delta < PanInterval )
				return;
			mPanTime = currentTime;

			PixelCoordinate prevCenterPix = mViewport->mCenterPositionPixels;
			PixelCoordinate targetPix = mViewport->mPanTargetPositionPixels;

			if ( mGliding )
			{
				//
				// Reduce momentum by friction
				//
				if ( UseAggregatedTime )
					mGlideDelta += PanInterval;
				//
				// time is elapsed time since glide started
				//
				int time = UseAggregatedTime ? 
					mGlideDelta : 
					currentTime - mGlideStartTime;

			#ifndef DeltaMomentum

				double frictionFactor = pow( 1.0 - GlideFriction, 1.0 + (double)time / 1000 );
				mMomentumX = mMomentumX * frictionFactor;
				mMomentumY = mMomentumY * frictionFactor;

				// pow( 1 - friction, 1 + delta )
				//
				// calc new location based on momentum
				//
				int interval = UseAggregatedTime ? PanInterval : delta;
				mViewport->mCenterPositionPixels = PixelCoordinate(	mViewport->getMagnification( ),
																	(int)( mViewport->mCenterPositionPixels.getX( ) + 0.001 * mMomentumX * interval + 0.5 ),
																	(int)( mViewport->mCenterPositionPixels.getY( ) + 0.001 * mMomentumY * interval + 0.5 ) );

			#else

				// d = d0 + v0t + at2/2

				//double invtime = 1000000.0 / time / time;
				double t = 0.001 * time;
				DebugPrintf( "t: %f delta: %d %d\n", 
					t, 
					(int)(mMomentumX * t - 5 * t * t / 2 + 0.5), 
					(int)(mMomentumY * t - 5 * t * t / 2 + 0.5) );

				mViewport->mCenterPositionPixels = PixelCoordinate(	
					mViewport->getMagnification( ),
					(int)( mGlideStartPixels.getX( ) + mMomentumX * t - 5000 * t * t / 2 + 0.5 ),
					(int)( mGlideStartPixels.getY( ) + mMomentumY * t - 5000 * t * t / 2 + 0.5 ) );

			#endif

				mViewport->mCenterPositionLonLat = LonLat( mViewport->mCenterPositionPixels );

				mViewport->mPanTargetPositionPixels = mViewport->mCenterPositionPixels;
				mViewport->mPanTargetPositionLonLat = mViewport->mCenterPositionLonLat;

				if ( fabs( mMomentumX ) < 1 && fabs( mMomentumY ) < 1 )
				{
					//
					// Done panning, stop timer and repaint
					//
					removeIdleListenerIfEverythingIsDone();
				}
			}
			else
			{
				//
				// Add to moving average
				//
				PixelCoordinate pxy = mViewport->mPanTargetPositionPixels;
				px[pointPtr] = pxy.getX( );
				py[pointPtr] = pxy.getY( );
				pointPtr++;
				if ( pointPtr >= PanAveragePoints )
					pointPtr = 0;
				if ( points < PanAveragePoints )
					points++;
				//
				// Calc average
				//
				int sumx = 0;
				int sumy = 0;
				for ( int i = 0; i < points; i++ )
				{
					sumx += px[i];
					sumy += py[i];
				}
				int x = (int)( (double)sumx / points + 0.5 );
				int y = (int)( (double)sumy / points + 0.5 );
				PixelCoordinate newXy = PixelCoordinate( mViewport->getMagnification( ), x, y );

				mViewport->mCenterPositionPixels = newXy;
				mViewport->mCenterPositionLonLat = LonLat( mViewport->mCenterPositionPixels );

				mMomentumX = (double)( newXy.getX( ) - prevCenterPix.getX( ) ) * 1000 / delta;
				mMomentumY = (double)( newXy.getY( ) - prevCenterPix.getY( ) ) * 1000 / delta;

				//
				// Stop panning if offset is small and no momentum
				//
				int offsetX = pxy.getX( ) - newXy.getX( );
				int offsetY = pxy.getY( ) - newXy.getY( );
				if ( abs( offsetX ) == 0 && abs( offsetY ) == 0 && fabs( mMomentumX ) < 1 && fabs( mMomentumY ) < 1 )
				{
					//
					// Done panning, stop timer and repaint
					//
					removeIdleListenerIfEverythingIsDone();
				}

			}

			mViewport->updateMap( );
		}

		//---------------------------------------------------------------------
		void startGlide( )
		//---------------------------------------------------------------------
		{
			mGliding = true;
			mGlideStartTime = maGetMilliSecondCount( );
			mGlideDelta = 0;
			mGlideStartPixels = mViewport->getCenterPositionPixels( );

		}

		//---------------------------------------------------------------------
		void stopGlide( )
		//---------------------------------------------------------------------
		{
			mGliding = false;
			points = 0;
			pointPtr = 0;
		}

		double mMomentumX;
		double mMomentumY;

	private:	
		MapViewport* mViewport;
		int mPanTime;
		int mGlideStartTime;
		int mGlideDelta;
		bool mGliding;
		PixelCoordinate mGlideStartPixels;

		int px[PanAveragePoints];
		int py[PanAveragePoints];
		int points;
		int pointPtr;

	};

	//=========================================================================

	//-------------------------------------------------------------------------
	MapViewport::MapViewport( )
	//-------------------------------------------------------------------------
	:			
		mMagnificationD(0.0),
		mZooming(false),
		mScale(1.0),
		mCenterPositionLonLat( ),
		mCenterPositionPixels( ),
		mPanTargetPositionLonLat( ),
		mPanTargetPositionPixels( ),
		mMagnification( 0 ),
		mSource( NULL ),
		mIdleListener( NULL ),
		mFont( NULL ),
		mInDraw( NULL ),
		mShowPixelScale( false ),
		mShowHairlineCross( false ),
		mShowLonLat( false )
	{
		mIdleListener = newobject( MapViewportIdleListener, new MapViewportIdleListener( this ) );
		Environment::getEnvironment( ).addIdleListener( mIdleListener );
		MapCache::get( )->addListener( this );
	}

	//-------------------------------------------------------------------------
	MapViewport::~MapViewport( )
	//-------------------------------------------------------------------------
	{
		MapCache::get( )->removeListener( this );
		if ( mHasTimer )
			Environment::getEnvironment( ).removeIdleListener( mIdleListener );
		deleteobject( mIdleListener );
	}

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
		return mPanTargetPositionPixels;
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
		return mPanTargetPositionLonLat;
	}

	//-------------------------------------------------------------------------
	void MapViewport::setCenterPosition( LonLat position, MagnificationType magnification, bool immediate, bool isPointerEvent )
	//-------------------------------------------------------------------------
	{
		int width = getScaledWidth( ); //getWidth( );
		int height = getScaledHeight( ); //getHeight( );

		if ( immediate || width <= 0 || height <= 0 )
		{
			mMagnification = magnification;
			setScale(MAG_TO_SCALE((double)magnification));	
			
			mCenterPositionLonLat = mPanTargetPositionLonLat = position;
			mCenterPositionPixels = mPanTargetPositionPixels = position.toPixels( magnification );
			mIdleListener->stopGlide( );

			return;
		}

		PixelCoordinate prevTarget = mPanTargetPositionPixels;

		PixelCoordinate newXy = position.toPixels( magnification );
		//
		// Make sure current position is nearby, so we only soft scroll less than one screen.
		//
		int deltaX = newXy.getX( ) - mCenterPositionPixels.getX( );
		int deltaY = newXy.getY( ) - mCenterPositionPixels.getY( );

		double factor = /* 6 * */ fabs( Max( (double)deltaX / width, (double)deltaY / height ) );
		if ( factor > 1 )
		{
			//
			// go directly to location if delta is more than 1/6 of viewport size.
			//
			newXy = PixelCoordinate(	magnification,
										mPanTargetPositionPixels.getX( ) - (int)( (double)deltaX / factor ),
										mPanTargetPositionPixels.getY( ) - (int)( (double)deltaY / factor ) );
		}
		if ( !isPointerEvent )
			mIdleListener->stopGlide( );

		mPanTargetPositionPixels = newXy;
		mPanTargetPositionLonLat = LonLat( newXy );
		
		mMagnification = magnification;
		setScale(MAG_TO_SCALE((double)magnification));	
		
		if ( !mHasTimer )
		{
			Environment::getEnvironment( ).addIdleListener( mIdleListener );
			mHasTimer = true;
		}
	}

	//-------------------------------------------------------------------------
	void MapViewport::setCenterPosition( LonLat position, bool immediate, bool isPointerEvent )
	//-------------------------------------------------------------------------
	{
		setCenterPosition( position, mMagnification, immediate, isPointerEvent );
	}

	//-------------------------------------------------------------------------
	void MapViewport::centerAndScaleToRectangle( LonLat lowerLeft, LonLat upperRight )
	//-------------------------------------------------------------------------
	{
		//
		// Find proper magnification
		//
		MapSource* source = getMapSource( );
		int magnification = source->getMagnificationMax( );
		while( magnification > source->getMagnificationMin( ) )
		{
			PixelCoordinate llPx = lowerLeft.toPixels( magnification );
			PixelCoordinate urPx = upperRight.toPixels( magnification );
			if (	urPx.getX( ) - llPx.getX( ) < getWidth( ) &&
					urPx.getY( ) - llPx.getY( ) < getHeight( ) )
				break;
			magnification--;
		}
		PixelCoordinate lowerLeftPx = lowerLeft.toPixels( magnification );
		PixelCoordinate upperRightPx = upperRight.toPixels( magnification );
		LonLat center = LonLat( PixelCoordinate( magnification, 
												( lowerLeftPx.getX( ) + upperRightPx.getX( ) ) / 2,
												( lowerLeftPx.getY( ) + upperRightPx.getY( ) ) / 2 ) );
		setCenterPosition( center, magnification, true, false );
	}

	//-------------------------------------------------------------------------
	void MapViewport::startGlide( )
	//-------------------------------------------------------------------------
	{
		mIdleListener->startGlide( );
	}

	//-------------------------------------------------------------------------
	void MapViewport::stopGlide( )
	//-------------------------------------------------------------------------
	{
		mIdleListener->stopGlide( );
		//
		// make sure we lock map in place
		//
		mPanTargetPositionPixels = mCenterPositionPixels;
		updateMap( );
	}

	//-------------------------------------------------------------------------
	MagnificationType MapViewport::getMagnification( ) const
	//-------------------------------------------------------------------------
	{
		return mMagnification;
	}

	//-------------------------------------------------------------------------
	void MapViewport::setMagnification( MagnificationType magnification )
	//-------------------------------------------------------------------------
	{
		mMagnification = magnification;
		setScale(MAG_TO_SCALE((double)magnification));
		
		mIdleListener->stopGlide( );
		setCenterPosition( mPanTargetPositionLonLat, true, false );
	}

	//-------------------------------------------------------------------------
	int getAlphaForTile(MapTile* tile)
	//-------------------------------------------------------------------------	
	{
		int timeSinceCreated = tile->getMilliSecondsSinceCreated();
		int alpha = (255*timeSinceCreated)/250;
		if(alpha>255) alpha = 255;
		return alpha;
	}
	
	//-------------------------------------------------------------------------
	void MapViewport::tileReceived( MapCache* sender, MapTile* tile, bool foundInCache )
	//-------------------------------------------------------------------------
	{
		if ( mInDraw )
		{
			//
			// draw context is set up, draw tile to viewport
			//
			PixelCoordinate tilePx = tile->getCenter( ).toPixels( tile->getMagnification( ) );
			MAPoint2d pt = worldPixelToViewport( tilePx );
			const int tileSize = mSource->getTileSize( );
			
			#ifndef WIN32
			Gfx_pushMatrix();

			if(mZooming) {
				int width = getWidth( );
				int height = getHeight( );
				MAPoint2d vec = {width>>1, height>>1};
	/*
				MAPoint2d vec = calculateVector(mOldCenter, mNewCenter);
				vec.x = (int)((double)vec.x / mScale + 0.5);
				vec.y = (int)((double)vec.y / mScale + 0.5);
				vec.x += mOldCenter.x;
				vec.y += mOldCenter.y;
				*/
				
				Gfx_translate(vec.x, vec.y);
				
				MAFixed fscale = (MAFixed)(mScale*65536.0);
				Gfx_scale(fscale,fscale);
				//Gfx_translate(-(width>>1), -(height>>1));
				Gfx_translate(-vec.x, -vec.y);
				
			}


			#endif // WIN32

#ifndef WIN32
#ifdef USE_ALPHAFADES
			int alpha = getAlphaForTile(tile);			
			if(alpha<255) {
				Gfx_setAlpha(alpha);
			}
#endif
#endif

			#ifdef StoreCompressedTilesInCache
			//
			// Unpack PNG
			//
			MAHandle placeholder = PlaceholderPool::alloc( );//maCreatePlaceholder( );
			maCreateImageFromData( placeholder, tile->getImage( ), 0, tile->getContentLength( ) );
			//
			// Draw image
			//
			Gfx_drawImage( placeholder,  pt.x - tileSize / 2, pt.y - tileSize / 2 );		
			//
			// Scrap PNG
			//
			MAUtil::PlaceholderPool::put( placeholder );
		
			#else // StoreCompressedTilesInCache

			Gfx_drawImage( tile->getImage( ),  pt.x - tileSize / 2, pt.y - tileSize / 2 );		

			#endif // StoreCompressedTilesInCache

#ifndef WIN32
#ifdef USE_ALPHAFADES
			Gfx_setAlpha(255);
#endif
#endif

			#ifndef WIN32

			Gfx_popMatrix();

			#endif
		}
		else
		{
#ifndef OnlyUpdateWhenJobComplete		
			{
				#ifndef WIN32
				
				if ( !foundInCache ) {
					Gfx_notifyImageUpdated( tile->getImage( ) );
				}

				#endif
				
				//
				// notify client that update is needed
				//
				onViewportUpdated( );
			}
		}
#endif		
	}

	//-------------------------------------------------------------------------
	void MapViewport::jobComplete( MapCache* sender )
	//-------------------------------------------------------------------------
	{
#ifdef OnlyUpdateWhenJobComplete
		{
			//
			// notify client that update is needed
			//
			onViewportUpdated( );
		}
#endif		
	}

	//-------------------------------------------------------------------------
	void MapViewport::error( MapCache* sender, int code )
	//-------------------------------------------------------------------------
	{
		onError( code );
	}

	//-------------------------------------------------------------------------
	void MapViewport::drawViewport( Point origin )
	//-------------------------------------------------------------------------
	{
		mInDraw = true;
		
		//
		// Save clip
		//
		(void)Gfx_pushClipRect( origin.x, origin.y, getWidth( ), getHeight( ) );
		Rect bounds = Rect( origin.x, origin.y, getWidth( ), getHeight( )  );
		//
		// Draw available tiles
		//

		MapCache::get( )->requestTiles( mSource, LonLat( mCenterPositionPixels ), mMagnification, getWidth( ), getHeight( ), mIdleListener->mMomentumX, mIdleListener->mMomentumY );
		
		//
		// Let subclass draw its overlay
		//
		drawOverlay( bounds, mMagnification );
		
		//
		// Draw scale indicator
		//
		if ( mShowPixelScale )
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
				MagnificationType mag = getMagnification( );
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
		if ( mShowHairlineCross )
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
		if ( mShowLonLat )
		{
			char buffer[100];
			sprintf( buffer, "%-3.4f %-3.4f", mCenterPositionLonLat.lon, mCenterPositionLonLat.lat );

			maSetColor( 0x000000 );

			if ( mFont != NULL )
				mFont->drawString( buffer, origin.x, origin.y );
			else
				Gfx_drawText( origin.x, origin.y, buffer );
		}
		//
		// Draw debug info
		//
		if ( mShowLonLat )
		{
			char buffer[100];
			sprintf( buffer, "Tiles: %d Cache: %d", this->mSource->getTileCount( ), MapCache::get( )->size( ) );
			maSetColor( 0x000000 );

			if ( mFont != NULL )
				mFont->drawString( buffer, origin.x, origin.y + 20 );
			else
				Gfx_drawText( origin.x, origin.y + 20, buffer );
				
			sprintf(buffer, "mScale: %f", mScale);
			mFont->drawString( buffer, origin.x, origin.y + 40 );			
			sprintf(buffer, "mMagnification: %d", mMagnification);
			mFont->drawString( buffer, origin.x, origin.y + 60 );			
			sprintf(buffer, "mMagnificationD: %f", mMagnificationD);
			mFont->drawString( buffer, origin.x, origin.y + 80 );			
				
		}
		//
		// Restore original clip
		//
		(void)Gfx_popClipRect( );

		mInDraw = false;
	}

	//-------------------------------------------------------------------------
	void MapViewport::drawOverlay( Rect& bounds, MagnificationType magnification )
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
		// Request tiles
		//
		// We want to use currently displayed center position here, so we bypass getCenterPosition( ).
		//
				
		MapCache::get( )->requestTiles( mSource, LonLat( mCenterPositionPixels ), mMagnification, getWidth( ), getHeight( ), mIdleListener->mMomentumX, mIdleListener->mMomentumY );
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
			setMagnification(mMagnification + 1);
			mCenterPositionPixels = mCenterPositionLonLat.toPixels( mMagnification );
			mPanTargetPositionPixels = mPanTargetPositionLonLat.toPixels( mMagnification );
		}
	}

	//-------------------------------------------------------------------------
	void MapViewport::zoomOut( )
	//-------------------------------------------------------------------------
	{
		if ( mMagnification > mSource->getMagnificationMin( ) )
		{
			setMagnification(mMagnification - 1);
			mCenterPositionPixels = mCenterPositionLonLat.toPixels( mMagnification );
			mPanTargetPositionPixels = mPanTargetPositionLonLat.toPixels( mMagnification );
		}
	}

	//-------------------------------------------------------------------------
	void MapViewport::scroll( MapViewportScrollDirection direction, bool largeStep )
	//-------------------------------------------------------------------------
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

	//-------------------------------------------------------------------------
	bool MapViewport::handleKeyPress( int keyCode )
	//-------------------------------------------------------------------------
	{
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
		this->updateMap( );
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
		mWidth = width;
		updateMap( );
	}

	//-------------------------------------------------------------------------
	void MapViewport::setHeight( int height )
	//-------------------------------------------------------------------------
	{
		mHeight = height;
		updateMap( );
	}
	
	//-------------------------------------------------------------------------		
	void MapViewport::moveCenterPositionInPixels(int xdelta, int ydelta)
	//-------------------------------------------------------------------------		
	{
		PixelCoordinateD newCoord = PixelCoordinateD(
						mMagnificationD,
						mCenterPositionPixels.getX() + xdelta,
						mCenterPositionPixels.getY() + ydelta
						);

		LonLatD position = LonLatD(newCoord);
		setCenterPosition(LonLat(position.lon, position.lat), true, false);
	}
	
	//-------------------------------------------------------------------------	
	void MapViewport::beginZooming(const MAPoint2d& p1, const MAPoint2d& p2) 
	//-------------------------------------------------------------------------	
	{
		mIdleListener->stopGlide( );
		mOldDistance = calculateDistance(p1, p2);
		MAPoint2d vector = calculateVector(p1, p2);
		mOldCenter.x = p1.x + vector.x/2;
		mOldCenter.y = p1.y + vector.y/2;
		
		//beginPanning(mOldCenter);
			
		mLinearMagnificationStart = MAG_TO_SCALE(mMagnification);
		
		mNewCenter = mOldCenter;	
		mZoomTime = maGetMilliSecondCount( );
		mZooming = true;
		
		mTouchPixelCoordinate = getCenterPositionPixels();	
	}
	
	
	static const int ZoomInterval = 30;
	static const int TapPanAcceleration = 1;
	
	double MapViewport::getScale() {
		return MAG_TO_SCALE(mMagnificationD);
	}	
	
	// updates magnification
	//-------------------------------------------------------------------------
	void MapViewport::setScale(double scale) 
	//-------------------------------------------------------------------------	
	{
		double tileMagnification = mMagnification;
		mMagnificationD = SCALE_TO_MAG(scale);

		if(mMagnificationD > mSource->getMagnificationMax( )) {
			mMagnificationD = mSource->getMagnificationMax( ); 
		}
		
		if(mMagnificationD < mSource->getMagnificationMin( )) {
			mMagnificationD = mSource->getMagnificationMin( );
		}
		
		mScale = (MAG_TO_SCALE(mMagnificationD)/MAG_TO_SCALE(tileMagnification));
		
		mCenterPositionPixels = mCenterPositionLonLat.toPixels( mMagnification );
		mPanTargetPositionPixels = mPanTargetPositionLonLat.toPixels( mMagnification );
		
		mIdleListener->stopGlide( );
		this->updateMap( );
		//onViewportUpdated( );
	}
	
	//-------------------------------------------------------------------------
	void MapViewport::updateZooming(const MAPoint2d& p1, const MAPoint2d& p2) 
	//-------------------------------------------------------------------------	
	{

		mIdleListener->stopGlide( );

		int currentTime = maGetMilliSecondCount( );
		//
		// Delta is time since previous idle event
		//
		int delta = currentTime - mZoomTime;
		if ( delta < ZoomInterval )
			return;
		mZoomTime = currentTime;
				
		MAPoint2d vector = calculateVector(p1, p2);
		mNewCenter.x = p1.x + vector.x/2;
		mNewCenter.y = p1.y + vector.y/2;	
		MAPoint2d direction = calculateVector(mOldCenter, mNewCenter);
			
		double newDistance = calculateDistance(p1, p2);
		double linearmag = mLinearMagnificationStart * newDistance/mOldDistance;
		setScale(linearmag);		

		PixelCoordinate px = PixelCoordinate(	mTouchPixelCoordinate.getMagnification( ),
												mTouchPixelCoordinate.getX( ) - (int)(((double)direction.x / mScale) + 0.5),
												mTouchPixelCoordinate.getY( ) + (int)(((double)direction.y / mScale) + 0.5)
												);
												
        LonLat newPos = LonLat( px );
        newPos = LonLat( clamp( newPos.lon, -180, 180 ), clamp( newPos.lat, -85, 85 ) );
        //setCenterPosition( newPos, mMagnification, false, true );
        	mCenterPositionLonLat = mPanTargetPositionLonLat = newPos;
       	mCenterPositionPixels = mPanTargetPositionPixels = px;

		//updatePanning(mNewCenter);	
		/*
		if(newTileMagnificationValue != mMagnification) {
			mMagnification = newTileMagnificationValue;
			mCenterPositionPixels = mCenterPositionLonLat.toPixels( mMagnification );
			mPanTargetPositionPixels = mPanTargetPositionLonLat.toPixels( mMagnification );
		} 
		*/
		//onViewportUpdated( );
	}
	
	//-------------------------------------------------------------------------	
	void MapViewport::endZooming() 
	//-------------------------------------------------------------------------	
	{
		mMagnification = (int)(mMagnificationD + 0.5);		
		
		mCenterPositionPixels = mCenterPositionLonLat.toPixels( mMagnification );
		mPanTargetPositionPixels = mPanTargetPositionLonLat.toPixels( mMagnification );		
	
		//endPanning();
	
		this->updateMap( );					
		//onViewportUpdated( );
		mZooming = false;
	}
	
	//-------------------------------------------------------------------------		
	bool MapViewport::isZooming() 
	//-------------------------------------------------------------------------		
	{
		return mZooming;
	}
	
	//-------------------------------------------------------------------------		
	void MapViewport::beginPanning(const MAPoint2d& p) 
	//-------------------------------------------------------------------------	
	{
		//mMagnification = (int)(mMagnificationD + 0.5);	
		mCenterPositionPixels = mCenterPositionLonLat.toPixels( mMagnification );
		mPanTargetPositionPixels = mPanTargetPositionLonLat.toPixels( mMagnification );
		
		mTouchScreenCoordinate = p;
		mTouchPixelCoordinate = getCenterPositionPixels();
		mIdleListener->stopGlide( );	
		mPanTime = maGetMilliSecondCount();
		
		mZooming = false;
	}
	
	//-------------------------------------------------------------------------		
	void MapViewport::updatePanning(const MAPoint2d& p)
	//-------------------------------------------------------------------------	
	{
	
		int currentTime = maGetMilliSecondCount( );
		//
		// Delta is time since previous idle event
		//
		int delta = currentTime - mPanTime;
		if ( delta < PanInterval )
			return;
		mPanTime = currentTime;
		
		setPositionInScreenCoordinatesSinceTouch( p );	
	
	}
	
	//-------------------------------------------------------------------------		
	void MapViewport::endPanning() 
	//-------------------------------------------------------------------------		
	{
		// maybe let this function receive last positons.
		//setPosition( p );
		mIdleListener->startGlide( );
	}
	
	//-------------------------------------------------------------------------
	void MapViewport::setPositionInScreenCoordinatesSinceTouch(const MAPoint2d& p )
	//-------------------------------------------------------------------------
	{
		//int curTimeMs = maGetMilliSecondCount( );
		//int deltaMs = curTimeMs - lastPointerMoveMs;
		//lastPointerMoveMs = curTimeMs;
		int dx = ( p.x - mTouchScreenCoordinate.x ) * TapPanAcceleration;
		int dy = ( p.y - mTouchScreenCoordinate.y ) * TapPanAcceleration;
		PixelCoordinate px = PixelCoordinate(	mTouchPixelCoordinate.getMagnification( ),
												mTouchPixelCoordinate.getX( ) - (int)(((double)dx / mScale) + 0.5),
												mTouchPixelCoordinate.getY( ) + (int)(((double)dy / mScale) + 0.5)
												);
												
        LonLat newPos = LonLat( px );
        newPos = LonLat( clamp( newPos.lon, -180, 180 ), clamp( newPos.lat, -85, 85 ) );
        setCenterPosition( newPos, mMagnification, false, true );
	}		

	//-------------------------------------------------------------------------
	void MapViewport::onViewportUpdated( )
	//-------------------------------------------------------------------------
	{
		Vector<IMapViewportListener*>* listeners = getBroadcasterListeners<IMapViewportListener>( *this );
		for ( int i = 0; i < listeners->size( ); i ++ )
			(*listeners)[i]->viewportUpdated( this );
	}

	//-------------------------------------------------------------------------
	void MapViewport::onError( int code )
	//-------------------------------------------------------------------------
	{
		Vector<IMapViewportListener*>* listeners = getBroadcasterListeners<IMapViewportListener>( *this );
		for ( int i = 0; i < listeners->size( ); i ++ )
			(*listeners)[i]->error( this, code );
	}
}
