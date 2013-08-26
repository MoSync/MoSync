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

#include <ma.h>
#include <conprint.h>
#include <maassert.h>
#include <MAUtil/Moblet.h>
#include <IX_RECORD.h>
#include <mastdlib.h>

using namespace MAUtil;

//Check ConnErr
#define CCE(func) { int temp = (func); if(temp < 0) { printf("Error %i on line %i\n",\
	temp, __LINE__); FREEZE; } }

#define BUFSIZE 22100
short gPcmBuf[BUFSIZE];

class VuMoblet : public Moblet, public ConnListener, public TimerListener {
public:
	int count;
	VuMoblet() {
		//start recording
		printf("maRecordSetup\n");
		mRec = maRecordSetup();
		CCE(mRec);

		setConnListener(mRec, this);

		printf("maRecordStart\n");
		CCE(maRecordStart(mRec));

		printf("Starting loop...\n");
		count = 0;
		maConnRead(mRec, gPcmBuf, BUFSIZE*sizeof(short));

		//addTimer(this, 4000, 1);
	}

	void runTimerEvent() {
		printf("pause\n");
		maRecordPause(mRec);
	}

	void connEvent(const MAConnEventData& data) {
		//printf("cE %i %i %i\n", data.handle, data.opType, data.result);
		CCE(data.result);
		//printf("Read %i bytes\n", data.result);
		MAASSERT(data.handle == mRec);
		if(data.opType == CONNOP_READ) {
			if(data.result <= 0) {
				printf("Error!\n");
				return;
			}
			count += data.result;
			//printf("Count: %i\n", count);
			drawVU(gPcmBuf, data.result / sizeof(short));
			maConnRead(mRec, gPcmBuf, BUFSIZE*sizeof(short));
		}
	}

	void drawVU(const short* buf, int len) {
		MAExtent scrSize = maGetScrSize();
		int scrWidth = EXTENT_X(scrSize);
		int scrHeight = EXTENT_Y(scrSize);
		int samplesPerLine = len / scrHeight;	//landscape mode
		maSetColor(0);
		maFillRect(0,0, scrWidth, scrHeight);
		maSetColor(0x00ff00);
		for(int i=0, y=0; i<len; y++) {
			int sum=0;
			int j=0;
			for(; j<samplesPerLine && i<len; j++, i++) {
				sum += abs(buf[i]);
			}
			int avg = sum / j;	//range: 0 - 2^16
			int x = (avg * scrWidth) >> 16;
			maLine(0,y,x,y);
		}
		maUpdateScreen();
	}

	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_0) {
			maExit(0);
		}
		if(keyCode == MAK_1) {
			printf("pause\n");
			maRecordPause(mRec);
		}
	}

private:
	MAHandle mRec;
};

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;

	Moblet::run(new VuMoblet());
	return 0;
}
