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

#ifndef APPSTYLE_H_
#define APPSTYLE_H_

#include <MAUI/Font.h>
#include <MAUI/WidgetSkin.h>

#include "FontSize.h"
#include "Color.h"

using namespace MAUI;
using namespace MapDemoUtil;

namespace MapDemoUI
{
	//=========================================================================
	class AppStyle 
	//=========================================================================
	{
	public:
		//
		// Constructor/destructor
		//
								AppStyle( ) { }
		virtual					~AppStyle( ) { }
		//
		// For client to implement
		//
		virtual Font*			getFont( FontSize size, Color color, bool bold ) const = 0;
		virtual WidgetSkin*		getWidgetSkin( ) const = 0;
		virtual WidgetSkin*		getButtonSkin( ) const = 0;
		virtual int				getTabHeight( ) const = 0;
		virtual int				getMenuItemHeight( ) const = 0;
		virtual int				getSoftKeyBarHeight( ) const = 0;
	};
}

#endif // APPSTYLE_H_
