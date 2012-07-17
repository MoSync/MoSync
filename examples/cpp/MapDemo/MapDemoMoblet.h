/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

#ifndef MAPDEMOMOBLET_H_
#define MAPDEMOMOBLET_H_

#include "MobletEx.h"
#include "MapDemoScreen.h"

using namespace MapDemoUI;

namespace MapDemo
{
	//=========================================================================
	class MapDemoMoblet : public MobletEx
	//=========================================================================
	{
	public:
		MapDemoMoblet( );
		virtual ~MapDemoMoblet( );

		virtual void shutdown( ); //GCCATTRIB(noreturn);

		void Terminate( ) { mRun = false; }

	private:
		MapDemoScreen* mScreen;
	};
}

#endif // MAPDEMOMOBLET_H_
