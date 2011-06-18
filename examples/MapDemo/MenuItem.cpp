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

#include "MenuItem.h"
#include "AppStyleMgr.h"
#include "Action.h"

namespace MapDemoUI
{
	//-------------------------------------------------------------------------
	MenuItem::MenuItem( Action* action )
	//-------------------------------------------------------------------------
		: Label( 0, 0, 0, 0, NULL ),
		mAction( action )
	{
		AppStyle* appStyle = AppStyleMgr::getStyle( );
		Font* font = appStyle->getFont( FontSize_Small, Color::white, false );
		setFont( font );
		setVerticalAlignment(Label::VA_CENTER );
		setCaption( mAction->getShortName( ) );
	}

	//-------------------------------------------------------------------------
	MenuItem::~MenuItem( )
	//-------------------------------------------------------------------------
	{
		deleteobject( mAction );
	}

	//-------------------------------------------------------------------------
	Action* MenuItem::getAction( ) const
	//-------------------------------------------------------------------------
	{
		return mAction;
	}
}
