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
 * \file Layer.h
 * \brief Map layers.
 * \author Lars Ake Vinberg
 */

#ifndef LAYER_H_
#define LAYER_H_

#include <MAUtil/String.h>

#include "MemoryMgr.h"
#include "Broadcaster.h"
#include "MapViewport.h"

namespace MAP
{

	using namespace MAPUtil;
	using namespace MAUtil;
	
	//
	// Forward class declarations
	//
	class Layer;
	class LayerRenderer;

	/**
	 * \brief Inteface for notifications about changes in a map layer.
	 */
	class ILayerListener
	{
	public:
		virtual void contentChanged( Layer* sender ) = 0;
	};

	/**
	 * \brief Representation of a single layer in a map.
	 */
	class Layer : 
		public Broadcaster<ILayerListener>
	{
	public:
		Layer( );

		virtual ~Layer( ) { }

		//
		// Render
		//
		virtual void draw( MapViewport* viewport, const Rect& bounds, MagnificationType magnification, bool isLayerSelected ) = 0;
		//
		// Title property
		//
		const char* getTitle( ) const { return mTitle.c_str( ); }
		void setTitle( const char* title ) { mTitle = title; }
		//
		// Layer item selection and activation
		//
		virtual void selectItem( int index ) = 0;
		virtual void selectNextItem( ) = 0;
		virtual void selectPreviousItem( ) = 0;
		virtual void activateSelectedItem( ) = 0;
		//
		// Bounding box for items
		//
		virtual void getBoundingBox( double& left, double& top, double& right, double& bottom ) = 0;
		virtual LonLat getSelectedItemLocation( ) = 0;
		//
		// Layer enabled flag
		//
		void setEnabled( bool enabled );
		bool getEnabled( ) const { return mEnabled; }
	protected:
		virtual void onContentChanged( );

	private:
		String mTitle;
		bool mEnabled;
	};
}
#endif // LAYER_H_

