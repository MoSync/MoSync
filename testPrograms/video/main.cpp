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
#include <IX_STREAMING.H>

using namespace MAUtil;

struct VIDEO {
	const char* url;
	const char* name;
};

static const VIDEO kVideos[] = {
	{ "rtsp://rtsp.youtube.com/youtube/videos/HtVbAazUekM/video.3gp", "lollipop land" },
	{ "rtsp://msdev.mine.nu/sample_h264_1mbit.mp4", "sample_h264_1mbit.mp4" },
	{ "rtsp://msdev.mine.nu/sample_h264_100kbit.mp4", "sample_h264_100kbit.mp4" },
	{ "rtsp://msdev.mine.nu/sample_50kbit.3gp", "sample_50kbit.3gp" },
	{ "rtsp://ugbf.rtsp-youtube.l.google.com/Ci4LENy73wIaJQkBHa3TNHbJehMYDSANFEIJbXYtZ29vZ2xlSARSB2RldGFpbHMM/0/0/0/video.3gp", "sorunda banjo" },
	{ "rtsp://prug.rtsp-youtube.l.google.com/CkQLENy73wIaOwkct7WabRer7BMYDSANFEIJbXYtZ29vZ2xlSARSB2RldGFpbHNaCUNsaWNrTGlua2Ds75-Pqsa5qv4BDA==/0/0/0/video.3gp","panicroom - fairlight" }
	//	{ "rtsp://rtsp.youtube.com/youtube/videos/Yu_moia-oVI/video.3gp", "RickRoll" },
//	{ "rtsp://rtsp.youtube.com/youtube/videos/EHzej7P5FPw/video.3gp", "Shortest" },
};

static const int kNVideos = sizeof(kVideos) / sizeof(VIDEO);

class MyMoblet : public Moblet, public TimerListener {
private:
	int mStartTime;
	Handle mStream;
	int mKeyDown;
	int mKeyRight;
	bool mTimerActive;
	bool mPlaying;
	MARect mFrame;
	bool mMoveSize;
public:
	MyMoblet() {
		mKeyDown = 0;
		mKeyRight = 0;
		mTimerActive = false;
		mStartTime = -1;
		mStream = 0;
		printf("Select %i-%i\n", 1, kNVideos);
	}

	int getTime() const {
		return maGetMilliSecondCount() - mStartTime;
	}
	
	void customEvent(const EVENT& event) {
		if(event.type == EVENT_TYPE_STREAM) {
			STREAM_EVENT_DATA* sed = (STREAM_EVENT_DATA*)event.data;
			printf("State %i.%i @ %i ms\n", sed->event, sed->result, getTime());
			mPlaying = (sed->event == STREAMEVENT_LOADING_COMPLETE) && (sed->result >= 0);
			if(sed->event == STREAMEVENT_PREPARE_COMPLETE) {
				mFrame.top = mFrame.left = 0;
				Extent e = maStreamVideoSize(mStream);
				if(e < 0) {
					printf("svs error %i\n", e);
					int res = maStreamClose(mStream);
					printf("close %i @ %i ms\n", res, getTime());
				} else {
					mFrame.width = EXTENT_X(e);
					mFrame.height = EXTENT_Y(e);
					printf("Size: %ix%i\n", mFrame.width, mFrame.height);
				}
				printf("Length: %i ms\n", maStreamLength(mStream));
			}
		}
	}

	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_0)
			close();
		if(mStream == 0) {
			int numCode = keyCode - MAK_1;	//assumes that MAK_1 to MAK_9 are sequential.
			if(numCode >= 0 && numCode < kNVideos) {
				const VIDEO& v(kVideos[numCode]);
				printf("%s\n", v.name);
				mStartTime = maGetMilliSecondCount();
				mStream = maStreamVideoStart(v.url);
				printf("Init %i @ %i ms\n", mStream, getTime());
				mMoveSize = true;
				if(mStream)
				{
					Extent e = maStreamVideoSize(mStream);
					mFrame.width = EXTENT_X(e);
					mFrame.height = EXTENT_Y(e);
				}
			}
		} else {
			if(keyCode == MAK_SOFTLEFT) {
				int res = maStreamPause(mStream);
				printf("pause %i %i @ %i ms\n", mStream, res, getTime());
			} else if(keyCode == MAK_SOFTRIGHT) {
				int res = maStreamResume(mStream);
				printf("resume %i %i @ %i ms\n", mStream, res, getTime());
			} else if(keyCode == MAK_FIRE) {
				int res = maStreamClose(mStream);
				printf("close %i %i @ %i ms\n", mStream, res, getTime());
				mStartTime = -1;
				mPlaying = false;
			} else if(keyCode == MAK_UP) {
				mKeyDown--;
			} else if(keyCode == MAK_DOWN) {
				mKeyDown++;
			} else if(keyCode == MAK_LEFT) {
				mKeyRight--;
			} else if(keyCode == MAK_RIGHT) {
				mKeyRight++;
			} else if(keyCode == MAK_4) {
				mFrame.left -= 2;
				maStreamVideoSetFrame(mStream, &mFrame);
			} else if(keyCode == MAK_6) {
				mFrame.left += 2;
				maStreamVideoSetFrame(mStream, &mFrame);
			} else if(keyCode == MAK_2) {
				mFrame.top -= 2;
				maStreamVideoSetFrame(mStream, &mFrame);
			} else if(keyCode == MAK_8) {
				mFrame.top += 2;
				maStreamVideoSetFrame(mStream, &mFrame);
			} else if(keyCode == MAK_1) {
				if(((mFrame.width-2)>0) && ((mFrame.height-2)>0))
				{
					mFrame.width -= 2;
					mFrame.height -= 2;
					maStreamVideoSetFrame(mStream, &mFrame);
				}
			} else if(keyCode == MAK_3) {
				mFrame.width += 2;
				mFrame.height += 2;
				maStreamVideoSetFrame(mStream, &mFrame);
			} else if(keyCode == MAK_9) {
				movePos(100000);
			} else if(keyCode == MAK_7) {
				movePos(-100000);
			} else if(keyCode == MAK_5) {
				mMoveSize = !mMoveSize;
			} 
			checkKeyState();
		}
	}

	void movePos(int offset) {
		int oldPos = maStreamPos(mStream);
		int tryPos = oldPos + offset;
		int newPos = maStreamSetPos(mStream, tryPos);
		printf("try %i\n   to %i   \nres %i", oldPos, tryPos, newPos);
	}

	void keyReleaseEvent(int keyCode) {
		if(keyCode == MAK_UP) {
			mKeyDown++;
		} else if(keyCode == MAK_DOWN) {
			mKeyDown--;
		} else if(keyCode == MAK_LEFT) {
			mKeyRight++;
		} else if(keyCode == MAK_RIGHT) {
			mKeyRight--;
		}
		checkKeyState();
	}

	void checkKeyState() {
		bool timerShouldBeOn = (mKeyDown != 0) || (mKeyRight != 0);
		if(!mTimerActive && timerShouldBeOn) {
			addTimer(this, 10, 0);
			mTimerActive = true;
		} else if(mTimerActive && !timerShouldBeOn) {
			removeTimer(this);
			mTimerActive = false;
		}
	}

	void runTimerEvent() {
		printf("timer event\n");
		if(mPlaying) {
			if(mMoveSize) {
				mFrame.width += mKeyRight;
				mFrame.height += mKeyDown;
			} else {	//move frame
				mFrame.top += mKeyDown;
				mFrame.left += mKeyRight;
			}
			printf("moving\n");
			maStreamVideoSetFrame(mStream, &mFrame);
		}
	}
};

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;
	Moblet::run(new MyMoblet());
	return 0;
};
