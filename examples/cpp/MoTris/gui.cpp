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

#include <ma.h>
#include <mastring.h>
#include <MAUtil/Geometry.h>

#include "gui.h"

static byte characterList[] = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
static byte characterMap[256];
static bool characterMapInitialized = false;

void drawShadowedText(int x, int y, int col, const char *text) {
	maSetColor(0);
	maDrawText(x-1, y-1, text);
	maSetColor(col);
	maDrawText(x, y, text);
}

void drawOutLinedRect(int x1, int y1, int x2, int y2, int outLineCol, int bkgCol) {
	maSetColor(bkgCol);
	maFillRect(x1, y1, x2-x1, y2-y1);
	maSetColor(outLineCol);
	maLine(x1-1, y1-1, x2, y1-1);
	maLine(x1-1, y2, x2, y2);
	maLine(x1-1, y1-1, x1-1, y2);
	maLine(x2, y1-1, x2, y2);
}

Menu::Menu(int textColor, int selectedTextColor, int x, int y, int width, int height, AlignmentX alignmentX,int selectable) {
	curSelMenuItem = 0;
	this->textColor = textColor;
	this->selectedTextColor = selectedTextColor;
	this->alignmentX = alignmentX;
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->selectable = selectable;
}

void Menu::setX(int x) { 
	this->x = x; 
}

void Menu::setY(int y) { 
	this->y = y; 
}

void Menu::clear() { 
	menuItems.clear();
}

void Menu::addMenuListener(MenuListener *menuListener) {
	menuListeners.add(menuListener);
}

void Menu::addMenuItem(String menuItem) {
	menuItems.add(menuItem);	
}

void Menu::setMenuItem(int i, String menuItem) {
	menuItems[i] = menuItem;
}

const String& Menu::getCurrentlySelectedMenuItem() {
	return menuItems[curSelMenuItem];
}

int Menu::getCurrentlySelectedMenuIndex() {
	return curSelMenuItem;
}

void Menu::show() {

	//int x = (w>>1);
	//y = beneathLogoY + MENU_OFFSET;

	MAExtent me = maGetTextSize("A");
	int sy;
	sy = y;

	int c;
	for(int i = 0; i < menuItems.size(); i++) {
		me = maGetTextSize(menuItems[i].c_str());
		int xSize = EXTENT_X(me);
		int ySize = EXTENT_Y(me);
		int sx = x;

		if(alignmentX == ALIGN_LEFT_X) {
			sx = x+(xSize>>1);
		}
		else if(alignmentX == ALIGN_RIGHT_X) {
			sx = x-(xSize>>1);
		}

		if(selectable && curSelMenuItem == i) {
			c = selectedTextColor;
		}
		else {
			c = textColor;
		}

		if(sy>=y && sy <y+height)
		drawShadowedText(sx-(xSize>>1), sy, c, (char*)menuItems[i].c_str());
		sy+=ySize+2;
	}
}

#ifdef MA_PROF_SUPPORT_STYLUS
int Menu::handlePointerPress(MAPoint2d point)
{
	int index = -1;
	MAExtent me = maGetTextSize("A");
	int sy;
	sy = y;

	for(int i = 0; i < menuItems.size(); i++) {
		me = maGetTextSize(menuItems[i].c_str());
		int xSize = EXTENT_X(me);
		int ySize = EXTENT_Y(me);
		int sx = x;

		if(alignmentX == ALIGN_LEFT_X) {
			sx = x+(xSize>>1);
		}
		else if(alignmentX == ALIGN_RIGHT_X) {
			sx = x-(xSize>>1);
		}

		Rect bounds(sx-(xSize>>1), sy, xSize, ySize);
		if(bounds.contains(point.x, point.y))
		{
			index = curSelMenuItem = i;
			break;
		}
		sy+=ySize+2;
	}
	return index;
}
#endif	// MA_PROF_SUPPORT_STYLUS

void Menu::update() {
	if(EventHandler::updated) {
		for(int i = 0; i < menuListeners.size(); i++) {
			menuListeners[i]->onEvent(curSelMenuItem, this);
		}
	}

	if(!menuItems.size()) return;

	//if(selectable) 
	{
		if(EventHandler::down_pressed) {
			//curSelMenuItem=(curSelMenuItem+1)%menuItems.size();
			curSelMenuItem++;
			if(curSelMenuItem>=menuItems.size()-1) curSelMenuItem = menuItems.size()-1;//curSelMenuItem=menuItems.size()-1;
		}
		else if(EventHandler::up_pressed) {
			curSelMenuItem=(curSelMenuItem-1);
			if(curSelMenuItem<0) curSelMenuItem = 0;//curSelMenuItem=menuItems.size()-1;
		}
	}
}

void EditBox::initCharacterMap() {
	memset(characterMap, 0xff, 256);
	for(unsigned int i = 0; i < sizeof(characterList); i++) {
		characterMap[characterList[i]] = i;
	}
}

EditBox::EditBox(int x, int y, int size, int textColor, int selectedTextColor, AlignmentX alignmentX, AlignmentY alignmentY, const char *caption) {
	curName = new byte[size+1];
	this->size = size;

	clear();

	this->caption = caption;
	this->textColor = textColor;
	this->selectedTextColor = selectedTextColor;
	this->alignmentX = alignmentX;
	this->x = x;
	this->y = y;

	if(!characterMapInitialized) {
		initCharacterMap();
		characterMapInitialized=true;
	}

}

EditBox::~EditBox() {
	delete curName;

}

void EditBox::setX(int x) { this->x = x; }
void EditBox::setY(int y) { this->y = y; }

void EditBox::clear() {
	memset(curName, ' ', size);
	curName[size] = 0;

	curNameIndex = 0;
	curCharacter = 0;
}

void EditBox::addEditBoxListener(EditBoxListener *editBoxListener) {
	editBoxListeners.add(editBoxListener);
}

byte* EditBox::getCurrentText() {
	return curName;
}

void EditBox::setCurrentText(char* text) {
	memcpy(curName, text, MIN(strlen(text), (unsigned int)size));
}


void EditBox::show() {
	MAExtent curNameExtent = maGetTextSize((char*)curName);
	MAExtent captionExtent = maGetTextSize(caption);
	int xSize = EXTENT_X(curNameExtent);
	int ySize = EXTENT_Y(curNameExtent);
	int xSizeC = EXTENT_X(captionExtent);
	int ySizeC = EXTENT_Y(captionExtent);

	int sx = x;
	int sy = y;
	int sxc = x;

	if(alignmentX == ALIGN_LEFT_X) {
		sx+=xSize>>1;
		sxc+=xSizeC>>1;
	}
	else if(alignmentX == ALIGN_RIGHT_X) {
		sx-=xSize>>1;
		sxc-=xSizeC>>1;
	}

	if(alignmentY == ALIGN_TOP_Y) {
		sy+=ySize;
		sy+=ySizeC;
	}
	else if(alignmentY == ALIGN_BOTTOM_Y) {
		sy-=ySize;
		sy-=ySizeC;
	}

	drawOutLinedRect(MIN(sxc-(xSizeC>>1)-1, sx-(xSize>>1)-1), sy, MAX(sxc+(xSizeC>>1)+1, sx+(xSize>>1)+1),sy+ySizeC+ySize+3-1,  textColor, 0);

	drawShadowedText(sxc-(xSizeC>>1), sy, textColor, (char*)caption);
	sy+=ySizeC;

	// begin draw marker
	char tempString[255];
	int mx, my;
	MAExtent markerExtent;

	memcpy(tempString, curName, curNameIndex);
	tempString[curNameIndex] = 0;
	markerExtent = maGetTextSize(tempString);
	mx = EXTENT_X(markerExtent);
	my = EXTENT_Y(markerExtent);

	tempString[0] = curName[curNameIndex];
	tempString[1] = 0;
	markerExtent = maGetTextSize(tempString);

	maSetColor(selectedTextColor);
	maFillRect(sx-(xSize>>1)+mx, sy, EXTENT_X(markerExtent), EXTENT_Y(markerExtent));

	// end draw marker

	//drawShadowedText(sx-(xSize>>1), sy, textColor, curName);
	maSetColor(textColor);
	maDrawText(sx-(xSize>>1), sy, (char*)curName);
}

void EditBox::update() {
	if(EventHandler::left_pressed) {
		curNameIndex--;
		if(curNameIndex<0) curNameIndex = 0;

		curCharacter = characterMap[curName[curNameIndex]];
	}
	if(EventHandler::right_pressed) {
		curNameIndex++;
		int s = size;
		if(curNameIndex>=s) curNameIndex = s-1;
		curCharacter = characterMap[curName[curNameIndex]];
	}
	if(EventHandler::up_pressed) {
		curCharacter--;
		if(curCharacter<0) curCharacter = strlen((char*)characterList)-1; 
		curName[curNameIndex] = characterList[curCharacter];
	}
	if(EventHandler::down_pressed) {
		curCharacter++;
		int s = strlen((char*)characterList);
		if(curCharacter>=s) curCharacter = 0;	
		curName[curNameIndex] = characterList[curCharacter];
	}

	if(EventHandler::updated) {
		for(int i = 0; i < editBoxListeners.size(); i++) {
			editBoxListeners[i]->onEvent(this);
		}
	}
}
