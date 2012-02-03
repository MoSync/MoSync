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

#include <MAP/MemoryMgr.h>
#include "MapDemoAppStyle.h"
#include "MAHeaders.h"
#include "Color.h"

using namespace MAPUtil;

namespace MapDemo
{
	//-------------------------------------------------------------------------
	MapDemoAppStyle::MapDemoAppStyle( ) :
	//-------------------------------------------------------------------------
		mWidgetSkin( NULL ),
		mButtonSkin( NULL ),
		mFontSmallestBoldBlack( NULL ),
		mFontSmallestBoldWhite( NULL ),
		mFontSmallestBlack( NULL ),
		mFontSmallestWhite( NULL ),
		mFontSmallBoldBlack( NULL ),
		mFontSmallBoldWhite( NULL ),
		mFontSmallWhite( NULL ),
		mFontSmallBlue( NULL ),
		mFontSmallBlack( NULL ),
		mFontMediumBoldWhite( NULL ),
		mFontMediumWhite( NULL ),
		mFontLargeBoldWhite( NULL ),
		mFontLargeBoldBlack( NULL ),
		mFontLargeWhite( NULL ),
		mFontLargestBoldWhite( NULL ),
		mFontLargestWhite( NULL )
	{
		mWidgetSkin = newobject( WidgetSkin, new WidgetSkin( RES_SELECTEDGRAYRAMP, RES_UNSELECTEDGRAYRAMP, 0, 25, 0, 25, false, false ) );
		mButtonSkin = newobject( WidgetSkin, new WidgetSkin( RES_SMOKESKIN, RES_SMOKESKIN, 8, 57, 8, 56, true, true ) );

		mFontSmallestBoldBlack = newobject( Font, new Font( RES_FONT_VERDANA13BLACKBOLD ) );
		mFontSmallestBoldWhite = newobject( Font, new Font( RES_FONT_VERDANA13WHITEBOLD ) );
		mFontSmallestWhite = newobject( Font, new Font( RES_FONT_VERDANA13WHITE ) );
		mFontSmallestBlack = newobject( Font, new Font( RES_FONT_VERDANA13BLACK ) );

		mFontSmallBoldWhite = newobject( Font, new Font( RES_FONT_VERDANA14WHITEBOLD ) );
		mFontSmallBoldBlack = newobject( Font, new Font( RES_FONT_VERDANA14BLACKBOLD ) );
		mFontSmallWhite = newobject( Font, new Font( RES_FONT_VERDANA14WHITE ) );
		mFontSmallBlue = newobject( Font, new Font( RES_FONT_VERDANA14BLUE ) );
		mFontSmallBlack = newobject( Font, new Font( RES_FONT_VERDANA14BLACK ) );

		mFontMediumBoldWhite = newobject( Font, new Font( RES_FONT_VERDANA16WHITEBOLD ) );
		mFontMediumWhite = newobject( Font, new Font( RES_FONT_VERDANA16WHITE ) );

		mFontLargeBoldWhite = newobject( Font, new Font( RES_FONT_VERDANA18WHITEBOLD ) );
		mFontLargeBoldBlack = newobject( Font, new Font( RES_FONT_VERDANA18BLACKBOLD ) );
		mFontLargeWhite = newobject( Font, new Font( RES_FONT_VERDANA18WHITE ) );

		mFontLargestBoldWhite = newobject( Font, new Font( RES_FONT_VERDANA20WHITEBOLD ) );
		mFontLargestWhite = newobject( Font, new Font( RES_FONT_VERDANA20WHITE ) );
	}

	//-------------------------------------------------------------------------
	MapDemoAppStyle::~MapDemoAppStyle( )
	//-------------------------------------------------------------------------
	{
		deleteobject( mWidgetSkin);
		deleteobject( mButtonSkin);
		deleteobject( mFontSmallestBoldBlack );
		deleteobject( mFontSmallestBoldWhite );
		deleteobject( mFontSmallestWhite );
		deleteobject( mFontSmallestBlack );
		deleteobject( mFontSmallBoldBlack );
		deleteobject( mFontSmallBoldWhite );
		deleteobject( mFontSmallWhite );
		deleteobject( mFontSmallBlue );
		deleteobject( mFontSmallBlack );
		deleteobject( mFontMediumBoldWhite );
		deleteobject( mFontMediumWhite );
		deleteobject( mFontLargeBoldWhite );
		deleteobject( mFontLargeBoldBlack );
		deleteobject( mFontLargeWhite );
		deleteobject( mFontLargestBoldWhite );
		deleteobject( mFontLargestWhite );
	}

	//-------------------------------------------------------------------------
	Font* MapDemoAppStyle::getFont( FontSize size, Color color, bool bold ) const
	//-------------------------------------------------------------------------
	{
		switch( size )
		{
		case FontSize_Smallest:
			if ( bold )
		{
				if ( color == Color::white )
					return mFontSmallestBoldWhite;
				if ( color == Color::black )
					return mFontSmallestBoldBlack;
			}
			else
			{
				if ( color == Color::white )
					return mFontSmallestWhite;
				if ( color == Color::black)
					return mFontSmallestBlack;
			}
			break;
		case FontSize_Small:
			if ( bold )
			{
				if ( color == Color::black )
					return mFontSmallBoldBlack;
				if ( color == Color::white )
					return mFontSmallBoldWhite;
			}
			else
			{
				if ( color == Color::white )
					return mFontSmallWhite;
				if ( color == Color::blue )
					return mFontSmallBlue;
				if ( color == Color::black )
					return mFontSmallBlack;
			}
			break;
		case FontSize_Medium:
			if ( bold )
			{
				if ( color == Color::white )
					return mFontMediumBoldWhite;
			}
			else
			{
				if ( color == Color::white )
					return mFontMediumWhite;
			}
			break;
		case FontSize_Large:
			if ( bold )
			{
				if ( color == Color::white )
					return mFontLargeBoldWhite;
				if( color == Color::black )
					return mFontLargeBoldBlack;
			}
			else
			{
				if ( color == Color::white )
					return mFontLargeWhite;
			}
			break;
		case FontSize_Largest:
			if ( bold )
			{
				if ( color == Color::white )
					return mFontLargestBoldWhite;
			}
			else
			{
				if ( color == Color::white )
					return mFontLargestWhite;
			}
			break;
		}
		return NULL;
	}

	//-------------------------------------------------------------------------
	WidgetSkin* MapDemoAppStyle::getWidgetSkin( ) const
	//-------------------------------------------------------------------------
	{
		return mWidgetSkin;
	}

	//-------------------------------------------------------------------------
	WidgetSkin* MapDemoAppStyle::getButtonSkin( ) const
	//-------------------------------------------------------------------------
	{
		return mButtonSkin;
	}

	//-------------------------------------------------------------------------
	int MapDemoAppStyle::getTabHeight( ) const
	//-------------------------------------------------------------------------
	{
		return 25;
	}

	//-------------------------------------------------------------------------
	int MapDemoAppStyle::getMenuItemHeight( ) const
	//-------------------------------------------------------------------------
	{
		return 25;
	}

	//-------------------------------------------------------------------------
	int MapDemoAppStyle::getSoftKeyBarHeight( ) const
	//-------------------------------------------------------------------------
	{
		return 25;
	}
}
