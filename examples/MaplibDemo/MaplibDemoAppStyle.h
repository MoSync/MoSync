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

#ifndef MAPLIBDEMOAPPSTYLE_H_
#define MAPLIBDEMOAPPSTYLE_H_

//#include "MemoryMgr.h"
#include "AppStyle.h"
#include "Color.h"

using namespace UI;
using namespace Util;

namespace MaplibDemo
{
	class MaplibDemoAppStyle : public AppStyle
	{
	public:
								MaplibDemoAppStyle( );
		virtual					~MaplibDemoAppStyle( );
		//
		// For client to implement
		//
		virtual Font*			getFont( FontSize size, Color color, bool bold ) const;
		virtual WidgetSkin*		getWidgetSkin( ) const;
		virtual WidgetSkin*		getButtonSkin( ) const;
		virtual int				getTabHeight( ) const;
		virtual int				getMenuItemHeight( ) const;
		virtual int				getSoftKeyBarHeight( ) const;
	private:
		WidgetSkin*				m_widgetSkin;
		WidgetSkin*				m_buttonSkin;
		Font*					m_fontSmallestBoldBlack;
		Font*					m_fontSmallestBoldWhite;
		Font*					m_fontSmallestWhite;
		Font*					m_fontSmallestBlack;
		Font*					m_fontSmallBoldBlack;
		Font*					m_fontSmallBoldWhite;
		Font*					m_fontSmallWhite;
		Font*					m_fontSmallBlue;
		Font*					m_fontSmallBlack;
		Font*					m_fontMediumBoldWhite;
		Font*					m_fontMediumWhite;
		Font*					m_fontLargeBoldWhite;
		Font*					m_fontLargeBoldBlack;
		Font*					m_fontLargeWhite;
		Font*					m_fontLargestBoldWhite;
		Font*					m_fontLargestWhite;
	};
}

#endif // MAPLIBDEMOAPPSTYLE_H_

