#include <MAUtil/Moblet.h>
#include <MAUI/Screen.h>
#include <MAUI/Label.h>

using namespace MAUtil;
using namespace MAUI;

class MyScreen : public Screen {
public:
	MyScreen() {
		/* 
			todo: initialize the widget hierarchy of this screen and possibly show it
			example:
				Label* l = new Label(0, 0, 50, 50, NULL, "", 0x1f7f1f, NULL);
				setMain(l);		
		*/
	}
	
	~MyScreen() {
		// todo: delete main widget of this screen 
	}
private:
};

class MAUIMoblet : public Moblet {
public:
	MAUIMoblet() {
		// initialize
		screen = new MyScreen();
		screen->show();
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		// todo: handle key presses
	}

	void keyReleaseEvent(int keyCode, int nativeCode) {
		// todo: handle key releases
	}
	MyScreen* screen;

	~MAUIMoblet() {
		delete screen;
	}
	
};

extern "C" int MAMain() {
	Moblet::run(new MAUIMoblet());
	return 0;
};
