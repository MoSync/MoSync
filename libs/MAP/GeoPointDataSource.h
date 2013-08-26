/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/**
* \file GeoPointDataSource.h
* \brief Geographical Point Data Source Handler
* \author Lars-Åke Vinberg
*/
#ifndef GEOPOINTDATASOURCE_H_
#define GEOPOINTDATASOURCE_H_

#include <MAUtil/Vector.h>
#include <MAUtil/String.h>
#include "MemoryMgr.h"
#include "Broadcaster.h"
#include "Enumerator.h"
#include "GeoPoint.h"


namespace MAP
{
	using namespace MAUtil;
	using namespace MAPUtil;

	class GeoPointDataSource;
	class LayerItem;

	/**
	* \brief Listener Class for GeoPointDataSource class.
	*
	*/
	class IGeoPointDataSourceListener
	//=========================================================================
	{
	public:
		virtual void dataChanged( GeoPointDataSource* sender ) = 0;
		virtual void loadComplete( GeoPointDataSource* sender ) = 0;
	};

	//class LayerItem;

	/**
	* \brief Abstract class for accessing GeoPoint data.
	*
	*/
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

