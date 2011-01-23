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
#include "MemoryMgr.h"
#include "MapCache.h"
#include "MapTile.h"
#include "MapSource.h"
#include "LonLat.h"
#include "MapTileCoordinate.h"
#include "DebugPrintf.h"
#include "TraceScope.h"

namespace MAP
{
	MapCache* MapCache::sSingleton = NULL;

	static const int DefaultCapacity = 40;

	//=========================================================================
	class MapTileKey
	//=========================================================================
	{
	public:
		MapTileKey( ) { }

		//---------------------------------------------------------------------
		MapTileKey( MapSource* source, int gridX, int gridY, int magnification )
		//---------------------------------------------------------------------
		:	mSource( source ),
			mGridX( gridX ),
			mGridY( gridY ),
			mMagnification( magnification )
		{
		}

		MapSource* mSource;
		int mGridX;
		int mGridY;
		int mMagnification;

		bool operator==(const MapTileKey& c) const 
		{
			return mSource == c.mSource && mGridX == c.mGridX && mGridY == c.mGridY && mMagnification == c.mMagnification;
		}

		bool operator<(const MapTileKey& c) const 
		{
			return mSource < c.mSource && mGridX < c.mGridX && mGridY < c.mGridY && mMagnification < c.mMagnification;
		}
					
	};
}

namespace MAUtil 
{
	//-------------------------------------------------------------------------
	template<> 
	hash_val_t THashFunction<MAP::MapTileKey>( const MAP::MapTileKey& data ) 
	//-------------------------------------------------------------------------
	{
		return ((int)data.mSource) ^ data.mGridX ^ data.mGridY ^ data.mMagnification;
	} 
}

namespace MAP
{
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
		mList( ),
		mHits( 0 ),
		mMisses( 0 ),
		mCapacity( DefaultCapacity )
	{
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
	}

	//-------------------------------------------------------------------------
	int MapCache::size( )
	//-------------------------------------------------------------------------
	{
		return mList.size( );
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
	void MapCache::requestTiles(	MapSource *source,
									const LonLat centerpoint,
									const int magnification,
									const int pixelWidth,
									const int pixelHeight,
									const double directionX,
									const double directionY )
	//-------------------------------------------------------------------------
	{
		DebugAssert( pixelWidth > 0 );
		DebugAssert( pixelHeight > 0 );

		if ( source == NULL ) 
			return;
		//
		// Clear queue
		//
		source->clearQueue( );

		const int offsetX = pixelWidth / 2;
		const int offsetY = pixelHeight / 2;
		//
		// conv center to pixels
		//
		PixelCoordinate centerPx = source->lonLatToPixel( centerpoint, magnification );
		//
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
		int left = Min( llTile.getX( ), urTile.getX( ) );
		int right = Max( llTile.getX( ), urTile.getX( ) ); 
		int top = Min( llTile.getY( ), urTile.getY( ) );
		int bottom = Max( llTile.getY( ), urTile.getY( ) );
		//
		// Queue all tiles in area
		//
		int xStep = directionX > 0 ? 1 : -1;
		int xMin = directionX > 0 ? left : right;
		int xMax = directionX > 0 ? right + 1 : left - 1;

		int yStep = directionY < 0 ? 1 : -1;
		int yMin = directionY < 0 ? top : bottom;
		int yMax = directionY < 0 ? bottom + 1 : top - 1;

		for ( int y = yMin; y != yMax; y += yStep )
		{
			for ( int x = xMin; x != xMax; x += xStep )
			{
				//
				// In cache? Then immediately return tile in cache
				//
				MapTileKey key = MapTileKey( source, x, y, magnification );
				HashMap<MapTileKey, MapTile*>::Iterator found = mList.find( key );

				if ( found != mList.end( ) )
				{
					HashMap<MapTileKey, MapTile*>::PairKV kv = *found;
					mHits++;
					MapTile* t = kv.second;
					t->stamp( );
					onTileReceived( t, true );
					continue;
				}
				//
				// Not in cache: request from map source.
				//
				mMisses++;
				source->requestTile( this, MapTileCoordinate( x, y, magnification ) );
			}
		}
		source->requestJobComplete( this );
	}

	//-------------------------------------------------------------------------
	MapTileKey MapCache::findLRU( )
	//-------------------------------------------------------------------------
	{
		DateTime oldest = DateTime::maxValue( );
		MapTileKey ret;
		for ( HashMap<MapTileKey, MapTile*>::ConstIterator i = mList.begin( ); i != mList.end( ); i++ )
		{
			MapTile* t = i->second;
			if ( t->getLastAccessTime( ) < oldest )
			{
				oldest = t->getLastAccessTime( );
				ret = i->first;
			}
		}
		return ret;
	}

	//-------------------------------------------------------------------------
	void MapCache::tileReceived( MapSource* sender, MapTile* tile )
	//-------------------------------------------------------------------------
	{
#if 0 // this block tries to test if memory is available

		//
		// try to alloc something bigger than a tile
		//
		MAHandle placeholder = maCreatePlaceholder( );
		int res = maCreateData( placeholder, 1 << 20 );
		//
		// Cleanup after test
		//
		maDestroyObject( placeholder );
		//
		// failed?
		//
		if ( res == RES_OUT_OF_MEMORY )

#else // this block just limits cache size

		if ( mList.size( ) >= (unsigned int)mCapacity )

#endif

		{
			//
			// find and remove oldest in cache
			//
			MapTileKey oldestKey = findLRU( );
			MapTile* oldestTile = mList[oldestKey];
			deleteobject( oldestTile );
			mList.erase( oldestKey );
		}
		//
		// Finally add to cache
		//
		tile->stamp( );
		MapTileKey newKey = MapTileKey( sender, tile->getGridX( ), tile->getGridY(), tile->getMagnification( ) );
		mList.insert( newKey, tile );

		onTileReceived( tile, false );
	}

	//-------------------------------------------------------------------------
	void MapCache::downloadCancelled( MapSource* sender )
	//-------------------------------------------------------------------------
	{
		// TODO: Handle
		DebugPrintf( "MapCache::downloadCancelled\n" );
	}

	//-------------------------------------------------------------------------
	void MapCache::error( MapSource* source, int code )
	//-------------------------------------------------------------------------
	{
		// TODO: Handle
		DebugPrintf( "MapCache::error %d\n", code );
	}

	//-------------------------------------------------------------------------
	void MapCache::jobComplete( MapSource* source )
	//-------------------------------------------------------------------------
	{
		onJobComplete( );
	}

	//-------------------------------------------------------------------------
	//
	// Deletes all tiles in cache.
	//
	void MapCache::clear( )
	//-------------------------------------------------------------------------
	{
		for ( HashMap<MapTileKey, MapTile*>::ConstIterator i = mList.begin( ); i != mList.end( ); i++ )
		{
			MapTile* t = i->second;
			deleteobject( t );
		}
		mList.clear( );
	}

	//-------------------------------------------------------------------------
	void MapCache::onTileReceived( MapTile* tile, bool foundInCache )
	//-------------------------------------------------------------------------
	{
		Vector<IMapCacheListener*>* listeners = getBroadcasterListeners<IMapCacheListener>( *this );
		for ( int i = 0; i < listeners->size( ); i ++ )
			(*listeners)[i]->tileReceived( this, tile, foundInCache );
	}

	//-------------------------------------------------------------------------
	void MapCache::onJobComplete( )
	//-------------------------------------------------------------------------
	{
		Vector<IMapCacheListener*>* listeners = getBroadcasterListeners<IMapCacheListener>( *this );
		for ( int i = 0; i < listeners->size( ); i ++ )
			(*listeners)[i]->jobComplete( this );
	}


}
