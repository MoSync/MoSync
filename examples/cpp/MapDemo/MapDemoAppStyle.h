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

#ifndef MAPDEMOAPPSTYLE_H_
#define MAPDEMOAPPSTYLE_H_

#include "AppStyle.h"
#include "Color.h"

using namespace MapDemoUI;
using namespace MapDemoUtil;

namespace MapDemo
{
	//=========================================================================
	class MapDemoAppStyle : public AppStyle
	//=========================================================================
	{
	public:
		MapDemoAppStyle( );
		
		virtual ~MapDemoAppStyle( );
		//
		// For client to implement
		//
		virtual Font* getFont( FontSize size, Color color, bool bold ) const;
		virtual WidgetSkin* getWidgetSkin( ) const;
		virtual WidgetSkin* getButtonSkin( ) const;
		virtual int getTabHeight( ) const;
		virtual int getMenuItemHeight( ) const;
		virtual int getSoftKeyBarHeight( ) const;

	private:
		WidgetSkin* mWidgetSkin;
		WidgetSkin* mButtonSkin;
		Font* mFontSmallestBoldBlack;
		Font* mFontSmallestBoldWhite;
		Font* mFontSmallestBlack;
		Font* mFontSmallestWhite;
		Font* mFontSmallBoldBlack;
		Font* mFontSmallBoldWhite;
		Font* mFontSmallWhite;
		Font* mFontSmallBlue;
		Font* mFontSmallBlack;
		Font* mFontMediumBoldWhite;
		Font* mFontMediumWhite;
		Font* mFontLargeBoldWhite;
		Font* mFontLargeBoldBlack;
		Font* mFontLargeWhite;
		Font* mFontLargestBoldWhite;
		Font* mFontLargestWhite;
	};
}

#endif // MAPDEMOAPPSTYLE_H_

