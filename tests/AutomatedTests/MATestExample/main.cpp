#include <MAUtil/Moblet.h>
#include <MAUtil/String.h>
#include <MATest/TestRunner.h>
#include <conprint.h>

using namespace MAUtil;
using namespace MATest;

class MyMoblet : public Moblet
{
public:
	MyMoblet()
	{
		printf("MATestExample\n");
		printf("Press zero or back to exit\n");

		// Set test listener that will report results.
		TestRunner::getInstance()->addTestListener(
			new HighLevelTestListener());

		// Run tests.
		TestRunner::getInstance()->runTests();
	}

	void keyPressEvent(int keyCode, int nativeCode)
	{
		if (MAK_BACK == keyCode || MAK_0 == keyCode)
		{
			// Call close to exit the application.
			close();
		}
	}
};

extern "C" int MAMain()
{
	Moblet::run(new MyMoblet());
	return 0;
}
