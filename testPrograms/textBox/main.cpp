#include <MAUtil/Moblet.h>
#include <conprint.h>
#include <ma.h>

using namespace MAUtil;

class MyMoblet : public Moblet {
public:
	MyMoblet() {
		printf("Hello World!\n");
#if 1
		wchar_t buf[1024];
		int res = maTextBox(L"Title", buf, 1024, MA_TB_TYPE_ANY);
		printf("maTextBox: %i\n", res);
#endif
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
