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

#ifndef TOOLS_H
#define TOOLS_H

#include <string>
#include <vector>

using namespace std;

/**
 * Injects an icon in a platform specific manner, using the icon injector.
 * Exits on failure.
 */
void injectIcon(const char* platform, const char* size, const char* srcIcon, const char* dst, bool silent, bool force);

void getExtensions(const char* extensionList, vector<string>& extensions);

/**
 * Returns a MoSync binary, located in the MOSYNCDIR/bin directory.
 */
std::string getBinary(const char* binaryName);

#endif	//TOOLS_H
