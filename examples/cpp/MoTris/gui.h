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

#ifndef _GUI_H_
#define _GUI_H_

#include <MAUtil/Vector.h>
#include <MAUtil/String.h>
#ifdef MAPIP
#include <maprofile.h>
#endif

#include "event.h"

using namespace MAUtil;

class Menu;
class EditBox;

#define MAX(a, b) (((a)>(b))?(a):(b))
#define MIN(a, b) (((a)<(b))?(a):(b))

enum AlignmentX {
	ALIGN_LEFT_X,
	ALIGN_MID_X,
	ALIGN_RIGHT_X
};

enum AlignmentY {
	ALIGN_TOP_Y,
	ALIGN_MID_Y,
	ALIGN_BOTTOM_Y,
};

class MenuListener {
public:
	virtual void onEvent(int index, Menu *menu) = 0;
};

class Menu {
public:

	Menu(int textColor, int selectedTextColor, int x, int y, int width, int height, AlignmentX alignmentX, int selectable=1);

	void setX(int x);
	void setY(int y);
	void addMenuListener(MenuListener *menuListener);
	void addMenuItem(String menuItem);
	void setMenuItem(int i, String menuItem);
	void clear();
	const String& getCurrentlySelectedMenuItem();
	int getCurrentlySelectedMenuIndex();
	void show();
	void update();

#ifdef MA_PROF_SUPPORT_STYLUS
	int handlePointerPress(MAPoint2d point);
#endif	// MA_PROF_SUPPORT_STYLUS

private:
	int curSelMenuItem;
	Vector<String> menuItems;
	Vector<MenuListener*> menuListeners;
	int textColor, selectedTextColor;
	AlignmentX alignmentX;
	int x, y;
	int width;
	int height;
	int selectable;
};

class EditBoxListener {
public:
	virtual void onEvent(EditBox *editBox) = 0;
};

class EditBox {
private:
	void initCharacterMap();

public:
	EditBox(int x, int y, int size, int textColor, int selectedTextColor, AlignmentX alignmentX, AlignmentY alignmentY, const char *caption);
	~EditBox();

	void setX(int x);
	void setY(int y);

	void clear();
	void addEditBoxListener(EditBoxListener *editBoxListener);
	byte* getCurrentText();
	void show();
	void update();
	void setCurrentText(char*);

private:
	int curNameIndex;
	int size;
	int curCharacter;
	const char *caption;
	byte *curName;		//=	"a       ";
	Vector<EditBoxListener*> editBoxListeners;
	int textColor, selectedTextColor;
	AlignmentX alignmentX;
	AlignmentY alignmentY;
	int x, y;
};

void drawShadowedText(int x, int y, int col, const char *text);
void drawOutLinedRect(int x1, int y1, int x2, int y2, int outLineCol, int bkgCol);

#endif // _GUI_H_
