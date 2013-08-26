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
