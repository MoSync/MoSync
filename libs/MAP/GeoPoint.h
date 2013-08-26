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

