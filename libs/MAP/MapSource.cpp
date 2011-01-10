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
			mListener( listener ), 
			mClientData( clientData ) 
		{
		}

		virtual ~MapSourceInnerClientData( ) 
		{
			deleteobject( mClientData );
		}

		IMapSourceListener* mListener;
		MapSourceClientData* mClientData;
	};

	//=========================================================================
	class MapSourceImageDownloader : public ImageDownloader
	//=========================================================================
	{
	public:
		MapSourceImageDownloader( ) :
			mUrl( 0 ),
			mTileXY( ),
			mClientData ( NULL )
		{
		}

		virtual ~MapSourceImageDownloader( )
		{
			if ( isDownloading( ) )
			{
				cancelDownloading( );
			}
			deleteobject( mClientData );
		}

		//
		// Field accessors
		//
		MapTileCoordinate getTileXY( ) const 
		{
			return mTileXY; 
		}

		void setTileXY( const MapTileCoordinate tileXY ) 
		{
			mTileXY = tileXY; 
		}

		MapSourceInnerClientData* getClientData( ) const 
		{
			return mClientData; 
		}
		
		void setClientData( MapSourceInnerClientData* clientData ) 
		{
			mClientData = clientData; 
		}

		const char* getUrl( ) 
		{
			return mUrl.c_str( ); 
		}

		int beginDownloading( const char* url, MAHandle placeholder = 0 )
		{
			//
			// Make a copy of url so we can reuse it if we get a timeout.
			//
			mUrl = url;
			return ImageDownloader::beginDownloading( url, placeholder );
		}

	private:
		String mUrl;
		MapTileCoordinate mTileXY;
		MapSourceInnerClientData* mClientData;
	};

	//=========================================================================
	class MapSourceQueueEntry
	//=========================================================================
	{
	public:
		MapSourceQueueEntry( const MapTileCoordinate tileXY, MapSourceInnerClientData* clientData ) 
			: mTileXY( tileXY ), 
			mClientData( clientData ) 
		{ 
		}

		virtual ~MapSourceQueueEntry( ) 
		{
			deleteobject( mClientData );
		}

		MapTileCoordinate getTileXY( ) const 
		{
			return mTileXY; 
		}
		
		MapSourceInnerClientData* getClientData( ) const 
		{
			return mClientData; 
		}

		void setClientData( MapSourceInnerClientData* clientData ) 
		{
			mClientData = clientData; 
		}

	private:
		MapTileCoordinate mTileXY;
		MapSourceInnerClientData* mClientData; //  lifespan managed by MapSourceImageDownloader
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
		: mQueue( NULL )
	{
		mQueue = newobject( MapSourceQueue, new MapSourceQueue( QueueSize ) );
		for (int i = 0; i < Downloaders; i++)
			mDownloaders[i] = NULL;
	}

	//-------------------------------------------------------------------------
	MapSource::~MapSource( )
	//-------------------------------------------------------------------------
	{
		mQueue->clear( );
		deleteobject( mQueue );
		//
		// Drop and delete downloader pool
		//
		for ( int i = 0; i < Downloaders; i++ )
		{
			if ( mDownloaders[i] != NULL )
			{
				if ( mDownloaders[i]->isDownloading( ) )
					mDownloaders[i]->cancelDownloading( );
				mDownloaders[i]->removeDownloadListener( this );
				deleteobject( mDownloaders[i] );
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
			mQueue->enqueue( entry );
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
		mQueue->clear( );
	}

	//-------------------------------------------------------------------------
	//
	// Is job already in queue?
	//
	bool MapSource::isInQueue( MapTileCoordinate tileXY )
	//-------------------------------------------------------------------------
	{
		for ( int i = 0; i < mQueue->getCapacity( ); i++ )
		{
			MapSourceQueueEntry* item = mQueue->peekAt( i );
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
			if ( mDownloaders[i] == downloader )
			{
				if ( mDownloaders[i]->isDownloading( ) )
					mDownloaders[i]->cancelDownloading( );
				mDownloaders[i]->removeDownloadListener( this );
				deleteobject( mDownloaders[i] );
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
			mDownloaders[slot] = newobject( MapSourceImageDownloader, new MapSourceImageDownloader( ) );
			mDownloaders[slot]->addDownloadListener( this );
			dequeueNextJob( mDownloaders[slot] );
		}
	}

	//-------------------------------------------------------------------------
	void MapSource::finishedDownloading( Downloader* downloader, MAHandle data )
	//-------------------------------------------------------------------------
	{
		MapSourceImageDownloader* dlr = (MapSourceImageDownloader*)downloader;

		MapTileCoordinate tileXY = dlr->getTileXY( );
		LonLat ll = tileCenterToLonLat( getTileSize( ), tileXY, 0, 0 );
		MapTile* tile = newobject( MapTile, new MapTile( this, tileXY.getX( ), tileXY.getY( ), tileXY.getMagnification( ), ll, data ) );
		MapSourceInnerClientData* clientData = (MapSourceInnerClientData*)dlr->getClientData( );
		clientData->mListener->tileReceived( this, tile, clientData->mClientData );
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
	void MapSource::downloadCancelled( Downloader* downloader )
	//-------------------------------------------------------------------------
	{
		MapSourceImageDownloader* dlr = (MapSourceImageDownloader*)downloader;
		MapSourceInnerClientData* clientData = (MapSourceInnerClientData*)dlr->getClientData( );
		clientData->mListener->downloadCancelled( this );
	}

	//-------------------------------------------------------------------------
	void MapSource::error( Downloader* downloader, int code )
	//-------------------------------------------------------------------------
	{
		MapSourceImageDownloader* dlr = (MapSourceImageDownloader*)downloader;
		MapSourceInnerClientData* clientData = (MapSourceInnerClientData*)dlr->getClientData( );
		clientData->mListener->error( this, code );
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
			if ( mDownloaders[i] == NULL )
				return i;
			if ( !mDownloaders[i]->isDownloading( ) && mDownloaders[i] != protectedDownloader )
			{
				//
				// Release to make slot available.
				//
				mDownloaders[i]->removeDownloadListener( this );
				deleteobject( mDownloaders[i] );
				return i;
			}
		}
		return -1; // all downloaders busy
	}

	//-------------------------------------------------------------------------
	void MapSource::dequeueNextJob( MapSourceImageDownloader* downloader )
	//-------------------------------------------------------------------------
	{
		if ( mQueue->getCount( ) < 1 )
			return;
		MapSourceQueueEntry* entry = mQueue->dequeue( );

		char url[1000];
		getTileUrl( url, entry->getTileXY( ) );

		downloader->setTileXY( entry->getTileXY( ) );
		downloader->setClientData( entry->getClientData( ) );
		// downloader now owns clientdata
		entry->setClientData( NULL );
		int res = downloader->beginDownloading( url, 0 );
		
		MapSourceImageDownloader* dlr = (MapSourceImageDownloader*)downloader;
		MapSourceInnerClientData* clientData = (MapSourceInnerClientData*)dlr->getClientData( );

		if ( res < 0 )
			clientData->mListener->error( this, 0 ); // TODO: Proper error code.

		deleteobject( entry );
	}
}
