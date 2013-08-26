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
* \file MapTileCoordinate.h
* \brief Location of a map tile in the tile grid
* \author Lars-Åke Vinberg
*/

#ifndef MAPTILECOORDINATE_H_
#define MAPTILECOORDINATE_H_

namespace MAP
{
	//=========================================================================
	/**
	 * \brief Describes location of a tile in the tile grid.
	 */
	class MapTileCoordinate
	//=========================================================================
	{
	public:
		/**
		 * Creates a map tile coordinate.
		 */
		MapTileCoordinate( ) 
			: mX( 0 ), 
			  mY( 0 ), 
			  mMagnification( 0 ) 
		{
		}
		/**
		 * Creates a map tile coordinate.
		 */
		MapTileCoordinate( int x, int y, int magnification ) 
			: mX( x ),
			  mY( y ),
			  mMagnification( magnification )
		{
		}
		/**
		 * Returns X coordinate of tile in grid.
		 */
		int getX( ) const 
		{ 
			return mX; 
		}
		/*
		 * Returns Y coordinate of tile in grid.
		 */
		int getY( ) const 
		{ 
			return mY; 
		}
		/*
		 * Returns zoom level of tile.
		 */ 
		int getMagnification( ) const 
		{ 
			return mMagnification; 
		}

	private:
		int mX;
		int mY;
		int mMagnification;
	};
}
#endif // MAPTILECOORDINATE_H_
