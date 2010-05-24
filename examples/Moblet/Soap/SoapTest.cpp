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

#include <conprint.h>
#include <MAUtil/Moblet.h>
#include <mavsprintf.h>

#include "Soap.h"

using namespace MAUtil;

// This is the WebServiceX Currency Convertor, as documented here:
// http://www.webservicex.net/CurrencyConvertor.asmx?op=ConversionRate

// This request will return the current exchange rate between
// the Pound Sterling and the Euro.

static const char sUrl[] = "http://www.webservicex.net/CurrencyConvertor.asmx";
static const char sData[] =
"<ConversionRate xmlns=\"http://www.webserviceX.NET/\">"
	"<FromCurrency>GBP</FromCurrency>"
	"<ToCurrency>EUR</ToCurrency>"
"</ConversionRate>"
;


class MyMoblet : public Moblet, SoapListener {
private:
	SoapRequest mSoap;
public:
	MyMoblet() {
		mSoap.start(sUrl, sData, sizeof(sData) - 1, this);
	}

	void soapError(int code) {
		printf("soapError(%i)\n", code);
		// an error code -6 (CONNERR_CLOSED) is normal after soapEnd(),
		// as the server closes the HTTP connection.
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
	void mtxParseError() {
		printf("parseError\n");
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
