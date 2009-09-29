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

using namespace MAUtil;

//Check ConnErr
#define CCE(func) { int temp = (func); if(temp < 0) { printf("Error %i on line %i\n",\
	temp, __LINE__); FREEZE; } }

#define BUFSIZE 22100
short gPcmBuf[BUFSIZE];

class VuMoblet : public Moblet, public ConnectionListener, public TimerListener {
public:
	VuMoblet() {
		//start recording
		printf("maRecordSetup\n");
		mRec = maRecordSetup();
		CCE(mRec);

		setConnectionListener(mRec, this);

		printf("maRecordStart\n");
		CCE(maRecordStart(mRec));

		printf("Starting loop...\n");
		maConnRead(mRec, gPcmBuf, BUFSIZE*sizeof(short));

		addTimer(this, 10000, 1);
	}

	void runTimerEvent() {
		printf("pause\n");
		maRecordPause(mRec);
	}

	void connEvent(const CONN_EVENT_DATA& data) {
		printf("cE %i %i %i\n", data.handle, data.opType, data.result);
		CCE(data.result);
		//printf("Read %i bytes\n", data.result);
		MAASSERT(data.handle == mRec);
		if(data.opType == CONNOP_READ) {
			maConnRead(mRec, gPcmBuf, BUFSIZE*sizeof(short));
		}
	}

	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_0) {
			maExit(0);
		}
	}

private:
	Handle mRec;
};

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;

	Moblet::run(new VuMoblet());
}
