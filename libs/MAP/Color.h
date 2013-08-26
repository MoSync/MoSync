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
 * \file Color.h
 * \brief Color conversions.
 * \author Lars Ake Vinberg
 */

#ifndef COLOR_H_
#define COLOR_H_

namespace MAPUtil
{
	/**
	 * \brief Small utility class for representing colors.
	 */
	class Color
	{
	public:
		Color( ) 
		{ 
		}

	private:
		Color( int value ) 
			: mVal( value ) 
		{ 
		}

	public:
		~Color( ) { } // must not be virtual, to avoid vtable

		static Color fromInt( int value ) 
		{
			return Color( value ); 
		}

		int val( ) const 
		{
			return mVal; 
		}

		static const Color black;
		static const Color white;
		static const Color red;
		static const Color green;
		static const Color blue;

	private:
		int mVal;
	};

	inline bool operator == ( const Color& a, const Color& b ) 
	{
		return a.val( ) == b.val( ); 
	}

	inline bool operator != ( const Color& a, const Color& b ) 
	{
		return a.val( ) != b.val( ); 
	}
}

#endif // COLOR_H_

