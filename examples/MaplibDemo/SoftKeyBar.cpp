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

#include <MAUI/Label.h>

#include "SoftKeyBar.h"
#include "AppStyleMgr.h"
#include "MemoryMgr.h"

using namespace Util;

namespace UI
{
	const int Padding = 3;

	//-------------------------------------------------------------------------
	SoftKeyBar::SoftKeyBar(  int x, int y, int width, int height, Widget* parent )
	//-------------------------------------------------------------------------
		: Layout( x, y, width, height, parent, 2, 1 ),
		m_leftAction( NULL ),
		m_rightAction( NULL ),
		m_leftLabel( NULL ),
		m_rightLabel( NULL )
	{
		m_leftAction = NULL;
		m_rightAction = NULL;
		AppStyle* appStyle = AppStyleMgr::getStyle( );
		m_font = appStyle->getFont( FontSize_Medium, Color::white, false );

		//Layout* layout = newobject( Layout, new Layout( 0, 0, width, height, NULL, 2, 1 ) );
		WidgetSkin* skin = appStyle->getWidgetSkin( );

		// Left label
		m_leftLabel = newobject( Label, new Label( 0, 0, width / 2, height, this, "", 0x404040, m_font ) );
		m_leftLabel->setSkin( skin );
		m_leftLabel->setPaddingLeft( Padding );
		m_leftLabel->setVerticalAlignment( Label::VA_CENTER );
		m_leftLabel->setHorizontalAlignment( Label::HA_LEFT );

		// Right label
		m_rightLabel = newobject( Label, new Label( 0,0, width / 2, height, this, "", 0x404040, m_font ) );
		m_rightLabel->setPaddingRight( Padding );
		m_rightLabel->setSkin( skin );
		m_rightLabel->setVerticalAlignment( Label::VA_CENTER );
		m_rightLabel->setHorizontalAlignment( Label::HA_RIGHT );
	}

	//-------------------------------------------------------------------------
	SoftKeyBar::~SoftKeyBar( )
	//-------------------------------------------------------------------------
	{
		getChildren( ).clear( );
		deleteobject( m_leftLabel );
		deleteobject( m_rightLabel );
		deleteobject( m_leftAction );
		deleteobject( m_rightAction );
	}

	//-------------------------------------------------------------------------
	void SoftKeyBar::setWidth( int width )
	//-------------------------------------------------------------------------
	{
		Layout::setWidth( width );

		m_leftLabel->setWidth( width / 2 );
		m_rightLabel->setWidth( width / 2 );
	}

	//-------------------------------------------------------------------------
	void SoftKeyBar::setHeight( int height )
	//-------------------------------------------------------------------------
	{
		Layout::setHeight( height );

		m_leftLabel->setHeight( height );
		m_rightLabel->setHeight( height );
	}

	//-------------------------------------------------------------------------
	bool SoftKeyBar::handleKeyPress( int keyCode )
	//-------------------------------------------------------------------------
	{
		switch( keyCode )
		{
		case MAK_SOFTLEFT:
			if ( m_leftAction != NULL )
			{
				m_leftAction->perform( );
				return true;
			}
			break;
		case MAK_SOFTRIGHT:
			if ( m_rightAction != NULL )
			{
				m_rightAction->perform( );
				return true;
			}
			break;
		}
		return false;
	}

	//-------------------------------------------------------------------------
	bool SoftKeyBar::handleKeyRelease( int keyCode )
	//-------------------------------------------------------------------------
	{
		return false;
	}

	//-------------------------------------------------------------------------
	Action* SoftKeyBar::getLeftAction( ) const
	//-------------------------------------------------------------------------
	{
		return m_leftAction;
	}

	//-------------------------------------------------------------------------
	void SoftKeyBar::setLeftAction( Action* action )
	//-------------------------------------------------------------------------
	{
		deleteobject( m_leftAction );
		m_leftAction = action;
		m_leftLabel->setCaption( action == NULL ? "" : action->getShortName( ) );
	}

	//-------------------------------------------------------------------------
	Action* SoftKeyBar::getRightAction( ) const
	//-------------------------------------------------------------------------
	{
		return m_rightAction;
	}

	//-------------------------------------------------------------------------
	void SoftKeyBar::setRightAction( Action* action )
	//-------------------------------------------------------------------------
	{
		deleteobject( m_rightAction );
		m_rightAction = action;
		m_rightLabel->setCaption( action == NULL ? "" : action->getShortName( ) );
	}
}

