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
		Font* mFontSmallestBlack;
		Font* mFontSmallestWhite;
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
