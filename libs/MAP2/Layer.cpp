//
// Layer.cpp
//
// Author: Lars Ake Vinberg
//

#include <MAP/MemoryMgr.h>
#include "Layer.h"
#include "GeoPointDataSource.h"
#include "LayerRenderer.h"
#include <MAP/DebugPrintf.h>

namespace MAP
{
	//-------------------------------------------------------------------------
	Layer::Layer( ) :
	//-------------------------------------------------------------------------
		mDataSource( NULL )//,
		//mTitle( NULL ),
		//mRenderer( NULL )
	{
	}

	//-------------------------------------------------------------------------
	GeoPointDataSource* Layer::getDataSource( ) const
	//-------------------------------------------------------------------------
	{
		return mDataSource;
	}

	//-------------------------------------------------------------------------
	void Layer::setDataSource( GeoPointDataSource* dataSource )
	//-------------------------------------------------------------------------
	{
		deleteobject( mDataSource );
		mDataSource = dataSource;
		mDataSource->addListener( this );
	}

	//-------------------------------------------------------------------------
	int Layer::size( )
	//-------------------------------------------------------------------------
	{
		return mDataSource->size( );
	}

	//-------------------------------------------------------------------------
	GeoPoint* Layer::getItem( int index )
	//-------------------------------------------------------------------------
	{
		GeoPoint* item = mDataSource->getItem( index );
		return item;
	}

	//-------------------------------------------------------------------------
	void Layer::dataChanged( GeoPointDataSource* sender )
	//-------------------------------------------------------------------------
	{
		for ( int i = 0; i < mListeners.size( ); i ++ )
			mListeners[i]->dataChanged( this );
	}

	//-------------------------------------------------------------------------
	void Layer::loadComplete( GeoPointDataSource* sender )
	//-------------------------------------------------------------------------
	{
		for ( int i = 0; i < mListeners.size( ); i ++ )
			mListeners[i]->loadComplete( this );
	}
}
