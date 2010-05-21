#include <ma.h>
#include "Manager.h"
#include "CategoriesFrame.h"
#include "Converter.h"

extern "C" int MAMain() {
	Converter::init();
	CategoriesFrame *mainFrame = new CategoriesFrame(Manager::Instance().getNextId());

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
