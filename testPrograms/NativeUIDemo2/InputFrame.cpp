#include "InputFrame.h"



InputFrame::InputFrame (int Id) : Frame(Id) {
	Manager::Instance().addWidget(this);
	this->addActionListener(this);
}

InputFrame::~InputFrame() {

}

void InputFrame::onCreate (int id) {
	if (id == this->getId()) {
		Label *label = new Label("Enter a value:", 100);
		Manager::Instance().addWidget(label);

		Edit *edit = new Edit("", 101);
		Manager::Instance().addWidget(edit);

		Button *button = new Button("Convert", 102);
		Manager::Instance().addWidget(button);
		button->addActionListener(this);

		Layout *tempLayout = new Layout(this);

		tempLayout->addWidget(label);
		tempLayout->addWidget(edit);
		tempLayout->addWidget(button);

		tempLayout->build();

	}
}

void InputFrame::onClick (int id) {

}

void InputFrame::onItemSelected (int id, int index) {

}
