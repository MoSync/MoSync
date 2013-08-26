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

#ifndef DLL_H
#define DLL_H

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

class Dll {
public:
	// sets up any needed variables.
	Dll();
	// calls close().
	~Dll();
	// returns false on fail.
	bool open(const char* fileName);
	// returns NULL on fail.
	void* get(const char* functionName);
	// cannot fail.
	void close();
private:
#ifdef WIN32
	HMODULE mHandle;
#else
	void* mHandle;
#endif
};

#endif	//DLL_H
