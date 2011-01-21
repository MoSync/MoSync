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
		//return THashFunction<int>(data.w | (data.h<<12) | (((int)data.type)<<24)) - THashFunction<int>((int)data.skin);
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
		//reallocateCache( mCapacity );
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
		//deleteobject( mList );
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
		//reallocateCache( capacity ); 
		mCapacity = capacity;
	}

	//-------------------------------------------------------------------------
	int MapCache::size( )
	//-------------------------------------------------------------------------
	{
		//int s = 0;
		//for ( int i = 0; i < mCapacity; i++ )
		//{
		//	const MapTile* t = mList[i];
		//	if ( t != NULL )
		//		s++;
		//}
		//return s;
		return mList.size( );
	}

	////-------------------------------------------------------------------------
	////
	//// Reallocates cache, content is flushed
	////
	//void MapCache::reallocateCache( int capacity )
	////-------------------------------------------------------------------------
	//{
	//	//
	//	// Free existing
	//	//
	//	if ( mList != NULL )
	//	{
	//		clear( );
	//		deleteobject( mList );
	//	}
	//	//
	//	// Set new capacity
	//	//
	//	mCapacity = capacity;
	//	//
	//	// Alloc new
	//	//
	//	mList = newobject( MapTile*, new MapTile*[mCapacity] );
	//	for ( int i = 0; i < mCapacity; i++ )
	//		mList[i] = NULL;
	//}

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

		//TraceScope ts = TraceScope( "MapCache::requestTiles" );

		if ( source == NULL ) 
			return;
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
			source->requestTile( this, tileXY );
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
		int left = Min( llTile.getX( ), urTile.getX( ) );
		int right = Max( llTile.getX( ), urTile.getX( ) ); 
		int top = Min( llTile.getY( ), urTile.getY( ) );
		int bottom = Max( llTile.getY( ), urTile.getY( ) );
		//
		// Queue all tiles in area
		//
		//const double halfTileSize = 0.5 * tileSize;
		int xStep = directionX < 0 ? 1 : -1;
		int yStep = directionY > 0 ? 1 : -1;
		int yMin = directionY > 0 ? top : bottom;
		int yMax = directionY > 0 ? bottom + 1 : top - 1;
		int xMin = directionX < 0 ? left : right;
		int xMax = directionX < 0 ? right + 1 : left - 1;

		//DebugPrintf( "y: %d to %d x: %d to %d\n", yMin, yMax, xMin, xMax );

		for ( int y = yMin; y != yMax; y += yStep )
		{
			for ( int x = xMin; x != xMax; x += xStep )
			{
				//DebugPrintf( "xy: %d, %d\n", x, y );
				//
				// In cache? Then immediately return tile in cache
				//
				//MapTileCoordinate tileXY = MapTileCoordinate( x, y, magnification );
				MapTileKey key = MapTileKey( source, x, y, magnification );

				//int loc = findInCache( key );
				//Iterator i = numbers.find(4);
				//if(i != numbers.end())
				//	erase(i);

				HashMap<MapTileKey, MapTile*>::Iterator found = mList.find( key );

				if ( found != mList.end( ) )
				{
					HashMap<MapTileKey, MapTile*>::PairKV kv = *found;
					mHits++;
					MapTile* t = kv.second;
					//MapTile* t = mList[loc];
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

	////-------------------------------------------------------------------------
	//int MapCache::findInCache( MapTileKey* key ) const
	////-------------------------------------------------------------------------
	//{
	//	//for ( int i = 0; i < mCapacity; i++ )
	//	//{
	//	//	const MapTile* t = mList[i];
	//	//	if ( t != NULL )
	//	//		if (	t->getMapSource( ) == source && 
	//	//				t->getGridX( ) == tileXY.getX( ) && 
	//	//				t->getGridY( ) == tileXY.getY( ) && 
	//	//				t->getMagnification( ) == tileXY.getMagnification( ) )
	//	//			return i;
	//	//}
	//	//return -1;

	//	//for ( int i = 0; i < mList.size( ); i++ )
	//	//{
	//	//	const MapTile* t = mList[i];
	//	//	if (	t->getMapSource( ) == source && 
	//	//			t->getGridX( ) == tileXY.getX( ) && 
	//	//			t->getGridY( ) == tileXY.getY( ) && 
	//	//			t->getMagnification( ) == tileXY.getMagnification( ) )
	//	//		return i;
	//	//}
	//	//return -1;
	//}

	//-------------------------------------------------------------------------
	MapTileKey MapCache::findLRU( )
	//-------------------------------------------------------------------------
	{
		//DateTime oldest = DateTime::maxValue( );
		//int loc = -1;
		//for ( int i = 0; i < mCapacity; i++ )
		//{
		//	const MapTile* t = mList[i];
		//	if( t == NULL )
		//		return i;
		//	if ( t->getLastAccessTime( ) < oldest )
		//	{
		//		oldest = t->getLastAccessTime( );
		//		loc = i;
		//	}
		//}
		//return loc;

		//DateTime oldest = DateTime::maxValue( );
		//int loc = -1;
		//for ( int i = 0; i < mList.size( ); i++ )
		//{
		//	const MapTile* t = mList[i];
		//	if ( t->getLastAccessTime( ) < oldest )
		//	{
		//		oldest = t->getLastAccessTime( );
		//		loc = i;
		//	}
		//}
		//return loc;

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

	////-------------------------------------------------------------------------
	//int MapCache::findFreeLocation( ) const
	////-------------------------------------------------------------------------
	//{
	//	for ( int i = 0; i < mCapacity; i++ )
	//		if( mList[i] == NULL )
	//			return i;
	//	return -1;
	//}

	//-------------------------------------------------------------------------
	void MapCache::tileReceived( MapSource* sender, MapTile* tile )
	//-------------------------------------------------------------------------
	{

		#if 0 // old . Niklas: new cache mechanism doesn't work? It just kept on adding stuff to the cache.

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

		onTileReceived( tile, false );
		
		#else

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
			mList.erase( oldestKey );
		}
		//
		// Finally add to cache
		//
		tile->stamp( );
		MapTileKey newKey = MapTileKey( sender, tile->getGridX( ), tile->getGridY(), tile->getMagnification( ) );
		mList.insert( newKey, tile );

		onTileReceived( tile, false );

		#endif
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
		//for ( int i = 0; i < mList.size( ); i++ )
		//{
		//	MapTile* tile = mList[i];
		//	deleteobject( tile );
		//}
		for ( HashMap<MapTileKey, MapTile*>::ConstIterator i = mList.begin( ); i != mList.end( ); i++ )
		{
			//MapTileKey key = i->first;
			MapTile* t = i->second;
			//deleteobject( key );
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
