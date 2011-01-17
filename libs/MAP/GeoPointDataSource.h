//
// GeoPointDataSource.h
//
// Author: Lars Ake Vinberg
//

#ifndef GEOPOINTDATASOURCE_H_
#define GEOPOINTDATASOURCE_H_

#include <MAUtil/Vector.h>
#include <MAUtil/String.h>
#include "MemoryMgr.h"
#include "Broadcaster.h"
#include "Enumerator.h"
#include "GeoPoint.h"

using namespace MAUtil;
using namespace MAPUtil;

namespace MAP
{
	class GeoPointDataSource;
	class LayerItem;

	//=========================================================================
	class IGeoPointDataSourceListener
	//=========================================================================
	{
	public:
		virtual void dataChanged( GeoPointDataSource* sender ) = 0;
		virtual void loadComplete( GeoPointDataSource* sender ) = 0;
	};

	//class LayerItem;

	//=========================================================================
	//
	// Abstract class
	//
	class GeoPointDataSource :
		public Broadcaster<IGeoPointDataSourceListener>,
		public IEnumerable<GeoPoint*>
	//=========================================================================
	{
	public:
		GeoPointDataSource( ) { }
		
		virtual ~GeoPointDataSource( ) { }

		virtual void refresh( ) = 0;
		virtual int size( ) = 0;
		virtual GeoPoint* getItem( int index ) = 0;

		void setTitle( const char* title ) { mTitle = title; }
		const char* getTitle( ) const { return mTitle.c_str( ); }

	protected:
		void onDataChanged( ) 
		{
			Vector<IGeoPointDataSourceListener*>* listeners = getBroadcasterListeners<IGeoPointDataSourceListener>( *this );
			for ( int i = 0; i < listeners->size( ); i++ )
				(*listeners)[i]->dataChanged( this );
		}

		void onLoadComplete( )
		{
			Vector<IGeoPointDataSourceListener*>* listeners = getBroadcasterListeners<IGeoPointDataSourceListener>( *this );
			for ( int i = 0; i < listeners->size( ); i++ )
				(*listeners)[i]->loadComplete( this );
		}

	private:
		String mTitle;
	};

}

#endif // GEOPOINTDATASOURCE_H_

