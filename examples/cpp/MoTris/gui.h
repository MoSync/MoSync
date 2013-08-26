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

#ifndef _GUI_H_
#define _GUI_H_

#include <MAUtil/Vector.h>
#include <MAUtil/String.h>

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
	int handlePointerPress(MAPoint2d point);

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
