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
	ListFrame *categoriesListFrame;
	ListFrame *lengthListFrame;
	Frame *inputFrame;
	Frame *resultsFrame;
	Button *button;
	Edit *edit;
	Label*label;
	Layout *inputFrameLayout, *resultsFrameLayout;
	MAWidgetHandle wh;
	/*Parser *parser;
	Button *button;
	Button *button2;
	MenuItem *item;
	MenuItem *item2;
	Image *image;*/
public:
	Main() {
		categoriesListFrame = NULL;
		lengthListFrame = NULL;
		inputFrame = NULL;
		resultsFrame = NULL;
		button = NULL;
		edit = NULL;
		label = NULL;
		inputFrameLayout = NULL;

		/*parser = NULL;
		button = NULL;
		button2 = NULL;*/

		categoriesListFrame = new ListFrame(50);

		Manager::Instance().addWidget(categoriesListFrame);
		categoriesListFrame->addActionListener(this);

		//maAndroidStartListActivity(700);
		//maAndroidAddListItem(701, "salut");
	}

	Main(const Main & m) {
		categoriesListFrame=m.categoriesListFrame;
		/*frame2=m.frame2;
		parser=m.parser;
		button=m.button;
		button2=m.button2;*/
	}

	~Main() {
		//delete(button);
		delete(categoriesListFrame);
		delete(lengthListFrame);
		delete(inputFrame);
		delete(resultsFrame);
		delete(button);
		delete(edit);
		delete(label);
		delete(inputFrameLayout);

		/*delete(parser);
		delete(button);
		delete(button2);
		delete(item);
		delete(item2);
		delete(image);*/
	}

	void onCreate (int id) {
		if(id==categoriesListFrame->getId()) {
			categoriesListFrame->add("Temperature");
			categoriesListFrame->add("Weight");
			categoriesListFrame->add("Length");
			categoriesListFrame->add("Currency");
			categoriesListFrame->add("Angle");
			categoriesListFrame->show();
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

		if(id==lengthListFrame->getId()) {
					lengthListFrame->add("Celsius");
					lengthListFrame->add("Fahrenheit");
					lengthListFrame->show();
		}

		if(id==inputFrame->getId()) {
			label = new Label("Enter a value:", 102);
			Manager::Instance().addWidget(label);

			edit = new Edit("gtgrt", 101);
			Manager::Instance().addWidget(edit);

			button = new Button("Convert", 100);
			Manager::Instance().addWidget(button);
			button->addActionListener(this);

			inputFrameLayout = new Layout(inputFrame);

			inputFrameLayout->addWidget(label);
			inputFrameLayout->addWidget(edit);
			inputFrameLayout->addWidget(button);

			inputFrameLayout->build();
		}

		if(id==resultsFrame->getId()) {
			Label *label1 = new Label(wh.buf, 103);
			Manager::Instance().addWidget(label1);

			Label *label2 = new Label("Fahrenheit: XX", 104);
			Manager::Instance().addWidget(label2);

			resultsFrameLayout = new Layout(resultsFrame);

			resultsFrameLayout->addWidget(label1);
			resultsFrameLayout->addWidget(label2);

			resultsFrameLayout->build();

			delete(label1);
			delete(label2);
		}
	}
	void onClick (int id) {
		if(id==100) {
			MAWidgetParameters wp;
			wp.pParent = inputFrame->getInstance();
			wp.widgetID = 101;


			maIPhoneGetText(&wp, &wh);
			//mystr=wh.buf;
			//strcpy(mystr, wh.buf);

			resultsFrame = new Frame(53);
			Manager::Instance().addWidget(resultsFrame);
			resultsFrame->addActionListener(this);
		}
	/*	if(id==item->getId()) {
			frame2 = new Frame(2);
			Manager::Instance().addWidget(frame2);
			frame2->addActionListener(this);
		}*/
	}

	void onItemSelected (int id, int index) {
		if(id==50 && index==1) {
			lengthListFrame = new ListFrame(51);
			Manager::Instance().addWidget(lengthListFrame);
			lengthListFrame->addActionListener(this);
		}
		if(id==51 && index==1) {
			inputFrame = new Frame(52);
			Manager::Instance().addWidget(inputFrame);
			inputFrame->addActionListener(this);
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
