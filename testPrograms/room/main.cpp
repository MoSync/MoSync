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
#include <MAUtil/PlaceholderPool.h>
#include <conprint.h>
#include <IX_SEGMENTED_DATA.h>

using namespace MAUtil;

#define ALLOC_MUL 4
#define ALLOC_SIZE (ALLOC_MUL*1024)

static char sBuf[ALLOC_SIZE];

class MyMoblet : public Moblet {
public:
	MyMoblet() {
		int total;
		printf("allocating...\n");
		int res;
		do {
			Handle h = PlaceholderPool::alloc();
			res = maCreateData(h, ALLOC_SIZE);
			if(res != RES_OK)
				break;
			maWriteData(h, sBuf, 0, ALLOC_SIZE);
			maReadData(h, sBuf, 0, ALLOC_SIZE);
			total += 4;
		} while(res == RES_OK);
		printf("%i KiB\n", total);
	}

	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_0)
			close();
	}
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};
