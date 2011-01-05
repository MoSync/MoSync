//
// CollectionLayer.h
//
// Author: Lars Ake Vinberg
//

#if 0 // later

#ifndef COLLECTIONLAYER_H_
#define COLLECTIONLAYER_H_

#include "Layer.h"
//#include <MAUtil/String.h>

#include <MAP/MemoryMgr.h>
//#include "PointDataSource.h"
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
	// Abstract Item Layer class
	//
	template <class T>
	class CollectionLayer : 
		public Layer,
		//IPointDataSourceListener, 
		public IEnumerable<T>
	//=========================================================================
	{
	public:
		CollectionLayer( ) { }

		//
		// Data source property
		//
		//PointDataSource* getDataSource( ) const;
		//void setDataSource( PointDataSource* dataSource );
		//
		// IEnumerable<T> implementation
		//
		virtual int size( ) = 0;
		virtual void getItem( int index, T& item ) = 0;
		////
		//// Selection
		////
		//virtual bool canSelect( ) = 0;
		////
		//// Selects first item
		//// Returns true if there are items
		////
		//virtual void selectFirst( ) = 0;
		////
		//// Returns true if not past end of set
		////
		//virtual void selectnext( ) = 0;
		////
		//// 

	private:
		//PointDataSource* mDataSource;
	};
}

#endif // COLLECTIONLAYER_H_

#endif
