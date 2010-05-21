#include "WeightFrame.h"



WeightFrame::WeightFrame (int Id) : ListFrame(Id) {
	Manager::Instance().addWidget(this);
	this->addActionListener(this);
}

WeightFrame::~WeightFrame() {

}

void WeightFrame::onCreate (int id) {
	if (id == this->getId()) {
		this->add("Kilograms");
		this->add("Pounds");
		this->show();

	}
}

void WeightFrame::onClick (int id) {

}

void WeightFrame::onItemSelected (int id, int index) {
	if (id == this->getId()) {
		switch(index) {
			case 1: {
				InputFrame *inputFrame = new InputFrame(300);
			}
			break;

			case 2: {
				InputFrame *inputFrame = new InputFrame(301);
			}
			break;
		}
	}
}
