/* Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
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
