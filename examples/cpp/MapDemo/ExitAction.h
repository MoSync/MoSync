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

#ifndef EXITACTION_H_
#define EXITACTION_H_

#include "Action.h"
#include "MobletEx.h"
#include <MAP/MemoryMgr.h>

using namespace MAPUtil;

namespace MapDemoUI
{
	//=========================================================================
	// Forces exit of the app.
	//
	class ExitAction: public Action
	//=========================================================================
	{
	public:
		ExitAction( MobletEx* moblet );

		virtual ~ExitAction( );
		//
		// Action overrides
		//
		virtual const char* getShortName( ) const;

		virtual Action* clone( ) const
		{
			return newobject( ExitAction, new ExitAction( mMoblet ) );
		}

	protected:
		//
		// Action protected overrides
		//
		virtual void performPrim( );

	private:
		MobletEx* mMoblet;
	};
}

#endif // EXITACTION_H_
