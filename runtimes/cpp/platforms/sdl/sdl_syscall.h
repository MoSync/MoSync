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

#define FE_ADD_EVENT (SDL_USEREVENT + 1)
#define FE_TIMER (SDL_USEREVENT + 2)
#define FE_DEFLUX_BINARY (SDL_USEREVENT + 3)
#define FE_MA_NETWORK_MESSAGE (SDL_USEREVENT + 4)
#define FE_INTERRUPT (SDL_USEREVENT + 5)
#define FE_CAMERA_VIEWFINDER_UPDATE (SDL_USEREVENT + 6)

namespace Base {
	class Syscall;
	extern Syscall* gSyscall;
	extern bool gReload;

#if defined(_MSC_VER) || defined(__SYMBIAN32__)
void __declspec(noreturn) reloadProgram();
#elif defined(__GNUC__)
void __attribute((noreturn)) reloadProgram();
#else
#error Unsupported platform!
#endif

	void reportCallStack();
	int maDumpCallStackEx(const char*, int);
	int getRuntimeIp();
	bool MAProcessEvents();
}
using namespace Base;

//dupes from network.h
void MANetworkInit();
void MANetworkReset();
void MANetworkClose();

#define NUMBER_KEYS(m) m(0) m(1) m(2) m(3) m(4)	m(5) m(6) m(7) m(8) m(9)
#define DIRECT_KEYS(m) m(LEFT) m(RIGHT) m(UP) m(DOWN) NUMBER_KEYS(m)
#define MULTI_KEYS(mac) mac(FIRE, RCTRL) mac(FIRE, LCTRL)\
	mac(STAR, KP_MULTIPLY) mac(POUND, KP_DIVIDE) mac(CLEAR, DELETE) mac(CLEAR, BACKSPACE)\
	mac(SOFTLEFT, LSHIFT) mac(SOFTRIGHT, RSHIFT)\
	mac(0, SPACE) mac(STAR, v) mac(POUND, COMMA)\
//	mac(1, t) mac(2, y) mac(3, u)
//	mac(4, g) mac(5, h) mac(6, j)
//	mac(7, b) mac(8, n) mac(9, m)

