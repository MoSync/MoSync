#ifndef PACKAGERS_H
#define PACKAGERS_H

#include "package.h"
#include <string>

struct RuntimeInfo {
	std::string path;
	std::string iconSize;
};

// if isBlackberry, then s.dst is ignored, and the files stored in
// the current working directory.
void packageJavaME(const SETTINGS&, const RuntimeInfo&, bool isBlackberry);

// reads JAD and JAR from cwd, outputs COD file to s.dst.
void packageBlackberry(const SETTINGS&, const RuntimeInfo&);

void packageS60v2(const SETTINGS&, const RuntimeInfo&);
void packageS60v3(const SETTINGS&, const RuntimeInfo&);
void packageS60v5(const SETTINGS&, const RuntimeInfo&);
void packageWM(const SETTINGS&, const RuntimeInfo&);
void packageMoblin(const SETTINGS&, const RuntimeInfo&);
void packageAndroid(const SETTINGS&, const RuntimeInfo&);

#endif	//PACKAGERS_H
