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
		virtual ~AppStyle( ) { }
		//
		// For client to implement
		//
		virtual Font* getFont( FontSize size, Color color, bool bold ) const = 0;
		virtual WidgetSkin* getWidgetSkin( ) const = 0;
		virtual WidgetSkin* getButtonSkin( ) const = 0;
		virtual int getTabHeight( ) const = 0;
		virtual int getMenuItemHeight( ) const = 0;
		virtual int getSoftKeyBarHeight( ) const = 0;
	};
}

#endif // APPSTYLE_H_
