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

#include "ModalScreen.h"
//#include "MemoryMgr.h"

using namespace Util;

namespace UI 
{
	//-------------------------------------------------------------------------
	ModalScreen::ModalScreen( )
	//-------------------------------------------------------------------------
	: Screen( )
	{
		//m_previous = previous;
		//
		// Screen size
		//
		MAExtent screenSize = maGetScrSize( );
		m_width = EXTENT_X( screenSize );
		m_height = EXTENT_Y( screenSize );
	}

	//-------------------------------------------------------------------------
	ModalScreen::~ModalScreen( )
	//-------------------------------------------------------------------------
	{
	}

	//-------------------------------------------------------------------------
	void ModalScreen::show( )
	//-------------------------------------------------------------------------
	{
		show( Screen::currentScreen );
	}

	//-------------------------------------------------------------------------
	void ModalScreen::show( Screen* previous )
	//-------------------------------------------------------------------------
	{
		m_previous = previous;
		Screen::show( );
	}

	//-------------------------------------------------------------------------
	//
	// Deletes this, so this pointer is invalid on return.
	// Must be last reference to object.
	//
	void ModalScreen::close( )
	//-------------------------------------------------------------------------
	{
		m_previous->show( );
		for ( int i = 0; i < m_listeners.size(); i++ )
			m_listeners[i]->screenClosed( this );
		//MemoryMgr::untrack( this );
		delete this;
	}

	//-------------------------------------------------------------------------
	void ModalScreen::keyPressEvent( int keyCode )
	//-------------------------------------------------------------------------
	{
		(void)handleKeyPress( keyCode );
	}

	//-------------------------------------------------------------------------
	void ModalScreen::keyReleaseEvent( int keyCode )
	//-------------------------------------------------------------------------
	{
		(void)handleKeyRelease( keyCode );
	}
}
