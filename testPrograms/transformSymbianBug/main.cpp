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
