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
