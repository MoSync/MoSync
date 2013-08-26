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
#include <IX_CONNSERVER.H>

using namespace MAUtil;

class MyMoblet : public Moblet {
public:
	MyMoblet() {
		printf("Hello World!\n");
		ConnAddr addr;
		addr.family = CONN_FAMILY_BT;
		int res = maConnGetAddr(HANDLE_LOCAL, &addr);
		printf("maConnGetAddr res: %i\n", res);
		byte* a = addr.bt.addr.a;
		printf("%02x%02x%02x%02x%02x%02x\n", a[0], a[1], a[2], a[3], a[4], a[5]);
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
