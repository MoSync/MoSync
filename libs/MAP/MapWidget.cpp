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

#include "MapConfig.h"
#include <matime.h>
#include "MemoryMgr.h"
#include <mastdlib.h>
#include <MAUtil/Graphics.h>
#include <MAUI/Image.h>
#include "MapWidget.h"

namespace MAP
{
	//-------------------------------------------------------------------------
	MapWidget::MapWidget( int x, int y, int width, int height, Widget* parentPtr )
	//-------------------------------------------------------------------------
	:	Widget( x, y, width, height, parentPtr ),
		mViewport( NULL )
	{
		setDrawBackground( false );
	}

	//-------------------------------------------------------------------------
	void MapWidget::setViewport( MapViewport* viewport )
	//-------------------------------------------------------------------------
	{
		if ( mViewport )
		{
			mViewport->removeListener( this );
			deleteobject( mViewport );
		}
		mViewport = viewport;
		mViewport->addListener( this );
	}

	//-------------------------------------------------------------------------
	MapWidget::~MapWidget( )
	//-------------------------------------------------------------------------
	{
		if ( mViewport )
		{
			mViewport->removeListener( this );
			deleteobject( mViewport );
		}
	}

	//-------------------------------------------------------------------------
	void MapWidget::setMapSource( MapSource* source )
	//-------------------------------------------------------------------------
	{
		mViewport->setMapSource( source );
	}

	//-------------------------------------------------------------------------
	PixelCoordinate MapWidget::getCenterPositionPixels( ) const
	//-------------------------------------------------------------------------
	{
		return mViewport->getCenterPositionPixels( );
	}

	//-------------------------------------------------------------------------
	inline double Max( double x, double y )
	//-------------------------------------------------------------------------
	{
		return x > y ? x : y;
	}

	//-------------------------------------------------------------------------
	LonLat MapWidget::getCenterPosition( ) const
	//-------------------------------------------------------------------------
	{
		return mViewport->getCenterPosition( );
	}

	//-------------------------------------------------------------------------
	void MapWidget::setCenterPosition( LonLat position, bool immediate, bool isPointerEvent )
	//-------------------------------------------------------------------------
	{
		mViewport->setCenterPosition( position, immediate, isPointerEvent );
	}

	//-------------------------------------------------------------------------
	void MapWidget::setCenterPosition( LonLat position, int magnification, bool immediate, bool isPointerEvent )
	//-------------------------------------------------------------------------
	{
		mViewport->setCenterPosition( position, magnification, immediate, isPointerEvent );
	}

	//-------------------------------------------------------------------------
	void MapWidget::centerAndScaleToRectangle( LonLat lowerLeft, LonLat upperRight )
	//-------------------------------------------------------------------------
	{
		mViewport->centerAndScaleToRectangle( lowerLeft, upperRight);
	}

	//-------------------------------------------------------------------------
	void MapWidget::startGlide( )
	//-------------------------------------------------------------------------
	{
		mViewport->startGlide( );
	}

	//-------------------------------------------------------------------------
	void MapWidget::stopGlide( )
	//-------------------------------------------------------------------------
	{
		mViewport->stopGlide( );
		mViewport->updateMap( );
		requestRepaint( );
	}

	//-------------------------------------------------------------------------
	int MapWidget::getMagnification( ) const
	//-------------------------------------------------------------------------
	{
		return mViewport->getMagnification( );
	}

	//-------------------------------------------------------------------------
	void MapWidget::setMagnification( int magnification )
	//-------------------------------------------------------------------------
	{
		mViewport->setMagnification( magnification );
	}

	//-------------------------------------------------------------------------
	Point MapWidget::getActualPosition( )
	//-------------------------------------------------------------------------
	{
		Widget* p = this->getParent( );
		Point pos = getPosition( );

		while( p != 0 )
		{
			pos.x += p->getPosition( ).x;
			pos.y += p->getPosition( ).y;
			p = p->getParent( );
		}
		return pos;
	}

	//-------------------------------------------------------------------------
	void MapWidget::drawWidget( )
	//-------------------------------------------------------------------------
	{
		mViewport->drawViewport( getPosition( ) );
		drawOverlay( );
	}

	//-------------------------------------------------------------------------
	void MapWidget::drawOverlay( )
	//-------------------------------------------------------------------------
	{
	}

	//-------------------------------------------------------------------------
	void MapWidget::zoomIn( )
	//-------------------------------------------------------------------------
	{
		mViewport->zoomIn( );
	}

	//-------------------------------------------------------------------------
	void MapWidget::zoomOut( )
	//-------------------------------------------------------------------------
	{
		mViewport->zoomOut( );
	}

	//-------------------------------------------------------------------------
	bool MapWidget::handleKeyPress( int keyCode )
	//-------------------------------------------------------------------------
	{
		return mViewport->handleKeyPress( keyCode );
	}

	//-------------------------------------------------------------------------
	bool MapWidget::handleKeyRelease( int keyCode )
	//-------------------------------------------------------------------------
	{
		return true;
	}

	//-------------------------------------------------------------------------
	void MapWidget::setWidth( int width )
	//-------------------------------------------------------------------------
	{
		Widget::setWidth( width );
		mViewport->setWidth( width );
	}

	//-------------------------------------------------------------------------
	void MapWidget::setHeight( int height )
	//-------------------------------------------------------------------------
	{
		Widget::setHeight( height );
		mViewport->setHeight( height );
	}

	//-------------------------------------------------------------------------
	void MapWidget::viewportUpdated( MapViewport* viewport )
	//-------------------------------------------------------------------------
	{
		requestRepaint( );
	}

	//-------------------------------------------------------------------------
	void MapWidget::error( MapViewport* viewport, int code )
	//-------------------------------------------------------------------------
	{
		// TODO: inform user that error occurred
	}

}
