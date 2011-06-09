#include "dll.h"

Dll::Dll() {
	mHandle = NULL;
}

Dll::~Dll() {
	close();
}

bool Dll::open(const char* fileName) {
	mHandle = LoadLibrary(fileName);
	return mHandle != NULL;
}

void* Dll::get(const char* functionName) {
	return (void*)GetProcAddress(mHandle, functionName);
}

void Dll::close() {
	if(mHandle != NULL) {
		FreeLibrary(mHandle);
		mHandle = NULL;
	}
}
