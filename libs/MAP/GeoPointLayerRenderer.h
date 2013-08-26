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

/**
* \file GeoPointLayerRenderer.h
* \brief Geographical Point Layer Renderer
* \author Lars-Åke Vinberg
*/
#ifndef GEOPOINTLAYERRENDERER_H_
#define GEOPOINTLAYERRENDERER_H_

namespace MAP
{
	class Layer;
	class GeoPoint;

	/**
	 * \brief Abstract base class for client to implement the rendering.
	 *
	 */
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

