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
#include <maapi.h>
#include "MapSource.h"
#include "MapTileCoordinate.h"

namespace MAP
{
	//=========================================================================
	class MapSourceInnerClientData
	//=========================================================================
	{
	public:
		MapSourceInnerClientData( IMapSourceListener* listener, MapSourceClientData* clientData ) :
			m_listener( listener ), 
			m_clientData( clientData ) 
		{
		}

		virtual ~MapSourceInnerClientData( ) 
		{
			deleteobject( m_clientData );
		}

		IMapSourceListener* m_listener;
		MapSourceClientData* m_clientData;
	};

	//=========================================================================
	class MapSourceImageDownloader : public ImageDownloader
	//=========================================================================
	{
	public:
		MapSourceImageDownloader( ) :
			m_sourceKind( MapSourceKind_OpenStreetMap ),
			m_url( 0 ),
			m_tileXY( ),
			m_clientData ( NULL )
		{
		}

		virtual ~MapSourceImageDownloader( )
		{
			if ( isDownloading( ) )
			{
				cancelDownloading( );
			}
			deleteobject( m_clientData );
		}

		//
		// Field accessors
		//
		MapSourceKind		getSourceKind( ) const { return m_sourceKind; }
		void				setSourceKind( MapSourceKind sourceKind ) { m_sourceKind = sourceKind; }
		MapTileCoordinate	getTileXY( ) const { return m_tileXY; }
		void				setTileXY( const MapTileCoordinate tileXY ) { m_tileXY = tileXY; }
		MapSourceInnerClientData* getClientData( ) const { return m_clientData; }
		void				setClientData( MapSourceInnerClientData* clientData ) { m_clientData = clientData; }
		const char*			getUrl( ) { return m_url.c_str( ); }

		int beginDownloading( const char* url, MAHandle placeholder = 0 )
		{
			//
			// Make a copy of url so we can reuse it if we get a timeout.
			//
			m_url = url;
			return ImageDownloader::beginDownloading( url, placeholder );
		}

	private:
		String				m_url;
		MapSourceKind		m_sourceKind;
		MapTileCoordinate	m_tileXY;
		MapSourceInnerClientData* m_clientData;
	};

	//=========================================================================
	class MapSourceQueueEntry
	//=========================================================================
	{
	public:
		MapSourceQueueEntry( const MapTileCoordinate tileXY, MapSourceInnerClientData* clientData ) 
			: m_tileXY( tileXY ), 
			m_clientData( clientData ) 
		{ 
		}

		virtual ~MapSourceQueueEntry( ) 
		{
			deleteobject( m_clientData );
		}

		MapTileCoordinate getTileXY( ) const { return m_tileXY; }
		MapSourceInnerClientData* getClientData( ) const { return m_clientData; }
		void setClientData( MapSourceInnerClientData* clientData ) { m_clientData = clientData; }

	private:
		MapTileCoordinate		m_tileXY;
		MapSourceInnerClientData* m_clientData; //  lifespan managed by MapSourceImageDownloader
	};

	//=========================================================================
	class MapSourceQueue: public Queue<MapSourceQueueEntry>
	//=========================================================================
	{
	public:
		MapSourceQueue( const int capacity ) 
			: Queue<MapSourceQueueEntry>( capacity ) 
		{
		}

		virtual	~MapSourceQueue( ) { }
	};

	//-------------------------------------------------------------------------
	//
	// Creates a new map source
	//
	MapSource::MapSource( )
	//-------------------------------------------------------------------------
		: m_queue( NULL )
	{
		m_queue = newobject( MapSourceQueue, new MapSourceQueue( QueueSize ) );
		for (int i = 0; i < Downloaders; i++)
			m_downloaders[i] = NULL;
	}

	//-------------------------------------------------------------------------
	MapSource::~MapSource( )
	//-------------------------------------------------------------------------
	{
		m_queue->clear( );
		deleteobject( m_queue );
		//
		// Drop and delete downloader pool
		//
		for ( int i = 0; i < Downloaders; i++ )
		{
			if ( m_downloaders[i] != NULL )
			{
				if ( m_downloaders[i]->isDownloading( ) )
					m_downloaders[i]->cancelDownloading( );
				m_downloaders[i]->removeDownloadListener( this );
				deleteobject( m_downloaders[i] );
			}
		}
	}

	//-------------------------------------------------------------------------
	//
	// Returns all tiles required to cover specified rectangle around centerpoint.
	//
	void MapSource::requestTile( MapTileCoordinate tileXY, IMapSourceListener* listener, MapSourceClientData* clientData )
	//-------------------------------------------------------------------------
	{
		if ( !isInQueue( tileXY ) )
		{
			MapSourceInnerClientData* icd = newobject( MapSourceInnerClientData, new MapSourceInnerClientData( listener, clientData ) );
			MapSourceQueueEntry* entry = newobject( MapSourceQueueEntry, new MapSourceQueueEntry( tileXY, icd ) );
			m_queue->enqueue( entry );
			dequeueIfIdleSlot( NULL );
		}
	}

	//-------------------------------------------------------------------------
	//
	// Clears any queued requests
	//
	void MapSource::clearQueue( )
	//-------------------------------------------------------------------------
	{
		m_queue->clear( );
	}

	//-------------------------------------------------------------------------
	//
	// Is job already in queue?
	//
	bool MapSource::isInQueue( MapTileCoordinate tileXY )
	//-------------------------------------------------------------------------
	{
		for ( int i = 0; i < m_queue->getCapacity( ); i++ )
		{
			MapSourceQueueEntry* item = m_queue->peekAt( i );
			if ( item != NULL )
			{
				MapTileCoordinate itemTileXY = item->getTileXY( );
				if ( itemTileXY.getX( ) == tileXY.getX( ) && itemTileXY.getY( ) == tileXY.getY( ) && itemTileXY.getMagnification( ) == tileXY.getMagnification( ) )
					return true;
			}
		}
		return false;
	}

	//-------------------------------------------------------------------------
	//
	// disconnects and removes downloader from list of downloaders
	//
	void MapSource::removeDownloader( MapSourceImageDownloader* downloader )
	//-------------------------------------------------------------------------
	{
		for ( int i = 0; i < Downloaders; i++ )
		{
			if ( m_downloaders[i] == downloader )
			{
				if ( m_downloaders[i]->isDownloading( ) )
					m_downloaders[i]->cancelDownloading( );
				m_downloaders[i]->removeDownloadListener( this );
				deleteobject( m_downloaders[i] );
				break;
			}
		}
	}

	//-------------------------------------------------------------------------
	void MapSource::dequeueIfIdleSlot( MapSourceImageDownloader* protectedDownloader )
	//-------------------------------------------------------------------------
	{
		int slot = findUnusedSlot( protectedDownloader );
		if ( slot != -1 )
		{
			m_downloaders[slot] = newobject( MapSourceImageDownloader, new MapSourceImageDownloader( ) );
			m_downloaders[slot]->addDownloadListener( this );
			dequeueNextJob( m_downloaders[slot] );
		}
	}

	//-------------------------------------------------------------------------
	void MapSource::finishedDownloading( Downloader* downloader, MAHandle data )
	//-------------------------------------------------------------------------
	{
		MapSourceImageDownloader* dlr = (MapSourceImageDownloader*)downloader;

		MapTileCoordinate tileXY = dlr->getTileXY( );
		LonLat ll = tileCenterToLonLat( getTileSize( ), tileXY, 0, 0 );
		MapTile* tile = newobject( MapTile, new MapTile( dlr->getSourceKind( ), tileXY.getX( ), tileXY.getY( ), tileXY.getMagnification( ), ll, data ) );
		MapSourceInnerClientData* clientData = (MapSourceInnerClientData*)dlr->getClientData( );
		clientData->m_listener->tileReceived( this, tile, clientData->m_clientData );
		//
		// Terminate clientdata lifespan
		//
		dlr->setClientData( NULL );
		deleteobject( clientData );
		//
		// NOTE: The reason we're not deleting the downloader here is that
		// it's referenced on return from this call.
		// So we have to defer deletion until either a new slot is requested, or
		// when deleting this MapSource.
		//
		dequeueIfIdleSlot( dlr );
	}

	//-------------------------------------------------------------------------
	void MapSource::downloadCancelled(Downloader* downloader)
	//-------------------------------------------------------------------------
	{
		MapSourceImageDownloader* dlr = (MapSourceImageDownloader*)downloader;
		MapSourceInnerClientData* clientData = (MapSourceInnerClientData*)dlr->getClientData( );
		clientData->m_listener->downloadCancelled( this );
	}

	//-------------------------------------------------------------------------
	void MapSource::error(Downloader* downloader, int code)
	//-------------------------------------------------------------------------
	{
		MapSourceImageDownloader* dlr = (MapSourceImageDownloader*)downloader;
		MapSourceInnerClientData* clientData = (MapSourceInnerClientData*)dlr->getClientData( );
		clientData->m_listener->error( this, code );
	}

	//-------------------------------------------------------------------------
	//
	// returns position of empty downloader slot, or -1 if all downloaders are busy.
	//
	int MapSource::findUnusedSlot( MapSourceImageDownloader* protectedDownloader )
	//-------------------------------------------------------------------------
	{
		for ( int i = 0; i < Downloaders; i++ )
		{
			if ( m_downloaders[i] == NULL )
				return i;
			if ( !m_downloaders[i]->isDownloading( ) && m_downloaders[i] != protectedDownloader )
			{
				//
				// Release to make slot available.
				//
				m_downloaders[i]->removeDownloadListener( this );
				deleteobject( m_downloaders[i] );
				return i;
			}
		}
		return -1; // all downloaders busy
	}

	//-------------------------------------------------------------------------
	void MapSource::dequeueNextJob( MapSourceImageDownloader* downloader )
	//-------------------------------------------------------------------------
	{
		if ( m_queue->getCount( ) < 1 )
			return;
		MapSourceQueueEntry* entry = m_queue->dequeue( );

		char url[1000];
		getTileUrl( url, entry->getTileXY( ) );

		downloader->setSourceKind( getSourceKind( ) );
		downloader->setTileXY( entry->getTileXY( ) );
		downloader->setClientData( entry->getClientData( ) );
		// downloader now owns clientdata
		entry->setClientData( NULL );
		int res = downloader->beginDownloading( url, 0 );
		
		MapSourceImageDownloader* dlr = (MapSourceImageDownloader*)downloader;
		MapSourceInnerClientData* clientData = (MapSourceInnerClientData*)dlr->getClientData( );

		if ( res < 0 )
			clientData->m_listener->error( this, 0 ); // TODO: Proper error code.

		deleteobject( entry );
	}
}
