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

#ifndef CLOSEMENUACTION_H_
#define CLOSEMENUACTION_H_

#include "Action.h"
#include "Menu.h"

namespace MapDemoUI
{
	//=========================================================================
	class CloseMenuAction: public Action 
	//=========================================================================
	{
	public:
		CloseMenuAction( Menu* menu );
		virtual ~CloseMenuAction( );
		//
		// Action overrides
		//
		virtual const char* getShortName( ) const {  return "Back"; }
		
		virtual Action* clone( ) const 
		{
			return newobject( CloseMenuAction, new CloseMenuAction( mMenu ) ); 
		}

	protected:
		//
		// Action protected overrides
		//
		virtual void performPrim( );

	private:
		Menu* mMenu;
	};
}

#endif // CLOSEMENUACTION_H_
