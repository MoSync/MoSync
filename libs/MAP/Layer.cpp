//
// Layer.cpp
//
// Author: Lars Ake Vinberg
//

#include "Layer.h"

namespace MAP
{
	//-------------------------------------------------------------------------
	Layer::Layer( ) 
	//-------------------------------------------------------------------------
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
}
