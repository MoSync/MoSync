#ifndef PACKAGERS_H
#define PACKAGERS_H

#include "package.h"
#include <string>

// if isBlackberry, then s.dst is ignored, and the files stored in
// the current working directory.
void packageJavaME(const SETTINGS& s, const std::string& runtimePath, bool isBlackberry);

// reads JAD and JAR from cwd, outputs COD file to s.dst.
void packageBlackberry(const SETTINGS& s);

void packageS60v2(const SETTINGS&, const std::string& runtimePath);
void packageS60v3(const SETTINGS&, const std::string& runtimePath);
void packageS60v5(const SETTINGS&, const std::string& runtimePath);
void packageWM(const SETTINGS&, const std::string& runtimePath);
void packageMoblin(const SETTINGS&, const std::string& runtimePath);
void packageAndroid(const SETTINGS&, const std::string& runtimePath);

#endif	//PACKAGERS_H
