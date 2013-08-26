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

#define FREEZE for(;;) { maWait(WAIT_EVENT); }

int printfln(const char *fmt, ...)
{
	va_list args;
	char buf[2048];
	int len;
	
	buf[0] = 0;

	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);

	PrintConsole(buf);

	len = strlen(buf);
	maWriteLog(buf, len);
	return len;
}

int MAMain() {
	int ndev=0, nserv=0;

	InitConsole();

	PrintConsole("Starting...");
	maBtStartDiscovery();
	PrintConsole("Started");

	do {
		MABtService s;
		MABtDevice d;
		char namebuf[1024];
		int res;

		maWait(WAIT_BT);

		d.name = namebuf;
		d.nameBufSize = sizeof(namebuf);

		do {
			res = maBtGetNewDevice(&d);
			if(res) {
				printfln("d%i: %i %s", ndev++, res, d.name);
				if(res < 0) {
					FREEZE;
				}
				printfln("%02x%02x%02x%02x%02x%02x", d.address[0], d.address[1],
					d.address[2], d.address[3], d.address[4], d.address[5]);
			}
		} while(res > 0);

		do {
			res = maBtGetNewService(&s);
			if(res) {
				printfln("s%i: %i %02x%02x%02x%02x%02x%02x %i %i", nserv++, res, d.address[0], d.address[1],
					d.address[2], d.address[3], d.address[4], d.address[5], s.type, s.port);
				if(res < -1) {
					FREEZE;
				}
			}
		} while(res > 0);
	} while(maBtDiscoveryState() == 0);
	printfln("Done %i", maBtDiscoveryState());
	FREEZE;
}
