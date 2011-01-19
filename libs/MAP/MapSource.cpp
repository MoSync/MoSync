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
#include "TraceScope.h"

namespace MAP
{
	//=========================================================================
	class MapSourceImageDownloader : public ImageDownloader
	//=========================================================================
	{
	public:
		MapSourceImageDownloader( ) :
			mUrl( 0 ),
			mTileXY( )
		{
		}

		virtual ~MapSourceImageDownloader( )
		{
			if ( isDownloading( ) )
			{
				cancelDownloading( );
			}
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

		IMapSourceListener* mListener;

	private:
		String mUrl;
		MapTileCoordinate mTileXY;
	};

	//=========================================================================
	// Abstract base class
	//
	class QueueEntry
	//=========================================================================
	{
	public:
		QueueEntry( IMapSourceListener* listener, bool jobComplete, const MapTileCoordinate tileXY ) 
		:	mListener( listener ),
			mJobComplete( jobComplete ),
			mTileXY( tileXY ) 
		{ 
		}

		virtual ~QueueEntry( ) { }

		IMapSourceListener* mListener;
		bool mJobComplete;
		MapTileCoordinate mTileXY;
	};

	//=========================================================================
	class MapSourceQueue: public Queue<QueueEntry>
	//=========================================================================
	{
	public:
		MapSourceQueue( const int capacity ) 
			: Queue<QueueEntry>( capacity ) 
		{
		}

		virtual	~MapSourceQueue( ) { }
	};

	int createdDownloaders;
	int busyDownloaders;

	//-------------------------------------------------------------------------
	//
	// Creates a new map source
	//
	MapSource::MapSource( ) :
	//-------------------------------------------------------------------------
		mQueue( NULL ),
		mTileCount( 0 )
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
				{
					mDownloaders[i]->cancelDownloading( );
					busyDownloaders--;
					//DebugPrintf( "busyDownloaders: %d\n", busyDownloaders );
				}
				mDownloaders[i]->removeDownloadListener( this );
				deleteobject( mDownloaders[i] );
				createdDownloaders++;
				//DebugPrintf( "createdDownloaders: %d\n", createdDownloaders );
			}
		}
	}

	//-------------------------------------------------------------------------
	//
	// Returns all tiles required to cover specified rectangle around centerpoint.
	//
	void MapSource::requestTile( IMapSourceListener* listener, MapTileCoordinate tileXY )
	//-------------------------------------------------------------------------
	{
		//TraceScope tr = TraceScope( "MapSource::requestTile" );
		if ( !isInQueue( tileXY ) )
		{
			QueueEntry* entry = newobject( QueueEntry, new QueueEntry( listener, false, tileXY ) );
			mQueue->enqueue( entry );
			dequeueIfIdleSlot( NULL );
		}
	}

	//-------------------------------------------------------------------------
	void MapSource::requestJobComplete( IMapSourceListener* listener )
	//-------------------------------------------------------------------------
	{
		QueueEntry* entry = newobject( QueueEntry, new QueueEntry( listener, true, MapTileCoordinate( ) ) );
		mQueue->enqueue( entry );
		dequeueIfIdleSlot( NULL );
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
			QueueEntry* item = mQueue->peekAt( i );
			if ( item != NULL )
			{
				MapTileCoordinate itemTileXY = item->mTileXY;
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
				{
					mDownloaders[i]->cancelDownloading( );
					busyDownloaders--;
					//DebugPrintf( "busyDownloaders: %d\n", busyDownloaders );
				}
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
			if ( mDownloaders[slot] == NULL )
			{
				mDownloaders[slot] = newobject( MapSourceImageDownloader, new MapSourceImageDownloader( ) );
				mDownloaders[slot]->addDownloadListener( this );
				createdDownloaders++;
				//DebugPrintf( "createdDownloaders: %d\n", createdDownloaders );
			}
			dequeueNextJob( mDownloaders[slot] );
		}
	}

	//-------------------------------------------------------------------------
	void MapSource::finishedDownloading( Downloader* downloader, MAHandle data )
	//-------------------------------------------------------------------------
	{
		//TraceScope ts = TraceScope( "MapSource::finishedDownloading" );

		MapSourceImageDownloader* dlr = (MapSourceImageDownloader*)downloader;

		mTileCount++;
		MapTileCoordinate tileXY = dlr->getTileXY( );
		LonLat ll = tileCenterToLonLat( getTileSize( ), tileXY, 0, 0 );
		MapTile* tile = newobject( MapTile, new MapTile( this, tileXY.getX( ), tileXY.getY( ), tileXY.getMagnification( ), ll, data ) );
		onTileReceived( dlr->mListener, tile );
		//
		// Terminate clientdata lifespan
		//

		busyDownloaders--;
		//DebugPrintf( "busyDownloaders: %d\n", busyDownloaders );
		//
		// NOTE: The reason we're not deleting the downloader here is that
		// it's referenced on return from this call.
		// So we have to defer deletion until either a new slot is requested, or
		// when deleting this MapSource.
		//
		dequeueIfIdleSlot( /*dlr*/NULL );
	}

	//-------------------------------------------------------------------------
	void MapSource::downloadCancelled( Downloader* downloader )
	//-------------------------------------------------------------------------
	{
		//TraceScope tr = TraceScope( "MapSource::downloadCancelled" );
		MapSourceImageDownloader* dlr = (MapSourceImageDownloader*)downloader;
		//MapSourceClientData* clientData = (MapSourceClientData*)dlr->getClientData( );
		//clientData->mListener->downloadCancelled( this );
		onDownloadCancelled( dlr->mListener );
	}

	//-------------------------------------------------------------------------
	void MapSource::error( Downloader* downloader, int code )
	//-------------------------------------------------------------------------
	{
		TraceScope tr = TraceScope( "MapSource::error" );
		MapSourceImageDownloader* dlr = (MapSourceImageDownloader*)downloader;
		//MapSourceInnerClientData* clientData = (MapSourceInnerClientData*)dlr->getClientData( );
		//clientData->mListener->error( this, code );
		onError( dlr->mListener, code );
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
				//mDownloaders[i]->removeDownloadListener( this );
				//deleteobject( mDownloaders[i] );
				mDownloaders[i]->mListener = NULL;
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
		QueueEntry* entry = mQueue->dequeue( );

		if ( entry->mJobComplete )
		{
			onJobComplete( entry->mListener );
		}
		else
		{
			char url[1000];
			getTileUrl( url, entry->mTileXY );

			downloader->setTileXY( entry->mTileXY );
			downloader->mListener = entry->mListener;
			int res = downloader->beginDownloading( url, 0 );
			busyDownloaders++;
			//DebugPrintf( "busyDownloaders: %d\n", busyDownloaders );

			//MapSourceImageDownloader* dlr = (MapSourceImageDownloader*)downloader;
			//MapSourceClientData* clientData = (MapSourceClientData*)dlr->getClientData( );

			if ( res < 0 )
				onError( downloader->mListener, 0 ); // TODO: Proper error code.

			deleteobject( entry );
		}
	}

	//-------------------------------------------------------------------------
	void MapSource::onTileReceived( IMapSourceListener* listener, MapTile* tile )
	//-------------------------------------------------------------------------
	{
		//Vector<IMapSourceListener*>* listeners = getBroadcasterListeners<IMapSourceListener>( *this );
		//for ( int i = 0; i < listeners->size( ); i ++ )
		//	(*listeners)[i]->tileReceived( this, tile );
		listener->tileReceived( this, tile );
	}

	//-------------------------------------------------------------------------
	void MapSource::onDownloadCancelled( IMapSourceListener* listener )
	//-------------------------------------------------------------------------
	{
		//Vector<IMapSourceListener*>* listeners = getBroadcasterListeners<IMapSourceListener>( *this );
		//for ( int i = 0; i < listeners->size( ); i ++ )
		//	(*listeners)[i]->downloadCancelled( this );
		listener->downloadCancelled( this );
	}

	//-------------------------------------------------------------------------
	void MapSource::onJobComplete( IMapSourceListener* listener )
	//-------------------------------------------------------------------------
	{
		//Vector<IMapSourceListener*>* listeners = getBroadcasterListeners<IMapSourceListener>( *this );
		//for ( int i = 0; i < listeners->size( ); i ++ )
		//	(*listeners)[i]->jobComplete( this );
		listener->jobComplete( this );
	}

	//-------------------------------------------------------------------------
	void MapSource::onError( IMapSourceListener* listener, int code )
	//-------------------------------------------------------------------------
	{
		//Vector<IMapSourceListener*>* listeners = getBroadcasterListeners<IMapSourceListener>( *this );
		//for ( int i = 0; i < listeners->size( ); i ++ )
		//	(*listeners)[i]->error( this, code );
		listener->error( this, code );
	}

}
