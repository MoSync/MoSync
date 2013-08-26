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

#ifndef MOBLETEX_H_
#define MOBLETEX_H_

#include <MAUtil/Moblet.h>

using namespace MAUtil;

namespace MapDemoUtil
{
	//=========================================================================
	//
	// Abstract class with shutdown handling.
	//
	class MobletEx : public Moblet
	//=========================================================================
	{
	public:
		MobletEx( )
			: Moblet( )
		{
		}

		virtual ~MobletEx( ) { }
		//
		// for client to override
		//
		virtual void shutdown( ) = 0;
	};
}

#endif // MOBLETEX_H_
