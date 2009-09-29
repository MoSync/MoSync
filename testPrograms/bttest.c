/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
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
