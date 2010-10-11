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

#include <maassert.h>

#include "StackLayout.h"

namespace MAUI {

	StackLayout::StackLayout(int x, int y, int width, int height, Orientation ori, Alignment ali) :
Widget(x, y, width, height),
mOrientation(ori),
mAlignment(ali),
mMarginX(0),
mMarginY(0)
{
	requestRepaint();
}

void StackLayout::boundsChanged(Widget *widget, const Rect& bounds) {
	//rebuild();
	requestUpdate();
}

void StackLayout::drawWidget() { }

void StackLayout::rebuild() {
	Vector_each(Widget*, itr, mChildren) {
		(*itr)->removeWidgetListener(this);
	}

	int padding = 0;
	int alignment = 0;

	switch(mOrientation) {
			case SLO_VERTICAL: 
				{
					int size = mChildren.size();
					for(int i = 0; i < size; i++) {
						int y;
						if(i>0)
							y = mChildren[i-1]->getPosition().y +
							mChildren[i-1]->getBounds().height + padding;
						else
							y = 0;

						int x = 0;
						if(mAlignment == SLA_AUTO_SIZE) {
							mChildren[i]->setWidth(mBounds.width);
						} else {
							switch(mAlignment) {
								case SLA_TOP_LEFT:
									x = alignment;
									break;
								case SLA_CENTER:
									x = (mBounds.width>>1) - (mChildren[i]->getWidth()>>1) + alignment;
									break;
								case SLA_BOTTOM_RIGHT:
									x = (mBounds.width) - mChildren[i]->getWidth() + alignment;
									break;
							}
						}

						mChildren[i]->setPosition(x, y);
					}
				}
				break;

			case SLO_HORIZONTAL: 
				{
					int size = mChildren.size();
					for(int i = 0; i < size; i++) {
						int x;
						if(i>0)
							x = mChildren[i-1]->getPosition().x +
							mChildren[i-1]->getBounds().width + padding;
						else
							x = 0;
						int y = 0;
						if(mAlignment == SLA_AUTO_SIZE) {
							mChildren[i]->setHeight(mBounds.height);
						} else {
							switch(mAlignment) {
								case SLA_TOP_LEFT:
									y = alignment;
									break;
								case SLA_CENTER:
									y = (mBounds.height>>1) - (mChildren[i]->getHeight()>>1);
									break;
								case SLA_BOTTOM_RIGHT:
									y = (mBounds.height) - mChildren[i]->getHeight();
									break;
							}
						}
						mChildren[i]->setPosition(x, y);
					}
				}
				break;
	}

	Vector_each(Widget*, itr, mChildren) {
		(*itr)->addWidgetListener(this);
	}
}


/*
void StackLayout::draw() {
Widget::draw();
}
*/


void StackLayout::add(Widget *child) {
	/*
	int size = mChildren.size();
	if(layoutType == VERTICAL_STACKING) {
	int y;
	if(size>0)
	y = mChildren[size-1]->getPosition().y +
	mChildren[size-1]->getBounds().height;
	else
	y = 0;

	Widget::add(child);
	child->setPosition(0, y);
	//	child->setWidth(bounds.width);
	}
	else if(layoutType == HORIZONTAL_STACKING) {
	int x;
	if(size>0)
	x = mChildren[size-1]->getPosition().x +
	mChildren[size-1]->getBounds().width;
	else
	x = 0;

	Widget::add(child);
	child->setPosition(x, 0);
	//	child->setHeight(bounds.height);	
	}
	*/
	Widget::add(child);
	child->addWidgetListener(this);
	//rebuild();
	requestUpdate();
	requestRepaint();
}

void StackLayout::clear() {
	for(int i = 0; i < mChildren.size(); i++)
		mChildren[i]->removeWidgetListener(this);
	Widget::clear();
	requestUpdate();
	requestRepaint();
}

void StackLayout::updateInternal() {
	rebuild();
}

bool StackLayout::isTransparent() const {
	return true;
}



/*
void StackLayout::updateAbsolutePositionChildren(int x, int y) {
switch(layoutType) {
case LT_VERTICAL_STACKING:
{
int lastY = 0;
Vector_each(Widget*,it,mChildren) {
(*it)->bounds.x = x;
(*it)->bounds.y = lastY + y;
(*it)->bounds.width = bounds.width; // adjust the width to fill up the whole layout
(*it)->updateAbsolutePositionChildren((*it)->bounds.x, (*it)->bounds.y);
lastY = (*it)->bounds.y + (*it)->bounds.height;
}
}
break;
case LT_HORIZONTAL_STACKING:
{
int lastX = 0;
Vector_each(Widget*,it,mChildren) {
(*it)->bounds.x = lastX + x;
(*it)->bounds.y = y;
(*it)->bounds.height = bounds.height; // adjust the height to fill up the whole layout
(*it)->updateAbsolutePositionChildren((*it)->bounds.x, (*it)->bounds.y);
lastX = (*it)->bounds.x + (*it)->bounds.width;
}
}
break;
}
}
*/

void StackLayout::setPosition(int x, int y) {
	bool changed = x!=this->mBounds.x || y!=this->mBounds.y;
	if(changed) {
		Widget::setPosition(x, y);
		requestUpdate();
	}
}

void StackLayout::setWidth(int width) {
	Widget::setWidth(width);
	requestUpdate();
}

void StackLayout::setHeight(int height) {
	Widget::setHeight(height);
	requestUpdate();
}

void StackLayout::setMarginX(int p) {
	this->mMarginX = p;
	requestUpdate();
}

void StackLayout::setMarginY(int p) {
	this->mMarginY = p;
	requestUpdate();
}

void StackLayout::setAlignment(Alignment alignment) {
	this->mAlignment = alignment;
	requestUpdate();
}

void StackLayout::setOrientation(Orientation orientation) {
	this->mOrientation = orientation;
	requestUpdate();
}

void StackLayout::focusChanged(Widget *widget, bool focused) {
	setFocused(focused);
}

}
