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
}
