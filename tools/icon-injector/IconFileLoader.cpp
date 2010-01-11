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

#include "iconfileloader.h"
#include "IconFileStructs.h"
#include "ErrorCheck.h"

#include <vector>

IconFileLoader::IconFileLoader(const char *filename)
:	icons()
{
	iconFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	checkResult(iconFile != INVALID_HANDLE_VALUE, "File does not exist");

	ICONDIR iconDirectory;
	DWORD bytesRead = 0;

	

	//Read reserved word
	BOOL result = ReadFile(iconFile, &iconDirectory.idReserved, sizeof(WORD), &bytesRead, NULL);
	checkResult(result, "Unable to read header from file");

	//Read resource type
	result = ReadFile(iconFile, &iconDirectory.idType, sizeof(WORD), &bytesRead, NULL);
	checkResult(result, "Unable to read header from file");

	if (iconDirectory.idType != 1)
		throw string("Not an icon file");

	//Read number of icons in file
	result = ReadFile(iconFile, &iconDirectory.idCount, sizeof(WORD), &bytesRead, NULL);
	checkResult(result, "Unable to read header from file");

	numIcons = iconDirectory.idCount;

	//Read each icon entry	
	for (int i = 0; i < iconDirectory.idCount; i++)
	{
		ICONDIRENTRY icon;

		result = ReadFile(iconFile, &icon, sizeof(ICONDIRENTRY), &bytesRead, NULL);

		checkResult(result, "Unable to read icon directory from file");

		icons.push_back(icon);
	}
}

IconFileLoader::~IconFileLoader(void)
{
	CloseHandle(iconFile);
}

int IconFileLoader::getNumIcons()
{
	return numIcons;
}

HICON IconFileLoader::getIcon(int index)
{
	byte *iconImage = createIconFromFile(icons[index]);

	HICON iconHandle = CreateIconFromResourceEx(iconImage, icons[index].dwBytesInRes, TRUE, 0x00030000, icons[index].bWidth, icons[index].bHeight, LR_DEFAULTCOLOR);	
	checkResult(iconHandle != NULL, "Failed to create icon from data");
	
	return iconHandle;
}


ICON IconFileLoader::getIconData(int index) {
	ICON icon;
	icon.data = IconFileLoader::createIconFromFile(icons[index]);
	icon.iconDir.idCount = 1;
	icon.iconDir.idReserved = 0;
	icon.iconDir.idType = 1;
	icon.iconDir.idEntries[0] = icons[index];
	return icon;
}

byte* IconFileLoader::createIconFromFile(ICONDIRENTRY icon)
{	
	DWORD bytesRead;
	
	byte *iconImage = new byte[icon.dwBytesInRes];

	SetFilePointer(iconFile, icon.dwImageOffset, NULL, FILE_BEGIN);

	BOOL result = ReadFile(iconFile, iconImage, icon.dwBytesInRes, &bytesRead, NULL);
	checkResult(result, "Unable to load icon data");

//	HICON iconHandle = CreateIconFromResourceEx(iconImage, icon.dwBytesInRes, TRUE, 0x00030000, icon.bWidth, icon.bHeight, LR_DEFAULTCOLOR);	
//	checkResult(iconHandle != NULL, "Failed to create icon from data");

	return iconImage;
}
