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
#include <conprint.h>
#include "ListBox.h"
#include "Label.h"
#include "Image.h"
#include "Engine.h"
#include <maxtoa.h>
#include <mastdlib.h>
#include <mavsprintf.h>

#include <MAUtil/Graphics.h>

#define FRAMES 1000
#define FPS 50
#define MS_PER_FRAME (1000/FPS)
#define DURATION 250

namespace MAUI {
	ListBox::ListBox(int x, int y, int width, int height, Widget *parent) : 
	Widget(x, y, width, height, parent),
		mustRebuild(false),
		wrapping(true),
		animationType(LBA_NONE),
		orientation(LBO_HORIZONTAL),
		yOffsetFrom(0),
		yOffsetTo(0),
		yOffset(0),
		selectedIndex(0),
		autoSize(false) {
		requestRepaint();
	}

	ListBox::ListBox(int x, int y, int width, int height, Widget *parent, ListBoxOrientation orientation, ListBoxAnimationType animationType, bool wrapping)
		: Widget(x, y, width, height, parent),
		mustRebuild(false),
		wrapping(wrapping),
		animationType(animationType),
		orientation(orientation),
		yOffsetFrom(0),
		yOffsetTo(0),
		yOffset(0),
		selectedIndex(0),
		autoSize(false) {
		requestRepaint();
	}

	void ListBox::setOrientation(ListBoxOrientation orientation) {
		this->orientation = orientation;
		//rebuild();
		mustRebuild = true;
	}

	void ListBox::boundsChanged(Widget *widget, const Rect& bounds) {
		//rebuild();
		mustRebuild = true;

	}

	void ListBox::rebuild() {
		Vector_each(Widget*, itr, children) {
			(*itr)->removeWidgetListener(this);
		}
		if(orientation == LBO_VERTICAL) {
			int size = children.size();
			for(int i = 0; i < size; i++) {
				int y;
				if(i>0)
					y = children[i-1]->getPosition().y +
						children[i-1]->getBounds().height;
				else
					y = 0;
				children[i]->setPosition(0, y);

				if(autoSize)
					children[i]->setWidth(paddedBounds.width);

			}
		} else {
			int size = children.size();
			for(int i = 0; i < size; i++) {
				int x;
				if(i>0)
					x = children[i-1]->getPosition().x +
						children[i-1]->getBounds().width;
				else
					x = 0;
				children[i]->setPosition(x, 0);

				if(autoSize)
					children[i]->setHeight(paddedBounds.height);
			}
		}
		Vector_each(Widget*, itr, children) {
			(*itr)->addWidgetListener(this);
		}

		mustRebuild = false;
	}

	void ListBox::setAutoSize(bool as) {
		this->autoSize = as;
		//rebuild();
		mustRebuild = true;
	}

	void ListBox::add(Widget *child) {
		int size = children.size();

		if(orientation == LBO_VERTICAL) {
			int y;
			if(size>0)
				y = children[size-1]->getPosition().y +
					children[size-1]->getBounds().height;
			else
				y = 0;

			Widget::add(child);
			child->setPosition(0, y);
			if(autoSize)
				children[children.size()-1]->setWidth(paddedBounds.width);
		}
		else if(orientation == LBO_HORIZONTAL) {
			int x;
			if(size>0)
				x = children[size-1]->getPosition().x +
					children[size-1]->getBounds().width;
			else
				x = 0;

			Widget::add(child);
			child->setPosition(x, 0);
			if(autoSize)
				children[children.size()-1]->setHeight(paddedBounds.height);
		}
		
		//listen
		child->addWidgetListener(this);

		if(selectedIndex>children.size()) {
			selectedIndex = 0;
			yOffset = 0;
			/*
			Vector_each(ItemSelectedListener*, i, itemSelectedListeners) {
				(*i)->itemSelected(this, children[this->selectedIndex], NULL);
			}
			*/
		}
		if(children.size() == 1)
			child->setSelected(true);

		requestRepaint();
	}

	bool ListBox::listFrontOutsideBounds() const {
		switch(orientation) {
			case LBO_VERTICAL:
			for(int i = 0; i < children.size(); i++) {
				int y = (children[i]->getPosition().y + (yOffset>>16));
				if(y < 0) {
					return true;
				}
			}
			break;
			case LBO_HORIZONTAL:
			for(int i = 0; i < children.size(); i++) {
				int x = (children[i]->getPosition().x + (yOffset>>16));
				if(x < 0) {
					return true;
				}
			}
			break;
		}
		return false;
	}

	bool ListBox::listBackOutsideBounds() const {
		switch(orientation) {
			case LBO_VERTICAL:
			for(int i = 0; i < children.size(); i++) {
				int y = (children[i]->getPosition().y + children[i]->getHeight() + (yOffset>>16));
				if(y > paddedBounds.height) {
					return true;
				}
			}
			break;
			case LBO_HORIZONTAL:
			for(int i = 0; i < children.size(); i++) {
				int x = (children[i]->getPosition().x + children[i]->getWidth() + (yOffset>>16));
				if(x > paddedBounds.width) {
					return true;
				}
			}
			break;
		}
		return false;
	}

	int ListBox::getScrollOffset() const {
		return yOffset>>16;
	}

	void ListBox::update() {
		Widget::update();	
		if(mustRebuild) rebuild();
	}

#if 0
	static void drawRect(int x1, int y1, int x2, int y2, int col) {
		maSetColor(col);
		Gfx_line(x1, y1, x2, y1);
		Gfx_line(x2, y1, x2, y2);
		Gfx_line(x1, y1, x1, y2);
		Gfx_line(x1, y2, x2, y2);
	}
#endif

	void ListBox::draw(bool forceDraw) {
		//Engine& engine = Engine::getSingleton();

		if(orientation == LBO_VERTICAL) {
		//	int x = paddedBounds.x;
		//	int y = paddedBounds.y+(yOffset>>16);

			int i = 0;
			//int cy = (yOffset>>16);
			//int startIndex = 0;
			//int size = children.size();
			//int endIndex = size;

			//printf("numWidgets: %d\n", size);

			//bool res = engine.pushClipRectIntersect(bounds.x, bounds.y,
			//bounds.width, bounds.height);
			Gfx_pushMatrix();
			Gfx_translate(relX, relY);
				
			BOOL res = Gfx_intersectClipRect(0, 0, bounds.width, bounds.height);

			if(res) {
				if((isDirty() || forceDraw) && shouldDrawBackground) {
					drawBackground();
				}

				//bool res = engine.pushClipRectIntersect(paddedBounds.x, paddedBounds.y,
				//paddedBounds.width, paddedBounds.height);	
				Gfx_translate(paddingLeft, paddingTop);
				res = Gfx_intersectClipRect(0, 0, paddedBounds.width, paddedBounds.height);

				MAPoint2d tBefore = Gfx_getTranslation();
				Gfx_translate(0, (yOffset>>16));
				MAPoint2d translation = Gfx_getTranslation();
				
				if(res) 
				{	
					srand(1);
					for(i = 0; i < children.size(); i++)
					{
						/**
						 * The check wether the child should be drawn or
						 * not is done in Widget::draw(), and we postpone
						 * the check until then.
						 *
						 * // Mattias
						 */
						children[i]->draw();
					}
				}

				//engine.popClipRect();
				Gfx_popClipRect();
			}
			setDirty(false);

			//engine.popClipRect();
			Gfx_popMatrix();
			Gfx_popClipRect();
		} else if(orientation == LBO_HORIZONTAL) {
			//int x = paddedBounds.x+(yOffset>>16);
			//int y = paddedBounds.y;

			int i = 0;
			//int cx = (yOffset>>16);
			//int startIndex = 0;
			//int size = children.size();
			//int endIndex = size;

			//printf("numWidgets: %d\n", size);

			//bool res = engine.pushClipRectIntersect(bounds.x, bounds.y, bounds.width, bounds.height);	
			Gfx_pushMatrix();	
			Gfx_translate(relX, relY);
			BOOL res = Gfx_intersectClipRect(0, 0, bounds.width, bounds.height);	

			if(res) 
			{
				if((isDirty() || forceDraw) && shouldDrawBackground) {
					drawBackground();
				}
	
				//bool res = engine.pushClipRectIntersect(paddedBounds.x, paddedBounds.y,	
				//paddedBounds.width, paddedBounds.height);	
				Gfx_translate(paddingLeft, paddingTop);
				res = Gfx_intersectClipRect(0, 0, paddedBounds.width, paddedBounds.height);	

				MAPoint2d tBefore = Gfx_getTranslation();
				Gfx_translate((yOffset>>16), 0);
				MAPoint2d translation = Gfx_getTranslation();
				if(res) 
				{
					for(i = 0; i < children.size(); i++)
					{
						/**
						 * The check wether the child should be drawn or
						 * not is done in Widget::draw(), and we postpone
						 * the check until then.
						 *
						 * // Mattias
						 */
						children[i]->draw();
					}
				}
				setDirty(false);

				//engine.popClipRect();
				Gfx_popClipRect();
			}
	
			Gfx_popMatrix();
			//engine.popClipRect();
			Gfx_popClipRect();
		}

	}

	void ListBox::addItemSelectedListener(ItemSelectedListener *listener) {
		itemSelectedListeners.add(listener);
	}

	void ListBox::setSelectedIndex(int selectedIndex) {
		if(selectedIndex < 0 || selectedIndex >= children.size()) {
			maPanic(0, "ListBox::setSelectedIndex, index out of bounds");
		}

		if(selectedIndex == this->selectedIndex) {
			Vector_each(ItemSelectedListener*, i, itemSelectedListeners) {
				(*i)->itemSelected(this, children[this->selectedIndex], children[this->selectedIndex]);
			}
			return;
		}

		Widget *unselectedWidget = children[this->selectedIndex];
		unselectedWidget->setSelected(false);
		int lastIndex = this->selectedIndex;
		int offset = (lastIndex-selectedIndex);
		if(offset>0) {
			while(offset--) {
				selectPreviousItem(false);
			}
		} else {
			offset=-offset;
			while(offset--){
				selectNextItem(false);
			}
		}
		Widget *selectedWidget = children[this->selectedIndex]; 
		selectedWidget->setSelected(true);

		Vector_each(ItemSelectedListener*, i, itemSelectedListeners) {
			(*i)->itemSelected(this, selectedWidget, unselectedWidget);
		}

		requestRepaint();
	}

	int ListBox::getSelectedIndex() const {
		return selectedIndex;
	}

	void ListBox::setAnimationType(ListBoxAnimationType type) {
		animationType = type;
	}

	void ListBox::setWrapping(bool mode) {
		wrapping = mode;
	}

	ListBox::ListBoxAnimationType ListBox::getAnimationType() const {
		return animationType;
	}

	bool	ListBox::isWrapping() const {
		return wrapping;
	}

	ListBox::~ListBox() {
	}

	void ListBox::selectPreviousItem(bool shouldFireListeners) {
		Widget *c;
		if(!getChildren().size()) return;

		int pos = 0;

		int prevIndex = selectedIndex;

		selectedIndex--;

		if(selectedIndex < 0) {
			selectedIndex = 0;
			if(!wrapping) {
				if(shouldFireListeners) {
					Vector_each(ItemSelectedListener*, i, itemSelectedListeners) {
						(*i)->blocked(this, -1);
					}
				}
			} else {
				setSelectedIndex(children.size()-1);
			}
			return;
		}
		//printf("element: %d\n", selectedIndex);

		Widget *unselectedWidget = children[prevIndex];
		children[prevIndex]->setSelected(false);

		c = children[selectedIndex];

		c->setSelected(true);

		switch(orientation) {
			case LBO_VERTICAL: pos = (c->getPosition().y<<16); break;
			case LBO_HORIZONTAL: pos = (c->getPosition().x<<16); break;
		}
		if((pos + yOffset) < 0) {
			yOffsetFrom = yOffset>>16;
			yOffset -= pos + yOffset;
			yOffsetTo = yOffset>>16;
			yOffset = yOffsetFrom<<16;
			animTimeStart = maGetMilliSecondCount();
			switch(animationType) {
				case LBA_LINEAR:
					Environment::getEnvironment().addTimer(this, MS_PER_FRAME, FRAMES+1);
					yOffsetInc = ((yOffsetTo - yOffsetFrom)<<16)/FRAMES;
					break;
				case LBA_NONE:
					yOffset = yOffsetTo<<16;
					requestRepaint();
					break;
			}
		}

		if(shouldFireListeners) {
			Vector_each(ItemSelectedListener*, i, itemSelectedListeners) {
				(*i)->itemSelected(this, c, unselectedWidget);
			}
		}

		requestRepaint();
	}

	void ListBox::selectNextItem(bool shouldFireListeners) {
		Widget *c;
		if(!getChildren().size()) return;

		int prevIndex = selectedIndex;

		if(selectedIndex < children.size() - 1) {
			selectedIndex++;
		} else {
			if(!wrapping) {
				if(shouldFireListeners) {
					Vector_each(ItemSelectedListener*, i, itemSelectedListeners) {
						(*i)->blocked(this, 1);
					}
				}
			} else {
				setSelectedIndex(0);
			}
			return;
		}

		Widget *unselectedWidget = children[prevIndex];
		children[prevIndex]->setSelected(false);

		c = children[selectedIndex];

		c->setSelected(true);

		//printf("element: %d\n", selectedIndex);

		int pos = 0;
		int bound = 0;
		int newPos = 0;
		switch(orientation) {
			case LBO_VERTICAL:
				newPos = c->getPosition().y<<16;
				pos = ((c->getPosition().y + c->getHeight())<<16);
				bound =  (paddedBounds.height<<16);
				break;
			case LBO_HORIZONTAL:
				newPos = c->getPosition().x<<16;
				pos = ((c->getPosition().x + c->getWidth())<<16);
				bound =  (paddedBounds.width<<16);
				break;
		}

		if( pos + yOffset > bound) {
			yOffsetFrom = yOffset>>16;
			yOffset -= (pos + yOffset) - bound;
			yOffsetTo = yOffset>>16;
			yOffset = yOffsetFrom<<16;
			animTimeStart = maGetMilliSecondCount();
			switch(animationType) {
				case LBA_LINEAR:
					Environment::getEnvironment().addTimer(this, MS_PER_FRAME, FRAMES+1);
					yOffsetInc = ((yOffsetTo - yOffsetFrom)<<16)/FRAMES;
					break;
				case LBA_NONE:
					yOffset = yOffsetTo<<16;
					requestRepaint();
					break;
			}
		}

		if(shouldFireListeners) {
			Vector_each(ItemSelectedListener*, i, itemSelectedListeners) {
				(*i)->itemSelected(this, c, unselectedWidget);
			}
		}

		requestRepaint();
	}

	void ListBox::runTimerEvent()
	{
		//yOffset += yOffsetInc;
		yOffset = (yOffsetFrom<<16) + (yOffsetTo-yOffsetFrom)*(((maGetMilliSecondCount()-animTimeStart)<<16)/DURATION);
		if(yOffsetInc<0 && yOffset<=yOffsetTo<<16) {
			yOffset = yOffsetTo<<16;
			Environment::getEnvironment().removeTimer(this);
		}
		else if(yOffsetInc>0 && yOffset>=yOffsetTo<<16) {
			yOffset = yOffsetTo<<16;
			Environment::getEnvironment().removeTimer(this);
		}


		requestRepaint();
	}

	void ListBox::drawWidget() {
	}

	void ListBox::setWidth(int w) {
		Widget::setWidth(w);
		if(autoSize) {
			if(orientation == LBO_VERTICAL) {
				for(int i = 0; i < children.size(); i++) {
					children[i]->setWidth(paddedBounds.width);
				}
			}
		}
	}

	void ListBox::setHeight(int h) {
		Widget::setHeight(h);
		if(autoSize) {
			if(orientation == LBO_HORIZONTAL) {
				for(int i = 0; i < children.size(); i++) {
					children[i]->setHeight(paddedBounds.height);
				}
			}
		}
	}
}
