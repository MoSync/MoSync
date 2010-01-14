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

#include "MemoryMgr.h"
#include "MaplibDemoAppStyle.h"
#include "../MAHeaders.h"
#include "Color.h"

namespace MaplibDemo
{
	//-------------------------------------------------------------------------
	MaplibDemoAppStyle::MaplibDemoAppStyle( ) :
	//-------------------------------------------------------------------------
		m_widgetSkin( NULL ),
		m_buttonSkin( NULL ),
		m_fontSmallestBoldBlack( NULL ),
		m_fontSmallestBoldWhite( NULL ),
		m_fontSmallestWhite( NULL ),
		m_fontSmallestBlack( NULL ),
		m_fontSmallBoldWhite( NULL ),
		m_fontSmallBoldBlack( NULL ),
		m_fontSmallWhite( NULL ),
		m_fontSmallBlue( NULL ),
		m_fontSmallBlack( NULL ),
		m_fontMediumBoldWhite( NULL ),
		m_fontMediumWhite( NULL ),
		m_fontLargeBoldWhite( NULL ),
		m_fontLargeBoldBlack( NULL ),
		m_fontLargeWhite( NULL ),
		m_fontLargestBoldWhite( NULL ),
		m_fontLargestWhite( NULL )
	{
		m_widgetSkin = newobject( WidgetSkin, new WidgetSkin( RES_SELECTEDGRAYRAMP, RES_UNSELECTEDGRAYRAMP, 0, 25, 0, 25, false, false ) );
		m_buttonSkin = newobject( WidgetSkin, new WidgetSkin( RES_SMOKESKIN, RES_SMOKESKIN, 8, 57, 8, 56, true, true ) );

		m_fontSmallestBoldBlack = newobject( Font, new Font( RES_FONT_VERDANA13BLACKBOLD ) );
		m_fontSmallestBoldWhite = newobject( Font, new Font( RES_FONT_VERDANA13WHITEBOLD ) );
		m_fontSmallestWhite = newobject( Font, new Font( RES_FONT_VERDANA13WHITE ) );
		m_fontSmallestBlack = newobject( Font, new Font( RES_FONT_VERDANA13BLACK ) );

		m_fontSmallBoldWhite = newobject( Font, new Font( RES_FONT_VERDANA14WHITEBOLD ) );
		m_fontSmallBoldBlack = newobject( Font, new Font( RES_FONT_VERDANA14BLACKBOLD ) );
		m_fontSmallWhite = newobject( Font, new Font( RES_FONT_VERDANA14WHITE ) );
		m_fontSmallBlue = newobject( Font, new Font( RES_FONT_VERDANA14BLUE ) );
		m_fontSmallBlack = newobject( Font, new Font( RES_FONT_VERDANA14BLACK ) );

		m_fontMediumBoldWhite = newobject( Font, new Font( RES_FONT_VERDANA16WHITEBOLD ) );
		m_fontMediumWhite = newobject( Font, new Font( RES_FONT_VERDANA16WHITE ) );

		m_fontLargeBoldWhite = newobject( Font, new Font( RES_FONT_VERDANA18WHITEBOLD ) );
		m_fontLargeBoldBlack = newobject( Font, new Font( RES_FONT_VERDANA18BLACKBOLD ) );
		m_fontLargeWhite = newobject( Font, new Font( RES_FONT_VERDANA18WHITE ) );

		m_fontLargestBoldWhite = newobject( Font, new Font( RES_FONT_VERDANA20WHITEBOLD ) );
		m_fontLargestWhite = newobject( Font, new Font( RES_FONT_VERDANA20WHITE ) );
	}

	//-------------------------------------------------------------------------
	MaplibDemoAppStyle::~MaplibDemoAppStyle( )
	//-------------------------------------------------------------------------
	{
		deleteobject( m_widgetSkin);
		deleteobject( m_buttonSkin);
		deleteobject( m_fontSmallestBoldBlack );
		deleteobject( m_fontSmallestBoldWhite );
		deleteobject( m_fontSmallestWhite );
		deleteobject( m_fontSmallestBlack );
		deleteobject( m_fontSmallBoldBlack );
		deleteobject( m_fontSmallBoldWhite );
		deleteobject( m_fontSmallWhite );
		deleteobject( m_fontSmallBlue );
		deleteobject( m_fontSmallBlack );
		deleteobject( m_fontMediumBoldWhite );
		deleteobject( m_fontMediumWhite );
		deleteobject( m_fontLargeBoldWhite );
		deleteobject( m_fontLargeBoldBlack );
		deleteobject( m_fontLargeWhite );
		deleteobject( m_fontLargestBoldWhite );
		deleteobject( m_fontLargestWhite );
	}

	//-------------------------------------------------------------------------
	Font* MaplibDemoAppStyle::getFont( FontSize size, Color color, bool bold ) const
	//-------------------------------------------------------------------------
	{
		switch( size )
		{
		case FontSize_Smallest:
			if ( bold )
		{
				if ( color == Color::white )
					return m_fontSmallestBoldWhite;
				if ( color == Color::black )
					return m_fontSmallestBoldBlack;
			}
			else
			{
				if ( color == Color::white )
					return m_fontSmallestWhite;
				if ( color == Color::black)
					return m_fontSmallestBlack;
			}
			break;
		case FontSize_Small:
			if ( bold )
			{
				if ( color == Color::black )
					return m_fontSmallBoldBlack;
				if ( color == Color::white )
					return m_fontSmallBoldWhite;
			}
			else
			{
				if ( color == Color::white )
					return m_fontSmallWhite;
				if ( color == Color::blue )
					return m_fontSmallBlue;
				if ( color == Color::black ) 
					return m_fontSmallBlack;
			}
			break;
		case FontSize_Medium:
			if ( bold )
			{
				if ( color == Color::white ) 
					return m_fontMediumBoldWhite;
			}
			else
			{
				if ( color == Color::white )
					return m_fontMediumWhite;
			}
			break;
		case FontSize_Large:
			if ( bold )
			{
				if ( color == Color::white )
					return m_fontLargeBoldWhite;
				if( color == Color::black )
					return m_fontLargeBoldBlack;
			}
			else
			{
				if ( color == Color::white )
					return m_fontLargeWhite;
			}
			break;
		case FontSize_Largest:
			if ( bold )
			{
				if ( color == Color::white )
					return m_fontLargestBoldWhite;
			}
			else
			{
				if ( color == Color::white )
					return m_fontLargestWhite;
			}
			break;
		}
		return NULL;
	}

	//-------------------------------------------------------------------------
	WidgetSkin* MaplibDemoAppStyle::getWidgetSkin( ) const
	//-------------------------------------------------------------------------
	{
		return m_widgetSkin;
	}

	//-------------------------------------------------------------------------
	WidgetSkin* MaplibDemoAppStyle::getButtonSkin( ) const
	//-------------------------------------------------------------------------
	{
		return m_buttonSkin;
	}

	//-------------------------------------------------------------------------
	int MaplibDemoAppStyle::getTabHeight( ) const
	//-------------------------------------------------------------------------
	{
		return 25;
	}

	//-------------------------------------------------------------------------
	int MaplibDemoAppStyle::getMenuItemHeight( ) const
	//-------------------------------------------------------------------------
	{
		return 25;
	}

	//-------------------------------------------------------------------------
	int MaplibDemoAppStyle::getSoftKeyBarHeight( ) const
	//-------------------------------------------------------------------------
	{
		return 25;
	}
}
