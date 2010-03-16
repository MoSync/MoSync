#include <ma.h>
#include "Frame.h"
#include "Button.h"
#include "Edit.h"
#include "Label.h"
#include "Manager.h"
#define MAIN_FRAME 10

class Main : public ActionListener {
private:
	Frame *frame;
	Button *button;
	Edit *edit;
	Label *label;
public:
	Main() {
		frame = new Frame(MAIN_FRAME);
		Manager::Instance().addWidget(frame);
		frame->addActionListener(this);
	}

	~Main() {
		delete(label);
		delete(edit);
		delete(button);
		delete(frame);
	}

	void onCreate (int id) {
		if(id==MAIN_FRAME) {
			button = new Button(6, frame->getInstance(), "I am a button");
			edit = new Edit(7, frame->getInstance(), "I am an Edit");
			//label = new Label(8, frame->getInstance(), "I am a Label");
			Manager::Instance().addWidget(button);
			Manager::Instance().addWidget(edit);
			//Manager::Instance().addWidget(label);
			button->addActionListener(this);
		}
	}
	void onClick (int id) {
		if(id==6) {
			label = new Label(8, frame->getInstance(), "I am a Label");
			Manager::Instance().addWidget(label);
		}
	}

};

extern "C" int MAMain() {
	// Handle events
	Main main = Main();
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
