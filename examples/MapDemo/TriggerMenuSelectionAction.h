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

#ifndef TRIGGERMENUSELECTIONACTION_H_
#define TRIGGERMENUSELECTIONACTION_H_

#include "Action.h"
#include "MemoryMgr.h"

using namespace MAPUtil;

namespace MapDemoUI
{
	class Menu;

	//=========================================================================
	class TriggerMenuSelectionAction: public Action 
	//=========================================================================
	{
	public:
		TriggerMenuSelectionAction( Menu* menu );
		
		virtual ~TriggerMenuSelectionAction( );
		//
		// Action overrides
		//
		virtual const char* getShortName( ) const 
		{  
			return "OK"; 
		}

		virtual Action* clone( ) const 
		{ 
			return newobject( TriggerMenuSelectionAction, new TriggerMenuSelectionAction( mMenu ) ); 
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

#endif // TRIGGERMENUSELECTIONACTION_H_
