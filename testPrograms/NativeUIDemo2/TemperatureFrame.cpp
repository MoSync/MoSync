#include "TemperatureFrame.h"



TemperatureFrame::TemperatureFrame (int Id) : ListFrame(Id) {
	Manager::Instance().addWidget(this);
	this->addActionListener(this);
}

TemperatureFrame::~TemperatureFrame() {

}

void TemperatureFrame::onCreate (int id) {
	if (id == this->getId()) {
		this->add("Celsius");
		this->add("Fahrenheit");
		this->show();

	}
}

void TemperatureFrame::onClick (int id) {

}

void TemperatureFrame::onItemSelected (int id, int index) {
	if (id == this->getId()) {
		switch(index) {
			case 1: {
				InputFrame *inputFrame = new InputFrame(Manager::Instance().getNextId());
			}
			break;

			case 2: {
				InputFrame *inputFrame = new InputFrame(Manager::Instance().getNextId());
			}
			break;
		}
	}
}
