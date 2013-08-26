/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <MAUtil/Moblet.h>
#include <conprint.h>
#include <ma.h>
#include <mawstring.h>

using namespace MAUtil;

class MyMoblet : public Moblet, TextBoxListener {
public:
	wchar_t mBuf[32];
	MyMoblet() {
		printf("Hello World!\n");
		addTextBoxListener(this);
		int res = maTextBox(L"Title", L"initial text", mBuf, sizeof(mBuf) / sizeof(wchar_t), MA_TB_TYPE_ANY);
		printf("maTextBox: %i\n", res);
	}

	void customEvent(const MAEvent& e) {
		printf("cE: %i\n", e.type);
	}
	void textBoxClosed(int result, int textLength) {
		printf("textbox complete: %i, %i\n", result, textLength);
		wprintf(L"text: '%S'\n", mBuf);
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		printf("kP: %i, %i\n", keyCode, nativeCode);
		if(keyCode == MAK_0 || keyCode == MAK_FIRE)
			close();
	}
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};
