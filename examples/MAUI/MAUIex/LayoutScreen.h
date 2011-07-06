/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

#ifndef _LAYOUTSCREEN_H_
#define _LAYOUTSCREEN_H_

#include <MAUI/Screen.h>
#include <MAUI/Layout.h>

using namespace MAUI;

class LayoutScreen : public Screen {
public:
	LayoutScreen(Screen *previous);
	~LayoutScreen();
	void keyPressEvent(int keyCode, int nativeCode);
	void pointerPressEvent(MAPoint2d point);
	void pointerReleaseEvent(MAPoint2d point);

private:
	Screen *previous;
	Layout *mainLayout;
	Widget *softKeys;
};

#endif	//_LAYOUTSCREEN_H_
