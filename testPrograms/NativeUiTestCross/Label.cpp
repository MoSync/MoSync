#include "Label.h"

Label::Label (int id, void *parent, char *str) {

	myid=id;

	MAWidgetParameters labelParams;
	labelParams.pParent=parent;
	labelParams.widgetID=id;
	labelParams.posX=30;
	labelParams.posY=210;
	labelParams.sizeX=420;
	labelParams.sizeY=60;
	strncpy(labelParams.buf, str, strlen(str) + 1);
	MAWidgetHandle labelHandle;
	maWinMobileLabel(&labelParams, NULL);
	me=labelHandle.pWidget;

	maAndroidAddTextView(id, str);

}

Label::~Label() {

}

int Label::getId() {
	Widget::getId();
}

void *Label::getInstance() {
	Widget::getInstance();
}
