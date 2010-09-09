#include <MAUtil/Moblet.h>
#include <MAUI/Screen.h>
#include <MAUI/Label.h>
#include <MAUI/Button.h>
#include <MAUI/Engine.h>
#include <MAUI/Layout.h>
#include <MAUI/ListBox.h>

#include "MAHeaders.h"

#include <conprint.h>

using namespace MAUtil;
using namespace MAUI;


class StringListbox : public ListBox {
public:
	StringListbox(int x, int y, int width, int height, Widget* parent=NULL) : ListBox(x, y, width, height, parent, ListBox::LBO_VERTICAL, ListBox::LBA_LINEAR, true) {
	}

	void addString(const String& str) {

	}

protected:

};

class MyScreen : public Screen {
public:
	MyScreen() {
			LabelStyle* labelStyle = new LabelStyle(RES_FONT);
			ButtonStyle* buttonStyle = new ButtonStyle(RES_SELECTED, RES_UNSELECTED, RES_FONT, 12, 19, 12, 18, true, true);
			ButtonStyle* buttonStyle2 = new ButtonStyle(RES_SELECTED2, RES_UNSELECTED2, RES_FONT, 16, 32, 16, 32, true, true);
			Engine::getSingleton().setDefaultStyle("Label", labelStyle);
			Engine::getSingleton().setDefaultStyle("Button", buttonStyle);

			//l = new Label(0, 0, 50, 50, NULL, "", 0, NULL);
			l = new Layout(0, 0, 50, 50, NULL, 3, 2);
			l->setAutoSizeX(true);
			l1 = new Button(10, 10, 50, 50, l, "a");
			l2 = new Button(70, 10, 50, 50, l, "b");
			l2->setStyle(buttonStyle2);
			l3 = new Button(10, 70, 50, 50, l, "c");
			l4 = new Button(70, 70, 50, 50, l, "d");
			l5 = new Button(130, 10, 50, 120, l, "e");
			l6 = new Label(10, 130, 120, 50, l, "Flump");

			setMain(l);
	}
	
	~MyScreen() {
		// todo: delete main widget of this screen 
	}
	Layout *l;
	Button *l1, *l2, *l3, *l4, *l5;
	Label *l6;
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
