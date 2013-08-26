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

#ifndef MAPDEMOMOBLET_H_
#define MAPDEMOMOBLET_H_

#include "MobletEx.h"
#include "MapDemoScreen.h"

using namespace MapDemoUI;

namespace MapDemo
{
	//=========================================================================
	class MapDemoMoblet : public MobletEx
	//=========================================================================
	{
	public:
		MapDemoMoblet( );
		virtual ~MapDemoMoblet( );

		virtual void shutdown( ); //GCCATTRIB(noreturn);

		void Terminate( ) { mRun = false; }

	private:
		MapDemoScreen* mScreen;
	};
}

#endif // MAPDEMOMOBLET_H_
