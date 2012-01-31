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

#ifndef APPFRAME_H_
#define APPFRAME_H_

#include <MAUI/Layout.h>
#include <MAUtil/String.h>
#include <MAUI/Font.h>

using namespace MAUI;
using namespace MAUtil;

namespace MapDemoUI
{
	//=========================================================================
	class AppFrame: public Layout
	//=========================================================================
	{
	public:
		AppFrame(int x, int y, int width, int height, Widget* parent, int columns, int rows );

		virtual ~AppFrame( );
		//
		// Message handling
		//
		void setMessage( const char* message, float progress );
		void clearMessage( );
		//
		// Font
		//
		void setfont( Font* font ) { mFont = font; }
		Font* getFont( ) const { return mFont; }

	protected:
		virtual void draw( bool forceDraw=false );

	private:
		String mMessage;
		float mProgress;
		Font* mFont;
		Rect mTextBounds;
		Rect mFillBounds;
		Rect mProgressBounds;
		WidgetSkin* mMsgSkin;
	};
}
#endif // APPFRAME_H_
