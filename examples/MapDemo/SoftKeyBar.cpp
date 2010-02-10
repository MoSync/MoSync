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
#include <MAP/MemoryMgr.h>

using namespace MapDemoUtil;

namespace MapDemoUI
{
	const int Padding = 3;

	//-------------------------------------------------------------------------
	SoftKeyBar::SoftKeyBar(  int x, int y, int width, int height, Widget* parent )
	//-------------------------------------------------------------------------
		: Layout( x, y, width, height, parent, 2, 1 ),
		mLeftLabel( NULL ),
		mRightLabel( NULL ),
		mLeftAction( NULL ),
		mRightAction( NULL )
	{
		mLeftAction = NULL;
		mRightAction = NULL;
		AppStyle* appStyle = AppStyleMgr::getStyle( );
		mFont = appStyle->getFont( FontSize_Medium, Color::white, false );

		WidgetSkin* skin = appStyle->getWidgetSkin( );

		// Left label
		mLeftLabel = newobject( Label, new Label( 0, 0, width / 2, height, this, "", 0x404040, mFont ) );
		mLeftLabel->setSkin( skin );
		mLeftLabel->setPaddingLeft( Padding );
		mLeftLabel->setVerticalAlignment( Label::VA_CENTER );
		mLeftLabel->setHorizontalAlignment( Label::HA_LEFT );

		// Right label
		mRightLabel = newobject( Label, new Label( 0,0, width / 2, height, this, "", 0x404040, mFont ) );
		mRightLabel->setPaddingRight( Padding );
		mRightLabel->setSkin( skin );
		mRightLabel->setVerticalAlignment( Label::VA_CENTER );
		mRightLabel->setHorizontalAlignment( Label::HA_RIGHT );
	}

	//-------------------------------------------------------------------------
	SoftKeyBar::~SoftKeyBar( )
	//-------------------------------------------------------------------------
	{
		getChildren( ).clear( );
		deleteobject( mLeftLabel );
		deleteobject( mRightLabel );
		deleteobject( mLeftAction );
		deleteobject( mRightAction );
	}

	//-------------------------------------------------------------------------
	void SoftKeyBar::setWidth( int width )
	//-------------------------------------------------------------------------
	{
		Layout::setWidth( width );

		mLeftLabel->setWidth( width / 2 );
		mRightLabel->setWidth( width / 2 );
	}

	//-------------------------------------------------------------------------
	void SoftKeyBar::setHeight( int height )
	//-------------------------------------------------------------------------
	{
		Layout::setHeight( height );

		mLeftLabel->setHeight( height );
		mRightLabel->setHeight( height );
	}

	//-------------------------------------------------------------------------
	bool SoftKeyBar::handleKeyPress( int keyCode )
	//-------------------------------------------------------------------------
	{
		switch( keyCode )
		{
		case MAK_SOFTLEFT:
			if ( mLeftAction != NULL )
			{
				mLeftAction->perform( );
				return true;
			}
			break;
		case MAK_SOFTRIGHT:
			if ( mRightAction != NULL )
			{
				mRightAction->perform( );
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
		return mLeftAction;
	}

	//-------------------------------------------------------------------------
	void SoftKeyBar::setLeftAction( Action* action )
	//-------------------------------------------------------------------------
	{
		deleteobject( mLeftAction );
		mLeftAction = action;
		mLeftLabel->setCaption( action == NULL ? "" : action->getShortName( ) );
	}

	//-------------------------------------------------------------------------
	Action* SoftKeyBar::getRightAction( ) const
	//-------------------------------------------------------------------------
	{
		return mRightAction;
	}

	//-------------------------------------------------------------------------
	void SoftKeyBar::setRightAction( Action* action )
	//-------------------------------------------------------------------------
	{
		deleteobject( mRightAction );
		mRightAction = action;
		mRightLabel->setCaption( action == NULL ? "" : action->getShortName( ) );
	}
}

