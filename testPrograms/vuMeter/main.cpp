/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
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
