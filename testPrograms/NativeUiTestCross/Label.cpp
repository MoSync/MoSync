#include "Label.h"

Label::Label (char *str) {


	text=str;


}

void Label::build(int x, int y, int h, int l, void *f) {
	MAWidgetParameters labelParams;
	labelParams.pParent=f;
	labelParams.widgetID=myid;
	labelParams.posX=x;
	labelParams.posY=y;
	labelParams.sizeX=l;
	labelParams.sizeY=h;
	strncpy(labelParams.buf, text, strlen(text) + 1);
	MAWidgetHandle labelHandle;
	maWinMobileLabel(&labelParams, NULL);
	me=labelHandle.pWidget;

	maAndroidAddTextView(myid, text);
}

Label::~Label() {

}

int Label::getId() {
	Widget::getId();
}

void *Label::getInstance() {
	Widget::getInstance();
}
