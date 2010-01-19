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

using namespace MAPUtil;

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
			mSourceKind( sourceKind ),
			mGridX( gridX ),
			mGridY( gridY ),
			mMagnification( magnification ),
			mCenter( center ),
			mImage( image ),
			mLastAccessTime( DateTime::minValue( ) )
		{
		}

		virtual ~MapTile( )
		{
			maDestroyObject( mImage );
		}

		void stamp( ) { mLastAccessTime = DateTime::now( ); }

		MapSourceKind getSourceKind( ) const	{ return mSourceKind; }
		int getGridX( ) const					{ return mGridX; }
		int getGridY( ) const					{ return mGridY; }
		int getMagnification( ) const			{ return mMagnification; }
		LonLat getCenter( ) const				{ return mCenter; }
		MAHandle getImage( ) const				{ return mImage; }
		DateTime getLastAccessTime( ) const		{ return mLastAccessTime; }

	private:
		MapSourceKind	mSourceKind;
		int				mGridX;
		int				mGridY;
		int				mMagnification;
		LonLat			mCenter;
		MAHandle		mImage;
		DateTime		mLastAccessTime;
	};
}
#endif // MAPTILE_H_
