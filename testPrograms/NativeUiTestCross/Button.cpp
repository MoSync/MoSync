#include "Button.h"


Button::Button(int id, void *parent, char *str) {

	myid=id;

	MAWidgetParameters buttonParams;
	buttonParams.pParent=parent;
	buttonParams.posX=30;
	buttonParams.posY=30;
	buttonParams.sizeX=100;//420;
	buttonParams.sizeY=30;//60;
	buttonParams.widgetID=id;
	strncpy(buttonParams.buf, str, strlen(str) + 1);
	MAWidgetHandle buttonHandle;
	maWinMobileButton(&buttonParams, &buttonHandle);
	me=buttonHandle.pWidget;

	maAndroidAddButton(id, str);

}

Button::~Button() {

}

int Button::getId() {
	Widget::getId();
}

void *Button::getInstance() {
	Widget::getInstance();
}

void Button::processEvent(const MAEvent & e) {
	//if(e.hi_wparam==0) {
		actionListener->onClick(e.lo_wparam);
	//}
}
