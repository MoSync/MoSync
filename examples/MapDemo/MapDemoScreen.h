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

#ifndef MAPDEMOSCREEN_H_
#define MAPDEMOSCREEN_H_

#include <MAUI/Screen.h>
#include <MAUtil/Moblet.h>
#include "AppScreen.h"
#include "MapWidget.h"
#include "MapSourceMgr.h"

using namespace MAP;
using namespace UI;

namespace MAPDemo 
{
	//=========================================================================
	//
	// Screen for MAPDemo app
	//
	class MAPDemoScreen : public AppScreen
	//=========================================================================
	{
	public:
								MAPDemoScreen( MobletEx* mMoblet );
		virtual					~MAPDemoScreen( );

		virtual bool			handleKeyPress( int keyCode );
		virtual bool			handleKeyRelease( int keyCode );
		virtual void			enumerateActions( Vector<Action*>& list );

	private:
		void					nextMapSource( );

		MapWidget*				mMap;
		MapSourceKind			mMapSourceKind;
	};
}

#endif // MAPDEMOSCREEN_H_
