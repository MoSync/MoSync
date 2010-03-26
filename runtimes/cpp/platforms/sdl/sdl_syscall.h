/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#define FE_ADD_EVENT (SDL_USEREVENT + 1)
#define FE_TIMER (SDL_USEREVENT + 2)
#define FE_DEFLUX_BINARY (SDL_USEREVENT + 3)
#define FE_MA_NETWORK_MESSAGE (SDL_USEREVENT + 4)
#define FE_INTERRUPT (SDL_USEREVENT + 5)

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

