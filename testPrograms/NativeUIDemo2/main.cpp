#include <ma.h>
#include "Manager.h"
#include "CategoriesFrame.h"

extern "C" int MAMain() {

	CategoriesFrame *mainFrame = new CategoriesFrame(1);

	MAEvent event;
	while(1) {
		while(maGetEvent(&event)) {
			if(event.type==EVENT_TYPE_NATIVE_UI_MSG) {
				Manager::Instance().dispatch( event );
			}
		}
	}
	return 0;
}
