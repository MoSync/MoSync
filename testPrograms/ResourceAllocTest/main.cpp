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

class MyMoblet : public Moblet {
public:
	MyMoblet() {
		// todo: initialize
		int mem = 1000000000;
		MAHandle resource = maCreatePlaceholder();

		bool testSucceded = false;

		printf("RESOURCE ALLOC TEST\n\n");

		while(mem > 100)
		{
			int result = maCreateData(resource, mem);
			if(result == RES_OUT_OF_MEMORY)
			{
				printf("FAILED - allocating %d bytes\n", mem);
				mem = mem/2;
			}
			else
			{
				testSucceded = true;
				printf("SUCEESS - allocated %d bytes\n", mem);
				break;
			}
		}


		if(testSucceded)
		{
			printf("\nREALLOCATING RESOURCE\n\n");
			for ( int i = 0; i < 100; i++)
			{
				maDestroyObject(resource);
				int result = maCreateData(resource, mem);
				if(result == RES_OUT_OF_MEMORY)
				{
					printf("FAILED REALLOCATING - step %d\n", i);
					testSucceded = false;
					break;
				}
			}

		}

		if(testSucceded)
			printf("SUCCESS - ALL MEMORY OPERATION SUCCEDED!\n");
		else
			printf("UNABLE TO ALLOCATE ANY MEMORY!\n");

	}

	void keyPressEvent(int keyCode, int nativeCode) {
		// todo: handle key presses
		if(keyCode == MAK_0 || keyCode == MAK_BACK)
			maExit(0);
	}

	void keyReleaseEvent(int keyCode, int nativeCode) {
		// todo: handle key releases
	}
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};
