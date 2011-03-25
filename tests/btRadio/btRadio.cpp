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
