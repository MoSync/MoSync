//
// GeoPointLayer.h
//
// Author: Lars Ake Vinberg
//

#ifndef GEOPOINTLAYER_H_
#define GEOPOINTLAYER_H_

#include <MAUtil/String.h>

#include "MemoryMgr.h"
#include "GeoPointDataSource.h"
#include "Broadcaster.h"
#include "Enumerator.h"

#include "Layer.h"
#include "GeoPointLayerRenderer.h"

using namespace MAPUtil;
using namespace MAUtil;

namespace MAP
{
	//
	// Forward class declarations
	//
	class GeoPointLayer;

	//================================================================================
	//
	// Listener class for Layer
	//
	class IGeoPointLayerListener
	//=========================================================================
	{
	public:
		virtual void itemActivated( GeoPointLayer* sender, GeoPoint* item ) = 0;
	};

	//===============================================================================
	//
	// Layer class
	//
	class GeoPointLayer : 
		public Layer,
		public Broadcaster<IGeoPointLayerListener>, 
		IGeoPointDataSourceListener, 
		public IEnumerable<GeoPoint*>
	//=========================================================================
	{
	public:
		GeoPointLayer( );

		virtual ~GeoPointLayer( ) { }

		virtual void draw( MapViewport* viewport, const Rect& bounds, int magnification, bool isLayerSelected );
		//
		// Data source property
		//
		GeoPointDataSource* getDataSource( ) const;
		void setDataSource( GeoPointDataSource* dataSource );
		//
		// Renderer property
		//
		GeoPointLayerRenderer* getRenderer( ) const { return mRenderer; }
		void setRenderer( GeoPointLayerRenderer* renderer ) { mRenderer = renderer; }
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
		// Layer item selection and activation
		//
		virtual void selectItem( int index );
		virtual void selectNextItem( );
		virtual void selectPreviousItem( );
		virtual void activateSelectedItem( );
		virtual void selectItemAtPixel( MapViewport *viewport, int magnification, MAPoint2d screenPixel );
		//
		// Bounding box for items
		//
		virtual void getBoundingBox( double& left, double& top, double& right, double& bottom );
		virtual LonLat getSelectedItemLocation( );

	private:
		void drawItem( MapViewport* viewport, GeoPoint* item, const Rect& bounds, int magnification, bool renderSelected, bool drawText );
		MAPoint2d getScreenPixel( MapViewport* viewport, int magnification, GeoPoint* point );

		GeoPointDataSource* mDataSource;
		GeoPointLayerRenderer* mRenderer;
		int mSelectedItem;
	};
}
#endif // GEOPOINTLAYER_H_

