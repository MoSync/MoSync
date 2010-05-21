#include "CategoriesFrame.h"



CategoriesFrame::CategoriesFrame (int Id) : ListFrame(Id) {
	Manager::Instance().addWidget(this);
	this->addActionListener(this);
}

CategoriesFrame::~CategoriesFrame() {

}

void CategoriesFrame::onCreate (int id) {
	if (id == this->getId()) {
		this->add("Temperature");
		this->add("Weight");
		this->add("Length");
		this->add("Currency");
		this->add("Angle");
		this->show();

	}
}

void CategoriesFrame::onClick (int id) {

}

void CategoriesFrame::onItemSelected (int id, int index) {
	if (id == this->getId()) {
		switch(index) {
			case 1: {
				TemperatureFrame *temperatureFrame = new TemperatureFrame(2);
			}
			break;

			case 2: {
				WeightFrame *weightFrame = new WeightFrame(3);
			}
			break;
		}
	}
}
