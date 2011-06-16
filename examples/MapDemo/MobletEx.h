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

#ifndef MOBLETEX_H_
#define MOBLETEX_H_

#include <MAUtil/Moblet.h>

using namespace MAUtil;

namespace MapDemoUtil
{
	//=========================================================================
	//
	// Abstract class with shutdown handling.
	//
	class MobletEx : public Moblet
	//=========================================================================
	{ 
	public:
		MobletEx( ) 
			: Moblet( ) 
		{ 
		}

		virtual ~MobletEx( ) { }
		//
		// for client to override
		//
		virtual void shutdown( ) = 0;
	};
}

#endif // MOBLETEX_H_
