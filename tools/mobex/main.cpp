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

#include <stdio.h>
#include <conio.h>

//hack
#define CONFIG_H
#define LOGGING_ENABLED

#include <helpers/array.h>
#include "obex.h"

const char* FileNameFromPath(const char* path) {
	const char* ptr = strrchr(path, '\\');
	return ptr ? (ptr + 1) : path;
}

//returns >0 on success
static int sendFile(const char* filename, const MABtAddr* address, int port, int maxPacketSize);
static int str2ba(const char* str, MABtAddr* bda);

#ifdef _DEBUG
#define DEBUG_GETCH _getch()
#else	//release
#define DEBUG_GETCH
#endif

int dummy = __COUNTER__;

int main(int argc, char** argv) {
	if(argc < 4 || argc > 5) {
		const char* exename = FileNameFromPath(argv[0]);
		printf(
			"Usage: %s <file> <address> <port> [maxPacketSize]\n"
			"\n"
			"Sends <file> to Bluetooth <address>:<port> using OBEX PUT\n"
			"with a maximum packet size decided by the target device,\n"
			"optionally limited by [maxPacketSize].\n"
			"\n"
			"Example: %s MoSync.jar 010203040506 9\n",
			exename, exename);
		DEBUG_GETCH;
		return -__COUNTER__;
	}

	MABtAddr address;
	str2ba(argv[2], &address);

	int port = atoi(argv[3]);
	if(port < 1 || port > 30) {
		printf("Bad port number: %i\n", port);
		return -__COUNTER__;
	}

	int maxPacketSize = 0xffff;
	if(argc == 5) {
		maxPacketSize = atoi(argv[4]);
		if(maxPacketSize < 1 || maxPacketSize > 0xffff) {
			printf("Bad maxPacketSize: %i\n", maxPacketSize);
			return -__COUNTER__;
		}
	}

	Bluetooth::MABtInit();
	int res = sendFile(argv[1], &address, port, maxPacketSize);
	Bluetooth::MABtClose();

	if(res != 1) {
		printf("Result: %i\n", res);
		return res;
	} else {
		printf("File sent successfully.\n");
		return 0;
	}
	DEBUG_GETCH;
}

static int sendFile(const char* path, const MABtAddr* address, int port, int maxPacketSize) {
	FILE* file = fopen(path, "rb");
	if(!file) {
		printf("Couldn't open file \"%s\"!\n", path);
		return -__COUNTER__;
	}
	const char* filename = MAX(MAX(path, strrchr(path, '\\')), strrchr(path, '/'));

	Array<u16> unicode_buf(strlen(filename));
	//Big Endian Unicode
	for(uint i=0; i<unicode_buf.size(); i++) {
		unicode_buf[i] = filename[i] << 8;
	}

	if(fseek(file, 0, SEEK_END)) {
		printf("Couldn't scan file \"%s\"!\n", filename);
		return -__COUNTER__;
	}
	int file_len = ftell(file);
	if(fseek(file, 0, SEEK_SET)) {
		printf("Couldn't scan file \"%s\"!\n", filename);
		return -__COUNTER__;
	}

	Array<char> file_buf(file_len);
	int res = fread(file_buf, 1, file_len, file);
	if(res != file_len) {
		DUMPINT(res);
		printf("Couldn't read file \"%s\"!\n", filename);
		return -__COUNTER__;
	}

	return sendObject(*address, file_buf, unicode_buf, port, maxPacketSize);
}

int str2ba(const char* str, MABtAddr *bta) {
	int ai[6];	//address ints
	if(6 != sscanf_s(str, "%2x%2x%2x%2x%2x%2x", &ai[0], &ai[1], &ai[2], &ai[3], &ai[4], &ai[5])) {
		return CONNERR_URL;
	}
	for(int i=0; i<BTADDR_LEN; i++) {
		bta->a[i] = (byte)ai[i];
	}
	return 1;
}

void MoSyncExit(int code) {
	exit(code);
}

void MoSyncErrorExit(int code) {
	LOG("MoSync Panic %i\n", code);
	MoSyncExit(code);
}
