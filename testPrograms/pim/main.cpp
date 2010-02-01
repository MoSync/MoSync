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
#include <maassert.h>
#include <conprint.h>
#include <MAUtil/Moblet.h>
#include <IX_PIM.h>
#include <IX_FILE.h>

using namespace MAUtil;

#define FAIL do{ printf("%i @ %s:%i\n", res, __FILE__, __LINE__); FREEZE; }while(0)
#define TEST(func) do{ int res = (func); if(res < 0) { FAIL; } }while(0)

static void dumpItem(MAHandle list, MAHandle item);

static void checkEvents() {
	MAEvent event;
	while(maGetEvent(&event)) {
		if(event.type == EVENT_TYPE_CLOSE ||
			(event.type == EVENT_TYPE_KEY_PRESSED && event.key == MAK_0))
		{
			maExit(1);
		}
	}
}

static void listContacts() {
	MAHandle list = maPimListOpen(MA_PIM_CONTACTS);
	TEST(list);
	while(1) {
		MAHandle item = maPimListNext(list);
		TEST(item);
		if(item == 0)
			break;
		dumpItem(list, item);
		checkEvents();
	}
	TEST(maPimListClose(list));
	printf("Done.\n");
}

static const char* sTypes[] = {
	"binary",
	"boolean",
	"date",
	"int",
	"string",
	"array",
};

static void dumpItem(MAHandle list, MAHandle item) {
	printf("Contact %i:\n", item);
	TEST(item);
	MA_PIM_ARGS args;
	args.item = item;
	char buf[64];
	args.buf = buf;
	args.bufSize = sizeof(buf);
	int nFields = maPimItemCount(item);
	TEST(nFields);
	for(int j=0; j<nFields; j++) {
		int field = maPimItemGetField(item, j);
		TEST(field);
		args.field = field;
		int type = maPimFieldType(list, field);
		TEST(type);
		//printf("Field %i:%i:%s\n", field, type, sFieldNames[field]);
		printf("Field %i:%i", field, type);
		MAASSERT(type > 0 && type <= MA_PIM_TYPE_STRING_ARRAY);
		printf("(%s)\n", sTypes[type]);
		int nValues = maPimItemFieldCount(item, field);
		TEST(nValues);
		for(int k=0; k<nValues; k++) {
			int attr = maPimItemGetAttributes(item, field, k);
			TEST(attr);
			buf[0] = 0;
			int len = maPimItemGetValue(&args, k);
			TEST(len);
			printf("0x%x %i(", attr, len);
			switch(type) {
			case MA_PIM_TYPE_BINARY:
				break;
			case MA_PIM_TYPE_STRING:
				printf("%s", buf);
				break;
			case MA_PIM_TYPE_BOOLEAN:
			case MA_PIM_TYPE_INT:
				printf("%i", *(int*)buf);
				break;
			case MA_PIM_TYPE_DATE:
				printf("%i (date unsupported)", *(int*)buf);
				break;
			case MA_PIM_TYPE_STRING_ARRAY:
				{
					int num = *(int*)buf;
					const char* ptr = buf + 4;
					printf("%i", num);
					for(int i=0; i<num; i++) {
						printf(",%s", ptr);
						ptr += strlen(ptr) + 1;
					}
				}
				break;
			}
			printf(")\n");
		}
	}
	TEST(maPimItemClose(item));
}

class MyMoblet : public Moblet {
public:
	MyMoblet() {
		printf("Hello World!\n");
		listContacts();
	}

	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_0)
			close();
	}
};

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 0;
	gConsoleFile = maFileOpen("C:/Data/Images/pclog.txt", MA_ACCESS_READ_WRITE);
	MAASSERT(gConsoleFile > 0);
	int res = maFileExists(gConsoleFile);
	MAASSERT(res >= 0);
	if(res) {
		maFileTruncate(gConsoleFile, 0);
	} else {
		res = maFileCreate(gConsoleFile);
		MAASSERT(res == 0);
	}
	Moblet::run(new MyMoblet());
	return 0;
};
