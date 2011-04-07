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

