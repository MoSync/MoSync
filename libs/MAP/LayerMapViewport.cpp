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
* \file LayerMapViewport.cpp
* \brief Viewport displaying a slippy map with layers overlaid
* \author Lars-Åke Vinberg
*/

#include "MapConfig.h"
#include "LayerMapViewport.h"


namespace MAP
{
	//
	// Config
	//
	static const bool ScrollToNearest = false;
	static const bool KeepSelectedCentered = true;

	//-------------------------------------------------------------------------
	LayerMapViewport::LayerMapViewport( )
	//-------------------------------------------------------------------------
	:	MapViewport( ),
		mLayers( ),
		mSelectedLayer( 0 )	
	{
	}
		
	//-------------------------------------------------------------------------
	LayerMapViewport::~LayerMapViewport( )
	//-------------------------------------------------------------------------
	{
		for ( int i = mLayers.size( ) - 1; i >= 0; i-- )
			deleteobject( mLayers[i] );
	}

	//-------------------------------------------------------------------------
	void LayerMapViewport::addLayer( Layer* layer )
	//-------------------------------------------------------------------------
	{
		mLayers.add( layer );
		layer->addListener( this );
	}
		
	//-------------------------------------------------------------------------
	void LayerMapViewport::removeLayer( Layer* layer )
	//-------------------------------------------------------------------------
	{
		// TODO: implement
	}
		
	//-------------------------------------------------------------------------
	void LayerMapViewport::clearLayers( )
	//-------------------------------------------------------------------------
	{
		mLayers.clear( );
		// TODO: disconnect listeners!!!!
	}
		
	//-------------------------------------------------------------------------
	void LayerMapViewport::selectNextLayer( )
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
	void LayerMapViewport::selectLayer( int layer )
	//-------------------------------------------------------------------------
	{
		DebugAssert( layer >= 0 && layer < mLayers.size( ) );

		mSelectedLayer = layer;
		centerAndScaleToLayer( );
	}

	//-------------------------------------------------------------------------
	void LayerMapViewport::selectItem( int index )
	//-------------------------------------------------------------------------
	{
		Layer* layer = mLayers[mSelectedLayer];
		layer->selectItem( index );
	}

	//-------------------------------------------------------------------------
	void LayerMapViewport::selectNextItem( )
	//-------------------------------------------------------------------------
	{
		Layer* layer = mLayers[mSelectedLayer];
		layer->selectNextItem( );

		panSelectionIntoView( );
	}

	//-------------------------------------------------------------------------
	void LayerMapViewport::selectPreviousItem( )
	//-------------------------------------------------------------------------
	{
		Layer* layer = mLayers[mSelectedLayer];
		layer->selectPreviousItem( );
	}

	//-------------------------------------------------------------------------
	void LayerMapViewport::activateSelectedItem( )
	//-------------------------------------------------------------------------
	{
		if ( mLayers.size( ) == 0 )
			return;

		Layer* layer = mLayers[mSelectedLayer];

		if ( layer == NULL )
			return;

		layer->activateSelectedItem( );
	}

	static inline double Max( double x, double y ) { return x > y ? x : y; }
	static inline double Min( double x, double y ) { return x < y ? x : y; }

	const double LargeNum = 1E20;

	//-------------------------------------------------------------------------
	void LayerMapViewport::centerAndScaleToLayer( )
	//-------------------------------------------------------------------------
	{
		double left = LargeNum;
		double bottom = LargeNum;
		double right = -LargeNum;
		double top = -LargeNum;

		Layer* layer = mLayers[mSelectedLayer];

		layer->getBoundingBox( left, top, right, bottom );
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

		setCenterPosition( LonLat( 0.5 * (left + right), 0.5 * (top + bottom ) ), magnification, true, false );
	}

#if 0  // retired but keep code
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
#endif

	//-------------------------------------------------------------------------
	bool LayerMapViewport::handleKeyPress( int keyCode )
	//-------------------------------------------------------------------------
	{
		switch( keyCode )
		{
		case MAK_5:
			selectNextItem( );
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
		
		return MapViewport::handleKeyPress( keyCode );
	}
	
	//-------------------------------------------------------------------------
	bool LayerMapViewport::handleKeyRelease( int keyCode )
	//-------------------------------------------------------------------------
	{
		return MapViewport::handleKeyRelease( keyCode );
	}

#if 0 // retired but keep code
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
#endif

	//-------------------------------------------------------------------------
	void LayerMapViewport::contentChanged( Layer* sender )
	//-------------------------------------------------------------------------
	{
		onViewportUpdated( );
	}

	//-------------------------------------------------------------------------
	void LayerMapViewport::drawOverlay( Rect& bounds, MagnificationType magnification )
	//-------------------------------------------------------------------------
	{
		for ( int i = 0; i < mLayers.size( ); i++ )
		{
			Layer* layer = mLayers[i];
			if ( ! layer->getEnabled( ) )
				continue;
			layer->draw( this, bounds, magnification, i == mSelectedLayer );
		}
	}

	//-------------------------------------------------------------------------
	void LayerMapViewport::panSelectionIntoView( )
	//-------------------------------------------------------------------------
	{
		Layer* layer = mLayers[mSelectedLayer];

		if ( KeepSelectedCentered )
		{
			LonLat itemLoc = layer->getSelectedItemLocation( );
			if ( itemLoc.isValid ( ) )
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
}
