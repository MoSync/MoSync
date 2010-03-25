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
