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

/**
 * @file rescompdefines.h
 * @author Florin Leu
 * @date 22 Nov 2011
 **/

#ifndef __RESCOMPDEFINES_H__
#define __RESCOMPDEFINES_H__

	//#define DEBUG_ENABLED
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
