//
// GeoPointLayer.h
//
// Author: Lars Ake Vinberg
//

#if 0 // later

#ifndef GEOPOINTLAYER_H_
#define GEOPOINTLAYER_H_

#include "CollectionLayer.h"
//#include <MAUtil/String.h>

#include <MAP/MemoryMgr.h>
#include "GeoPointDataSource.h"
//#include <MAP/Broadcaster.h>
//#include "Enumerator.h"

using namespace MAUtil;

namespace MAP
{
	//
	// Forward class declarations
	//
	//class Layer;
	//class LayerItem;
	//class LayerRenderer;

	//===============================================================================
	//
	// Point Layer class
	//
	class GeoPointLayer : 
		public CollectionLayer<GeoPoint>,
		IGeoPointDataSourceListener 
		//public IEnumerable<T>
	//=========================================================================
	{
	public:
		GeoPointLayer( );

		virtual ~GeoPointLayer( ) { }

		//
		// Data source property
		//
		GeoPointDataSource* getDataSource( ) const;
		void setDataSource( GeoPointDataSource* dataSource );
		//
		// IEnumerable<T> implementation
		//
		int size( );
		void getItem( int index, GeoPoint& item );
		//
		// IGeoPointDataSourceListener implementation
		//
		virtual void dataChanged( GeoPointDataSource* sender );
		virtual void loadComplete( GeoPointDataSource* sender );

	private:
		GeoPointDataSource* mDataSource;
	};
}

#endif // GEOPOINTLAYER_H_

#endif
