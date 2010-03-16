#include <ma.h>
#include "Frame.h"
#include "Button.h"
#include "Edit.h"
#include "Label.h"
#include "Manager.h"
#include "Layout.h"


class Main : public ActionListener {
private:
	Frame *frame;
	Frame *frame2;
	Button *button;
	Button *button2;
	Edit *edit;
	Label *label;
	Label *label2;
	Layout *l;
public:
	Main() {
		frame = new Frame();
		Manager::Instance().addWidget(frame);
		frame->addActionListener(this);
	}

	~Main() {
		delete(label);
		delete(label2);
		delete(edit);
		delete(button);
		delete(button2);
		delete(frame);
	}

	void onCreate (int id) {
		if(id==frame->getId()) {
			button = new Button("I am a button");
			Manager::Instance().addWidget(button);

			button2 = new Button("I am a button");
			Manager::Instance().addWidget(button2);

			edit = new Edit("I am an Edit");
			Manager::Instance().addWidget(edit);

			label = new Label("I am a Label");
			Manager::Instance().addWidget(label);

			l = new Layout(frame);
			l->addWidget(button);
			l->addWidget(button2);
			l->addWidget(edit);
			l->addWidget(label);
			l->build();

			button->addActionListener(this);
			button2->addActionListener(this);
		}
	}
	void onClick (int id) {
		if(id==button->getId()) {
			/*label2 = new Label("I am a Label too");
			Manager::Instance().addWidget(label2);
			l->addWidget(label2);
			l->build();*/
			frame2 = new Frame();
			Manager::Instance().addWidget(frame2);
			frame2->addActionListener(this);
		}
	}

};

extern "C" int MAMain() {

	Main main = Main();
	// Handle events
	MAEvent event;
	while(1) {
		while(maGetEvent(&event)) {
			if(event.type==EVENT_TYPE_NATIVE_UI_MSG) {
				Manager::Instance().dispatch( event );
			}
		}
	}
	return 0;
}
