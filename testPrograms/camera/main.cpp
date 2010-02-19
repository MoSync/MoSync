#include <MAUtil/Moblet.h>
#include <MAUtil/PlaceholderPool.h>
#include <conprint.h>
#include <MAUtil/String.h>

using namespace MAUtil;

#define USE_FORMATS 1

class MyMoblet : public Moblet {
	MAHandle mLastEnc, mLastPic;
#if USE_FORMATS
	bool mSelectMode;
	int mFormatIndex;
	int mnFormats;
#endif
public:
	MyMoblet() {
		mLastEnc = mLastPic = 0;
#if USE_FORMATS
		mSelectMode = false;
		mFormatIndex = 0;
		mnFormats = 0;
		dump("camera.resolutions");
		showFormats();
#else
		int res = maCameraStart();
		printf("start: %i\n", res);
#endif
	}
	
#if USE_FORMATS
	void dump(const char* key) {
		int len = maGetSystemProperty(key, NULL, 0);
		printf("%s: %i\n", key, len);
		if(len < 0) {
			return;
		}
		String s(len);
		maGetSystemProperty(key, s.pointer(), len);
		printf("%s\n", s.c_str());
	}
	
	void showFormats() {
		MAASSERT(!mSelectMode);
		mnFormats = maCameraFormatNumber();
		printf("fnum: %i\n", mnFormats);
		if(mnFormats <= 0)
			return;
		
		for(int i=0; i<mnFormats; i++) {
			MA_CAMERA_FORMAT cf;
			int res = maCameraFormat(i, &cf);
			MAASSERT(res == 0);
			printf("%i: %ix%i\n", i+1, cf.width, cf.height);
		}
		
		mSelectMode = true;
	}
#endif
	// 0: close
	// FIRE: snapshot
	// 1: start
	// 3: stop
	// 7: show snapshot
	// 9: select format
	void keyPressEvent(int keyCode, int nativeCode) {
		int res;
#if USE_FORMATS
		if(mSelectMode) {
			if(keyCode == MAK_0)
				close();
			if(keyCode > MAK_0 && keyCode <= MAK_9) {
				int index = keyCode - MAK_1;
				if(index >= mnFormats) {
					return;
				}
				mFormatIndex = index;
				printf("selected %i\n", index+1);
				mSelectMode = false;
			}
			return;
		}
#endif
		switch(keyCode) {
#if USE_FORMATS
		case MAK_9:
			showFormats();
			break;
#endif
		case MAK_0:
			close();
		case MAK_FIRE:
			mLastEnc = PlaceholderPool::alloc();
#if USE_FORMATS
			res = maCameraSnapshot(mFormatIndex, mLastEnc);
#else
			res = maCameraSnapshot(0, mLastEnc);
#endif
			printf("snap: %i\n", res);
			printf("size: %i\n", maGetDataSize(mLastEnc));
			break;
		case MAK_3:
			res = maCameraStop();
			printf("stop: %i\n", res);
			if(mLastEnc != 0) {
				if(mLastPic != 0) {
					PlaceholderPool::put(mLastPic);
				}
				mLastPic = PlaceholderPool::alloc();
				res = maCreateImageFromData(mLastPic, mLastEnc, 0, maGetDataSize(mLastEnc));
				printf("cifd: %i\n", res);
				res = maGetImageSize(mLastPic);
				printf("wh: %ix%i\n", EXTENT_X(res), EXTENT_Y(res));
				PlaceholderPool::put(mLastEnc);
				mLastEnc = 0;
			}
			break;
		case MAK_7:
			maDrawImage(mLastPic, 0,0);
			maUpdateScreen();
			break;
		case MAK_1:
			res = maCameraStart();
			printf("start: %i\n", res);
			break;
		}
	}
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};
