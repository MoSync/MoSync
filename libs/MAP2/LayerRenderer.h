//
// LayerRenderer.h
//
// Author: Lars Ake Vinberg
//

#ifndef LAYERRENDERER_H_
#define LAYERRENDERER_H_

//#include <MAUtil/String.h>

//#include <MAP/MemoryMgr.h>
//#include <MAP/LonLat.h>
//#include <MAP/DateTime.h>

//using namespace MAUtil;
//using namespace MAPUtil;

namespace MAP
{
	class Layer;
	class GeoPoint;

	//=========================================================================
	// Abstract base class for client to implement.
	class LayerRenderer
	//=========================================================================
	{
	public:
		virtual ~LayerRenderer( ) { };
		virtual void renderItem( Layer* layer, GeoPoint* item, int x, int y, bool selected ) = 0;
		virtual void renderItemText( Layer* layer, GeoPoint* item, int x, int y, int canvasWidth, int canvasHeight, int offsetX, int offsetY ) = 0;
	};
}
#endif // LAYERITEM_H_

