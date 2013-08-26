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

#include <windows.h>
#include <Winsock2.h>
#include <Ws2bth.h>
#include <BluetoothAPIs.h>
#include <stdio.h>

static bool haveRadio() {
	BLUETOOTH_FIND_RADIO_PARAMS btfrp;
	btfrp.dwSize = sizeof(btfrp);
	HANDLE hRadio;
	HBLUETOOTH_RADIO_FIND hbtrf = BluetoothFindFirstRadio(&btfrp, &hRadio);
	if(hbtrf == NULL) {
		printf("BluetoothFindFirstRadio error %lu\n", GetLastError());
		return false;
	}
	BluetoothFindRadioClose(hbtrf);
	return true;
}

int main() {
	printf("Hello world!\n");
	printf("haveRadio: %i\n", haveRadio());
	printf("End of Line.\n");
}
