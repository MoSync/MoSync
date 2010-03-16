#include "Frame.h"


Frame::Frame() {




	MAWidgetParameters wp;
	MAWidgetHandle handle;

	wp.pParent=NULL;
	wp.widgetID=myid;
	maWinMobileWindow(&wp, &handle);
	me=handle.pWidget;

	maAndroidStartActivity(myid);


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
