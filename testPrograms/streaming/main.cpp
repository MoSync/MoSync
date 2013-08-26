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

#include <maapi.h>
#include <IX_STREAMING.H>
#include <conprint.h>
#include <MAUtil/Moblet.h>

using namespace MAUtil;

class StreaMoblet : public Moblet, public ConnListener {
	Handle mHttp;
public:
	StreaMoblet() {
#if 0
		printf("starting...\n");
		int result = maSoundPlayConn(0, "audio/mpeg");
		printf("spc: %i\n", result);
	}
#else
		printf("connecting...\n");
		mHttp = maConnect("http://localhost:5002/machinae_supremacy-sidology_2-trinity.mp3");
		setConnListener(mHttp, this);
	}

	void connEvent(const CONN_EVENT_DATA& data) {
		int result = data.result;
		printf("finish %i\n", result);
		if(result >= 200 && result <= 299) {
			char ctBuf[256];
			result = maHttpGetResponseHeader(mHttp, "content-type", ctBuf, 256);
			printf("hrh: %i\n", result);
			if(result > 0) {
				printf("ct: %s\n", ctBuf);
				result = maSoundPlayConn(mHttp, ctBuf);
				printf("spc: %i\n", result);
			}
		}
	}
#endif
	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_0) {
			maExit(0);
		}
	}
};

void MAMain() {
	InitConsole();
	Moblet::run(new StreaMoblet);
}
