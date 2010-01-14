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

#ifndef MAPCACHE_H_
#define MAPCACHE_H_

#include <MAUtil/Vector.h>
#include "DateTime.h"

#include "MapSource.h"

namespace MAP
{
	class MapTile;
	class MapCache;
	class MapSource;
	class LonLat;

	//=========================================================================
	class IMapCacheListener
	//=========================================================================
	{
	public:
		virtual void tileReceived( MapCache* sender, MapTile* tile ) = 0;
	};

	//=========================================================================
	// Manages map caches for clients to access.
	// Implemented as singleton.
	//
	class MapCache : IMapSourceListener
	//=========================================================================
	{
	private:
							MapCache( );

	public:
		static MapCache*	get( );
		static void			shutdown( );

		virtual				~MapCache( );
		//
		// Requests tiles to cover specified rectangle
		//
		void				requestTiles( IMapCacheListener* listener, MapSourceKind source, const LonLat centerpoint, const int magnification, const int pixelWidth, const int pixelHeight );
		//
		// Deletes all tiles in cache.
		//
		void				clear( );
		//
		// IMapSourceListener implementation
		//
		void				tileReceived( MapSource* sender, MapTile* tile, MapSourceClientData* clientData );
		void				downloadCancelled( MapSource* sender );
		void				error( MapSource* source, int code );
		//
		// Capacity property
		//
		int					getCapacity( ) const;
		void				setCapacity( int capacity );

	private:
		static MapCache*	s_singleton;
		//
		// Returns tile from cache, if available
		//
		int					findInCache( MapSourceKind source, MapTileCoordinate tileXY ) const;
		//
		// returns first unused location in cache
		//
		int					findFreeLocation( ) const;
		//
		// returns location of least recently used tile
		//
		int					findLRU( ) const;
		//
		// Reallocates cache, content is flushed
		//
		void				reallocateCache( );

		MapTile**			m_list;
		int					m_hits;
		int					m_misses;
		int					m_capacity;
	};
}

#endif // MAPCACHE_H_

