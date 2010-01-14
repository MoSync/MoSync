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

#include "MapLocationAction.h"

namespace MAP
{
	//-------------------------------------------------------------------------
	MapLocationAction::	MapLocationAction( MapWidget* widget, LonLat location, const char* label ) :
	//-------------------------------------------------------------------------
		m_widget( widget ),
		m_location( location ),
		m_label( label )
	{
	}

	//-------------------------------------------------------------------------
	MapLocationAction::~MapLocationAction( )
	//-------------------------------------------------------------------------
	{
	}

	//-------------------------------------------------------------------------
	const char*	MapLocationAction::getShortName( ) const
	//-------------------------------------------------------------------------
	{
		return m_label;
	}

	//-------------------------------------------------------------------------
	void MapLocationAction::performPrim( )
	//-------------------------------------------------------------------------
	{
		m_widget->enterMapUpdateScope( );
		m_widget->setCenterPosition( m_location );
		m_widget->exitMapUpdateScope( false );
	}
}
