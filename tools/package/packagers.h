/* Copyright (C) 2010 MoSync AB

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

#ifndef PACKAGERS_H
#define PACKAGERS_H

#include "package.h"
#include <string>

struct RuntimeInfo {
	std::string path;
	std::string iconSize;
	bool isBlackberry;
	bool isCldc10;
	int androidVersion;
	bool hasLimitedResourceSize;
	int blackberryVersion, blackberryMinor;
};

// if isBlackberry, then s.dst is ignored, and the files stored in
// the current working directory.
void packageJavaME(const SETTINGS&, const RuntimeInfo&);

// reads JAD and JAR from cwd, outputs COD file to s.dst.
void packageBlackberry(const SETTINGS&, const RuntimeInfo&);

void packageS60v2(const SETTINGS&, const RuntimeInfo&);
void packageS60v3(const SETTINGS&, const RuntimeInfo&);
void packageS60v5(const SETTINGS&, const RuntimeInfo&);
void packageWM(const SETTINGS&, const RuntimeInfo&);
void packageMoblin(const SETTINGS&, const RuntimeInfo&);
void packageAndroid(const SETTINGS&, const RuntimeInfo&);
void packageIOS(const SETTINGS&, const RuntimeInfo&);

#endif	//PACKAGERS_H
