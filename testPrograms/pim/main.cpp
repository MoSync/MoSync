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
#include <mawstring.h>

#define DUMP_ITEMS 1
#define USE_FILE 0
#define ADD_REMOVE_ITEMS 1

#if 1
#define LOG printf
#define CONSOLE_LOGGING 1
#else
#define LOG(...) lprintf(__VA_ARGS__)
#define CONSOLE_LOGGING 0

int lprintf(const char* fmt, ...) {
	va_list args;
	char buf[2048];
	int len;
	static int lastWLres = 0;
	
	if(lastWLres < 0)
		return lastWLres;

	buf[0] = 0;

	va_start(args, fmt);
	len = vsprintf(buf, fmt, args);
	if(len > (int)sizeof(buf) - 2) {
		maPanic(0, "lprintfln buffer overrun!");
	}
	va_end(args);

	lastWLres = maWriteLog(buf, len);
	return len;
}
#endif

using namespace MAUtil;

#define FAIL do{ printf("%i @ %s:%i\n", res, __FILE__, __LINE__); FREEZE; }while(0)
#define TEST(func) do{ int res = (func); if(res < 0) { FAIL; } }while(0)

#if DUMP_ITEMS
static void dumpItem(MAHandle list, MAHandle item);
#endif

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
	printf("Contact list opened.\n");
	while(1) {
		MAHandle item = maPimListNext(list);
		TEST(item);
		if(item == 0)
			break;
#if DUMP_ITEMS
		dumpItem(list, item);
#endif
		TEST(maPimItemClose(item));
		checkEvents();
	}
	TEST(maPimListClose(list));
	printf("Done.\n");
}

#if DUMP_ITEMS
const char* sTypes[] = {
	"binary",
	"boolean",
	"date",
	"int",
	"string",
	"array",
};

static void dumpItem(MAHandle list, MAHandle item) {
	LOG("Contact %i:\n", item);
	TEST(item);
	MA_PIM_ARGS args;
	args.item = item;
	char buf[512];
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
		LOG("Field %i:%i", field, type);
		MAASSERT(type > 0 && type <= MA_PIM_TYPE_STRING_ARRAY);
		LOG("(%s)\n", sTypes[type]);
		int nValues = maPimItemFieldCount(item, field);
		TEST(nValues);
		for(int k=0; k<nValues; k++) {
			int attr = maPimItemGetAttributes(item, field, k);
			TEST(attr);
			buf[0] = 0;
			int len = maPimItemGetValue(&args, k);
			TEST(len);
			LOG("0x%x %i(", attr, len);
			switch(type) {
			case MA_PIM_TYPE_BINARY:
				break;
			case MA_PIM_TYPE_STRING:
				LOG("%S", (wchar*)buf);
				break;
			case MA_PIM_TYPE_BOOLEAN:
			case MA_PIM_TYPE_INT:
				LOG("%i", *(int*)buf);
				break;
			case MA_PIM_TYPE_DATE:
				LOG("%i (date printing unsupported)", *(int*)buf);
				break;
			case MA_PIM_TYPE_STRING_ARRAY:
				{
					int num = *(int*)buf;
					const wchar* ptr = (wchar*)(buf + 4);
					LOG("%i", num);
#if 0
					LOG("\n");
					const wchar* t = ptr;
					for(int i=0; i<num; ) {
						if(*t == 0)
							i++;
						LOG(",%i", *t);
						t++;
					}
					LOG("\n");
#endif
					for(int i=0; i<num; i++) {
						LOG(",%S", ptr);
						ptr += wcslen(ptr) + 1;
					}
				}
				break;
			}
			LOG(")\n");
		}
	}
}
#endif

#if ADD_REMOVE_ITEMS

// return the number of bytes written
template<int NUM>
int writeStringArray(char* buf, const wchar_t* srca[NUM]) {
	*(int*)buf = NUM;
	wchar_t* dst = (wchar_t*)(buf + 4);
	for(int i=0; i<NUM; i++) {
		const wchar_t* src = srca[i];
		while(true) {
			*dst = *src;
			dst++;
			if(*src == 0)
				break;
			src++;
		}
	}
	return (char*)dst - buf;
}

// add some dummy contacts
// identify by single-value NOTE "tp_pim_dummy"
static const wchar_t sDummyNote[] = L"tp_pim_dummy";
static const wchar_t sDummyTelA[] = L"0123456789";
static const wchar_t sDummyTelB[] = L"0987654321";
static const wchar_t* sDummyName[] = { L"Smith", L"Agent", L"", L"", L"" };
static const wchar_t sDummyEmail[] = L"foo@example.com";

static void addContacts() {
	char buf[512];
	
	printf("Adding dummy contact...\n");
	// open & create
	MAHandle list = maPimListOpen(MA_PIM_CONTACTS);
	TEST(list);
	MA_PIM_ARGS args;
	TEST(args.item = maPimItemCreate(list));
	
	// add values
	args.field = MA_PIM_FIELD_CONTACT_NOTE;
	args.buf = (void*)sDummyNote;
	args.bufSize = sizeof(sDummyNote);
	TEST(maPimItemAddValue(&args, 0));
	
	args.field = MA_PIM_FIELD_CONTACT_TEL;
	args.buf = (void*)sDummyTelA;
	args.bufSize = sizeof(sDummyTelA);
	TEST(maPimItemAddValue(&args, MA_PIM_ATTR_HOME));
	
	args.field = MA_PIM_FIELD_CONTACT_NAME;
	args.buf = buf;
	args.bufSize = writeStringArray<5>(buf, sDummyName);
	TEST(maPimItemAddValue(&args, 0));
	
	// try SetValue
	args.field = MA_PIM_FIELD_CONTACT_TEL;
	args.buf = (void*)sDummyTelB;
	args.bufSize = sizeof(sDummyTelB);
	TEST(maPimItemSetValue(&args, 0, MA_PIM_ATTR_WORK | MA_PIM_ATTR_PREFERRED));
	
	// try RemoveValue
	args.field = MA_PIM_FIELD_CONTACT_EMAIL;
	args.buf = (void*)sDummyEmail;
	args.bufSize = sizeof(sDummyEmail);
	TEST(maPimItemAddValue(&args, 0));
	
	TEST(maPimItemRemoveValue(args.item, args.field, 0));
	
	// check maPimItemCount()
	int count = maPimItemCount(args.item);
	printf("Item count: %i\n", count);
	if(count != 3) {
		printf("BAD ITEM COUNT!\n");
	}
	
	// close it up real good at the top
	TEST(maPimItemClose(args.item));
	TEST(maPimListClose(list));
	printf("Done.\n");
}

// remove the dummy contacts added by addContacts
static void removeContacts() {
	printf("Removing dummy contacts...\n");
	wchar_t buf[128];

	MAHandle list = maPimListOpen(MA_PIM_CONTACTS);
	TEST(list);
	int dummyCount = 0;
	int noteCount = 0;
	while(1) {
		MAHandle item = maPimListNext(list);
		TEST(item);
		if(item == 0)
			break;
		bool isDummy = false;
		int valueCount;
		TEST(valueCount = maPimItemFieldCount(item, MA_PIM_FIELD_CONTACT_NOTE));
		if(valueCount == 1) {
			noteCount++;
			MA_PIM_ARGS args;
			args.item = item;
			args.field = MA_PIM_FIELD_CONTACT_NOTE;
			args.buf = buf;
			args.bufSize = sizeof(buf);
			TEST(maPimItemGetValue(&args, 0));
			isDummy = wcscmp((wchar_t*)args.buf, sDummyNote) == 0;
		}
		if(isDummy) {
			printf("Found dummy item!\n");
			dummyCount++;
			TEST(maPimItemRemove(list, item));
		} else {
			TEST(maPimItemClose(item));
		}
		checkEvents();
	}
	TEST(maPimListClose(list));
	printf("Done. %i dummies, %i notes.\n", dummyCount, noteCount);
}
#endif

class MyMoblet : public Moblet {
public:
	MyMoblet() {
		printf("Hello PIM!\n");
#if ADD_REMOVE_ITEMS
		addContacts();
#endif
		listContacts();
#if ADD_REMOVE_ITEMS
		removeContacts();
#endif
	}

	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_0)
			close();
	}
};

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = CONSOLE_LOGGING;
#if USE_FILE
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
#endif
	Moblet::run(new MyMoblet());
	return 0;
};
