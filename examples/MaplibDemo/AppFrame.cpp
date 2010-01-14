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

#include <MAUtil/Graphics.h>

#include "AppFrame.h"
#include "AppStyleMgr.h"
#include "Color.h"

using namespace MAUI;

namespace UI
{
	const int OutsideMargin = 30;
	const int InsideMargin = 10;
	const Color MessageBackColor = Color::fromInt( 0x202020 );
	const Color MessageForeColor = Color::fromInt( 0xffffff );

	//-------------------------------------------------------------------------
	AppFrame::AppFrame(int x, int y, int width, int height, Widget* parent, int columns, int rows )
	//-------------------------------------------------------------------------
		: Layout( x, y, width, height, parent, columns, rows ),
		m_font( NULL ),
		m_progress( -1 )
	{
		AppStyle* style = AppStyleMgr::getStyle( );
		m_msgSkin = style->getButtonSkin( );
	}

	//-------------------------------------------------------------------------
	AppFrame::~AppFrame( )
	//-------------------------------------------------------------------------
	{
	}

	//-------------------------------------------------------------------------
	void AppFrame::setMessage( const char* message, float progress )
	//-------------------------------------------------------------------------
	{
		if ( progress >= 0 )
		{
			//m_message = message;
			m_progress = progress;
			//Rect bounds( OutsideMargin + InsideMargin, 0, getWidth( ) - 2 * ( OutsideMargin + InsideMargin ), 32767 );
			Rect bounds( OutsideMargin, OutsideMargin, getWidth() - 2 * OutsideMargin, 10 );
			m_progressBounds = bounds;
			requestRepaint( );
		}
		else
		{
			m_message = message;
			m_progress = -1;
			Rect bounds( OutsideMargin + InsideMargin, 0, getWidth( ) - 2 * ( OutsideMargin + InsideMargin ), 32767 );
			MAExtent ext = m_font->getBoundedStringDimensions( m_message.c_str( ), bounds );
			int width = EXTENT_X( ext );
			int height = EXTENT_Y( ext );
			int center = getWidth( ) / 2;
			//int middle = getHeight( ) * 2 / 3;
			int top = OutsideMargin;
			//m_textBounds = Rect( center - width / 2, getHeight( ) - OutsideMargin - InsideMargin - height, width, height );
			m_textBounds = Rect( center - width / 2, top + InsideMargin, width, height );
			m_fillBounds = Rect( m_textBounds.x - InsideMargin, m_textBounds.y - InsideMargin, width + 2 * InsideMargin, height + 2 * InsideMargin );
			requestRepaint( );
		}
	}

	//-------------------------------------------------------------------------
	void AppFrame::clearMessage( )
	//-------------------------------------------------------------------------
	{
		m_message.clear( );
		requestRepaint( );
	}

	//-------------------------------------------------------------------------
	void AppFrame::draw( bool forceDraw )
	//-------------------------------------------------------------------------
	{
		Layout::draw( forceDraw );

		if ( m_message.length( ) > 0 )
		{
			// draw message, if any
			maSetColor( MessageBackColor.val( ) );

			if ( m_progress < 0 )
			{
				//Gfx_fillRect( m_fillBounds.x, m_fillBounds.y, m_fillBounds.width, m_fillBounds.height );

				m_msgSkin->draw( m_fillBounds.x, m_fillBounds.y, m_fillBounds.width, m_fillBounds.height, WidgetSkin::/*eType::*/SELECTED );
				m_font->drawBoundedString( m_message.c_str( ), m_textBounds.x, m_textBounds.y, m_textBounds );
			}
			else
			{
				Gfx_fillRect( m_progressBounds.x, m_progressBounds.y, m_progressBounds.width, m_progressBounds.height );
				maSetColor( MessageForeColor.val( ) );
				Gfx_fillRect( m_progressBounds.x, m_progressBounds.y, (int)( m_progressBounds.width * m_progress ), m_progressBounds.height );
			}
		}
	}
}
