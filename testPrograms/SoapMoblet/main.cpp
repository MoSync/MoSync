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
#include <MAUtil/SOAP.h>

using namespace MAUtil;

static const char sVSR[] =
"<GetVirtualSignWithVirtualSignID xmlns=\"http://www.astando.se/vSign-0.1/\">"
"<id>0016b866dfc3</id>"
"</GetVirtualSignWithVirtualSignID>";

void error(SOAP_CONTEXT* scp, int code) {
	printf("error %i\n", code);
}
void responseCode(SOAP_CONTEXT* scp, int code) {
	printf("Code %i\n", code);
}
void responseData(SOAP_CONTEXT* scp, char* data, int length) {
	int pos = 0;
	printf("Data %i\n", length);
#define LINE_LENGTH 40
	while(pos + LINE_LENGTH < length) {
		char* ptr = data + pos;
		char temp = ptr[LINE_LENGTH];
		ptr[LINE_LENGTH] = 0;
		printf("%s", ptr);
		ptr[LINE_LENGTH] = temp;
		pos += LINE_LENGTH;
	}
	printf("%s", data + pos);
}
void responseFinished(SOAP_CONTEXT* scp) {
	printf("Finished\n");
}

class SoapMoblet : public Moblet, public ConnectionListener {
public:
	SoapMoblet() {
		printf("Hello World!\n");

		SOAP_CALLBACKS cb;
		cb.error = error;
		cb.responseCode = responseCode;
		cb.responseData = responseData;
		cb.responseFinished = responseFinished;
		mScp = soapStartRequest("http://link.astando.se/VSignWS/VirtualSign.asmx",
			sVSR, sizeof(sVSR) - 1, &cb);

		if(mScp != NULL) {
			setConnectionListener(mScp->conn, this);
			mScp->userData = this;
			printf("Started\n");
		}
	}

	void connEvent(const CONN_EVENT_DATA& data) {
		printf("cE %i %i %i\n", data.handle, data.opType, data.result);
		soapHandleEvent(mScp, &data);
	}

	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_0)
			maExit(0);
	}

private:
	SOAP_CONTEXT* mScp;
};

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;

	Moblet::run(new SoapMoblet());
}
