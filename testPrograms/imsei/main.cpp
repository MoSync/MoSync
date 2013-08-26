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
		char buf[64] = "";
		int res;
		printf("Hello World!\n");
		res = maGetSystemProperty("mosync.imei", buf, sizeof(buf));
		printf("IMEI:%i: %s\n", res, buf);
		buf[0] = 0;
		res = maGetSystemProperty("mosync.imsi", buf, sizeof(buf));
		printf("IMSI:%i: %s\n", res, buf);

#if 0
		if(res > 0) {
			// print byte values
			char buf2[128];
			char* ptr = buf2;
			for(int i=0; i<res; i++) {
				ptr += sprintf(ptr, "%i,", buf[i]);
			}
			printf("%s\n", buf2);

			//print individual chars
			ptr = buf2;
			for(int i=0; i<res; i++) {
				ptr += sprintf(ptr, "%c", buf[i]);
			}
			printf("%s\n", buf2);
		}
#endif	//0
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		if(keyCode == MAK_0)
			close();
	}
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};
