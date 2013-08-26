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

#include "Shared.h"
#include "Icon.h"
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <stdlib.h>
#include <stdio.h>

using namespace std;

namespace MoSync {

void errorExit(const string& reason) {
	printf("%s\n", reason.c_str());
	exit(1);
}

string getExtension(const string& str) {
	size_t i = str.find_last_of('.');
	if(i == string::npos)
		return "";
	else
		return str.substr(i + 1);
}

string getAbsolutePath(const string& path) {
	size_t i = path.length()-1;
	while(path[i]!='/'&&path[i]!='\\'&&i!=0) i--;
	return path.substr(0, i);
}

void sizeString(const std::string& size, int* w, int* h) {
	size_t ofs;
	if((ofs=size.find("x"))!=std::string::npos) {
		*w = atoi(size.substr(0, ofs).c_str());
		*h = atoi(size.substr(ofs+1, size.length()-(ofs+1)).c_str());
	} else {
		errorExit("Invalid size string.");
	}
}

int run(const char* cmd) {
	printf("%s\n", cmd);
#ifdef WIN32
#define GLE(e, func, args) { DWORD res = (func args); if(res == e) { int err = GetLastError();\
	printf(#func " error %i\n", err); return err; } }

	// hopefully this method should be more stable than the system() function.
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	GLE(0, CreateProcess, (NULL, (char*)cmd, NULL, NULL, false, 0, NULL, NULL, &si, &pi));
	GLE(0, CloseHandle, (pi.hThread));
	GLE(WAIT_FAILED, WaitForSingleObject, (pi.hProcess, INFINITE));
	DWORD exitCode;
	GLE(0, GetExitCodeProcess, (pi.hProcess, &exitCode));
	if(exitCode != 0) {
		printf("exitCode: %lu\n", exitCode);
	}
	return exitCode;
#else
	int res = system(cmd);
	if(res != 0) {
		printf("Error %i\n", res);
	}
	return res;
#endif
}

}
