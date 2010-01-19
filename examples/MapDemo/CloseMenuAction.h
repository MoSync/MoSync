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
		virtual					~CloseMenuAction( );
		//
		// Action overrides
		//
		virtual const char*		getShortName( ) const {  return "Back"; }
		virtual Action*			clone( ) const { return newobject( CloseMenuAction, new CloseMenuAction( mMenu ) ); }

	protected:
		//
		// Action protected overrides
		//
		virtual void			performPrim( );

	private:
		Menu*					mMenu;
	};
}

#endif // CLOSEMENUACTION_H_
