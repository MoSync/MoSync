#ifndef ANDROID_H
#define ANDROID_H

#include <map>
#include <string>

#include "args.h"

using namespace std;

int buildAndroidNative(Arguments* args);
int generateMakefile(Arguments* args, string& config);
int generateMakefiles(Arguments* params);
string toMakefileFile(string file);
string getNdkRoot(Arguments* params);
string getNdkBuildScript(Arguments* params);
string getNdkBuildCommand(string commandLine);
string getAppPlatform(Arguments* params);
int executeNdkBuild(Arguments* params);
string getTempBuildDir(Arguments* params, string* cfg = NULL);
string getStlportDir(Arguments* params, bool useMakeParam);
#ifdef WIN32
string getCygpath(bool& exists);
#endif

#endif
