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
* \file LayerMapViewport.h
* \brief Viewport displaying a slippy map with layers overlaid
* \author Lars-�ke Vinberg
*/

#ifndef LAYERMAPVIEWPORT_H_
#define LAYERMAPVIEWPORT_H_

#include "MapViewport.h"
#include "GeoPoint.h"
#include "Broadcaster.h"
#include "Layer.h"



namespace MAP
{
	using namespace MAUI;
	
	class LayerMapViewport;
	class Layer;

	/**
	 * \brief Listener class for LayerMapViewport.
	 *
	 */
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
		//void drawItem( Layer* layer, GeoPoint* item, bool selected, bool drawText );
		virtual void drawOverlay( Rect& bounds, MagnificationType magnification );
		void panSelectionIntoView( );

	private:
		Vector<Layer*> mLayers;
		int mSelectedLayer;
	};
}
#endif // MAPVIEWPORT_H_
