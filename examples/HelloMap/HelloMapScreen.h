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

#ifndef HELLOMAPSCREEN_H_
#define HELLOMAPSCREEN_H_

#include <MAUI/Screen.h>
#include <MAUtil/Moblet.h>
#include <MAP/MapWidget.h>
#include <MAP/MapSourceMgr.h>

using namespace MAP;

namespace HelloMap 
{
	//=========================================================================
	//
	// Screen for HelloMap app
	//
	class HelloMapScreen : public Screen
	//=========================================================================
	{
	public:
								HelloMapScreen( );
		virtual					~HelloMapScreen( );
		//
		// Screen overrides
		//
		void					keyPressEvent( int keyCode );
		void					keyReleaseEvent( int keyCode );

	private:
		void					nextMapSource( );

		MapWidget*				mMap;
		MapSourceKind			mMapSourceKind;
	};
}

#endif // HELLOMAPSCREEN_H_
