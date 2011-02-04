//
// GeoPointLayerRenderer.h
//
// Author: Lars Ake Vinberg
//

#ifndef GEOPOINTLAYERRENDERER_H_
#define GEOPOINTLAYERRENDERER_H_

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
		virtual void renderItem( Layer* layer, GeoPoint* item, const Rect& bounds, int x, int y, bool selected ) = 0;
		virtual void renderItemText( Layer* layer, GeoPoint* item, const Rect&, int x, int y ) = 0;

		virtual Rect getItemRect( Layer* layer, GeoPoint* item, int x, int y, bool selected ) const {
			int size = item->getMarkerSize();
			int halfsize = size / 2;
			return Rect(x - halfsize, y - halfsize, size, size);
		}
	};
}
#endif // GEOPOINTLAYERRENDERER_H_

