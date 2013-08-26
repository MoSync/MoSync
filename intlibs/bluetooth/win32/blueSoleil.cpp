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

#include "../config_bluetooth.h"
#ifdef BLUESOLEIL_SUPPORTED

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <BlueSoleil/Btsdk_ui.h>
#include <helpers/CriticalSection.h>

#include "../discInternal.h"
#include "blueSoleil.h"


//BlueSoleilTest
#define BST(func) { int res = (func); if(res != BTSDK_OK) {\
	LOG("%s:%i %i\n", __FILE__, __LINE__, res);\
	setDiscoveryState(CONNERR_GENERIC); gBt.deviceCallback(); return; } }


static void InquiryResult(BTDEVHDL dev_hdl);
static void InquiryComplete();
static void ConnEvent(BTCONNHDL conn_hdl, BTUINT16 event, BTUINT8 *arg);
static const char* btaddr2string(const MABtAddr& a);


void Soleil::setup() {
	BtSdkCallbackStru cb;

	cb.type = BTSDK_INQUIRY_RESULT_IND;
	cb.func = (void*)InquiryResult;
	Btsdk_RegisterCallback4ThirdParty(&cb);

	cb.type = BTSDK_INQUIRY_COMPLETE_IND;
	cb.func = (void*)InquiryComplete;
	Btsdk_RegisterCallback4ThirdParty(&cb);

	cb.type = BTSDK_CONNECTION_EVENT_IND;
	cb.func = (void*)ConnEvent;
	Btsdk_RegisterCallback4ThirdParty(&cb);
}

void Soleil::startDeviceDiscovery() {
	BST(Btsdk_StartDeviceDiscovery(0, 0, 0));
}

static void InquiryResult(BTDEVHDL dev_hdl) {
	BtDevice dev;

	BTUINT32 dev_class;
	BST(Btsdk_GetRemoteDeviceClass(dev_hdl, &dev_class));

	BtSdkRemoteDevicePropertyStru props;
	BST(Btsdk_GetRemoteDeviceProperty(dev_hdl, &props));

	byte nameBuf[256];
	WORD len = sizeof(nameBuf);
	BST(Btsdk_GetRemoteDeviceName(dev_hdl, nameBuf, &len));
	dev.name = (char*)nameBuf;

	BST(Btsdk_GetRemoteDeviceAddress(dev_hdl, dev.address.a));

	{
		CriticalSectionHandler csh(&gBt.critSec);
		LOGBT("d%i: \"%s\" (%s)\n", gBt.devices.size(),
			dev.name.c_str(), btaddr2string(dev.addr));
		gBt.devices.push_back(dev);
	}
	gBt.deviceCallback();
}

static void InquiryComplete() {
	{
		CriticalSectionHandler csh(&gBt.critSec);
		LOGBT("Discovery done\n");
		setDiscoveryState(gBt.devices.size() + 1);
	}
	gBt.deviceCallback();
}

static const char* btaddr2string(const MABtAddr& a) {
	static char buffer[16];
	sprintf(buffer, "%02x%02x%02x%02x%02x%02x", a.a[0], a.a[1], a.a[2], a.a[3], a.a[4], a.a[5]);
	return buffer;
}

static void ConnEvent(BTCONNHDL conn_hdl, BTUINT16 event, BTUINT8 *arg) {
	LOG("ConnEvent\n");
}


#endif	//BLUESOLEIL_SUPPORTED
