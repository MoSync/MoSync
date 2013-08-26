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
#include <conprint.h>

int MAMain() {
	Handle sock;
	char buffer[1024];

	InitConsole();
	printf("Hello World");
	maConnect("socket://dev.mopix.se:8080", NULL);
	printf("Connecting...");
	maWait(WAIT_CONN, 0);
	sock = maConnState();
	printf("Connected");
	maConnRead(sock, buffer, sizeof(buffer));
	printf("Reading...");
	maWait(WAIT_CONN, 2000);
	printf("Read %i\n", maConnState());
	printf("End\n");
	return 0;
}
