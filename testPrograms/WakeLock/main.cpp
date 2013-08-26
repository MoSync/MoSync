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
