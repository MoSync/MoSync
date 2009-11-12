#include <MAUtil/Moblet.h>
#include <MAUI/Screen.h>
#include <MAUI/EditBox.h>

using namespace MAUtil;
using namespace MAUI;

class MyScreen : public Screen {
public:
	MyScreen() {
		//todo: get a maui font, or this editbox will not render.
		EditBox* eb = new EditBox(0, 0, 200, 30, NULL);
		setMain(eb);
		eb->setQwerty();
		eb->activate();
	}
	
	~MyScreen() {
		// todo: delete main widget of this screen 
	}
private:
};

class MAUIMoblet : public Moblet {
public:
	MAUIMoblet() {
		// todo: initializtion
		screen = new MyScreen();
		screen->show();
	}

	void keyPressEvent(int keyCode) {
		// todo: handle key presses
	}

	void keyReleaseEvent(int keyCode) {
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
