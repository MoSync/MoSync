/* Copyright (C) 2011 MoSync AB

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

/**
 * @file rescompdefines.h
 * @author Florin Leu
 * @date 22 Nov 2011
 **/

#ifndef __RESCOMPDEFINES_H__
#define __RESCOMPDEFINES_H__

	#define DEBUG_ENABLED
	#ifdef DEBUG_ENABLED
		#define PRINTINT(v) 					printf("%s = %d", #v, v);
		#define PRINTINTARRAY(v, size)			{if (v){for (int _i=0; _i<size; _i++){printf("%s[%d] = %d", #v, _i, v[_i]);}}}
		#define PRINTSTRING(v)					{if (v){printf("%s = %s", #v, v);}}
		#define PRINT(...)						printf(__VA_ARGS__);
	#else
		#define PRINTINT(v)
		#define PRINTINTARRAY(v, size)
		#define PRINTSTRING(v)
		#define PRINT(...)
	#endif

	#define DELETE(a)						{if ((a)!=NULL) delete (a);(a)=NULL;}
	#define DELETE_ARRAY(a,size)			{if(a){for(int _i=0; _i<size; _i++) if(a[_i]){delete(a[_i]);a[_i]=NULL;}delete (a);a=NULL;}}

	#define SMALL_SCREEN_AREA				(426*320)
	#define MEDIUM_SCREEN_AREA				(470*320)
	#define LARGE_SCREEN_AREA				(640*480)
	#define XLARGE_SCREEN_AREA				(960*720)

	#define SMALL_SCREEN_VARIANT			"screenSize:small"
	#define MEDIUM_SCREEN_VARIANT			"screenSize:medium"
	#define LARGE_SCREEN_VARIANT			"screenSize:large"
	#define XLARGE_SCREEN_VARIANT			"screenSize:xlarge"

	#ifdef DEBUG_ENABLED
		#include <conprint.h>
	#endif

#endif //__RESCOMPDEFINES_H__
