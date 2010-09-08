#include <MAUtil/Moblet.h>
#include <MAUI/Screen.h>
#include <MAUI/Label.h>
#include <MAUI/Button.h>
#include <MAUI/Engine.h>

#include "MAHeaders.h"

#include <conprint.h>

using namespace MAUtil;
using namespace MAUI;

class MyScreen : public Screen {
public:
	MyScreen() {
			ButtonStyle* buttonStyle = new ButtonStyle(RES_SELECTED, RES_UNSELECTED);
			Engine::getSingleton().setDefaultStyle("Button", buttonStyle);


	//		todo: initialize the widget hierarchy of this screen and possibly show it
	//		example:
				l = new Label(0, 0, 50, 50, NULL, "", 0, NULL);
				l1 = new Button(10, 10, 50, 50, l);
				l2 = new Button(70, 10, 50, 50, l);
				l3 = new Button(10, 70, 50, 50, l);
				l4 = new Button(70, 70, 50, 50, l);
				l5 = new Button(130, 10, 50, 120, l);
				l6 = new Button(10, 130, 120, 50, l);

				setMain(l);		

	}
	
	~MyScreen() {
		// todo: delete main widget of this screen 
	}
	Label *l;
	Button *l1, *l2, *l3, *l4, *l5, *l6;
private:
};

class MAUIMoblet : public Moblet {
public:
	MAUIMoblet() {
		// initialize
		screen = new MyScreen();
		screen->show();
		/*
		screen->l->setSelected(false);
		screen->l1->setSelected(false);
		screen->l2->setSelected(false);
		screen->l3->setSelected(false);
		screen->l4->setSelected(false);
		*/
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		// todo: handle key presses
		//lprintfln("l->selected == %d", screen->l->isSelected());

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
