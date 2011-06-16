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

#ifndef MAPLOCATIONACTION_H_
#define MAPLOCATIONCTION_H_

#include "Action.h"
#include <MAP/MapWidget.h>
#include <MAP/MemoryMgr.h>

using namespace MapDemoUI;
using namespace MAP;

namespace MapDemo
{
	//=========================================================================
	//
	// Repositions map of a MapWidget to the specified location
	//
	class MapLocationAction: public Action 
	//=========================================================================
	{
	public:
		MapLocationAction( MapWidget* widget, LonLat location, const char* label );
		
		virtual	~MapLocationAction( );
		//
		// Action overrides
		//
		virtual const char* getShortName( ) const;
		
		virtual Action* clone( ) const 
		{ 
			return newobject( MapLocationAction, new MapLocationAction( mWidget, mLocation, mLabel ) ); 
		}
		
	protected:
		//
		// Action protected overrides
		//
		virtual void performPrim( );

	private:
		MapWidget* mWidget;
		LonLat mLocation;
		const char* mLabel;
	};
}

#endif // MAPLOCATIONACTION_H_
