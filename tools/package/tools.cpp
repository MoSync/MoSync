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

#include "util.h"
#include "tools.h"
#include "package.h"
#include <string>
#include <cstring>
#include <sstream>
#include <ostream>

using namespace std;

void injectIcon(const char* platform, const char* size, const char* srcIcon, const char* dst, bool silent, bool force) {
	ostringstream iconInjectCmd;

	// TODO: That "-lenient yes" switch must be removed...
	string forceArg = force ? " -force" : "";
	iconInjectCmd << getBinary("icon-injector") << forceArg << " -lenient yes -platform \"" << platform << "\" -src " <<
		file(srcIcon) << " -size " << size << " -dst " << file(dst);

	sh(iconInjectCmd.str().c_str(), silent);
}

void getExtensions(const char* extensionList, vector<string>& extensions) {
	if (extensionList && strlen(extensionList) > 0) {
		split(extensions, extensionList, ",");
	}
}

string getBinary(const char* binaryName) {
	return file(mosyncdir() + string("/bin/") + binaryName);
}
