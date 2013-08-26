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
