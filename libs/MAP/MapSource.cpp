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
#include <maapi.h>
#include "MapSource.h"
#include "MapTileCoordinate.h"
#include "TraceScope.h"

namespace MAP
{
	//=========================================================================
	class MapSourceImageDownloader :
		#ifdef StoreCompressedTilesInCache
		public Downloader
		#else
		public ImageDownloader
		#endif
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

		int beginDownloading( const char* url, MAHandle placeholder = 0 )
		{
			//
			// Make a copy of url so we can reuse it if we get a timeout.
			//
			mUrl = url;
		
			#ifdef StoreCompressedTilesInCache
			return Downloader::beginDownloading( url, placeholder );
			#else
			return ImageDownloader::beginDownloading( url, placeholder );
			#endif
		}

		#ifdef StoreCompressedTilesInCache

		int getContentLength( ) const
		{
			return mReader->getContentLength( );
		}

		#endif

		IMapSourceListener* mListener;
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
		QueueEntry( )
			: mListener( NULL ),
			mJobComplete( false ),
			mTileXY( MapTileCoordinate( ) )
		{
		}

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
	template <typename T>
	class Stack 
	//=========================================================================
	{
	public:
		//---------------------------------------------------------------------
		void clear( ) 
		//---------------------------------------------------------------------
		{
			mData.clear( ); 
		}
			
		//---------------------------------------------------------------------
		const T& peek( ) const 
		//---------------------------------------------------------------------
		{
			return mData[mData.size( ) - 1];
		}
		
		//---------------------------------------------------------------------
		T& peek( ) 
		//---------------------------------------------------------------------
		{
			return mData[mData.size( ) - 1];
		}	

		//---------------------------------------------------------------------
		T& peek( int i ) 
		//---------------------------------------------------------------------
		{
			return mData[i];
		}	

		//---------------------------------------------------------------------
		T pop( ) 
		//---------------------------------------------------------------------
		{
			T t = peek( );
			mData.resize( mData.size( ) - 1 );
			return t;
		}
		
		//---------------------------------------------------------------------
		void push( const T& d ) 
		//---------------------------------------------------------------------
		{
			mData.add( d );
		}
		
		//---------------------------------------------------------------------
		int size( ) const 
		//---------------------------------------------------------------------
		{ 
			return mData.size( );
		}
		
		//---------------------------------------------------------------------
		bool empty( ) const 
		//---------------------------------------------------------------------
		{
			return mData.size( ) == 0;
		}
		
	private:
		Vector<T> mData;
	};

	//=========================================================================
	class MapSourceQueue: public Stack<QueueEntry>
	//=========================================================================
	{
	public:
		MapSourceQueue( ) 
			: Stack<QueueEntry>( ) 
		{
		}

		virtual	~MapSourceQueue( ) { }
	};

	//-------------------------------------------------------------------------
	//
	// Creates a new map source
	//
	MapSource::MapSource( ) :
	//-------------------------------------------------------------------------
		mQueue( NULL ),
		mTileCount( 0 )
	{
		mDownloaders = new MapSourceImageDownloader*[MapSourceDownloaders];
		mQueue = newobject( MapSourceQueue, new MapSourceQueue( ) );
		for (int i = 0; i < MapSourceDownloaders; i++)
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
		for ( int i = 0; i < MapSourceDownloaders; i++ )
		{
			if ( mDownloaders[i] != NULL )
			{
				if ( mDownloaders[i]->isDownloading( ) )
				{
					mDownloaders[i]->cancelDownloading( );
				}
				mDownloaders[i]->removeDownloadListener( this );
				deleteobject( mDownloaders[i] );
			}
		}
		delete[] mDownloaders;
	}

	//-------------------------------------------------------------------------
	//
	// Returns all tiles required to cover specified rectangle around centerpoint.
	//
	void MapSource::requestTile( IMapSourceListener* listener, MapTileCoordinate tileXY )
	//-------------------------------------------------------------------------
	{
		if ( !isInQueue( tileXY ) && !isInDownloaders( tileXY ) )
		{
			QueueEntry entry = QueueEntry( listener, false, tileXY );
			mQueue->push( entry );
			dequeueIfIdleSlot( NULL );
		}
	}

	//-------------------------------------------------------------------------
	void MapSource::requestJobComplete( IMapSourceListener* listener )
	//-------------------------------------------------------------------------
	{
		QueueEntry entry = QueueEntry( listener, true, MapTileCoordinate( ) );
		mQueue->push( entry );
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
		for ( int i = 0; i < mQueue->size( ); i++ )
		{
			QueueEntry item = mQueue->peek( i );
			MapTileCoordinate itemTileXY = item.mTileXY;
			if ( itemTileXY.getX( ) == tileXY.getX( ) && itemTileXY.getY( ) == tileXY.getY( ) && itemTileXY.getMagnification( ) == tileXY.getMagnification( ) )
				return true;
		}
		return false;
	}

	//-------------------------------------------------------------------------
	//
	// Is job already in queue?
	//
	bool MapSource::isInDownloaders( MapTileCoordinate tileXY )
	//-------------------------------------------------------------------------
	{
		for ( int i = 0; i <  MapSourceDownloaders; i++ )
		{
			MapSourceImageDownloader* dlr = mDownloaders[i];
			if ( dlr == NULL )
				continue;
			MapTileCoordinate itemTileXY = dlr->mTileXY;
			if ( itemTileXY.getX( ) == tileXY.getX( ) && itemTileXY.getY( ) == tileXY.getY( ) && itemTileXY.getMagnification( ) == tileXY.getMagnification( ) )
				return true;
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
		for ( int i = 0; i < MapSourceDownloaders; i++ )
		{
			if ( mDownloaders[i] == downloader )
			{
				if ( mDownloaders[i]->isDownloading( ) )
				{
					mDownloaders[i]->cancelDownloading( );
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
			}
			dequeueNextJob( mDownloaders[slot] );
		}
	}

	//-------------------------------------------------------------------------
	void MapSource::finishedDownloading( Downloader* downloader, MAHandle data )
	//-------------------------------------------------------------------------
	{
		MapSourceImageDownloader* dlr = (MapSourceImageDownloader*)downloader;

		mTileCount++;
		MapTileCoordinate tileXY = dlr->mTileXY;

		LonLat ll = tileCenterToLonLat( getTileSize( ), tileXY, 0, 0 );

		#ifdef StoreCompressedTilesInCache 
		DebugPrintf("PNG size: %d\n", dlr->getContentLength( ) );
		MapTile* tile = newobject( MapTile, new MapTile( this, tileXY.getX( ), tileXY.getY( ), tileXY.getMagnification( ), ll, data, dlr->getContentLength( ) ) );
		#else
		MapTile* tile = newobject( MapTile, new MapTile( this, tileXY.getX( ), tileXY.getY( ), tileXY.getMagnification( ), ll, data ) );
		#endif // StoreCompressedTilesInCache

		onTileReceived( dlr->mListener, tile );
		//
		// Terminate clientdata lifespan
		//

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
		MapSourceImageDownloader* dlr = (MapSourceImageDownloader*)downloader;
		onDownloadCancelled( dlr->mListener );
	}

	//-------------------------------------------------------------------------
	void MapSource::error( Downloader* downloader, int code )
	//-------------------------------------------------------------------------
	{
		MapSourceImageDownloader* dlr = (MapSourceImageDownloader*)downloader;
		onError( dlr->mListener, code );
	}

	//-------------------------------------------------------------------------
	//
	// returns position of empty downloader slot, or -1 if all downloaders are busy.
	//
	int MapSource::findUnusedSlot( MapSourceImageDownloader* protectedDownloader )
	//-------------------------------------------------------------------------
	{
		for ( int i = 0; i < MapSourceDownloaders; i++ )
		{
			if ( mDownloaders[i] == NULL )
				return i;
			if ( !mDownloaders[i]->isDownloading( ) && mDownloaders[i] != protectedDownloader )
			{
				//
				// Release to make slot available.
				//
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
		if ( mQueue->size( ) < 1 )
			return;
		QueueEntry entry = mQueue->pop( );

		if ( entry.mJobComplete )
		{
			onJobComplete( entry.mListener );
		}
		else
		{
			char url[1000];
			getTileUrl( url, entry.mTileXY );

			downloader->mTileXY = entry.mTileXY;
			downloader->mListener = entry.mListener;
			int res = downloader->beginDownloading( url, 0 );

			if ( res < 0 )
				onError( downloader->mListener, 0 ); // TODO: Proper error code.
		}
	}

	//-------------------------------------------------------------------------
	void MapSource::onTileReceived( IMapSourceListener* listener, MapTile* tile )
	//-------------------------------------------------------------------------
	{
		listener->tileReceived( this, tile );
	}

	//-------------------------------------------------------------------------
	void MapSource::onDownloadCancelled( IMapSourceListener* listener )
	//-------------------------------------------------------------------------
	{
		listener->downloadCancelled( this );
	}

	//-------------------------------------------------------------------------
	void MapSource::onJobComplete( IMapSourceListener* listener )
	//-------------------------------------------------------------------------
	{
		listener->jobComplete( this );
	}

	//-------------------------------------------------------------------------
	void MapSource::onError( IMapSourceListener* listener, int code )
	//-------------------------------------------------------------------------
	{
		listener->error( this, code );
	}

}
