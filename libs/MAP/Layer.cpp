//
// Layer.cpp
//
// Author: Lars Ake Vinberg
//

#include "MapConfig.h"
#include "Layer.h"

namespace MAP
{
	//-------------------------------------------------------------------------
	Layer::Layer( ) 
	//-------------------------------------------------------------------------
	:	mTitle( "" ),
		mEnabled( true )
	{
	}

	//-------------------------------------------------------------------------
	void Layer::onContentChanged( )
	//-------------------------------------------------------------------------
	{
		Vector<ILayerListener*>* listeners = getBroadcasterListeners<ILayerListener>( *this );
		for ( int i = 0; i < listeners->size( ); i ++ )
			(*listeners)[i]->contentChanged( this );
	}

	//-------------------------------------------------------------------------
	void Layer::setEnabled( bool enabled )
	//-------------------------------------------------------------------------
	{
		mEnabled = enabled;
	}
}
