#include "Frame.h"


Frame::Frame(int id) {

	myid=id;


	MAWidgetParameters wp;
	MAWidgetHandle handle;

	wp.pParent=NULL;
	wp.widgetID=id;
	maWinMobileWindow(&wp, &handle);
	me=handle.pWidget;

	maAndroidStartActivity(id);


}

Frame::~Frame() {

}

int Frame::getId() {
	Widget::getId();
}

void *Frame::getInstance() {
	Widget::getInstance();
}

void Frame::processEvent(const MAEvent &e) {
	actionListener->onCreate(e.lo_wparam);
}
