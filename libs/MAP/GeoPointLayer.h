//
// GeoPointLayer.h
//
// Author: Lars Ake Vinberg
//
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

