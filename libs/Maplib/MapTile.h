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

#ifndef MAPTILE_H_
#define MAPTILE_H_

#include "DateTime.h"
#include "LonLat.h"
#include "MapSourceKind.h"

using namespace Util;

namespace MAP
{
	//=========================================================================
	//
	// Tiles use a numbering scheme like Google Maps.
	// http://www.maptiler.org/google-maps-coordinate-system-projection-epsg-900913-3785/
	//
	class MapTile
	//=========================================================================
	{
	public:
		MapTile( MapSourceKind sourceKind, const int gridX, const int gridY, const int magnification, const LonLat center, MAHandle image ) :
			m_sourceKind( sourceKind ),
			m_gridX( gridX ),
			m_gridY( gridY ),
			m_magnification( magnification ),
			m_center( center ),
			m_image( image ),
			m_lastAccessTime( DateTime::minValue( ) )
		{
		}

		virtual ~MapTile( )
		{
			maDestroyObject( m_image );
		}

		void stamp( ) { m_lastAccessTime = DateTime::now( ); }

		MapSourceKind getSourceKind( ) const	{ return m_sourceKind; }
		int getGridX( ) const					{ return m_gridX; }
		int getGridY( ) const					{ return m_gridY; }
		int getMagnification( ) const			{ return m_magnification; }
		LonLat getCenter( ) const				{ return m_center; }
		MAHandle getImage( ) const				{ return m_image; }
		DateTime getLastAccessTime( ) const		{ return m_lastAccessTime; }

	private:
		MapSourceKind	m_sourceKind;
		int				m_gridX;
		int				m_gridY;
		int				m_magnification;
		LonLat			m_center;
		MAHandle		m_image;
		DateTime		m_lastAccessTime;
	};
}
#endif // MAPTILE_H_
