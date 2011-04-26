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

