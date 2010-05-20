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
#include "Converter.h"



class Main : public ActionListener {
private:
	ListFrame *categoriesListFrame;
	ListFrame *tempListFrame, *weightListFrame;
	Frame *tempInputFrame;
	Frame *tempInputFrame2;
	Frame *weightInputFrame;

	Frame *resultsFrame;
	Frame *resultsFrame2;
	Frame *resultsFrame3;
	Button *button;
	Edit *edit;
	Label*label;
	Layout *tempLayout;
	char *buf;

public:
	Main() {
		categoriesListFrame = NULL;
		tempListFrame = NULL;
		weightListFrame = NULL;
		tempInputFrame = NULL;
		tempInputFrame2 = NULL;
		weightInputFrame = NULL;
		resultsFrame = NULL;
		resultsFrame2 = NULL;
		resultsFrame3 = NULL;
		button = NULL;
		edit = NULL;
		label = NULL;
		tempLayout = NULL;
		buf=NULL;

		Converter::init();

		categoriesListFrame = new ListFrame(50);

		Manager::Instance().addWidget(categoriesListFrame);
		categoriesListFrame->addActionListener(this);


	}

	Main(const Main & m) {
		categoriesListFrame=m.categoriesListFrame;

	}

	~Main() {

		delete(categoriesListFrame);
		delete(tempListFrame);
		delete(weightListFrame);
		delete(tempInputFrame);
		delete(tempInputFrame2);
		delete(weightInputFrame);
		delete(resultsFrame);
		delete(resultsFrame2);
		delete(resultsFrame3);
		delete(button);
		delete(edit);
		delete(label);
		delete(tempLayout);
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

		if(id==tempListFrame->getId()) {
					tempListFrame->add("Celsius");
					tempListFrame->add("Fahrenheit");
					tempListFrame->show();
		}

		if(id==weightListFrame->getId()) {
					weightListFrame->add("Kilograms");
					weightListFrame->add("Pounds");
					weightListFrame->show();
		}

		if(id==tempInputFrame->getId()) {
			label = new Label("Enter a value:", 102);
			Manager::Instance().addWidget(label);

			edit = new Edit("", 101);
			Manager::Instance().addWidget(edit);

			button = new Button("Convert", 100);
			Manager::Instance().addWidget(button);
			button->addActionListener(this);

			tempLayout = new Layout(tempInputFrame);

			tempLayout->addWidget(label);
			tempLayout->addWidget(edit);
			tempLayout->addWidget(button);

			tempLayout->build();
		}

		if(id==tempInputFrame2->getId()) {
			label = new Label("Enter a value:", 102);
			Manager::Instance().addWidget(label);

			edit = new Edit("", 101);
			Manager::Instance().addWidget(edit);

			button = new Button("Convert", 300);
			Manager::Instance().addWidget(button);
			button->addActionListener(this);

			tempLayout = new Layout(tempInputFrame2);

			tempLayout->addWidget(label);
			tempLayout->addWidget(edit);
			tempLayout->addWidget(button);

			tempLayout->build();
		}

		if (id == weightInputFrame->getId()) {


			label = new Label("Enter a value:", 201);
			Manager::Instance().addWidget(label);

			edit = new Edit("", 202);
			Manager::Instance().addWidget(edit);

			button = new Button("Convert", 200);
			Manager::Instance().addWidget(button);
			button->addActionListener(this);

			tempLayout = new Layout(weightInputFrame);


			tempLayout->addWidget(label);
			tempLayout->addWidget(edit);
			tempLayout->addWidget(button);

			tempLayout->build();
		}

		if(id==resultsFrame->getId()) {

			MAWidgetParameters wparamsBar;
			char *str1="Results";
			wparamsBar.pParent = resultsFrame->getInstance();
			strncpy(wparamsBar.buf, str1, strlen(str1)+1);
			maIPhoneNavigationBar(&wparamsBar, NULL);

			Label *label0 = new Label(" ", 103);
			Manager::Instance().addWidget(label0);

			Label *label1 = new Label("Celsius:", 103);
			Manager::Instance().addWidget(label1);

			Label *label2 = new Label(buf, 104);
			Manager::Instance().addWidget(label2);

			Label *label3 = new Label("Fahrenheit:", 105);
			Manager::Instance().addWidget(label3);

			Label *label4 = new Label(Converter::convertUnit("Temperature", "C", "F", buf).c_str(), 106);
			Manager::Instance().addWidget(label4);

			Layout *resultsFrameLayout = new Layout(resultsFrame);

			resultsFrameLayout->addWidget(label0);
			resultsFrameLayout->addWidget(label1);
			resultsFrameLayout->addWidget(label2);
			resultsFrameLayout->addWidget(label3);
			resultsFrameLayout->addWidget(label4);

			resultsFrameLayout->build();

			delete(label1);
			delete(label2);
			delete(label3);
			delete(label4);
			delete(resultsFrameLayout);
		}

		if(id==resultsFrame2->getId()) {
			MAWidgetParameters wparamsBar;
			char *str1="Results";
			wparamsBar.pParent = resultsFrame2->getInstance();
			strncpy(wparamsBar.buf, str1, strlen(str1)+1);
			maIPhoneNavigationBar(&wparamsBar, NULL);

			Label *label0 = new Label(" ", 103);
			Manager::Instance().addWidget(label0);

			Label *label1 = new Label("Kilograms:", 703);
			Manager::Instance().addWidget(label1);

			Label *label2 = new Label(buf, 704);
			Manager::Instance().addWidget(label2);

			Label *label3 = new Label("Pounds:", 705);
			Manager::Instance().addWidget(label3);

			Label *label4 = new Label(Converter::convertUnit("Mass", "kg", "lbs", buf).c_str(), 706);
			Manager::Instance().addWidget(label4);

			Layout *resultsFrameLayout = new Layout(resultsFrame2);

			resultsFrameLayout->addWidget(label0);
			resultsFrameLayout->addWidget(label1);
			resultsFrameLayout->addWidget(label2);
			resultsFrameLayout->addWidget(label3);
			resultsFrameLayout->addWidget(label4);

			resultsFrameLayout->build();

			delete(label1);
			delete(label2);
			delete(label3);
			delete(label4);
			delete(resultsFrameLayout);
		}

		if(id==resultsFrame3->getId()) {
			MAWidgetParameters wparamsBar;
			char *str1="Results";
			wparamsBar.pParent = resultsFrame3->getInstance();
			strncpy(wparamsBar.buf, str1, strlen(str1)+1);
			maIPhoneNavigationBar(&wparamsBar, NULL);

			Label *label0 = new Label(" ", 103);
			Manager::Instance().addWidget(label0);


			Layout *resultsFrameLayout3 = new Layout(resultsFrame3);
			resultsFrameLayout3->addWidget(label0);

			Label *label10 = new Label("Celsius:", 803);
			Manager::Instance().addWidget(label10);
			resultsFrameLayout3->addWidget(label10);

			Label *label20 = new Label(Converter::convertUnit("Temperature", "F", "C", buf).c_str(), 804);
			Manager::Instance().addWidget(label20);
			resultsFrameLayout3->addWidget(label20);

			Label *label30 = new Label("Fahrenheit:", 805);
			Manager::Instance().addWidget(label30);
			resultsFrameLayout3->addWidget(label30);

			Label *label40 = new Label(buf, 806);
			Manager::Instance().addWidget(label40);
			resultsFrameLayout3->addWidget(label40);








			resultsFrameLayout3->build();

			delete(label10);
			delete(label20);
			delete(label30);
			delete(label40);
			delete(resultsFrameLayout3);
		}
	}
	void onClick (int id) {
		if(id==100) {

			buf = edit->getText();

			resultsFrame = new Frame(53);
			Manager::Instance().addWidget(resultsFrame);
			resultsFrame->addActionListener(this);
		}

		if (id == 200) {

			buf = edit->getText();

			resultsFrame2 = new Frame(53);
			Manager::Instance().addWidget(resultsFrame2);
			resultsFrame2->addActionListener(this);
		}

		if (id == 300) {

			buf = edit->getText();

			resultsFrame3 = new Frame(57);
			Manager::Instance().addWidget(resultsFrame3);
			resultsFrame3->addActionListener(this);
		}

	}

	void onItemSelected (int id, int index) {
		if(id==50 && index==1) {
			tempListFrame = new ListFrame(51);
			Manager::Instance().addWidget(tempListFrame);
			tempListFrame->addActionListener(this);
		}

		if(id==50 && index==2) {
			weightListFrame = new ListFrame(54);
			Manager::Instance().addWidget(weightListFrame);
			weightListFrame->addActionListener(this);
		}

		if(id==51 && index==1) {
			tempInputFrame = new Frame(52);
			Manager::Instance().addWidget(tempInputFrame);
			tempInputFrame->addActionListener(this);
		}

		if(id==51 && index==2) {
			tempInputFrame2 = new Frame(56);
			Manager::Instance().addWidget(tempInputFrame2);
			tempInputFrame2->addActionListener(this);
		}

		if(id==54 && index==1) {
			weightInputFrame = new Frame(55);
			Manager::Instance().addWidget(weightInputFrame);
			weightInputFrame->addActionListener(this);
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
