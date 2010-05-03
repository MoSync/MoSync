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
* \file PixelCoordinate.h
* \brief Global pixel coordinates at a specified magnification
* \author Lars-Åke Vinberg
*/

#ifndef PIXELCOORDINATE_H_
#define PIXELCOORDINATE_H_

namespace MAP
{
	//=========================================================================
	/**
	 * \brief Describes a pixel coordinate on the global grid of pixels.
	 * 
	 * Range is determined by magnification, e.g. for a tile size of 256 and
	 * a magnification of 2 the global grid is 1024x1024 pixels.
	 * For a magnification of 10, the global grid is 262144x262144 pixels.
	 */
	class PixelCoordinate
	//=========================================================================
	{
	public:
		/**
		 * Creates a new pixel coordinate.
		 */
		PixelCoordinate( ) :
			mX( 0 ),
			mY( 0 ),
			mMagnification( 0 )
		{
		}
		/**
		 * Creates a new pixel coordinate.
		 */
		PixelCoordinate( const int magnification, const int x, const int y)
		:	mX( x ),
			mY( y ),
			mMagnification( magnification )
		{
		}
		/**
		 * Returns the horizontal (longitude) component of the pixel coordinate.
		 */
		int getX( ) const					
		{ 
			return mX; 
		}
		/**
		 * Returns the vertical (latitude) component of the pixel coordinate.
		 */
		int getY( ) const					
		{ 
			return mY; 
		}
		/**
		 * Returns the magnification component of the pixel coordinate.
		 */
		int getMagnification( ) const		
		{ 
			return mMagnification; 
		}
		/**
		 * Compares two pixel coordinates, returns true if the first coordinate is equal to the second.
		 */
		bool operator == ( const PixelCoordinate& b ) const
		{
			return getX( ) == b.getX( ) && getY( ) == b.getY( ) && getMagnification( ) == b.getMagnification( );
		}
		/**
		 * Compare two pixel coordinates, returns true if the first coordinate is not equal to the second.
		 */
		bool operator != ( const PixelCoordinate& b ) const
		{
			return getX( ) != b.getX( ) || getY( ) != b.getY( ) || getMagnification( ) != b.getMagnification( );
		}
	private:
		int mX;
		int mY;
		int mMagnification;
	};
}

#endif // PIXELCOORDINATE_H_
