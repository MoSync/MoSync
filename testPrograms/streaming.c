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

