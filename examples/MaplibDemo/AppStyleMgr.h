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

#ifndef APPSTYLEMGR_H_
#define APPSTYLEMGR_H_

#include "AppStyle.h"
#include "MemoryMgr.h"

using namespace Util;

namespace UI
{
	//=========================================================================
	//
	// Implemented as singleton.
	//
	class AppStyleMgr
	//=========================================================================
	{
	private:
								AppStyleMgr( ) : m_style( NULL ) { }
	public:
		virtual					~AppStyleMgr( ) { deleteobject( m_style ); }

		static AppStyleMgr*		get( ) { if ( s_singleton == NULL ) { s_singleton = newobject( AppStyleMgr, new AppStyleMgr( ) ); } return s_singleton; }
		static void				shutdown( ) { deleteobject( s_singleton ); }

		static void				setStyle( AppStyle* style ) { get( )->setStylePrim( style ); }
		static AppStyle*		getStyle( ) { return get( )->m_style; }

	private:
		void					setStylePrim( AppStyle* style ) { deleteobject( m_style ); m_style = style; }

		static AppStyleMgr*		s_singleton;
		AppStyle*				m_style;
	};
}

#endif // APPSTYLEMGR_H_
