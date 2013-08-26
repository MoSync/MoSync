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
