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
		Font* mFontSmallestWhite;
		Font* mFontSmallestBlack;
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

