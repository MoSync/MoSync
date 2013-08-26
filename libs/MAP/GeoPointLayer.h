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

//
// GeoPointLayer.h
//
// Author: Lars Ake Vinberg
//

/**
* \file GeoPointLayer.h
* \brief Geographical Point Layer Handler
* \author Lars-Åke Vinberg
*/
#ifndef GEOPOINTLAYER_H_
#define GEOPOINTLAYER_H_

#include <MAUtil/String.h>

#include "MemoryMgr.h"
#include "GeoPointDataSource.h"
#include "Broadcaster.h"
#include "Enumerator.h"

#include "Layer.h"
#include "GeoPointLayerRenderer.h"


namespace MAP
{
	
	using namespace MAPUtil;
	using namespace MAUtil;

	//
	// Forward class declarations
	//
	class GeoPointLayer;

	//================================================================================
	/**
	 * \brief Listener class for GeoPointLayer.
	 *
	 */
	class IGeoPointLayerListener
	//=========================================================================
	{
	public:
		virtual void itemActivated( GeoPointLayer* sender, GeoPoint* item ) = 0;
	};

	//===============================================================================

	/**
	 * \brief Layer class for GeoPoint.
	 *
	 */
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

		virtual void draw( MapViewport* viewport, const Rect& bounds, MagnificationType magnification, bool isLayerSelected );
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
		virtual void selectItemAtPixel( MapViewport *viewport, MagnificationType magnification, MAPoint2d screenPixel );
		//
		// Bounding box for items
		//
		virtual void getBoundingBox( double& left, double& top, double& right, double& bottom );
		virtual LonLat getSelectedItemLocation( );

	private:
		void drawItem( MapViewport* viewport, GeoPoint* item, const Rect& bounds, MagnificationType magnification, bool renderSelected, bool drawText );
		MAPoint2d getScreenPixel( MapViewport* viewport, MagnificationType magnification, GeoPoint* point );

		GeoPointDataSource* mDataSource;
		GeoPointLayerRenderer* mRenderer;
		int mSelectedItem;
	};
}
#endif // GEOPOINTLAYER_H_

