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
		: mParent(NULL), mBounds(0,0,width,height), mRelX(x), mRelY(y),
			mDirty(false),
			mSkin(NULL),
			mBackColor(0),
			mShouldDrawBackground(true),
			mSelected(false),
			mEnabled(true),
			mPaddingLeft(0),
			mPaddingTop(0),
			mPaddingBottom(0),
			mPaddingRight(0)
		{
		
		mSkin = Engine::getSingleton().getDefaultSkin();
		
		if(parent) {
			parent->add(this);
		}
		updateAbsolutePosition();
	}

	Widget::~Widget() {
		Vector_each(Widget*,it,mChildren)
			delete (*it);
	}

	void Widget::add(Widget* w) {
		mChildren.add(w);
		w->setParent(this);
		requestRepaint();
	}

	void Widget::clear() {
		for(int i = 0; i < mChildren.size(); i++)
			mChildren[i]->setParent(NULL);
		mChildren.clear();
		requestRepaint();
	}
	
	const Rect& Widget::getBounds() {
		return mBounds;
	}
	
	bool Widget::contains(const Point& p) {
		return mBounds.contains(p);
	}

	bool Widget::contains(int x, int y) {
		return mBounds.contains(x, y);
	}

	bool Widget::isTransparent() const {
		if(mSelected)
			return (mSkin!=NULL && mSkin->isSelectedTransparent()==true)  || (!mShouldDrawBackground);
		else
			return (mSkin!=NULL && mSkin->isUnselectedTransparent()==true)  || (!mShouldDrawBackground);
	}

	void Widget::drawBackground() {
		//printf("in drawBackground!\n");
		if(mSkin) {
			if(mSelected)
				//mSkin->draw(mBounds.x, mBounds.y, mBounds.width, mBounds.height, WidgetSkin::SELECTED);
				mSkin->draw(0, 0, mBounds.width, mBounds.height, WidgetSkin::SELECTED);
			else
				//mSkin->draw(mBounds.x, mBounds.y, mBounds.width, mBounds.height, WidgetSkin::UNSELECTED);
					mSkin->draw(0, 0, mBounds.width, mBounds.height, WidgetSkin::UNSELECTED);
		} else {
			maSetColor(mBackColor);
			//maSetColor(0xffff00ff);
			//printf("filling rect!!!\n");
			//maFillRect(mBounds.x,mBounds.y, mBounds.width, mBounds.height);
			Gfx_fillRect(0, 0, mBounds.width, mBounds.height);
		}
	}

	/*
	void Widget::draw() {
		WLOG("Widget:draw");
		Engine &engine = Engine::getSingleton();
		
		bool res = engine.pushClipRectIntersect(mBounds.x, mBounds.y,
			mBounds.width, mBounds.height);
		if(res && isDirty()) {
			if(mShouldDrawBackground) {
				drawBackground();
			}
			drawWidget();
			setDirty(false);
		}

		Vector_each(Widget*, it, mChildren)
			(*it)->draw();

		engine.popClipRect();
	}*/

	void Widget::update() {
		Vector_each(Widget*, it, mChildren)
			(*it)->update();
	}

	void Widget::draw(bool forceDraw) {
		if(!mEnabled && !forceDraw) return;
	
		//Engine &engine = Engine::getSingleton();

		//bool res = engine.pushClipRectIntersect(mBounds.x, mBounds.y,
		//	mBounds.width, mBounds.height);
		
		Gfx_pushMatrix();
		Gfx_translate(mRelX, mRelY);
		BOOL res = Gfx_intersectClipRect(0, 0, mBounds.width, mBounds.height);
		
		if(res) 
		{
			if(isDirty() || forceDraw) 
			{
				if(mShouldDrawBackground)
				{
					drawBackground();
				}
			}
			//bool res = engine.pushClipRectIntersect(mPaddedBounds.x, mPaddedBounds.y,
			//	mPaddedBounds.width, mPaddedBounds.height);
			Gfx_translate(mPaddingLeft, mPaddingTop);
			BOOL res = Gfx_intersectClipRect(0, 0, mPaddedBounds.width, mPaddedBounds.height);

			if(res) {

				if(isDirty() || forceDraw) 
					drawWidget();
				Vector_each(Widget*, it, mChildren)
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
		bool changed = mRelX != x || mRelY != y;
		mRelX = x;
		mRelY = y;
		updateAbsolutePosition();

		if(changed) {
			requestRepaint();
			fireBoundsChanged();
		}
	}

	const Point& Widget::getPosition() const {
		static Point pnt;
		pnt = Point(mRelX, mRelY);
		return pnt;
	}

	const Point& Widget::getPaddedPosition() const {
		static Point pnt;
		pnt = Point(mRelX+mPaddingLeft, mRelY+mPaddingTop);
		return pnt;
	}

	void Widget::setWidth(int width) {
		bool changed = width != mBounds.width;
		mBounds.width = width;
		updatePaddedBounds();
		requestRepaint();
		if(changed) {
			fireBoundsChanged();
		}
	}

	int Widget::getWidth() const {
		return mBounds.width;
	}

	void Widget::setHeight(int height) {
		bool changed = height != mBounds.height;
		mBounds.height = height;
		updatePaddedBounds();
		requestRepaint();
		if(changed) {
			fireBoundsChanged();
		}
	}

	int	Widget::getHeight() const {
		return mBounds.height;
	}

	void Widget::setParent(Widget *w) {
		if(w != NULL && mParent != NULL && mParent != w) {
			PANIC_MESSAGE("Widget already has a mParent!");
		}
		mParent = w;
		updateAbsolutePosition();
		requestRepaint();
	}

	Widget* Widget::getParent() {
		return mParent;
	}

	Widget* Widget::widgetAt(const Point& p) {
		return widgetAt(p.x, p.y);
	}

	Widget* Widget::widgetAt(int x, int y) {
		Vector_each(Widget *, it, mChildren) {
			Widget *ret = (*it)->widgetAt(x, y);
			if(ret) {
				return ret;
			}
		}

		if(mBounds.contains(x, y)) {
			return this;
		}

		return NULL;
	}

	void Widget::requestRepaint() {
		Engine::getSingleton().requestUIUpdate();
		setDirty();

		if(isTransparent()) {
			if(mParent) {
				mParent->requestRepaint();
			}
		}
	}

	bool Widget::isDirty() const {
		return mDirty;
	}

	void Widget::setDirty(bool d) {
		mDirty = d;

		if(d == true) {
			Vector_each(Widget*, it, mChildren)
				(*it)->setDirty(d);
		}
	}

	void Widget::setSkin(WidgetSkin *mSkin) {
		this->mSkin = mSkin;
		requestRepaint();
	}

	void Widget::setDrawBackground(bool b) {
		this->mShouldDrawBackground = b;
		requestRepaint();
	}

	void Widget::setBackgroundColor(int col) {
		this->mBackColor = col;
		requestRepaint();
	}

	void Widget::updateAbsolutePositionChildren(int x, int y) {
		Vector_each(Widget*,it,mChildren) {
			(*it)->mBounds.x = (*it)->mRelX + x;
			(*it)->mBounds.y = (*it)->mRelY + y;
			(*it)->updatePaddedBounds();
			(*it)->updateAbsolutePositionChildren((*it)->mPaddedBounds.x, (*it)->mPaddedBounds.y);
		}
	}

	void Widget::updatePaddedBounds() {
		mPaddedBounds = mBounds;
		mPaddedBounds.x+=mPaddingLeft;
		mPaddedBounds.y+=mPaddingTop;
		mPaddedBounds.width-=mPaddingLeft+mPaddingRight;
		mPaddedBounds.height-=mPaddingTop+mPaddingBottom;
	}

	// fixme, precalc absolute for mParent
	void Widget::updateAbsolutePosition() {
		Widget *p = this;
		mBounds.x = mRelX;
		mBounds.y = mRelY;
		while((p = p->getParent())) {
			mBounds.x += p->getPosition().x + p->mPaddingLeft;
			mBounds.y += p->getPosition().y + p->mPaddingTop;
		}
		updatePaddedBounds();
		updateAbsolutePositionChildren(mPaddedBounds.x, mPaddedBounds.y);

		fireBoundsChanged();
	}

	Vector<Widget*>& Widget::getChildren() {
		return mChildren;
	}

	const Vector<Widget*>& Widget::getChildren() const {
		return mChildren;
	}

	void Widget::addWidgetListener(WidgetListener* wl) {
		
		Vector_each(WidgetListener*, i, mWidgetListeners) {
			if((*i) == wl) return;
		}
		mWidgetListeners.add(wl);
	}

	void Widget::removeWidgetListener(WidgetListener* wl) {

		Vector_each(WidgetListener*, i, mWidgetListeners) {
			if((*i) == wl) {
				mWidgetListeners.remove(i);
				return;	//or crash
			}
		}
	}

	Vector<WidgetListener*>& Widget::getWidgetListeners()
	{
		return mWidgetListeners;
	}

	void Widget::trigger() {
		Vector_each(WidgetListener*, wl, mWidgetListeners) {
			(*wl)->triggered(this);
		}
	}

	void Widget::setSelected(bool mSelected) {
		this->mSelected = mSelected;
		Vector_each(WidgetListener*, wl, mWidgetListeners) {
			(*wl)->selectionChanged(this, mSelected);
		}
		requestRepaint();
	}
	
	bool Widget::isSelected() const {
		return mSelected;
	}

	void Widget::setEnabled(bool mEnabled) {
		this->mEnabled = mEnabled;

		Vector_each(Widget*,it,mChildren) {
			(*it)->setEnabled(mEnabled);
		}
		
		Vector_each(WidgetListener*, wl, mWidgetListeners) {
			(*wl)->enableStateChanged(this, mSelected);
		}
		requestRepaint();
	}
	
	bool Widget::isEnabled() const {
		return mEnabled;
	}

	void Widget::setPaddingLeft(int l) {
		mPaddingLeft = l;
		updateAbsolutePosition();
		requestRepaint();
	}

	void Widget::setPaddingTop(int t) {
		mPaddingTop = t;
		updateAbsolutePosition();
		requestRepaint();
	}

	void Widget::setPaddingRight(int r) {
		mPaddingRight = r;
		updateAbsolutePosition();
		requestRepaint();
	}

	void Widget::setPaddingBottom(int b) {
		mPaddingBottom = b;
		updateAbsolutePosition();
		requestRepaint();
	}

	int Widget::getPaddingLeft() const {
		return mPaddingLeft;
	}

	int Widget::getPaddingTop() const {
		return mPaddingTop;
	}

	int Widget::getPaddingRight() const {
		return mPaddingRight;
	}

	int Widget::getPaddingBottom() const {
		return mPaddingBottom;
	}

	const Rect& Widget::getPaddedBounds() const {
		return mPaddedBounds;
	}

	void Widget::setParameter(const String& name, const String& value) {
		if(name == "mPaddingLeft") setPaddingLeft(stringToInteger(value));
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

	bool Widget::keyPressed(int keyCode, int nativeCode) {
		return false;
	}

	bool Widget::keyReleased(int keyCode, int nativeCode) {
		return false;
	}

	bool Widget::pointerPressed(MAPoint2d p, int id) {
		return false;
	}

	bool Widget::pointerMoved(MAPoint2d p, int id) {
		return false;
	}

	bool Widget::pointerReleased(MAPoint2d p, int id) {
		return false;
	}

	bool Widget::isFocusable() const {
		return mChildren.size()==0;
	}


	Widget* Widget::nearestWidget(Widget* w1, Widget* w2, Direction dir) {
		if(w1==NULL) return w2;
		if(w2==NULL) return w1;
		/*
		switch(dir) {
			case LEFT:
			{

				int x1 = w1->getBounds().x + w1->getBounds().width;
				int x2 = w2->getBounds().x + w2->getBounds().width;
				int x = this->getBounds().x;
				Vector_each(Widget*, i, mChildren) {
					if(*i==w1) continue;

				}
			}
			break;
			case RIGHT:
			{
			}
			break;
			case UP:
			{
			}
			break;
			case DOWN:
			{
			}
			break;
		}
		*/

		return NULL;
	}

	Widget* Widget::getNearestFocusableInDirectionFrom(Widget* w, Direction dir, Widget* best) {

		for(int i = 0; i < mChildren.size(); i++) {
			if(mChildren[i]->isFocusable()) {
				best = w->nearestWidget(mChildren[i], best, dir);
			} else {
				Widget* ret = mChildren[i]->getNearestFocusableInDirectionFrom(w, dir, best);
				if(ret) {
					best = w->nearestWidget(w, best, dir);
				}
			}
		}

		return best;
	}

	Widget* Widget::getFocusableInDirectionFrom(Widget* w, Direction dir) {
		if(!mParent) return NULL;

		if(mParent->getChildren().size() > 1) {
			return mParent->getNearestFocusableInDirectionFrom(w, dir);
		} else {
			return mParent->getFocusableInDirectionFrom(w, dir);
		}

	}
}
