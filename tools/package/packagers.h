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
void packageWindowsPhone(const SETTINGS&, const RuntimeInfo&);

#endif	//PACKAGERS_H
