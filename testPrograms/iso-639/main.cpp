#include <MAUtil/Moblet.h>
#include <conprint.h>

using namespace MAUtil;

class MyMoblet : public Moblet {
public:
	MyMoblet() {
		char buf[64] = "";
		int res;
		printf("Hello World!\n");
		res = maGetSystemProperty("mosync.iso-639-1", buf, sizeof(buf));
		printf("ISO-639-1:%i: %s\n", res, buf);
		buf[0] = 0;
		res = maGetSystemProperty("mosync.iso-639-2", buf, sizeof(buf));
		printf("ISO-639-2:%i: %s\n", res, buf);
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		if(keyCode == MAK_0)
			close();
	}
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};
