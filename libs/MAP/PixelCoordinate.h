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

//#include <madmath.h>
#ifndef WIN32
extern "C" double floor(double x);
#endif

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
	template <typename MagnificationT>
	class PixelCoordinateT
	//=========================================================================
	{
	public:
		/**
		 * Creates a new pixel coordinate.
		 */
		PixelCoordinateT( ) :
			mX( 0 ),
			mY( 0 ),
			mMagnification( 0 )
		{
		}
		/**
		 * Creates a new pixel coordinate.
		 */
		PixelCoordinateT( const MagnificationT magnification, const int x, const int y)
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
		MagnificationT getMagnification( ) const		
		{ 
			return mMagnification; 
		}
		/**
		 * Compares two pixel coordinates, returns true if the first coordinate is equal to the second.
		 */
		bool operator == ( const PixelCoordinateT<MagnificationT>& b ) const
		{
			return getX( ) == b.getX( ) && getY( ) == b.getY( ) && getMagnification( ) == b.getMagnification( );
		}
		/**
		 * Compare two pixel coordinates, returns true if the first coordinate is not equal to the second.
		 */
		bool operator != ( const PixelCoordinateT<MagnificationT>& b ) const
		{
			return getX( ) != b.getX( ) || getY( ) != b.getY( ) || getMagnification( ) != b.getMagnification( );
		}
	private:
		int mX;
		int mY;
		MagnificationT mMagnification;
	};
	
	/**
	 * \brief The type used to represent a magnification. 
	 * 
	 * It simultaneously represents a discrete and continuous magnification.
	 */
	class MagnificationType {
		public:
		
		MagnificationType(int v) : value(v) {
		}
		
		MagnificationType(double v): value(v) {
		}
		
		
		MagnificationType& operator=(const MagnificationType& rhs) {
			if(this != &rhs) {
				value = rhs.value;
			}
			return *this;
		}
		
		MagnificationType& operator+=(const MagnificationType& rhs) {
			value += rhs.value;
			return *this;
		}
		
		MagnificationType& operator-=(const MagnificationType& rhs) {
			value -= rhs.value;
			return *this;
		}
		
		MagnificationType& operator*=(const MagnificationType& rhs) {
			value *= rhs.value;
			return *this;
		}
	
		MagnificationType& operator/=(const MagnificationType& rhs) {
			value /= rhs.value;
			return *this;
		}	
	
	  	const MagnificationType operator+(const MagnificationType &other) const {
    			MagnificationType result = *this;
    			result += other;
    			return result; 
  		}

	  	const MagnificationType operator-(const MagnificationType &other) const {
    			MagnificationType result = *this;
    			result -= other;
    			return result; 
  		}
	
	  	const MagnificationType operator*(const MagnificationType &other) const {
    			MagnificationType result = *this;
    			result *= other;
    			return result; 
  		}

	  	const MagnificationType operator/(const MagnificationType &other) const {
    			MagnificationType result = *this;
    			result /= other;
    			return result; 
  		}
  		
  	 	bool operator==(const MagnificationType &other) const {
    			return (int)floor(value) == (int)floor(other.value);
  		}	

 	 	bool operator!=(const MagnificationType &other) const {
    			return !(*this == other);
  		}
  		
 		operator int() const { 
 			return (int)floor(value); 
 		}
	
 		operator double() const { 
 			return value; 
 		}
	
		
		private:
			double value;
	
	};
	
	typedef PixelCoordinateT<int> PixelCoordinateI;
	typedef PixelCoordinateT<double> PixelCoordinateD;
	typedef PixelCoordinateT<MagnificationType> PixelCoordinate;
	
}

#endif // PIXELCOORDINATE_H_
