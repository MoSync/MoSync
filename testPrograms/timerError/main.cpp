#include <MAUtil/Moblet.h>
#include <conprint.h>
using namespace MAUtil;

class MyMoblet : public Moblet, public TimerListener {
public:

	MyMoblet() {
		printf("Hello World!\n");
		addTimer(this, 20, 0);
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		printf("Button %d pressed.  Native %d",keyCode,nativeCode);
		if (keyCode == MAK_0){
			maExit(0);
		}
	}

	void keyReleaseEvent(int keyCode, int nativeCode) {
	}

	void runTimerEvent() {
		maResetBacklight();	//does not help
	}
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};
