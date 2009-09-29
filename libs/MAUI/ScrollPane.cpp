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

#include <ma.h>
#include "ScrollPane.h"
#include "Engine.h"
#include "mavsprintf.h"
#include <mastdlib.h>

namespace MAUI {

	ScrollPane::ScrollPane(int x, int y, int width, int height, Widget *parent) :
		Widget(x, y, width, height, parent),
			offsetX(0),
			offsetY(0),
			scrollBarX(0),
			scrollBarY(0) {

	}


	ScrollPane::ScrollPane(int x, int y, int width, int height, Widget *parent, int offsetX, int offsetY, bool scrollBarX, bool scrollBarY) :
		Widget(x, y, width, height, parent),
			offsetX(offsetX),
			offsetY(offsetY),
			scrollBarX(scrollBarX),
			scrollBarY(scrollBarY)
		{
	}

	ScrollPane::~ScrollPane() {

	}

void ScrollPane::draw() {
	
	Engine &engine = Engine::getSingleton();
	int i;
//	int x = MAUIWidget_getAbsolutePositionX(w)+s->offsetX;
//	int y = MAUIWidget_getAbsolutePositionY(w)+s->offsetY;
	Point c;

	engine.pushClipRectIntersect(	bounds.x,
									bounds.y, 
									bounds.width, 
									bounds.height);	

	for(i = 0; i < children.size(); i++)
	{
		Widget* child = children[i];
		c = child->getPosition();
		child->setPosition(c.x + offsetX, c.y + offsetY);
		child->draw();
		child->setPosition(c.x, c.y);
	}

	engine.popClipRect();
}

void ScrollPane::setOffsetX(int offsetX) {
	this->offsetX = offsetX;
}

void ScrollPane::setOffsetY(int offsetY) {
	this->offsetY = offsetY;
}

void ScrollPane::setScrollBarX(bool scrollBarX) {
	this->scrollBarX = scrollBarX;
}

void ScrollPane::setScrollBarY(bool scrollBarY) {
	this->scrollBarY = scrollBarY;
}

int ScrollPane::getOffsetX() const {
	return offsetX;
}

int ScrollPane::getOffsetY() const {
	return offsetY;
}

bool ScrollPane::getScrollBarX() const {
	return scrollBarX;
}

bool ScrollPane::getScrollBarY() const {
	return scrollBarY;
}

}
