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
