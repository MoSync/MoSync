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

#include "BluetoothConnection.h"
#include <maassert.h>
#include <mavsprintf.h>
#include <mastdlib.h>
#include <mastring.h>

using namespace MAUtil;

static byte hexsubstr2byte(const char* str, int start);
static int hexsubstr2int(const char* str, int start);
static int hex2int(const char* hex);

// mPort: <0 when closed, ==0 when discovering, >0 when connect-ing/-ed.

BluetoothConnection::BluetoothConnection(ConnectionListener* listener)
: Connection(listener), mPort(-1) {}

BluetoothConnection::~BluetoothConnection() {
	this->close();
}

int BluetoothConnection::connect(
	const MABtAddr& address, const MAUUID& uuid) {
	ASSERT_MSG(mPort < 0, "Connection already active");
	mAddress = address;
	mPort = 0;
	int res = mBluetoothDiscoverer.startServiceDiscovery(address, uuid, this);
	if(res == IOCTL_UNAVAILABLE) {
		// No service discovery. Either we're on Android,
		// or this device doesn't have bluetooth at all.
		// Attempt to use the Android maConnect() extension.
		char buf[64];
		char* ptr = buf;
		const byte* a = address.a;
		ptr += sprintf(ptr, "btspp://%02x%02x%02x%02x%02x%02x:",
			a[0], a[1], a[2], a[3], a[4], a[5]);
		for(int i=0; i<4; i++) {
			ptr += sprintf(ptr, "%08x", uuid.i[i]);
		}
		return Connection::connect(buf);
	}
	return res;
}

/**
 * Connect to a Bluetooth URL that specifies a device address and a UUID.
 * Example URL: "btspp://38E7D8C7605E:DCBDB690C17111DF851A0800200C9A66"
 */
int BluetoothConnection::connect(const char* url) {
	MABtAddr address;
	MAUUID uuid;

	// Check that the url is a valid.
	if (53 != (int)strlen(url) ||
		strcmp(url, strstr(url, "btspp://")) != 0 ||
		':' != url[20])
	{
		return CONNERR_URL;
	}

	// Parse Bluetooth address values from url.
	address.a[0] = hexsubstr2byte(url, 8);
	address.a[1] = hexsubstr2byte(url, 10);
	address.a[2] = hexsubstr2byte(url, 12);
	address.a[3] = hexsubstr2byte(url, 14);
	address.a[4] = hexsubstr2byte(url, 16);
	address.a[5] = hexsubstr2byte(url, 18);

	// Parse UUID values from url.
	uuid.i[0] = hexsubstr2int(url, 21);
	uuid.i[1] = hexsubstr2int(url, 29);
	uuid.i[2] = hexsubstr2int(url, 37);
	uuid.i[3] = hexsubstr2int(url, 45);

	return connect(address, uuid);
}

void BluetoothConnection::close() {
	mPort = -1;
	mBluetoothDiscoverer.cancel();
	Connection::close();
}

void BluetoothConnection::btNewService(const BtService& serv) {
	if(mPort > 0)
		return;
	MAASSERT(serv.port > 0);
	mPort = serv.port;

	char buf[64];
	const byte* a = mAddress.a;
	sprintf(buf, "btspp://%02x%02x%02x%02x%02x%02x:%i",
		a[0], a[1], a[2], a[3], a[4], a[5], serv.port);
	int res = Connection::connect(buf);
	if(res < 0)
		mListener->connectFinished(this, res);
}

void BluetoothConnection::btServiceDiscoveryFinished(int state) {
	if(mPort <= 0) {
		if(state >= 0)
			state = CONNERR_NOTFOUND;
		mListener->connectFinished(this, state);
	}
}

/**
 * Convert a two character substring of a string that contains a hex byte
 * value to a byte.
 * @param str The string that contains the hex value that we want.
 * @param start The start index of the hex value.
 * @return The byte value of the two hex digits that begin at start.
 */
static byte hexsubstr2byte(const char* str, int start)
{
	char hex[3];
	hex[0] = str[start];
	hex[1] = str[start + 1];
	hex[2] = 0;
	return (byte) hex2int(hex);
}

/**
 * Convert a four character substring of a string that contains a hex byte
 * value to an int.
 * @param str The string that contains the hex value that we want.
 * @param start The start index of the hex value.
 * @return The int value of the four hex digits that begin at start.
 */
static int hexsubstr2int(const char* str, int start)
{
	char hex[9];
	hex[0] = str[start];
	hex[1] = str[start + 1];
	hex[2] = str[start + 2];
	hex[3] = str[start + 3];
	hex[4] = str[start + 4];
	hex[5] = str[start + 5];
	hex[6] = str[start + 6];
	hex[7] = str[start + 7];
	hex[8] = 0;
	return hex2int(hex);
}

static int hex2int(const char* hex)
{
	return (int) strtoul(hex, NULL, 16);
}
