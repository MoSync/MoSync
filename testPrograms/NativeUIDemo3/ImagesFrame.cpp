#include "ImagesFrame.h"
#include "MAHeaders.h"


ImagesFrame::ImagesFrame (int Id) : ListFrame(Id) {
	Manager::Instance().addWidget(this);
	this->addActionListener(this);
}

ImagesFrame::~ImagesFrame() {

}

void ImagesFrame::onCreate (int id) {
	if (id == this->getId()) {
		this->add("Cirques");
		this->add("Clown Fish");
		this->add("Stones");
		this->add("Earth");
		this->show();

	}
}

void ImagesFrame::onClick (int id) {

}

void ImagesFrame::onItemSelected (int id, int index) {
	if (id == this->getId()) {
		switch(index) {
			case 1: {
				DisplayFrame *displayFrame = new DisplayFrame(Manager::Instance().getNextId(), CIRQUES);
			}
			break;

			case 2: {
				DisplayFrame *displayFrame = new DisplayFrame(Manager::Instance().getNextId(), CLOWN_FISH);
			}
			break;

			case 3: {
				DisplayFrame *displayFrame = new DisplayFrame(Manager::Instance().getNextId(), STONES);
			}
			break;

			case 4: {
				DisplayFrame *displayFrame = new DisplayFrame(Manager::Instance().getNextId(), EARTH);
			}
			break;
		}
	}
}
