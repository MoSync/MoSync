#include <MAUtil/Moblet.h>
#include <conprint.h>
#include <ma.h>
#include <mawstring.h>

using namespace MAUtil;

class MyMoblet : public Moblet {
public:
	wchar_t mBuf[32];
	MyMoblet() {
		printf("Hello World!\n");
		int res = maTextBox(L"Title", L"initial text", mBuf, sizeof(mBuf) / sizeof(wchar_t), MA_TB_TYPE_ANY);
		printf("maTextBox: %i\n", res);
	}

	void customEvent(const MAEvent& e) {
		if(e.type != EVENT_TYPE_TEXTBOX)
			return;
		printf("textbox complete: %i, %i\n", e.textboxResult, e.textboxLength);
		wprintf(L"text: '%S'\n", mBuf);
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
