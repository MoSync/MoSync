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
