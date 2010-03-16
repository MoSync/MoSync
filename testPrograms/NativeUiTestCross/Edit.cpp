#include "Edit.h"

Edit::Edit(int id, void *parent, char *str) {

	myid=id;

	MAWidgetParameters editParams;
	editParams.pParent=parent;//mywin_out.hwnd;
	editParams.posX=30;
	editParams.posY=120;
	editParams.sizeX=420;
	editParams.sizeY=60;
	editParams.widgetID=id;
	strncpy(editParams.buf, str, strlen(str) + 1);
	MAWidgetHandle editHandle;
	maWinMobileEdit(&editParams, &editHandle);
	me=editHandle.pWidget;

	maAndroidAddEditText(id, str);


}

Edit::~Edit() {

}

int Edit::getId() {
	Widget::getId();
}

void *Edit::getInstance() {
	Widget::getInstance();
}
