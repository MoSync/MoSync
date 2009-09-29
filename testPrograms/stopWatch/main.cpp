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

#include <MAUtil/Moblet.h>
#include <conprint.h>

using namespace MAUtil;

//******************************************************************************
// Config
//******************************************************************************
#define RUNNING_TIME

//******************************************************************************
// Defines
//******************************************************************************
#define MS_MINUTE (60*1000)
#define MS_HOUR (MS_MINUTE*60)
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#ifdef RUNNING_TIME
//******************************************************************************
// Console hack
//******************************************************************************
#define CONSOLE_WIDTH 48

typedef struct
{
	char line[CONSOLE_WIDTH];
} ConLine;

extern int CONSOLE_DEPTH;
extern int CON_FONT_SIZE_Y;
extern ConLine* conlines;
extern Extent scrsize;

void OverwriteConsole(const char* text) {
	char* lastLine = conlines[CONSOLE_DEPTH-1].line;
	int copylen = MIN(strlen(text), CONSOLE_WIDTH-1);
	strncpy(lastLine, text, copylen);
	lastLine[copylen] = 0;

	int prev = maSetColor(0);
	maFillRect(0, (CONSOLE_DEPTH-1)*CON_FONT_SIZE_Y, EXTENT_X(scrsize), CON_FONT_SIZE_Y);
	maSetColor(prev);
	maDrawText(0, (CONSOLE_DEPTH-1)*CON_FONT_SIZE_Y, lastLine); 
	maUpdateScreen();
}
#endif

//******************************************************************************
// Moblet
//******************************************************************************

class MyMoblet : public Moblet, public TimerListener {
private:
	enum State {
		IDLE, RUNNING
	} mState;

	//in milliseconds.
	//will probably wrap around to zero at about 47 or 23½ hours, due to 32(31)-bit ints.
	int mRunningTime, mStartTime;

public:
	MyMoblet() {
		printf("Hello World!\n");
		reset();
	}
	
	void sprintTime(char* buf, uint ms, const char* pre = NULL) {
		char* ptr = buf;
		if(pre != NULL) {
			ptr += sprintf(ptr, "%s", pre);
		}
		if(ms > MS_HOUR) {
			ptr += sprintf(ptr, "%i.", ms / MS_HOUR);
			ms %= MS_HOUR;
		}
		if(ms > MS_MINUTE) {
			ptr += sprintf(ptr, "%02i.", ms / MS_MINUTE);
			ms %= MS_MINUTE;
		}
		ptr += sprintf(ptr, "%02i.%03i", ms / 1000, ms % 1000);
	}

	void printTime(uint ms, const char* pre = NULL) {
		char buffer[64];
		sprintTime(buffer, ms, pre);
		PrintConsole(buffer);
	}
	
	void overwriteTime(uint ms, const char* pre = NULL) {
		char buffer[64];
		sprintTime(buffer, ms, pre);
		OverwriteConsole(buffer);
	}

	void start() {
		mState = RUNNING;
		mStartTime = maGetMilliSecondCount();
		printf("Start.\n");
#ifdef RUNNING_TIME
		printTime(mRunningTime);
		addTimer(this, 100, 0);	//10 ms
	}

	void runTimerEvent() {
		char buffer[64];
		sprintTime(buffer, mRunningTime + maGetMilliSecondCount() - mStartTime);
		OverwriteConsole(buffer);
	}
#endif

	void lap() {
#ifdef RUNNING_TIME
		overwriteTime(mRunningTime + maGetMilliSecondCount() - mStartTime, "Lap: ");
		printTime(mRunningTime);
#else
		printTime(mRunningTime + maGetMilliSecondCount() - mStartTime, "Lap: ");
#endif
	}

	void stop() {
		mState = IDLE;
		mRunningTime += maGetMilliSecondCount() - mStartTime;
#ifdef RUNNING_TIME
		overwriteTime(mRunningTime, "Stop: ");
		removeTimer(this);
#else
		printTime(mRunningTime, "Stop: ");
#endif
	}

	void reset() {
		mRunningTime = 0;
		printf("Reset.\n");
	}

	void keyPressEvent(int keyCode) {
		switch(keyCode) {
		case MAK_SOFTLEFT:
			//start or lap
			if(mState == IDLE)
				start();
			else if(mState == RUNNING)
				lap();
			break;
		case MAK_SOFTRIGHT:
			//stop
			if(mState == RUNNING)
				stop();
			break;
		case MAK_5:
			//reset. stop if running first.
			if(mState == RUNNING)
				stop();
			reset();
			break;
		case MAK_0:
			close();
			break;
		}
	}

	void keyReleaseEvent(int keyCode) {
		// todo: handle key releases
	}
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};
