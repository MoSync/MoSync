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
	char *buf;

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
		buf=NULL;


		categoriesListFrame = new ListFrame(50);

		Manager::Instance().addWidget(categoriesListFrame);
		categoriesListFrame->addActionListener(this);


	}

	Main(const Main & m) {
		categoriesListFrame=m.categoriesListFrame;

	}

	~Main() {

		delete(categoriesListFrame);
		delete(lengthListFrame);
		delete(inputFrame);
		delete(resultsFrame);
		delete(button);
		delete(edit);
		delete(label);
		delete(inputFrameLayout);
		delete(buf);

	}

	void onCreate (int id) {
		if(id==categoriesListFrame->getId()) {
			categoriesListFrame->add("Temperature");
			categoriesListFrame->add("Weight");
			categoriesListFrame->add("Length");
			categoriesListFrame->add("Currency");
			categoriesListFrame->add("Angle");
			categoriesListFrame->show();


		}

		if(id==lengthListFrame->getId()) {
					lengthListFrame->add("Celsius");
					lengthListFrame->add("Fahrenheit");
					lengthListFrame->show();
		}

		if(id==inputFrame->getId()) {
			label = new Label("Enter a value:", 102);
			Manager::Instance().addWidget(label);

			edit = new Edit("", 101);
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
			Label *label1 = new Label("Celsius:", 103);
			Manager::Instance().addWidget(label1);

			Label *label2 = new Label(buf, 104);
			Manager::Instance().addWidget(label2);

			Label *label3 = new Label("Fahrenheit:", 104);
			Manager::Instance().addWidget(label2);

			Label *label4 = new Label(buf, 104);
			Manager::Instance().addWidget(label2);

			resultsFrameLayout = new Layout(resultsFrame);

			resultsFrameLayout->addWidget(label1);
			resultsFrameLayout->addWidget(label2);
			resultsFrameLayout->addWidget(label3);
			resultsFrameLayout->addWidget(label4);

			resultsFrameLayout->build();

			delete(label1);
			delete(label2);
			delete(label3);
			delete(label4);
		}
	}
	void onClick (int id) {
		if(id==100) {

			buf = edit->getText();

			resultsFrame = new Frame(53);
			Manager::Instance().addWidget(resultsFrame);
			resultsFrame->addActionListener(this);
		}

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
