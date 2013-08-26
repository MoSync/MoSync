/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
	bool SoftKeyBar::handlePointerPress( MAPoint2d point )
	//-------------------------------------------------------------------------
	{
		Point p;
		p.set(point.x, point.y);

		if(getChildren()[0]->contains(p))	// LSK
		{
			if ( mLeftAction != NULL )
			{
				mLeftAction->perform();
				return true;
			}
		}
		else if(getChildren()[1]->contains(p))	// RSK
		{
			if ( mRightAction != NULL )
			{
				mRightAction->perform();
				return true;
			}
		}
		return false;
	}

	//-------------------------------------------------------------------------
	bool SoftKeyBar::handlePointerMove( MAPoint2d point )
	//-------------------------------------------------------------------------
	{
		return false;
	}

	//-------------------------------------------------------------------------
	bool SoftKeyBar::handlePointerRelease( MAPoint2d point )
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
