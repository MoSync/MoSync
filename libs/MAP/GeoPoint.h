//
// GeoPoint.h
//
// Author: Lars Ake Vinberg
//

#ifndef GEOPOINT_H_
#define GEOPOINT_H_

#include <MAUtil/String.h>

#include "MemoryMgr.h"
#include "LonLat.h"
#include "DateTime.h"

using namespace MAUtil;
using namespace MAPUtil;

namespace MAP
{
	//=========================================================================
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

