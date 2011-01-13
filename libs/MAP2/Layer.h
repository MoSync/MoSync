//
// Layer.h
//
// Author: Lars Ake Vinberg
//

#ifndef LAYER_H_
#define LAYER_H_

#include <MAUtil/String.h>

#include <MAP/MemoryMgr.h>
#include "GeoPointDataSource.h"
#include <MAP/Broadcaster.h>
#include "Enumerator.h"

using namespace MAPUtil;
using namespace MAUtil;

namespace MAP
{
	//
	// Forward class declarations
	//
	class Layer;
	//class LayerItem;
	class LayerRenderer;

	//================================================================================
	//
	// Listener class for Layer
	//
	class ILayerListener
	//=========================================================================
	{
	public:
		virtual void dataChanged( Layer* sender ) = 0;
		virtual void loadComplete( Layer* sender ) = 0;
	};

	//===============================================================================
	//
	// Layer class
	//
	class Layer : 
		public Broadcaster<ILayerListener>, 
		IGeoPointDataSourceListener, 
		public IEnumerable<GeoPoint*>
	//=========================================================================
	{
	public:
		Layer( );

		virtual ~Layer( ) { }

		//int size( ) const;
		//
		// Data source property
		//
		GeoPointDataSource* getDataSource( ) const;
		void setDataSource( GeoPointDataSource* dataSource );
		//
		// Renderer property
		//
		LayerRenderer* getRenderer( ) const { return mRenderer; }
		void setRenderer( LayerRenderer* renderer ) { mRenderer = renderer; }
		//
		// Title property
		//
		const char* getTitle( ) const { return mTitle.c_str( ); }
		void setTitle( const char* title ) { mTitle = title; }
		//
		// IGeoPointDataSourceListener implementation
		//
		void dataChanged( GeoPointDataSource* sender );
		void loadComplete( GeoPointDataSource* sender );
		//
		// IEnumerable<GeoPoint> implementation
		//
		int size( );
		GeoPoint* getItem( int index );
		//
		// Selection
		//
		//virtual bool canSelect( ) = 0;

		//virtual void selectItem( int item ) = 0;

		//virtual void selectNextItem( ) = 0;

		//virtual int itemCount( ) = 0;

	private:
		String mTitle;
		GeoPointDataSource* mDataSource;
		LayerRenderer* mRenderer;
	};
}
#endif // LAYER_H_

