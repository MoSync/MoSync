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

#include "Layout.h"

namespace MAUI {

	Layout::Layout(int x, int y, int width, int height, Widget* parent) :
	Widget(x, y, width, height, parent),
	mustRebuild(false),
	alignmentX(HA_LEFT),
	alignmentY(VA_TOP),
	marginX(0),
	marginY(0),
	autoSizeX(false),
	autoSizeY(false),
	gridXSize(gridXSize),
	gridYSize(gridYSize),
	selectedIndex(0) {
		setDrawBackground(false);	
		requestRepaint();
	}

	Layout::Layout(int x, int y, int width, int height, Widget* parent, int gridXSize, int gridYSize) :
	Widget(x, y, width, height, parent),
	mustRebuild(false),
	alignmentX(HA_LEFT),
	alignmentY(VA_TOP),
	marginX(0),
	marginY(0),
	autoSizeX(false),
	autoSizeY(false),
	gridXSize(gridXSize),
	gridYSize(gridYSize),
	selectedIndex(0)
	{
		setDrawBackground(false);	
		requestRepaint();
	}

	void Layout::boundsChanged(Widget *widget, const Rect& bounds) {
		//rebuild();
		mustRebuild = true;
	}

	void Layout::drawWidget() {}

	void Layout::rebuild() {
		mustRebuild = false;

		if(gridXSize*gridYSize<this->children.size()) {
			PANIC_MESSAGE("Your MAUI::Layout has more children than gridXSize*gridYSize");
		}

		int *xOffsets = new int[gridXSize];
		int *yOffsets = new int[gridYSize];
		for(int i = 0; i < gridXSize; i++) xOffsets[i] = 0;
		for(int i = 0; i < gridYSize; i++) yOffsets[i] = 0;

		if(autoSizeX) {
			int xStep = paddedBounds.width / gridXSize;
			for(int i = 0; i < gridXSize; i++) {
				xOffsets[i] = xStep;
			}
		}
		if(autoSizeY) {
			int yStep = paddedBounds.height / gridYSize;
			for(int i = 0; i < gridYSize; i++) {
				yOffsets[i] = yStep;
			}
		}

		if(!(autoSizeX && autoSizeY)) {
			for(int y = 0; y < gridYSize; y++) {
				int highestY = 0;
				for(int x = 0; x < gridXSize; x++) {
					int childIndex = y * gridXSize + x;
					if(children.size() <= childIndex)
						break;
					if(!autoSizeX && xOffsets[x] < children[childIndex]->getBounds().width)
						xOffsets[x] = children[childIndex]->getBounds().width;
					if(children[childIndex]->getBounds().height > highestY)
						highestY = children[childIndex]->getBounds().height;
				}
				if(!autoSizeY) yOffsets[y] = highestY;
			}
		}
		
		int currentY = 0;
		for(int y = 0; y < gridYSize; y++) {
			int currentX = 0;
			for(int x = 0; x < gridXSize; x++) {
				int childIndex = y * gridXSize + x;
				if(children.size() <= childIndex) break;
				
				int alignX = 0;
				int alignY = 0;

				if(autoSizeX) {
					children[childIndex]->setWidth(xOffsets[x]);
				} else {
					switch(alignmentX) {
						case HA_LEFT:
							alignX = 0;
							break;
						case HA_CENTER:
							alignX = (xOffsets[x]>>1) - (children[childIndex]->getWidth()>>1);
							break;
						case HA_RIGHT:
							alignX = (xOffsets[x]) - children[childIndex]->getWidth();
							break;
					}
				}
				if(autoSizeY) {
					children[childIndex]->setHeight(yOffsets[y]);
				} else {
					switch(alignmentY) {
						case VA_TOP:
							alignY = 0;
							break;
						case VA_CENTER:
							alignY = (yOffsets[y]>>1) - (children[childIndex]->getHeight()>>1);
							break;
						case VA_BOTTOM:
							alignY = (yOffsets[y]) - children[childIndex]->getHeight();
							break;
					}
				}


				children[childIndex]->setPosition(currentX + alignX, currentY + alignY);
				currentX+=xOffsets[x] + marginX;
			}
			currentY+=yOffsets[y] + marginY;
		}

		delete []xOffsets;
		delete []yOffsets;

		/*
		switch(layoutType) {
			case VERTICAL_STACKING: 
				{
					if(!autoSizeY) {
						int size = children.size();
						for(int i = 0; i < size; i++) {
							int y;
							if(i>0)
								y = children[i-1]->getPosition().y +
								children[i-1]->getBounds().height + padding;
							else
								y = 0;

							int x = 0;
							if(autoSizeX) {
								children[i]->setWidth(bounds.width);
							} else {
								switch(alignment) {
									case LEFT:
										x = alignment;
										break;
									case MIDDLE:
										x = (bounds.width>>1) - (children[i]->getWidth()>>1) + alignment;
										break;
									case RIGHT:
										x = (bounds.width) - children[i]->getWidth() - alignment;
										break;
								}
							}

							children[i]->setPosition(x, y);
						}
					} else {
						int size = children.size();
						if(size == 0) return;
						int heightOfChildren = bounds.height/size;
						int y = 0;
						for(int i = 0; i < size; i++) {
							int x = 0;
							if(autoSizeX) {
								children[i]->setWidth(bounds.width);
							} else {
								switch(alignment) {
									case LEFT:
										x = alignment;
										break;
									case MIDDLE:
										x = (bounds.width>>1) - (children[i]->getWidth()>>1) + alignment;
										break;
									case RIGHT:
										x = (bounds.width) - children[i]->getWidth() - alignment;
										break;
								}
							}
							children[i]->setPosition(x, y);
							children[i]->setHeight(heightOfChildren);
							y+=heightOfChildren;
						}				
					}
				}
				break;

			case HORIZONTAL_STACKING: 
				{
					if(!autoSizeX) {
						int size = children.size();
						for(int i = 0; i < size; i++) {
							int x;
							if(i>0)
								x = children[i-1]->getPosition().x +
								children[i-1]->getBounds().width + padding;
							else
								x = 0;
							int y = 0;
							if(autoSizeY) {
								children[i]->setHeight(bounds.height);
							} else {
								switch(alignment) {
									case TOP:
										y = alignment;
										break;
									case MIDDLE:
										y = (bounds.height>>1) - (children[i]->getHeight()>>1) + alignment;
										break;
									case BOTTOM:
										y = (bounds.height) - children[i]->getHeight() - alignment;
										break;
								}
							}
							children[i]->setPosition(x, y);
						}
					} else {
						int size = children.size();
						if(size == 0) return;
						int widthOfChildren = bounds.width/size;
						int x = 0;
						for(int i = 0; i < size; i++) {
							int y = 0;
							if(autoSizeY) {
								children[i]->setHeight(bounds.height);
							} else {
								switch(alignment) {
									case TOP:
										y = alignment;
										break;
									case MIDDLE:
										y = (bounds.height>>1) - (children[i]->getHeight()>>1) + alignment;
										break;
									case BOTTOM:
										y = (bounds.height) - children[i]->getHeight() - alignment;
										break;
								}
							}
							children[i]->setPosition(x, y);
							children[i]->setWidth(widthOfChildren);
							x+=widthOfChildren;
						}
					}
				}
				break;
		}*/
	}


	/*
	void Layout::draw() {
		Widget::draw();
	}
	*/


	void Layout::add(Widget *child) {
		/*
		int size = children.size();
		if(layoutType == VERTICAL_STACKING) {
			int y;
			if(size>0)
				y = children[size-1]->getPosition().y +
					children[size-1]->getBounds().height;
			else
				y = 0;

			Widget::add(child);
			child->setPosition(0, y);
		//	child->setWidth(bounds.width);
		}
		else if(layoutType == HORIZONTAL_STACKING) {
			int x;
			if(size>0)
				x = children[size-1]->getPosition().x +
					children[size-1]->getBounds().width;
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
		mustRebuild = true;
		requestRepaint();
	}

	void Layout::clear() {
		for(int i = 0; i < children.size(); i++)
			children[i]->removeWidgetListener(this);
		Widget::clear();
		mustRebuild = true;
		requestRepaint();
	}

	void Layout::update() {
		Widget::update();
		if(mustRebuild) rebuild();		
	}


	/*
	void Layout::updateAbsolutePositionChildren(int x, int y) {
		switch(layoutType) {
			case LT_VERTICAL_STACKING:
			{
				int lastY = 0;
				Vector_each(Widget*,it,children) {
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
				Vector_each(Widget*,it,children) {
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
	
	void Layout::setPosition(int x, int y) {
		Widget::setPosition(x, y);
		mustRebuild = true;
	}
	
	void Layout::setWidth(int width) {
		Widget::setWidth(width);
		mustRebuild = true;
	}
	
	void Layout::setHeight(int height) {
		Widget::setHeight(height);
		mustRebuild = true;
	}

	void Layout::setNumColumns(int numColumns) {
		gridXSize = numColumns;
		mustRebuild = true;
	}

	void Layout::setNumRows(int numRows) {
		gridYSize = numRows;
		mustRebuild = true;
	}

	void Layout::setMarginX(int p) {
		this->marginX = p;
		mustRebuild = true;
	}

	void Layout::setMarginY(int p) {
		this->marginY = p;
		mustRebuild = true;
	}
	
	void Layout::setHorizontalAlignment(HorizontalAlignment alignment) {
		this->alignmentX = alignment;
		mustRebuild = true;
	}

	void Layout::setVerticalAlignment(VerticalAlignment alignment) {
		this->alignmentY = alignment;
		mustRebuild = true;
	}

	void Layout::setAutoSizeX(bool f ) {
		autoSizeX = f;
		mustRebuild = true;
	}

	void Layout::setAutoSizeY(bool f) {
		autoSizeY = f;
		mustRebuild = true;
	}


	void Layout::goUp() {
		if(selectedIndex < children.size()) children[selectedIndex]->setSelected(false);
		if(selectedIndex - gridXSize >= 0) selectedIndex-=gridXSize;
		if(selectedIndex < children.size()) children[selectedIndex]->setSelected(true);
	}
	
	void Layout::goDown() {
		if(selectedIndex < children.size()) children[selectedIndex]->setSelected(false);
		if(selectedIndex + gridXSize < children.size()) selectedIndex+=gridXSize;
		if(selectedIndex < children.size()) children[selectedIndex]->setSelected(true);
	}

	void Layout::goRight() {
		if(selectedIndex < children.size()) children[selectedIndex]->setSelected(false);
		if(selectedIndex + 1 < children.size()) selectedIndex++;
		if(selectedIndex < children.size()) children[selectedIndex]->setSelected(true);
	}

	void Layout::goLeft() {
		if(selectedIndex < children.size()) children[selectedIndex]->setSelected(false);
		if(selectedIndex -1 >= 0) selectedIndex--;
		if(selectedIndex < children.size()) children[selectedIndex]->setSelected(true);
	}

	void Layout::trigger() {
		if(selectedIndex < children.size()) children[selectedIndex]->trigger();
	}

}
