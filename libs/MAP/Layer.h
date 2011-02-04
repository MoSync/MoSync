//
// Layer.h
//
// Author: Lars Ake Vinberg
//

#ifndef LAYER_H_
#define LAYER_H_

#include <MAUtil/String.h>

#include "MemoryMgr.h"
#include "Broadcaster.h"
#include "MapViewport.h"

using namespace MAPUtil;
using namespace MAUtil;

namespace MAP
{
	//
	// Forward class declarations
	//
	class Layer;
	class LayerRenderer;

	//================================================================================
	//
	// Listener class for Layer
	//
	class ILayerListener
	//=========================================================================
	{
	public:
		virtual void contentChanged( Layer* sender ) = 0;
	};

	//===============================================================================
	//
	// Layer class
	//
	class Layer : 
		public Broadcaster<ILayerListener>
	//=========================================================================
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

