//
// LayerMapWidget.cpp
//
// Author: Lars Ake Vinberg
//

#include <MAUtil/Graphics.h>
#include <MAUtil/Vector.h>

#include <MAP/MemoryMgr.h>
#include <MAP2/LayerMapWidget.h>
#include "GeoPoint.h"
#include "LayerRenderer.h"
#include "Enumerator.h"
#include <MAP/DebugPrintf.h>

using namespace MAPUtil;

namespace MAP
{
	//
	// Config
	//
	static const bool ScrollToNearest = false;
	static const bool KeepSelectedCentered = true;

	//-------------------------------------------------------------------------
	LayerMapWidget::LayerMapWidget( int x, int y, int width, int height, Widget* parentPtr, bool drawAsPin ) :
	//-------------------------------------------------------------------------
		MapWidget( x, y, width, height, parentPtr ),
		mLayers( ),
		mSelectedLayer( 0 ),
		mSelectedItem( 0 )
	{
	}

	//-------------------------------------------------------------------------
	LayerMapWidget::~LayerMapWidget( )
	//-------------------------------------------------------------------------
	{
		for ( int i = mLayers.size( ) - 1; i >= 0; i-- )
			deleteobject( mLayers[i] );
	}

	//-------------------------------------------------------------------------
	void LayerMapWidget::addLayer( Layer* layer )
	//-------------------------------------------------------------------------
	{
		mLayers.add( layer );
		layer->addListener( this );
	}

	//-------------------------------------------------------------------------
	void LayerMapWidget::removeLayer( Layer* layer )
	//-------------------------------------------------------------------------
	{
	}

	//-------------------------------------------------------------------------
	void LayerMapWidget::clearLayers( )
	//-------------------------------------------------------------------------
	{
		mLayers.clear( );
	}

	//-------------------------------------------------------------------------
	void LayerMapWidget::selectNextLayer( )
	//-------------------------------------------------------------------------
	{
		if ( mLayers.size( ) < 2 )
			return;

		int newLayer = mSelectedLayer +1;
		if ( newLayer >= mLayers.size( ) )
			newLayer = 0;

		selectLayer( newLayer );
	}

	//-------------------------------------------------------------------------
	void LayerMapWidget::selectLayer( int layer )
	//-------------------------------------------------------------------------
	{
		DebugAssert( layer >= 0 && layer < mLayers.size( ) );

		mSelectedLayer = layer;
		centerAndScaleToLayer( );

	}

	//-------------------------------------------------------------------------
	void LayerMapWidget::selectItem( int index )
	//-------------------------------------------------------------------------
	{
		Layer* layer = mLayers[mSelectedLayer];
		mSelectedItem = index;
		if ( mSelectedItem >= layer->size( ) )
			mSelectedItem = 0;
	}

	//-------------------------------------------------------------------------
	void LayerMapWidget::selectNextItem( )
	//-------------------------------------------------------------------------
	{
		Layer* layer = mLayers[mSelectedLayer];

		mSelectedItem++;
		if ( mSelectedItem >= layer->size( ) )
			mSelectedItem = 0;
		panSelectedItemIntoView( );
	}

	//-------------------------------------------------------------------------
	void LayerMapWidget::selectPreviousItem( )
	//-------------------------------------------------------------------------
	{
		DebugAssert( false ); // not implemented
	}

	static inline double Max( double x, double y ) { return x > y ? x : y; }
	static inline double Min( double x, double y ) { return x < y ? x : y; }

	const double LargeNum = 1E20;

	//-------------------------------------------------------------------------
	//
	// Centers map on bounding rect for all items in layer,
	// and scales map so all items are visible.
	//
	void LayerMapWidget::centerAndScaleToLayer( )
	//-------------------------------------------------------------------------
	{
		double left = LargeNum;
		double bottom = LargeNum;
		double right = -LargeNum;
		double top = -LargeNum;
		Layer* layer = mLayers[mSelectedLayer];
		//
		// Skip if layer has no points
		//
		if ( layer->size( ) < 1 )
			return;

		Enumerator<GeoPoint*> e = Enumerator<GeoPoint*>( *layer );

		while ( e.moveNext( ) )
		{
			GeoPoint* item = e.current( );
			LonLat location = item->getLocation( );
			left = Min( left, location.lon );
			right = Max( right, location.lon );
			top = Max( top, location.lat );
			bottom = Min( bottom, location.lat );
		}
		//
		// Skip if no points with location
		//
		if ( right < left || top < bottom )
			return;
		//
		// Find proper magnification
		//
		MapSource* source = getMapSource( );
		int magnification = source->getMagnificationMax( );
		while( magnification > source->getMagnificationMin( ) )
		{
			PixelCoordinate topleft = LonLat( left, top ).toPixels( magnification );
			PixelCoordinate bottomright = LonLat( right, bottom ).toPixels( magnification );
			if (	bottomright.getX( ) - topleft.getX( ) < getWidth( ) - 10 &&
					topleft.getY( ) - bottomright.getY( ) < getHeight( ) - 10 )
				break;
			magnification--;
		}

		//enterMapUpdateScope( );
		//setCenterPosition( LonLat( 0.5 * (left + right), 0.5 * (top + bottom ) ), true, false );
		//setMagnification( magnification );
		//exitMapUpdateScope( false );
		setCenterPosition( LonLat( 0.5 * (left + right), 0.5 * (top + bottom ) ), magnification, true, false );
	}

	//-------------------------------------------------------------------------
	void LayerMapWidget::panSelectedItemIntoView( )
	//-------------------------------------------------------------------------
	{
		Layer* layer = mLayers[mSelectedLayer];
		GeoPoint* item = layer->getItem( mSelectedItem );

		if ( KeepSelectedCentered )
		{
			LonLat itemLoc = item->getLocation( );
			setCenterPosition( itemLoc, false, false );
		}
#if 0 // niklas: always false
		else
		{
			const int ScrollMargin = 60;
			LonLat loc = this->getCenterPosition( );
			LonLat itemLoc = item->getLocation( );
			Point widgetPos = getPosition( );
			PixelCoordinate worldPx = item->getCachedPixelLocation( );
			if ( worldPx.getMagnification() != getMagnification( ) )
			{
				worldPx = itemLoc.toPixels( getMagnification( ) );
				item->setCachedPixelLocation( worldPx );
			}
			MAPoint2d widgetPx = mViewport->worldPixelToViewport( worldPx );
			int centerX = getWidth( ) / 2;
			int centerY = getHeight( ) / 2;
			MAPoint2d newWidgetPx;
			newWidgetPx.x = centerX;
			newWidgetPx.y = centerY;
			int dx = widgetPx.x - centerX;
			int dy = widgetPx.y - centerY;
			
			if ( dx > centerX - ScrollMargin )
				newWidgetPx.x += ( dx - ( centerX - ScrollMargin ) );
			else if ( dx < -( centerX - ScrollMargin ) )
				newWidgetPx.x += ( dx + ( centerX - ScrollMargin ) );

			if ( dy > centerY - ScrollMargin )
				newWidgetPx.y += ( dy - ( centerY - ScrollMargin ) );
			else if ( dy < -( centerY - ScrollMargin ) )
				newWidgetPx.y += ( dy + ( centerY - ScrollMargin ) );

			PixelCoordinate newWorldPx = mViewport->viewportToWorldPixel( newWidgetPx );
			LonLat newLoc = LonLat( newWorldPx );
			setCenterPosition( newLoc, false, false );
		}
#endif // #if 0		
	}

	//-------------------------------------------------------------------------
	void LayerMapWidget::activateSelectedItem( )
	//-------------------------------------------------------------------------
	{
		if ( mLayers.size() == 0 )
			return;

		Layer* layer = mLayers[mSelectedLayer];

		if ( layer == NULL )
			return;

		GeoPoint* current = layer->getItem( mSelectedItem );
		//if ( current == NULL )
		//	return;

		for ( int i = 0; i < mListeners.size( ); i ++ )
			mListeners[i]->layerItemActivated( this, layer, current );
	}

	//-------------------------------------------------------------------------
	bool LayerMapWidget::scrolltoNearest( int keyCode )
	//-------------------------------------------------------------------------
	{
		Layer* layer = mLayers[mSelectedLayer];

		if ( layer == NULL )
			return false;

		LonLat mapCenter = getCenterPosition( );
		double dLeft = 1E5;
		double dRight = 1E5;
		double dUp = 1E5;
		double dDown = 1E5;
		int nearestUp = -1;
		int nearestDown = -1;
		int nearestLeft = -1;
		int nearestRight = -1;
		Point widgetPos = getPosition( );

		GeoPoint* current = layer->getItem( mSelectedItem );

		LonLat currentLoc = /*( current != NULL ) ?*/ current->getLocation( ) /*: mapCenter*/;
		
		Enumerator<GeoPoint*> e = Enumerator<GeoPoint*>( *layer );

		while ( e.moveNext( ) )
		{
			GeoPoint* item = e.current( );
			if ( item == current )
				continue;

			LonLat itemLoc = item->getLocation( );
			//
			// Skip if not on screen
			//
			PixelCoordinate itemWorldPx = item->getCachedPixelLocation( );
			// todo: move into getCachedPixelLocation
			if ( itemWorldPx.getMagnification( ) != getMagnification( ) )
			{
				itemWorldPx = itemLoc.toPixels( getMagnification( ) );
				item->setCachedPixelLocation( itemWorldPx );
			}
			MAPoint2d itemWidgetPx = mViewport->worldPixelToViewport( itemWorldPx );
			if ( itemWidgetPx.x < 0 || itemWidgetPx.x >= getWidth( ) || itemWidgetPx.y < 0 || itemWidgetPx.y >= getHeight( ) )
				continue;

			double dx = currentLoc.lon - itemLoc.lon;
			if ( dx > 0 )
			{
				// Left is lower
				if ( fabs( dx ) < dLeft )
				{
					dLeft = fabs( dx );
					nearestLeft = e.indexOfCurrent( );
				}
			}
			else
			{
				if ( fabs( dx ) < dRight )
				{
					dRight = fabs( dx );
					nearestRight = e.indexOfCurrent( );
				}
			}

			double dy = currentLoc.lat - itemLoc.lat;
			if ( dy > 0 )
			{
				// Down is lower
				if ( fabs( dy ) < dDown )
				{
					dDown = fabs( dy );
					nearestDown = e.indexOfCurrent( );
				}
			}
			else
			{
				if ( fabs( dy ) < dUp )
				{
					dUp = fabs( dy );
					nearestUp = e.indexOfCurrent( );
				}
			}
		}

		switch( keyCode )
		{
		case MAK_UP:
			if ( nearestUp != -1 )
			{
				//enterMapUpdateScope( );
				selectItem( nearestUp );
				//exitMapUpdateScope( false );
				return true;
			}
			break;
		case MAK_DOWN:
			if ( nearestDown != -1 )
			{
				//enterMapUpdateScope( );
				selectItem( nearestDown );
				//exitMapUpdateScope( false );
				return true;
			}
			break;
		case MAK_LEFT:
			if ( nearestLeft != -1 )
			{
				//enterMapUpdateScope( );
				selectItem( nearestLeft );
				//exitMapUpdateScope( false );
				return true;
			}
			break;
		case MAK_RIGHT:
			if ( nearestRight != -1 )
			{
				//enterMapUpdateScope( );
				selectItem( nearestRight );
				//exitMapUpdateScope( false );
				return true;
			}
			break;
		}
		return false;
	}

	//-------------------------------------------------------------------------
	bool LayerMapWidget::handleKeyPress( int keyCode )
	//-------------------------------------------------------------------------
	{
		switch( keyCode )
		{
		case MAK_5:
			//enterMapUpdateScope( );
			selectNextItem( );
			//exitMapUpdateScope( false );
			return true;
		case MAK_FIRE:
		case MAK_0:
			activateSelectedItem( );
			return true;
		case MAK_8:
			selectNextLayer( );
			return true;
		case MAK_UP:
		case MAK_DOWN:
		case MAK_LEFT:
		case MAK_RIGHT:
			
#if 0 // niklas: always false			
			if ( ScrollToNearest && scrolltoNearest( keyCode ) )
				return true;
#endif	
			break;
			
		}
		
		return MapWidget::handleKeyPress( keyCode );
	}

	//-------------------------------------------------------------------------
	bool LayerMapWidget::handleKeyRelease( int keyCode )
	//-------------------------------------------------------------------------
	{
		return MapWidget::handleKeyRelease( keyCode );
	}

	//-------------------------------------------------------------------------
	void LayerMapWidget::drawItem( Layer* layer, GeoPoint* item, bool renderSelected, bool drawText )
	//-------------------------------------------------------------------------
	{
		Point widgetPos = getPosition( );
		PixelCoordinate worldPx = item->getCachedPixelLocation( );
		if ( worldPx.getMagnification() != getMagnification( ) )
		{
			LonLat lonlat = item->getLocation( );
			worldPx = lonlat.toPixels( getMagnification( ) );
			item->setCachedPixelLocation( worldPx );
		}
		MAPoint2d widgetPx = mViewport->worldPixelToViewport( worldPx );
		if ( widgetPx.x >= 0 && widgetPx.x < getWidth( ) && widgetPx.y >= 0 && widgetPx.y < getHeight( ) )
		{
			LayerRenderer* renderer = layer->getRenderer( );
			//
			// Render marker
			//
			renderer->renderItem( layer, item, widgetPos.x + widgetPx.x, widgetPos.y + widgetPx.y, selected );
			//
			// Render item text
			//
			if ( drawText )
			{
				renderer->renderItemText( layer, item,  widgetPos.x + widgetPx.x,  widgetPos.y + widgetPx.y, getWidth( ), getHeight( ), widgetPos.x, widgetPos.y );
			}
		}
	}

	//-------------------------------------------------------------------------
	void LayerMapWidget::drawOverlay( )
	//-------------------------------------------------------------------------
	{
		for ( int i = 0; i < mLayers.size( ); i++ )
		{
			Layer* layer = mLayers[i];
			//Layer* layer = mLayers[mSelectedLayer];

			//if ( !layer->isVisible( ) )
			//	continue;

			Enumerator<GeoPoint*> e = Enumerator<GeoPoint*>( *layer );

			while ( e.moveNext( ) )
			{
				GeoPoint* item = e.current( );
				drawItem( layer, item, false, false );
			}

			//
			// Draw description text
			//
			if ( i == mSelectedLayer )
			{
				GeoPoint* item = layer->getItem( mSelectedItem );
				if ( item != NULL )
				{
					if ( mSelectedLayer != -1 /*&& item != NULL*/ )
						drawItem( layer, item, true, true );
				}
			}
		}
	}

	//-------------------------------------------------------------------------
	void LayerMapWidget::dataChanged( Layer* sender )
	//-------------------------------------------------------------------------
	{
		requestRepaint( );
	}

	//-------------------------------------------------------------------------
	void LayerMapWidget::loadComplete( Layer* sender )
	//-------------------------------------------------------------------------
	{
		requestRepaint( );
	}
}
