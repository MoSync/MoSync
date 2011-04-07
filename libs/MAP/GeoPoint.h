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
* \file GeoPoint.h
* \brief Geographical Point Information Classes
* \author Lars-Åke Vinberg
*/
#ifndef GEOPOINT_H_
#define GEOPOINT_H_

#include <MAUtil/String.h>

#include "MemoryMgr.h"
#include "LonLat.h"
#include "DateTime.h"



namespace MAP
{

	using namespace MAUtil;
	using namespace MAPUtil;

	/**
	 * \brief A Geographical location point that stores location information for a point. 
	 * 
	 */
	class GeoPoint
	//=========================================================================
	{
	public:
		GeoPoint( );

		GeoPoint( LonLat location, const char* title, const char* description, int markerSize, const DateTime timestamp );
		
		virtual ~GeoPoint( );

		LonLat getLocation( ) const			
		{ 
			return mLocation; 
		}

		void setLocation( LonLat location )	
		{ 
			mLocation = location; 
			mCachedWorldPixelLocation = PixelCoordinate( -1, 0, 0 ); //mark as invalid
		}

		PixelCoordinate getCachedPixelLocation( ) const	
		{ 
			return mCachedWorldPixelLocation; 
		}

		void setCachedPixelLocation( PixelCoordinate location ) 
		{ 
			mCachedWorldPixelLocation = location; 
		}

		const char* getTitle( ) const				
		{ 
			return mTitle.c_str( ); 
		}

		void setTitle( const char* title )
		{
			mTitle = title;
		}

		const char* getDescription( ) const			
		{ 
			return mDescription.c_str( ); 
		}

		int getMarkerSize() const			
		{ 
			return mMarkerSize; 
		}

		DateTime getTimestamp( ) const				
		{ 
			return mTimestamp; 
		}

	private:
		LonLat mLocation;
		PixelCoordinate mCachedWorldPixelLocation;
		String mTitle;
		String mDescription;
		int mMarkerSize;
		DateTime mTimestamp;
	};
}
#endif // GEOPOINT_H_

