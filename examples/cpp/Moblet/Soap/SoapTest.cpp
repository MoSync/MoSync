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

#include <conprint.h>
#include <MAUtil/Moblet.h>
#include <mavsprintf.h>

#include "Soap.h"

using namespace MAUtil;

// This request will return a the result of a multiplication.

static const char sUrl[] = "http://modev.mine.nu:12346/";
static const char sData[] =
"<ns:mul>\n"
	"<param-7>6.0</param-7>\n"
	"<param-8>7.0</param-8>\n"
"</ns:mul>\n"
;


class MyMoblet : public Moblet, SoapListener {
private:
	SoapRequest mSoap;
public:
	MyMoblet() {
		mSoap.start(sUrl, sData, sizeof(sData) - 1, this);
	}

	void soapError(int code) {
		// an error code -6 (CONNERR_CLOSED) is normal after soapEnd(),
		// as the server closes the HTTP connection.
		if(code == CONNERR_CLOSED) {
			printf("Connection closed by server.\n");
			return;
		}
		printf("soapError(%i)\n", code);
	}
	void soapEnd() {
		printf("soapEnd()\n");
	}

	void mtxEncoding(const char* name) {
		printf("encoding '%s'\n", name);
	}
	void mtxTagStart(const char* name, int len) {
		printf("start '%s'\n", name);
	}
	void mtxTagAttr(const char* attrName, const char* attrValue) {
		printf("attr %s='%s'\n", attrName, attrValue);
	}
	void mtxTagStartEnd() {
		//printf("startEnd\n");
	}
	void mtxTagData(const char* data, int len) {
		printf("data '%s'\n", data);
	}
	void mtxTagEnd(const char* name, int len) {
		printf("end '%s'\n", name);
	}
	void mtxParseError(int offset) {
		printf("parseError %i\n", offset);
	}
	void mtxEmptyTagEnd() {
		printf("emptyTagEnd\n");
	}
	unsigned char mtxUnicodeCharacter(int unicode) {
		char c = XmlListener::mtxUnicodeCharacter(unicode);
		if(c == 0) {	//we don't know how to convert this character
			c = '?';
			lprintfln("Unknown unicode character: 0x%x", unicode);
		}
		return c;
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		switch(keyCode) {
		case MAK_SOFTRIGHT:
		case MAK_0:
		case MAK_BACK:
			maExit(0);
		}
	}

	void keyReleaseEvent(int keyCode, int nativeCode) {
	}
};

extern "C" int MAMain() {
	InitConsole();
	Moblet::run(new MyMoblet());
	return 0;
}
