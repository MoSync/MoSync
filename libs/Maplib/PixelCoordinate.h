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

#ifndef PIXELCOORDINATE_H_
#define PIXELCOORDINATE_H_

namespace MAP
{
	//=========================================================================
	class PixelCoordinate
	//=========================================================================
	{
	public:
		PixelCoordinate( ) :
			m_x( 0 ),
			m_y( 0 ),
			m_magnification( 0 )
		{
		}

		PixelCoordinate( const int magnification, const int x, const int y)
		:	m_x( x ),
			m_y( y ),
			m_magnification( magnification )
		{
		}

		const int getX( ) const					{ return m_x; }
		const int getY( ) const					{ return m_y; }
		const int getMagnification( ) const		{ return m_magnification; }

	private:
		int m_x;
		int m_y;
		int m_magnification;
	};

	static bool operator == ( const PixelCoordinate& a, const PixelCoordinate& b ) { return a.getX( ) == b.getX( ) && a.getY( ) == b.getY( ) && a.getMagnification( ) == b.getMagnification( ); }
	static bool operator != ( const PixelCoordinate& a, const PixelCoordinate& b ) { return a.getX( ) != b.getX( ) || a.getY( ) != b.getY( ) || a.getMagnification( ) != b.getMagnification( ); }
}

#endif // PIXELCOORDINATE_H_
