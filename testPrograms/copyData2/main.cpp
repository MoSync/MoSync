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

#include <conprint.h>

/* Attempt to copy 0xff from data[1] to dest[0] */
void CopyWithOffset(){
	unsigned char data[2] = { 0x7f, 0xff };
	unsigned char dest[2] = { 0, 0 };

	MAHandle placeholder = maCreatePlaceholder();
	maCreateData(placeholder, 2);
	maWriteData(placeholder, data, 0, 2);

	MAHandle dstPlaceholder = maCreatePlaceholder();
	maCreateData(dstPlaceholder, 1);

	MACopyData params = { dstPlaceholder, 0, placeholder, 1, 1};
	maCopyData(&params);

	maReadData(dstPlaceholder, dest, 0, 1);

	maDestroyObject(placeholder);
	maDestroyObject(dstPlaceholder);

	printf("dest[0] should be 255, is %i.", dest[0]);
}

extern "C" int MAMain() {
	CopyWithOffset();
	return 0;
}
