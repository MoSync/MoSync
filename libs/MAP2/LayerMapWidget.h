//
// LayerMapWidget.h
//
// Author: Lars Ake Vinberg
//

#ifndef LAYERMAPWIDGET_H_
#define LAYERMAPWIDGET_H_

#include <MAP/MapWidget.h>
#include "Layer.h"
#include "GeoPoint.h"


using namespace MAUI;
using namespace MAPUtil;

namespace MAP
{
	class LayerMapWidget;

	//=========================================================================
	class ILayerMapWidgetListener
	//=========================================================================
	{
	public:
		virtual void layerItemActivated( LayerMapWidget* sender, Layer* layer, GeoPoint* item ) = 0;
		virtual void layerItemDeactivated( LayerMapWidget* sender, Layer* layer, GeoPoint* item ) = 0;
	};

	//=========================================================================
	class LayerMapWidget : 
		public MapWidget, 
		public Broadcaster<ILayerMapWidgetListener>,
		ILayerListener
	//=========================================================================
	{
	public:
		LayerMapWidget( int x, int y, int width, int height, Widget* parent, bool drawAsPin );
		
		virtual ~LayerMapWidget( );
		//
		// Layer handling
		//
		Vector<Layer*>& getLayers( ) 
		{ 
			return mLayers; 
		}

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
		virtual void dataChanged( Layer* sender );
		virtual void loadComplete( Layer* sender );

	protected:
		//
		// Redraw
		//
		void drawItem( Layer* layer, GeoPoint* item, bool selected, bool drawText );
		void drawOverlay( );
		void panSelectedItemIntoView( );

	private:
		bool scrolltoNearest( int keyCode );

		Vector<Layer*> mLayers;
		int mSelectedLayer;
		int mSelectedItem;
	};
}
#endif // LAYERMAPWIDGET_H_
