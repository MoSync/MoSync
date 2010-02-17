#include <MAUtil/Moblet.h>
#include <MAUtil/PlaceholderPool.h>
#include <IX_CAMERA.h>
#include <conprint.h>

using namespace MAUtil;

class MyMoblet : public Moblet {
	MAHandle mLastEnc, mLastPic;
public:
	MyMoblet() {
		mLastEnc = mLastPic = 0;
		int res = maCameraStart();
		printf("start: %i\n", res);
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		int res;
		switch(keyCode) {
		case MAK_0:
			close();
		case MAK_FIRE:
			mLastEnc = PlaceholderPool::alloc();
			res = maCameraSnapshot(0, mLastEnc);
			printf("snap: %i\n", res);
			printf("size: %i\n", maGetDataSize(mLastEnc));
			break;
		case MAK_6:
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
		case MAK_2:
			maDrawImage(mLastPic, 0,0);
			maUpdateScreen();
			break;
		case MAK_4:
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
