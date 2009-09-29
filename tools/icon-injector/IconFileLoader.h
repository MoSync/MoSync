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

#pragma once
#include <windows.h>
#include <string>

#include <fstream>
#include <iostream>

#include <vector>

#include "IconFileStructs.h"

using namespace std;

typedef struct {
	ICONDIR iconDir;
	unsigned char *data;
} ICON;

class IconFileLoader
{
public:
	IconFileLoader(const char *filename);
	~IconFileLoader(void);	

	int getNumIcons();
	
	HICON getIcon(int index);

	ICON getIconData(int index);

private:
	vector<ICONDIRENTRY> icons;
	HANDLE iconFile;

	int numIcons;

	byte* createIconFromFile(ICONDIRENTRY icon);
};
