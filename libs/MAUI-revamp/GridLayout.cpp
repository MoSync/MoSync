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

#include "GridLayout.h"

namespace MAUI {

	GridLayout::GridLayout(int x, int y, int width, int height, int gridXSize, int gridYSize) :
Widget(x, y, width, height),
mMustRebuild(false),
mAlignmentX(HA_LEFT),
mAlignmentY(VA_TOP),
mMarginX(0),
mMarginY(0),
mAutoSizeX(true),
mAutoSizeY(true),
mGridXSize(gridXSize),
mGridYSize(gridYSize)
{
	requestRepaint();
}

void GridLayout::boundsChanged(Widget *widget, const Rect& bounds) {
	//rebuild();
	requestUpdate();
}

void GridLayout::drawWidget() { }

void GridLayout::rebuild() {

	if(mGridXSize*mGridYSize<this->mChildren.size()) {
		PANIC_MESSAGE("Your MAUI::Layout has more mChildren than mGridXSize*mGridYSize");
	}

	Vector_each(Widget*, itr, mChildren) {
		(*itr)->removeWidgetListener(this);
	}

	int *xOffsets = new int[mGridXSize];
	int *yOffsets = new int[mGridYSize];
	for(int i = 0; i < mGridXSize; i++) xOffsets[i] = 0;
	for(int i = 0; i < mGridYSize; i++) yOffsets[i] = 0;

	if(mAutoSizeX) {
		int xStep = mPaddedBounds.width / mGridXSize;
		for(int i = 0; i < mGridXSize; i++) {
			xOffsets[i] = xStep;
		}
	}
	if(mAutoSizeY) {
		int yStep = mPaddedBounds.height / mGridYSize;
		for(int i = 0; i < mGridYSize; i++) {
			yOffsets[i] = yStep;
		}
	}

	if(!(mAutoSizeX && mAutoSizeY)) {
		for(int y = 0; y < mGridYSize; y++) {
			int highestY = 0;
			for(int x = 0; x < mGridXSize; x++) {
				int childIndex = y * mGridXSize + x;
				if(mChildren.size() <= childIndex)
					break;
				if(!mAutoSizeX && xOffsets[x] < mChildren[childIndex]->getBounds().width)
					xOffsets[x] = mChildren[childIndex]->getBounds().width;
				if(mChildren[childIndex]->getBounds().height > highestY)
					highestY = mChildren[childIndex]->getBounds().height;
			}
			if(!mAutoSizeY) yOffsets[y] = highestY;
		}
	}

	int currentY = 0;
	for(int y = 0; y < mGridYSize; y++) {
		int currentX = 0;
		for(int x = 0; x < mGridXSize; x++) {
			int childIndex = y * mGridXSize + x;
			if(mChildren.size() <= childIndex) break;

			int alignX = 0;
			int alignY = 0;

			if(mAutoSizeX) {
				mChildren[childIndex]->setWidth(xOffsets[x]);
			} else {
				switch(mAlignmentX) {
						case HA_LEFT:
							alignX = 0;
							break;
						case HA_CENTER:
							alignX = (xOffsets[x]>>1) - (mChildren[childIndex]->getWidth()>>1);
							break;
						case HA_RIGHT:
							alignX = (xOffsets[x]) - mChildren[childIndex]->getWidth();
							break;
				}
			}
			if(mAutoSizeY) {
				mChildren[childIndex]->setHeight(yOffsets[y]);
			} else {
				switch(mAlignmentY) {
						case VA_TOP:
							alignY = 0;
							break;
						case VA_CENTER:
							alignY = (yOffsets[y]>>1) - (mChildren[childIndex]->getHeight()>>1);
							break;
						case VA_BOTTOM:
							alignY = (yOffsets[y]) - mChildren[childIndex]->getHeight();
							break;
				}
			}


			mChildren[childIndex]->setPosition(currentX + alignX, currentY + alignY);
			currentX+=xOffsets[x] + mMarginX;
		}
		currentY+=yOffsets[y] + mMarginY;
	}

	delete []xOffsets;
	delete []yOffsets;

	Vector_each(Widget*, itr, mChildren) {
		(*itr)->addWidgetListener(this);
	}

	/*
	switch(layoutType) {
	case VERTICAL_STACKING: 
	{
	if(!mAutoSizeY) {
	int size = mChildren.size();
	for(int i = 0; i < size; i++) {
	int y;
	if(i>0)
	y = mChildren[i-1]->getPosition().y +
	mChildren[i-1]->getBounds().height + padding;
	else
	y = 0;

	int x = 0;
	if(mAutoSizeX) {
	mChildren[i]->setWidth(bounds.width);
	} else {
	switch(alignment) {
	case LEFT:
	x = alignment;
	break;
	case MIDDLE:
	x = (bounds.width>>1) - (mChildren[i]->getWidth()>>1) + alignment;
	break;
	case RIGHT:
	x = (bounds.width) - mChildren[i]->getWidth() - alignment;
	break;
	}
	}

	mChildren[i]->setPosition(x, y);
	}
	} else {
	int size = mChildren.size();
	if(size == 0) return;
	int heightOfChildren = bounds.height/size;
	int y = 0;
	for(int i = 0; i < size; i++) {
	int x = 0;
	if(mAutoSizeX) {
	mChildren[i]->setWidth(bounds.width);
	} else {
	switch(alignment) {
	case LEFT:
	x = alignment;
	break;
	case MIDDLE:
	x = (bounds.width>>1) - (mChildren[i]->getWidth()>>1) + alignment;
	break;
	case RIGHT:
	x = (bounds.width) - mChildren[i]->getWidth() - alignment;
	break;
	}
	}
	mChildren[i]->setPosition(x, y);
	mChildren[i]->setHeight(heightOfChildren);
	y+=heightOfChildren;
	}				
	}
	}
	break;

	case HORIZONTAL_STACKING: 
	{
	if(!mAutoSizeX) {
	int size = mChildren.size();
	for(int i = 0; i < size; i++) {
	int x;
	if(i>0)
	x = mChildren[i-1]->getPosition().x +
	mChildren[i-1]->getBounds().width + padding;
	else
	x = 0;
	int y = 0;
	if(mAutoSizeY) {
	mChildren[i]->setHeight(bounds.height);
	} else {
	switch(alignment) {
	case TOP:
	y = alignment;
	break;
	case MIDDLE:
	y = (bounds.height>>1) - (mChildren[i]->getHeight()>>1) + alignment;
	break;
	case BOTTOM:
	y = (bounds.height) - mChildren[i]->getHeight() - alignment;
	break;
	}
	}
	mChildren[i]->setPosition(x, y);
	}
	} else {
	int size = mChildren.size();
	if(size == 0) return;
	int widthOfChildren = bounds.width/size;
	int x = 0;
	for(int i = 0; i < size; i++) {
	int y = 0;
	if(mAutoSizeY) {
	mChildren[i]->setHeight(bounds.height);
	} else {
	switch(alignment) {
	case TOP:
	y = alignment;
	break;
	case MIDDLE:
	y = (bounds.height>>1) - (mChildren[i]->getHeight()>>1) + alignment;
	break;
	case BOTTOM:
	y = (bounds.height) - mChildren[i]->getHeight() - alignment;
	break;
	}
	}
	mChildren[i]->setPosition(x, y);
	mChildren[i]->setWidth(widthOfChildren);
	x+=widthOfChildren;
	}
	}
	}
	break;
	}*/
}


/*
void GridLayout::draw() {
Widget::draw();
}
*/


void GridLayout::add(Widget *child) {
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

void GridLayout::remove(Widget *child) {
	Widget::remove(child);
	child->removeWidgetListener(this);
	//rebuild();
	requestUpdate();
	requestRepaint();
}

void GridLayout::clear() {
	for(int i = 0; i < mChildren.size(); i++)
		mChildren[i]->removeWidgetListener(this);
	Widget::clear();
	requestUpdate();
	requestRepaint();
}

void GridLayout::updateInternal() {
	rebuild();
}

bool GridLayout::isTransparent() const {
	return true;
}



/*
void GridLayout::updateAbsolutePositionChildren(int x, int y) {
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

void GridLayout::setPosition(int x, int y) {
	Widget::setPosition(x, y);
	requestUpdate();
}

void GridLayout::setWidth(int width) {
	Widget::setWidth(width);
	requestUpdate();
}

void GridLayout::setHeight(int height) {
	Widget::setHeight(height);
	requestUpdate();
}

void GridLayout::setNumColumns(int numColumns) {
	mGridXSize = numColumns;
	requestUpdate();
}

void GridLayout::setNumRows(int numRows) {
	mGridYSize = numRows;
	requestUpdate();
}

void GridLayout::setMarginX(int p) {
	this->mMarginX = p;
	requestUpdate();
}

void GridLayout::setMarginY(int p) {
	this->mMarginY = p;
	requestUpdate();
}

void GridLayout::setHorizontalAlignment(HorizontalAlignment alignment) {
	this->mAlignmentX = alignment;
	requestUpdate();
}

void GridLayout::setVerticalAlignment(VerticalAlignment alignment) {
	this->mAlignmentY = alignment;
	requestUpdate();
}

void GridLayout::setAutoSizeX(bool f ) {
	mAutoSizeX = f;
	requestUpdate();
}

void GridLayout::setAutoSizeY(bool f) {
	mAutoSizeY = f;
	requestUpdate();
}

void GridLayout::focusChanged(Widget *widget, bool focused) {
	setFocused(focused);
}
}
