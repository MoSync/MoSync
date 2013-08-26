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

#include <ma.h>
#include <maheap.h>
#include <conprint.h>

int Error(int code);

int MAMain() {
	const char* url = "http://kumquat.spotify.net/~magnus/encoding-test/no-leaf-clover-abr-32.mp3";
	Handle conn;
	int size;

	printf("Streaming test\n");
	printf("%s\n", url);

	printf("Connecting...\n");
	maConnect(url, &size);
	maWait(WAIT_CONN);
	conn = maConnState();
	if(conn <= 0)
		Error(conn);

	printf("Size: %i. Init...\n", size);
	maSoundInitConn(0, conn, 1);
	printf("Starting...\n");
	maSoundSetVolume(0, 50);
	maSoundStart(0);
	printf("Done.\n");

	//Freeze
	for(;;) {
		maWait(WAIT_KEY);
	}
}

int Error(int code) {
	printf("Result %i\n", code);
	//Freeze
	for(;;) {
		maWait(WAIT_KEY);
	}
}

