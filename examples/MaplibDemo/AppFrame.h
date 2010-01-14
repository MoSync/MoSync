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

#ifndef APPFRAME_H_
#define APPFRAME_H_

#include <MAUI/Layout.h>
#include <MAUtil/String.h>
#include <MAUI/Font.h>

//#include "MemoryMgr.h"

using namespace MAUI;
using namespace MAUtil;
//using namespace Util;

namespace UI
{
	//=========================================================================
	class AppFrame: public Layout
	//=========================================================================
	{
	public:
							AppFrame(int x, int y, int width, int height, Widget* parent, int columns, int rows );
		virtual				~AppFrame( );
		//
		// Message handling
		//
		void				setMessage( const char* message, float progress );
		void				clearMessage( );
		//
		// Font
		//
		void				setfont( Font* font ) { m_font = font; }
		Font*				getFont( ) const { return m_font; }

	protected:
		virtual void		draw( bool forceDraw=false );
	private:
		String				m_message;
		float				m_progress;
		Font*				m_font;
		Rect				m_textBounds;
		Rect				m_fillBounds;
		Rect				m_progressBounds;
		WidgetSkin*			m_msgSkin;
	};
}
#endif // APPFRAME_H_
