#include <ma.h>
#include "Frame.h"
#include "ListFrame.h"
#include "Button.h"
#include "Edit.h"
#include "Label.h"
#include "Manager.h"
#include "Layout.h"
#include "MenuItem.h"
#include "Parser.h"
#include "MAHeaders.h"
#include "Image.h"


class Main : public ActionListener {
private:
	ListFrame *listFrame;
	ListFrame *listFrame2;
	Frame *frame2;
	/*Parser *parser;
	Button *button;
	Button *button2;
	MenuItem *item;
	MenuItem *item2;
	Image *image;*/
public:
	Main() {
		listFrame = NULL;
		listFrame2 = NULL;
		frame2 = NULL;
		/*parser = NULL;
		button = NULL;
		button2 = NULL;*/

		listFrame = new ListFrame(50);

		Manager::Instance().addWidget(listFrame);
		listFrame->addActionListener(this);

		//maAndroidStartListActivity(700);
		//maAndroidAddListItem(701, "salut");
	}

	Main(const Main & m) {
		listFrame=m.listFrame;
		/*frame2=m.frame2;
		parser=m.parser;
		button=m.button;
		button2=m.button2;*/
	}

	~Main() {
		//delete(button);
		delete(listFrame);
		delete(listFrame2);
		delete(frame2);
		/*delete(parser);
		delete(button);
		delete(button2);
		delete(item);
		delete(item2);
		delete(image);*/
	}

	void onCreate (int id) {
		if(id==listFrame->getId()) {
			listFrame->add("Temperature");
			listFrame->add("Weight");
			listFrame->add("Length");
			listFrame->add("Currency");
			listFrame->add("Angle");
			listFrame->show();
/*
			parser = new Parser(MAIN_LAYOUT, frame);


			button = (Button *) Manager::Instance().findById(32);
			button->addActionListener(this);
			
			button2 = (Button *) Manager::Instance().findById(34);
			button2->addActionListener(this);
			*/
			
/*

			image = new Image(SKY_ICON, 21);
			Manager::Instance().addWidget(image);
			
			button = new Button("New frame", 20);
			Manager::Instance().addWidget(button);
			button->addActionListener(this);
			
			
			
			Layout *l = new Layout(frame);
			l->addWidget(image);
			l->addWidget(button);
			
			l->build();
			
			item = new MenuItem("New frame", frame, STAR_ICON);
			Manager::Instance().addWidget(item);
			item->addActionListener(this);

			item2 = new MenuItem("New frame", frame, SKULL_ICON);
			Manager::Instance().addWidget(item2);
			item2->addActionListener(this);
 */
/*
			button = new Button("New frame");
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

			item = new MenuItem("New frame", frame);
			Manager::Instance().addWidget(item);
			item->addActionListener(this);

			item2 = new MenuItem("Useless", frame);
			Manager::Instance().addWidget(item2);
			item2->addActionListener(this);
*/
		}

		if(id==listFrame2->getId()) {
					listFrame2->add("Centimeters");
					listFrame2->add("Meters");
					listFrame2->add("Feet");
					listFrame2->add("Inches");
					listFrame2->show();
		}
	}
	void onClick (int id) {
		if(id==32) {
			/*frame2 = new Frame(2);
			Manager::Instance().addWidget(frame2);
			frame2->addActionListener(this);*/
		}
	/*	if(id==item->getId()) {
			frame2 = new Frame(2);
			Manager::Instance().addWidget(frame2);
			frame2->addActionListener(this);
		}*/
	}

	void onItemSelected (int id, int index) {
		if(id==50 && index==3) {
			listFrame2 = new ListFrame(51);
			Manager::Instance().addWidget(listFrame2);
			listFrame2->addActionListener(this);
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
