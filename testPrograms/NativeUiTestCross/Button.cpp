#include "Button.h"


Button::Button(char *str) {


	text=str;




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

void Button::build(int x, int y, int h, int l, void *f) {
	MAWidgetParameters buttonParams;
	buttonParams.pParent=f;
	buttonParams.posX=x;
	buttonParams.posY=y;
	buttonParams.sizeX=l;//420;
	buttonParams.sizeY=h;//60;
	buttonParams.widgetID=myid;
	strncpy(buttonParams.buf, text, strlen(text) + 1);
	MAWidgetHandle buttonHandle;
	maWinMobileButton(&buttonParams, &buttonHandle);
	me=buttonHandle.pWidget;

	maAndroidAddButton(myid, text);
}
