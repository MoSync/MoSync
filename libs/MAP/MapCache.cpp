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
#include "MapCache.h"
#include "MapTile.h"
#include "MapSource.h"
#include "LonLat.h"
#include "MapTileCoordinate.h"
#include "DebugPrintf.h"

namespace MAP
{
	MapCache* MapCache::sSingleton = NULL;

	//=========================================================================
	class MapCacheClientData : public MapSourceClientData
	//=========================================================================
	{
	public:
		MapCacheClientData( IMapCacheListener* listener ) 
			: mListener( listener ) 
		{
		}

		IMapCacheListener*	mListener;
	};

	static const int DefaultCapacity = 20;

	//-------------------------------------------------------------------------
	MapCache* MapCache::get( ) 
	//-------------------------------------------------------------------------
	{ 
		if ( sSingleton == NULL ) 
			sSingleton = newobject( MapCache, new MapCache( ) ); 
		return sSingleton; 
	}
	
	//-------------------------------------------------------------------------
	void MapCache::shutdown( ) 
	//-------------------------------------------------------------------------
	{ 
		deleteobject( sSingleton ); 
	}

	//-------------------------------------------------------------------------
	//
	// Construct a new MapCache.
	//
	MapCache::MapCache( ) :
	//-------------------------------------------------------------------------
		mList( NULL ),
		mHits( 0 ),
		mMisses( 0 ),
		mCapacity( DefaultCapacity )
	{
		reallocateCache( );
	}

	//-------------------------------------------------------------------------
	//
	// Destruct a map cache.
	//
	MapCache::~MapCache( )
	//-------------------------------------------------------------------------
	{
		// dispose tile list
		clear( );
		deleteobject( mList );
	}

	//-------------------------------------------------------------------------
	int MapCache::getCapacity( ) const 
	//-------------------------------------------------------------------------
	{ 
		return mCapacity; 
	}

	//-------------------------------------------------------------------------
	void MapCache::setCapacity( int capacity ) 
	//-------------------------------------------------------------------------
	{ 
		mCapacity = capacity; 
		reallocateCache( ); 
	}

	//-------------------------------------------------------------------------
	//
	// Reallocates cache, content is flushed
	//
	void MapCache::reallocateCache( )
	//-------------------------------------------------------------------------
	{
		//
		// Free existing
		//
		if ( mList != NULL )
		{
			clear( );
			deleteobject( mList );
		}
		//
		// Alloc new
		//
		mList = newobject( MapTile*, new MapTile*[mCapacity] );
		for ( int i = 0; i < mCapacity; i++ )
			mList[i] = NULL;
	}
	//
	// Min, Max
	//
	static inline double Min( double x, double y ) { return x < y ? x : y; }
	static inline int Min( int x, int y ) { return x < y ? x : y; }
	static inline double Max( double x, double y ) { return x > y ? x : y; }
	static inline int Max( int x, int y ) { return x > y ? x : y; }

	//-------------------------------------------------------------------------
	//
	// Requests tiles to cover specified rectangle
	//
	void MapCache::requestTiles(	IMapCacheListener* listener,
									MapSource *source,
									const LonLat centerpoint,
									const int magnification,
									const int pixelWidth,
									const int pixelHeight )
	//-------------------------------------------------------------------------
	{
		DebugAssert( pixelWidth > 0 );
		DebugAssert( pixelHeight > 0 );

		//
		// Clear queue
		//
		source->clearQueue( );

		//
		// Test code
		//
		if ( /*true*/false )
		{
			//
			// Test code only requests tile at centerpoint.
			//
			LonLat cp = LonLat( centerpoint.lon, centerpoint.lat + 40 );
			MapTileCoordinate tileXY = source->lonLatToTile( centerpoint, magnification );
			source->requestTile( tileXY, this, newobject( MapCacheClientData, new MapCacheClientData( listener ) ) );
			return;
		}


		const int offsetX = pixelWidth / 2;
		const int offsetY = pixelHeight / 2;
		//
		// conv center to pixels
		//
		PixelCoordinate centerPx = source->lonLatToPixel( centerpoint, magnification );
		// Find corners
		//
		PixelCoordinate llpix = PixelCoordinate( magnification, centerPx.getX( ) - offsetX, centerPx.getY( ) - offsetY );
		PixelCoordinate urpix = PixelCoordinate( magnification, centerPx.getX( ) + offsetX, centerPx.getY( ) + offsetY );
		LonLat ll = LonLat( llpix );
		LonLat ur = LonLat( urpix );
		//
		// find tiles at corners
		//
		MapTileCoordinate llTile = source->lonLatToTile( ll, magnification );
		MapTileCoordinate urTile = source->lonLatToTile( ur, magnification );

		//
		// Queue all tiles in area
		//
		//const double halfTileSize = 0.5 * tileSize;
		int yMin = Min( llTile.getY( ), urTile.getY( ) );
		int yMax = Max( llTile.getY( ), urTile.getY( ) );
		int xMin = Min( llTile.getX( ), urTile.getX( ) );
		int xMax = Max( llTile.getX( ), urTile.getX( ) );

		for ( int y = yMin; y <= yMax; y++ )
		{
			for ( int x = xMin; x <= xMax; x++)
			{
				//
				// In cache? Then return tile in cache
				//
				MapTileCoordinate tileXY = MapTileCoordinate( x, y, magnification );

				int loc = findInCache( source, tileXY );
				if ( loc != -1 )
				{
					mHits++;
					MapTile* t = mList[loc];
					t->stamp( );
					listener->tileReceived( this, t );
					continue;
				}
				else
				{
				}
				//
				// Not in cache: request from map source.
				//
				mMisses++;
				source->requestTile( tileXY, this, newobject( MapCacheClientData, new MapCacheClientData( listener ) ) );
			}
		}
	}

	//-------------------------------------------------------------------------
	int MapCache::findInCache( MapSource* source, MapTileCoordinate tileXY ) const
	//-------------------------------------------------------------------------
	{
		for ( int i = 0; i < mCapacity; i++ )
		{
			const MapTile* t = mList[i];
			if ( t != NULL )
				if ( t->getMapSource( ) == source && t->getGridX( ) == tileXY.getX( ) && t->getGridY( ) == tileXY.getY( ) && t->getMagnification( ) == tileXY.getMagnification( ) )
					return i;
		}
		return -1;
	}

	//-------------------------------------------------------------------------
	int MapCache::findLRU( ) const
	//-------------------------------------------------------------------------
	{
		DateTime oldest = DateTime::maxValue( );
		int loc = -1;
		for ( int i = 0; i < mCapacity; i++ )
		{
			const MapTile* t = mList[i];
			if( t == NULL )
				return i;
			if ( t->getLastAccessTime( ) < oldest )
			{
				oldest = t->getLastAccessTime( );
				loc = i;
			}
		}
		return loc;
	}

	//-------------------------------------------------------------------------
	int MapCache::findFreeLocation( ) const
	//-------------------------------------------------------------------------
	{
		for ( int i = 0; i < mCapacity; i++ )
			if( mList[i] == NULL )
				return i;
		return -1;
	}

	//-------------------------------------------------------------------------
	void MapCache::tileReceived( MapSource* sender, MapTile* tile, MapSourceClientData* cd )
	//-------------------------------------------------------------------------
	{
		MapCacheClientData* clientData = (MapCacheClientData*)cd;
		//
		// Add to cache in free location
		//
		int loc = findFreeLocation( );
		//
		// No free location? Then find oldest accessed tile and overwrite.
		//
		if ( loc == -1 )
			loc = findLRU( );
		//
		// Drop old tile, if any
		//
		if ( loc != -1 )
			deleteobject( mList[loc] );
		//
		// Finally add to cache
		//
		tile->stamp( );
		mList[loc] = tile;

		IMapCacheListener* listener = clientData->mListener;
		listener->tileReceived( this, tile );
	}

	//-------------------------------------------------------------------------
	void MapCache::downloadCancelled( MapSource* sender )
	//-------------------------------------------------------------------------
	{
		// TODO: Handle
	}

	//-------------------------------------------------------------------------
	void MapCache::error( MapSource* source, int code )
	//-------------------------------------------------------------------------
	{
		// TODO: Handle
	}

	//-------------------------------------------------------------------------
	//
	// Deletes all tiles in cache.
	//
	void MapCache::clear( )
	//-------------------------------------------------------------------------
	{
		for ( int i = 0; i < mCapacity; i++ )
			deleteobject( mList[i] );
	}
}
