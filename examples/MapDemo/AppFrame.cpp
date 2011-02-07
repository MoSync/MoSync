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

#include <MAUtil/Graphics.h>

#include "AppFrame.h"
#include "AppStyleMgr.h"
#include "Color.h"

using namespace MAUI;

namespace MapDemoUI
{
	const int OutsideMargin = 30;
	const int InsideMargin = 10;
	const Color MessageBackColor = Color::fromInt( 0x202020 );
	const Color MessageForeColor = Color::fromInt( 0xffffff );

	//-------------------------------------------------------------------------
	AppFrame::AppFrame(int x, int y, int width, int height, Widget* parent, int columns, int rows )
	//-------------------------------------------------------------------------
		: Layout( x, y, width, height, parent, columns, rows ),
		mProgress( -1 ),
		mFont( NULL )
	{
		AppStyle* style = AppStyleMgr::getStyle( );
		mMsgSkin = style->getButtonSkin( );
	}

	//-------------------------------------------------------------------------
	AppFrame::~AppFrame( )
	//-------------------------------------------------------------------------
	{
	}

	//-------------------------------------------------------------------------
	void AppFrame::setMessage( const char* message, float progress )
	//-------------------------------------------------------------------------
	{
		if ( progress >= 0 )
		{
			mProgress = progress;
			Rect bounds( OutsideMargin, OutsideMargin, getWidth() - 2 * OutsideMargin, 10 );
			mProgressBounds = bounds;
			requestRepaint( );
		}
		else
		{
			mMessage = message;
			mProgress = -1;
			Rect bounds( OutsideMargin + InsideMargin, 0, getWidth( ) - 2 * ( OutsideMargin + InsideMargin ), 32767 );
			MAExtent ext = mFont->getBoundedStringDimensions( mMessage.c_str( ), bounds );
			int width = EXTENT_X( ext );
			int height = EXTENT_Y( ext );
			int center = getWidth( ) / 2;
			int top = OutsideMargin;
			mTextBounds = Rect( center - width / 2, top + InsideMargin, width, height );
			mFillBounds = Rect( mTextBounds.x - InsideMargin, mTextBounds.y - InsideMargin, width + 2 * InsideMargin, height + 2 * InsideMargin );
			requestRepaint( );
		}
	}

	//-------------------------------------------------------------------------
	void AppFrame::clearMessage( )
	//-------------------------------------------------------------------------
	{
		mMessage.clear( );
		requestRepaint( );
	}

	//-------------------------------------------------------------------------
	void AppFrame::draw( bool forceDraw )
	//-------------------------------------------------------------------------
	{
		Layout::draw( forceDraw );

		if ( mMessage.length( ) > 0 )
		{
			// draw message, if any
			maSetColor( MessageBackColor.val( ) );

			if ( mProgress < 0 )
			{
				mMsgSkin->draw( mFillBounds.x, mFillBounds.y, mFillBounds.width, mFillBounds.height, WidgetSkin::/*eType::*/SELECTED );
				mFont->drawBoundedString( mMessage.c_str( ), mTextBounds.x, mTextBounds.y, mTextBounds );
			}
			else
			{
				Gfx_fillRect( mProgressBounds.x, mProgressBounds.y, mProgressBounds.width, mProgressBounds.height );
				maSetColor( MessageForeColor.val( ) );
				Gfx_fillRect( mProgressBounds.x, mProgressBounds.y, (int)( mProgressBounds.width * mProgress ), mProgressBounds.height );
			}
		}
	}
}
