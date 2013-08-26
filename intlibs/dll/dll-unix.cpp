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

#include "dll.h"
#include <dlfcn.h>

#ifndef NULL
#define NULL 0
#endif

Dll::Dll() {
	mHandle = NULL;
}

Dll::~Dll() {
	close();
}

bool Dll::open(const char* fileName) {
	mHandle = dlopen(fileName, RTLD_LOCAL | RTLD_NOW);
	return mHandle != NULL;
}

void* Dll::get(const char* functionName) {
	return dlsym(mHandle, functionName);
}

void Dll::close() {
	if(mHandle != NULL) {
		dlclose(mHandle);
		mHandle = NULL;
	}
}
