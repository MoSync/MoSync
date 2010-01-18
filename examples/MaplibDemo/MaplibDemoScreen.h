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

#ifndef MAPLIBDEMOSCREEN_H_
#define MAPLIBDEMOSCREEN_H_

#include <MAUI/Screen.h>
#include <MAUtil/Moblet.h>
#include "AppScreen.h"
#include "MapWidget.h"
#include "MapSourceMgr.h"

using namespace MAP;
using namespace UI;

namespace MaplibDemo 
{
	//=========================================================================
	//
	// Screen for MaplibDemo app
	//
	class MaplibDemoScreen : public AppScreen
	//=========================================================================
	{
	public:
								MaplibDemoScreen( MobletEx* mMoblet );
		virtual					~MaplibDemoScreen( );

		virtual bool			handleKeyPress( int keyCode );
		virtual bool			handleKeyRelease( int keyCode );
		virtual void			enumerateActions( Vector<Action*>& list );

	private:
		void					nextMapSource( );

		MapWidget*				mMap;
		MapSourceKind			mMapSourceKind;
	};
}

#endif // MAPLIBDEMOSCREEN_H_
