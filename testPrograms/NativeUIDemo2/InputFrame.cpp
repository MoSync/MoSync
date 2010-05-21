#include "InputFrame.h"



InputFrame::InputFrame (int Id) : Frame(Id) {
	Manager::Instance().addWidget(this);
	this->addActionListener(this);
}

InputFrame::~InputFrame() {
	delete(mLayout);
	delete(mConvertButton);
	delete(mEdit);
	delete(mLabel);
}

void InputFrame::onCreate (int id) {
	if (id == this->getId()) {
		mLabel = new Label("Enter a value:", Manager::Instance().getNextId());
		Manager::Instance().addWidget(mLabel);

		mEdit = new Edit("", Manager::Instance().getNextId());
		Manager::Instance().addWidget(mEdit);

		mConvertButton = new Button("Convert", Manager::Instance().getNextId());
		Manager::Instance().addWidget(mConvertButton);
		mConvertButton->addActionListener(this);

		mLayout = new Layout(this);

		mLayout->addWidget(mLabel);
		mLayout->addWidget(mEdit);
		mLayout->addWidget(mConvertButton);

		mLayout->build();


	}
}

void InputFrame::onClick (int id) {
	if (id == mConvertButton->getId()) {
		ResultsFrame *resultsFrame = new ResultsFrame(Manager::Instance().getNextId(), "Temperature");
	}
}

void InputFrame::onItemSelected (int id, int index) {

}
