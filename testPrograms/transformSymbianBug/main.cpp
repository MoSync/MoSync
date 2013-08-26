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

#include <MAUtil/Moblet.h>
#include <MAUtil/String.h>
#include <MAUtil/Environment.h>
#include <mastdlib.h>
#include <MAUI/Screen.h>
#include <MAUI/Image.h>
#include <MAUI/Layout.h>
#include "MAHeaders.h"
#include <conprint.h>
#include <maheap.h>
#include <MAUtil/Graphics.h>
using namespace MAUtil;
using namespace MAUI;
class MyMoblet : public Moblet{

public:
	MyMoblet()
	{
		MARect rect;
			rect.top = 0;
			rect.left = 0;
			rect.width = 82;
			rect.height = 57;
			MAPoint2d pos;
			pos.x = 50;
			pos.y = 20;

		maDrawImageRegion(IMAGE,&rect,&pos,TRANS_NONE);
		pos.y = 80;
		Gfx_drawImageRegion(IMAGE,&rect,&pos,TRANS_MIRROR);
		 maUpdateScreen();


	}

};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};
