//
// GeoPointLayerRenderer.h
//
// Author: Lars Ake Vinberg
//

#ifndef GEOPOINTLAYERRENDERER_H_
#define GEOPOINTLAYERRENDERER_H_

//#include <MAUtil/String.h>

//#include "MemoryMgr.h"
//#include "LonLat.h"
//#include "DateTime.h"

//using namespace MAUtil;
//using namespace MAPUtil;

namespace MAP
{
	class Layer;
	class GeoPoint;

	//=========================================================================
	// Abstract base class for client to implement.
	class GeoPointLayerRenderer
	//=========================================================================
	{
	public:
		virtual ~GeoPointLayerRenderer( ) { };
		virtual void renderItem( Layer* layer, GeoPoint* item, int x, int y, bool selected ) = 0;
		virtual void renderItemText( Layer* layer, GeoPoint* item, int x, int y, const Rect& bounds ) = 0;
	};
}
#endif // GEOPOINTLAYERRENDERER_H_

