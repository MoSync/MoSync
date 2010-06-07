#include "DisplayFrame.h"




DisplayFrame::DisplayFrame (int Id, int img) : Frame(Id) {
	Manager::Instance().addWidget(this);
	this->addActionListener(this);
	mImage = new Image(img, Manager::Instance().getNextId());

	/*MAWidgetParameters navBarPar;
	navBarPar.pParent = me;
	maIPhoneNavigationBar(&navBarPar, NULL);*/


}

DisplayFrame::~DisplayFrame() {
	delete(mLayout);
	delete(mImage);

}

void DisplayFrame::onCreate(int id) {
	if (id == this->getId()) {
		mLayout = new Layout(this);
		mLayout->addWidget( Manager::Instance().addWidget(mImage) );
		mLayout->build();

	}
}

void DisplayFrame::onClick (int id) {

}

void DisplayFrame::onItemSelected (int id, int index) {

}
