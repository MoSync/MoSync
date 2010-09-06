#include <MAUtil/Moblet.h>
#include <MAUI/Screen.h>
#include <MAUI/Label.h>
#include <conprint.h>

using namespace MAUtil;
using namespace MAUI;

class MyScreen : public Screen {
public:
	MyScreen() {

	//		todo: initialize the widget hierarchy of this screen and possibly show it
	//		example:
				l = new Label(0, 0, 50, 50, NULL, "", 0x1f7f1f, NULL);
				l1 = new Label(10, 10, 50, 50, l, "", 0xffff00, NULL);
				l2 = new Label(70, 10, 50, 50, l, "", 0xffff00, NULL);
				l3 = new Label(10, 70, 50, 50, l, "", 0xffff00, NULL);
				l4 = new Label(70, 70, 50, 50, l, "", 0xffff00, NULL);
				l5 = new Label(130, 10, 50, 120, l, "", 0xffff00, NULL);
				l6 = new Label(10, 130, 120, 50, l, "", 0xffff00, NULL);

				setMain(l);		

	}
	
	~MyScreen() {
		// todo: delete main widget of this screen 
	}
	Label *l, *l1, *l2, *l3, *l4, *l5, *l6;
private:
};

class MAUIMoblet : public Moblet {
public:
	MAUIMoblet() {
		// initialize
		screen = new MyScreen();
		screen->show();
		screen->l->setSelected(false);
		screen->l1->setSelected(false);
		screen->l2->setSelected(false);
		screen->l3->setSelected(false);
		screen->l4->setSelected(false);
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		// todo: handle key presses
		lprintfln("l->selected == %d", screen->l->isSelected());

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
