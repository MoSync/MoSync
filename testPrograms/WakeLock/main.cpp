#include <MAUtil/Moblet.h>
#include <conprint.h>

using namespace MAUtil;

class MyMoblet : public Moblet
{
public:
	MyMoblet() : mWakeLockOn(false)
	{
		printf("Test wake lock\n");
		printf("Touch screen to toggle wake lock on/off\n");
		printf("Press zero or back to exit\n");
	}

	void keyPressEvent(int keyCode, int nativeCode)
	{
		if (MAK_BACK == keyCode || MAK_0 == keyCode)
		{
			// Call close to exit the application.
			close();
		}
	}

	void pointerReleaseEvent(MAPoint2d point)
	{
		if (mWakeLockOn)
		{
			maWakeLock(MA_WAKE_LOCK_OFF);
			printf("Wake lock is OFF\n");
		}
		else
		{
			maWakeLock(MA_WAKE_LOCK_ON);
			printf("Wake lock is ON\n");
		}
		mWakeLockOn = !mWakeLockOn;
	}

	bool mWakeLockOn;
};

extern "C" int MAMain()
{
	Moblet::run(new MyMoblet());
	return 0;
}
