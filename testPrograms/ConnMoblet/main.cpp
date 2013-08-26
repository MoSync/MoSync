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

#define BUFSIZE 1024
#define MIN(a, b) ((a) < (b) ? (a) : (b))

//first iteration
class ConnMoblet : public Moblet, public ConnectionListener {
public:
	ConnMoblet() {
		printf("Hello World!\n");

		Handle conn = maConnect("socket://130.237.3.118:6666");
		setConnectionListener(conn, this);
	}

	void connEvent(const CONN_EVENT_DATA& data) {
		printf("cE %i %i %i\n", data.handle, data.opType, data.result);
		if(data.opType == CONNOP_CONNECT) {
			if(data.result > 0) {
				maConnRead(data.handle, buffer, BUFSIZE);
			}
		} else if(data.opType == CONNOP_READ) {
			if(data.result > 0) {
				buffer[MIN(80, data.result)] = 0;	//hack
				printf("%s", buffer);
				maConnRead(data.handle, buffer, BUFSIZE);
			}
		} else {
			printf("Unhandled!\n");
		}
	}

	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_0)
			maExit(0);
	}

private:
	char buffer[BUFSIZE];
};

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;

	Moblet::run(new ConnMoblet());
}
