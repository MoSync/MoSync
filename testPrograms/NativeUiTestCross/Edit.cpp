#include "Edit.h"

Edit::Edit(char *str) {


	text=str;


}

void Edit::build(int x, int y, int h, int l, void *f) {
	MAWidgetParameters editParams;
	editParams.pParent=f;//mywin_out.hwnd;
	editParams.posX=x;
	editParams.posY=y;
	editParams.sizeX=l;
	editParams.sizeY=h;
	editParams.widgetID=myid;
	strncpy(editParams.buf, text, strlen(text) + 1);
	MAWidgetHandle editHandle;
	maWinMobileEdit(&editParams, &editHandle);
	me=editHandle.pWidget;

	maAndroidAddEditText(myid, text);
}

Edit::~Edit() {

}

int Edit::getId() {
	Widget::getId();
}

void *Edit::getInstance() {
	Widget::getInstance();
}
