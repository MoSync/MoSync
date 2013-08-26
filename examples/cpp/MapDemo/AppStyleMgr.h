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

#ifndef APPSTYLEMGR_H_
#define APPSTYLEMGR_H_

#include "AppStyle.h"
#include <MAP/MemoryMgr.h>

using namespace MAPUtil;

namespace MapDemoUI
{
	//=========================================================================
	//
	// Implemented as singleton.
	//
	class AppStyleMgr
	//=========================================================================
	{
	private:
		AppStyleMgr( )
			: mStyle( NULL )
		{
		}
	public:
		virtual ~AppStyleMgr( )
		{
			deleteobject( mStyle );
		}

		static AppStyleMgr* get( )
		{
			if ( sSingleton == NULL )
			{
				sSingleton = newobject( AppStyleMgr, new AppStyleMgr( ) );
			}
			return sSingleton;
		}

		static void shutdown( )
		{
			deleteobject( sSingleton );
		}

		static void setStyle( AppStyle* style )
		{
			get( )->setStylePrim( style );
		}

		static AppStyle* getStyle( )
		{
			return get( )->mStyle;
		}

	private:
		void setStylePrim( AppStyle* style )
		{
			deleteobject( mStyle );
			mStyle = style;
		}

		static AppStyleMgr* sSingleton;
		AppStyle* mStyle;
	};
}

#endif // APPSTYLEMGR_H_
