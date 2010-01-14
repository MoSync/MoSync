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

#ifndef MAPLOCATIONACTION_H_
#define MAPLOCATIONCTION_H_

#include "Action.h"
#include "MapWidget.h"
#include "MemoryMgr.h"

using namespace UI;

namespace MAP
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
		virtual					~MapLocationAction( );
		//
		// Action overrides
		//
		virtual const char*		getShortName( ) const;
		virtual Action*			clone( ) const { return newobject( MapLocationAction, new MapLocationAction( m_widget, m_location, m_label ) ); }
		
	protected:
		//
		// Action protected overrides
		//
		virtual void			performPrim( );

	private:
		MapWidget*				m_widget;
		LonLat					m_location;
		const char*				m_label;
	};
}

#endif // MAPLOCATIONACTION_H_
