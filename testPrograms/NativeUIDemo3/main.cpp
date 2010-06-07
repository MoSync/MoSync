#include <ma.h>
#include "Manager.h"
#include "ImagesFrame.h"


extern "C" int MAMain() {

	ImagesFrame *imagesFrame = new ImagesFrame(Manager::Instance().getNextId());

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
