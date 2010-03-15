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
#include "Widget.h"
#include <maheap.h>
#include <MAUtil/String.h>
#include <mastdlib.h>
//#include <mavsprintf.h>
#include <MAUtil/Vector.h>
#include "Engine.h"
#include <MAUtil/Graphics.h>
#include <MAUtil/util.h>

#ifdef MAPIP
#include <conprint.h>
#define WLOG(...) //printf
#else
#define WLOG(...)
#endif

namespace MAUI {

	Widget::Widget(int x, int y, int width, int height, Widget *parent=NULL) 
		: parent(NULL), bounds(0,0,width,height), relX(x), relY(y), 
			dirty(false),
			skin(NULL),
			backColor(0),
			shouldDrawBackground(true),
			selected(false),
			enabled(true),
			paddingLeft(0),
			paddingTop(0),
			paddingBottom(0),
			paddingRight(0)
		{
		
		skin = Engine::getSingleton().getDefaultSkin();
		
		if(parent) {
			parent->add(this);
		}
		updateAbsolutePosition();
	}

	Widget::~Widget() {
		Vector_each(Widget*,it,children)
			delete (*it);
	}

	void Widget::add(Widget* w) {
		children.add(w);
		w->setParent(this);
	}
	
	const Rect& Widget::getBounds() {
		return bounds;
	}
	
	bool Widget::contains(const Point& p) {
		return bounds.contains(p);
	}

	bool Widget::contains(int x, int y) {
		return bounds.contains(x, y);
	}

	bool Widget::isTransparent() const {
		if(selected)
			return (skin!=NULL && skin->isSelectedTransparent()==true)  || (!shouldDrawBackground);
		else
			return (skin!=NULL && skin->isUnselectedTransparent()==true)  || (!shouldDrawBackground);
	}

	void Widget::drawBackground() {
		//printf("in drawBackground!\n");
		if(skin) {
			if(selected)		
				//skin->draw(bounds.x, bounds.y, bounds.width, bounds.height, WidgetSkin::SELECTED);
				skin->draw(0, 0, bounds.width, bounds.height, WidgetSkin::SELECTED);
			else
				//skin->draw(bounds.x, bounds.y, bounds.width, bounds.height, WidgetSkin::UNSELECTED);
					skin->draw(0, 0, bounds.width, bounds.height, WidgetSkin::UNSELECTED);			
		} else {
			maSetColor(backColor);
			//maSetColor(0xffff00ff);
			//printf("filling rect!!!\n");
			//maFillRect(bounds.x,bounds.y, bounds.width, bounds.height);
			Gfx_fillRect(0, 0, bounds.width, bounds.height);
		}
	}

	/*
	void Widget::draw() {
		WLOG("Widget:draw");
		Engine &engine = Engine::getSingleton();
		
		bool res = engine.pushClipRectIntersect(bounds.x, bounds.y,
			bounds.width, bounds.height);	
		if(res && isDirty()) {
			if(shouldDrawBackground) {
				drawBackground();
			}
			drawWidget();
			setDirty(false);
		}

		Vector_each(Widget*, it, children)
			(*it)->draw();

		engine.popClipRect();
	}*/

	void Widget::update() {
		Vector_each(Widget*, it, children)
			(*it)->update();
	}

	void Widget::draw(bool forceDraw) {
		if(!enabled && !forceDraw) return;
	
		//Engine &engine = Engine::getSingleton();

		//bool res = engine.pushClipRectIntersect(bounds.x, bounds.y,
		//	bounds.width, bounds.height);
		
		Gfx_pushMatrix();
		Gfx_translate(relX, relY);
		BOOL res = Gfx_intersectClipRect(0, 0, bounds.width, bounds.height);
		
		if(res) 
		{
			if(isDirty() || forceDraw) 
			{
				if(shouldDrawBackground) 
				{
					drawBackground();
				}
			}
			//bool res = engine.pushClipRectIntersect(paddedBounds.x, paddedBounds.y,
			//	paddedBounds.width, paddedBounds.height);
			Gfx_translate(paddingLeft, paddingTop);
			BOOL res = Gfx_intersectClipRect(0, 0, paddedBounds.width, paddedBounds.height);

			if(res) {

				if(isDirty() || forceDraw) 
					drawWidget();
				Vector_each(Widget*, it, children)
					(*it)->draw();	

			}

			//engine.popClipRect();
			Gfx_popClipRect();
			setDirty(false);
		}
		Gfx_popMatrix();
		//engine.popClipRect();
		Gfx_popClipRect();
	}

	void Widget::setPosition(int x, int y) {
		bool changed = relX != x || relY != y;
		relX = x;
		relY = y;
		updateAbsolutePosition();

		if(changed) {
			requestRepaint();
			fireBoundsChanged();
		}
	}

	const Point& Widget::getPosition() const {
		static Point pnt;
		pnt = Point(relX, relY);
		return pnt;
	}

	const Point& Widget::getPaddedPosition() const {
		static Point pnt;
		pnt = Point(relX+paddingLeft, relY+paddingTop);
		return pnt;
	}

	void Widget::setWidth(int width) {
		bool changed = width != bounds.width;
		bounds.width = width;
		updatePaddedBounds();
		requestRepaint();
		if(changed) {
			fireBoundsChanged();
		}
	}

	int Widget::getWidth() const {
		return bounds.width;
	}

	void Widget::setHeight(int height) {
		bool changed = height != bounds.height;
		bounds.height = height;
		updatePaddedBounds();
		requestRepaint();
		if(changed) {
			fireBoundsChanged();
		}
	}

	int	Widget::getHeight() const {
		return bounds.height;
	}

	void Widget::setParent(Widget *w) {
		if(w != NULL && parent != NULL && parent != w) {
			PANIC_MESSAGE("Widget already has a parent!");
		}
		parent = w;
		updateAbsolutePosition();
		requestRepaint();
	}

	Widget* Widget::getParent() {
		return parent;
	}

	Widget* Widget::widgetAt(const Point& p) {
		return widgetAt(p.x, p.y);
	}

	Widget* Widget::widgetAt(int x, int y) {
		Vector_each(Widget*,it,children) {
			Widget *ret = widgetAt(x, y);
			if(ret) return ret;
		}

		if(bounds.contains(x, y)) {
			return this;
		}

		return NULL;
	}

	void Widget::requestRepaint() {
		Engine::getSingleton().requestUIUpdate();
		setDirty();

		if(isTransparent()) {
			if(parent) {
				parent->requestRepaint();
			}
		}
	}

	bool Widget::isDirty() const {
		return dirty;
	}

	void Widget::setDirty(bool d) {
		dirty = d;

		if(d == true) {
			Vector_each(Widget*, it, children)
				(*it)->setDirty(d);
		}
	}

	void Widget::setSkin(WidgetSkin *skin) {
		this->skin = skin;
		requestRepaint();
	}

	void Widget::setDrawBackground(bool b) {
		this->shouldDrawBackground = b;
		requestRepaint();
	}

	void Widget::setBackgroundColor(int col) {
		this->backColor = col;
		requestRepaint();
	}

	void Widget::updateAbsolutePositionChildren(int x, int y) {
		Vector_each(Widget*,it,children) {
			(*it)->bounds.x = (*it)->relX + x;
			(*it)->bounds.y = (*it)->relY + y;
			(*it)->updatePaddedBounds();
			(*it)->updateAbsolutePositionChildren((*it)->paddedBounds.x, (*it)->paddedBounds.y);
		}
	}

	void Widget::updatePaddedBounds() {
		paddedBounds = bounds;
		paddedBounds.x+=paddingLeft;
		paddedBounds.y+=paddingTop;
		paddedBounds.width-=paddingLeft+paddingRight;
		paddedBounds.height-=paddingTop+paddingBottom;
	}

	// fixme, precalc absolute for parent
	void Widget::updateAbsolutePosition() {
		Widget *p = this;
		bounds.x = relX;
		bounds.y = relY;
		while((p = p->getParent())) {
			bounds.x += p->getPosition().x + p->paddingLeft;  
			bounds.y += p->getPosition().y + p->paddingTop;
		}
		updatePaddedBounds();
		updateAbsolutePositionChildren(paddedBounds.x, paddedBounds.y);

		fireBoundsChanged();
	}

	Vector<Widget*>& Widget::getChildren() {
		return children;
	}

	const Vector<Widget*>& Widget::getChildren() const {
		return children;
	}

	void Widget::addWidgetListener(WidgetListener* wl) {
		
		Vector_each(WidgetListener*, i, widgetListeners) {
			if((*i) == wl) return;
		}
		widgetListeners.add(wl);
	}

	void Widget::removeWidgetListener(WidgetListener* wl) {

		Vector_each(WidgetListener*, i, widgetListeners) {
			if((*i) == wl) {
				widgetListeners.remove(i);
				return;	//or crash
			}
		}
	}

	Vector<WidgetListener*>& Widget::getWidgetListeners()
	{
		return widgetListeners;
	}

	void Widget::trigger() {
		Vector_each(WidgetListener*, wl, widgetListeners) {
			(*wl)->triggered(this);
		}
	}

	void Widget::setSelected(bool selected) {
		this->selected = selected;
		Vector_each(WidgetListener*, wl, widgetListeners) {
			(*wl)->selectionChanged(this, selected);
		}
		requestRepaint();
	}
	
	bool Widget::isSelected() const {
		return selected;
	}

	void Widget::setEnabled(bool enabled) {
		this->enabled = enabled;

		Vector_each(Widget*,it,children) {
			(*it)->setEnabled(enabled);
		}
		
		Vector_each(WidgetListener*, wl, widgetListeners) {
			(*wl)->enableStateChanged(this, selected);
		}
		requestRepaint();
	}
	
	bool Widget::isEnabled() const {
		return enabled;
	}

	void Widget::setPaddingLeft(int l) {
		paddingLeft = l;
		updateAbsolutePosition();
		requestRepaint();
	}

	void Widget::setPaddingTop(int t) {
		paddingTop = t;
		updateAbsolutePosition();
		requestRepaint();
	}

	void Widget::setPaddingRight(int r) {
		paddingRight = r;
		updateAbsolutePosition();
		requestRepaint();
	}

	void Widget::setPaddingBottom(int b) {
		paddingBottom = b;
		updateAbsolutePosition();
		requestRepaint();
	}

	int Widget::getPaddingLeft() const {
		return paddingLeft;
	}

	int Widget::getPaddingTop() const {
		return paddingTop;
	}

	int Widget::getPaddingRight() const {
		return paddingRight;
	}

	int Widget::getPaddingBottom() const {
		return paddingBottom;
	}

	const Rect& Widget::getPaddedBounds() const {
		return paddedBounds;
	}

	void Widget::setParameter(const String& name, const String& value) {
		if(name == "paddingLeft") setPaddingLeft(stringToInteger(value));
		else if(name == "paddingTop") setPaddingTop(stringToInteger(value));
		else if(name == "paddingBottom") setPaddingBottom(stringToInteger(value));
		else if(name == "paddingRight") setPaddingRight(stringToInteger(value));
		else if(name == "backgroundColor") setBackgroundColor(stringToInteger(value));
		else if(name == "drawBackground") setDrawBackground(value=="true"?true:false);
		else if(name == "width") setWidth(stringToInteger(value));
		else if(name == "height") setHeight(stringToInteger(value));
		else if(name == "x") this->setPosition(stringToInteger(value), getPosition().y);
		else if(name == "y") this->setPosition(getPosition().x, stringToInteger(value));
		else maPanic(0, "MAUI::Widget wrong parameter");
	}
}
