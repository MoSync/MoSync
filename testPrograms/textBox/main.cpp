#include <MAUtil/Moblet.h>
#include <conprint.h>
#include <ma.h>

using namespace MAUtil;

class MyMoblet : public Moblet {
public:
	MyMoblet() {
		printf("Hello World!\n");
#if 1
		wchar_t buf[32] = L"buf";
		int res = maTextBox(L"Title", buf, 32, MA_TB_TYPE_ANY);
		printf("maTextBox: %i\n", res);
		wprintf(L"text: '%S'\n", buf);
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
