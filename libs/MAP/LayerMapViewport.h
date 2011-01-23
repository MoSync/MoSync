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
* \file LayerMapViewport.h
* \brief Viewport displaying a slippy map with layers overlaid
* \author Lars-Åke Vinberg
*/

#ifndef LAYERMAPVIEWPORT_H_
#define LAYERMAPVIEWPORT_H_

#include "MapViewport.h"
#include "GeoPoint.h"
#include "Broadcaster.h"
#include "Layer.h"

using namespace MAUI;

namespace MAP
{
	class LayerMapViewport;
	class Layer;

	//=========================================================================
	class ILayerMapViewportListener
	//=========================================================================
	{
	public:
		virtual void layerItemActivated( LayerMapViewport* sender, Layer* layer, GeoPoint* item ) = 0;
		virtual void layerItemDeactivated( LayerMapViewport* sender, Layer* layer, GeoPoint* item ) = 0;
	};

	//=========================================================================
	/**
	 * \brief Simple slippy map viewport.
	 */
	class LayerMapViewport : 
		public MapViewport,
		public Broadcaster<ILayerMapViewportListener>,
		ILayerListener
	//=========================================================================
	{
	public:
		LayerMapViewport( );
		
		virtual ~LayerMapViewport( );
		//
		// Layer handling
		//
		Vector<Layer*>& getLayers( ) { return mLayers; }

		void addLayer( Layer* layer );
		
		void removeLayer( Layer* layer );
		
		void clearLayers( );
		
		void selectNextLayer( );
		void selectLayer( int layer );
		//
		// Layer item selection and activation
		//
		void selectItem( int index );
		void selectNextItem( );
		void selectPreviousItem( );
		void activateSelectedItem( );
		void centerAndScaleToLayer( );
		//
		// MapWidget overrides
		//
		virtual bool handleKeyPress( int keyCode );
		virtual bool handleKeyRelease( int keyCode );
		//
		// ILayerListener implementation
		//
		virtual void contentChanged( Layer* sender );

	protected:
		//
		// Redraw
		//
		virtual void drawOverlay( Rect& bounds, int magnification );
		void panSelectionIntoView( );

	private:
		Vector<Layer*> mLayers;
		int mSelectedLayer;
	};
}
#endif // MAPVIEWPORT_H_
