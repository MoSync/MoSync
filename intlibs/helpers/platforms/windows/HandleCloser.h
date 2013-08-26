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

#ifndef HANDLECLOSER_H
#define HANDLECLOSER_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class HandleCloser {
public:
	HandleCloser() : mH(0) {}
	HandleCloser(HANDLE h) : mH(h) {}
	void close() {
		if(mH != 0) {
			CloseHandle(mH);
			mH = 0;
		}
	}
	operator int() {
		return (int)mH;
	}
	operator HANDLE() {
		return mH;
	}
	HandleCloser& operator=(HANDLE h) {
		close();
		mH = h;
		return *this;
	}
	HANDLE* p() {
		return &mH;
	}
	~HandleCloser() {
		close();
	}
private:
	HANDLE mH;
};

#endif	//HANDLECLOSER_H
