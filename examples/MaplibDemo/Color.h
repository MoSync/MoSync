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

#ifndef COLOR_H_
#define COLOR_H_

//#include "DebugPrintf.h"

namespace Util
{
	//=========================================================================
	class Color
	//=========================================================================
	{
	public:
								Color( ) { /*DebugAssert( sizeof( Color ) == sizeof( int ) );*/ }
	private:
								Color( int val ) : m_val( val ) { }
	public:
								~Color( ) { } // must not be virtual, to avoid vtable

		static Color			fromInt( int val ) { return Color( val ); }

		int						val( ) const { return m_val; }

		static const Color		black;
		static const Color		white;
		static const Color		red;
		static const Color		green;
		static const Color		blue;

	private:
		int						m_val;
	};

	inline bool operator == ( const Color& a, const Color& b ) { return a.val( ) == b.val( ); }
	inline bool operator != ( const Color& a, const Color& b ) { return a.val( ) != b.val( ); }
}

#endif // COLOR_H_

