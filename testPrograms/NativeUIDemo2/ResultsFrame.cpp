#include "ResultsFrame.h"


using namespace MAUtil;

ResultsFrame::ResultsFrame (int Id, const String& category) : Frame(Id) { //params category, unit
	Manager::Instance().addWidget(this);
	this->addActionListener(this);

	mVec = Converter::getUnitsInCategory(category);

}

ResultsFrame::~ResultsFrame() {
	delete(mLayout);

}

void ResultsFrame::onCreate(int id) {
	if (id == this->getId()) {
		mLayout = new Layout(this);
		int i;
		for(i=0; i<2; i++) {

			mLayout->addWidget( Manager::Instance().addWidget(new Label(mVec[i].c_str(), Manager::Instance().getNextId())) );

		}

		mLayout->build();
		/*Label *tempLabel = new Label("salut", Manager::Instance().getNextId());
		Manager::Instance().addWidget(tempLabel);
		mLayout->addWidget(tempLabel);
		mLayout->build();*/
		/*mLabel1 = new Label("Celsius:", 103);
		Manager::Instance().addWidget(mLabel1);

		mLabel2 = new Label(buf, 104);
		Manager::Instance().addWidget(mLabel2);

		mLabel3 = new Label("Fahrenheit:", 105);
		Manager::Instance().addWidget(mLabel3);

		mLabel4 = new Label(Converter::convertUnit("Temperature", "C",
				"F", buf).c_str(), 106);
		Manager::Instance().addWidget(mLabel4);

		mLayout = new Layout(this);


		mLayout->addWidget(mLabel1);
		mLayout->addWidget(mLabel2);
		mLayout->addWidget(mLabel3);
		mLayout->addWidget(mLabel4);

		mLayout->build();*/


	}
}

void ResultsFrame::onClick (int id) {

}

void ResultsFrame::onItemSelected (int id, int index) {

}
