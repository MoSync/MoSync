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
