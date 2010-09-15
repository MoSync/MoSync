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
			mFocused(false),
			mEnabled(true),
			mPaddingLeft(0),
			mPaddingTop(0),
			mPaddingBottom(0),
			mPaddingRight(0),
			mInputPolicy(NULL),
			mStyle(NULL)
		{
		MAUI_LOG("What? 1");
		mInputPolicy = new DefaultInputPolicy(this);
		MAUI_LOG("What? 2");
			if(parent) {
			parent->add(this);
		}
		MAUI_LOG("What? 3");

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


	void Widget::update() {
		if(mStyle == NULL) {
			restyle();
		}
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
				drawBackground();
			}
			//MAUI_LOG("Widget::draw, paddingLeft= %d, paddingTop = %d", mPaddingLeft, mPaddingTop);
			Gfx_translate(mPaddingLeft, mPaddingTop);
			BOOL res = Gfx_intersectClipRect(0, 0, mPaddedBounds.width, mPaddedBounds.height);

			if(res) {
				if(isDirty() || forceDraw) {
					drawWidget();
				}

				Vector_each(Widget*, it, mChildren)
					(*it)->draw();	

			}
			// This commented out to match removal of above intersectClipRect() call.
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

	void Widget::drawBackground() {
		//MAUI_LOG("Widget::drawBackground() 1");
		if(!mStyle) return;
		WidgetSkin* focusedSkin   = mStyle->get<SkinProperty>("backgroundSkinFocused");
		WidgetSkin* unfocusedSkin = mStyle->get<SkinProperty>("backgroundSkinUnfocused");
		//MAUI_LOG("Widget::drawBackground() 2");
		if(mFocused) {
			//mSkin->draw(mBounds.x, mBounds.y, mBounds.width, mBounds.height, WidgetSkin::SELECTED);
			//MAUI_LOG("Widget::drawBackground() 3a");
			if(focusedSkin) {
				//MAUI_LOG("Widget::drawBackground() 4a");
				focusedSkin->draw(0, 0, mBounds.width, mBounds.height);
			}
		}
		else {
			//MAUI_LOG("Widget::drawBackground() 3b");
			//mSkin->draw(mBounds.x, mBounds.y, mBounds.width, mBounds.height, WidgetSkin::UNSELECTED);
			if(unfocusedSkin) {
				//MAUI_LOG("Widget::drawBackground() 4b");
				unfocusedSkin->draw(0, 0, mBounds.width, mBounds.height);
			}
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

	Widget* Widget::focusableWidgetAt(const Point& p) {
		return focusableWidgetAt(p.x, p.y);
	}

	Widget* Widget::focusableWidgetAt(int x, int y) {
		if(!isFocusable()) {
			Vector_each(Widget *, it, mChildren) {
				Widget *ret = (*it)->focusableWidgetAt(x, y);
				if(ret) {
					return ret;
				}
			}
		} else {
			if(mBounds.contains(x, y)) {
				return this;
			}
		}
		return NULL;
	}

	void Widget::requestRepaint() {
		Engine::getSingleton().requestUIUpdate();
		setDirty();

		//if(isTransparent()) {
			//MAUI_LOG("bäh? 2");
			if(mParent) {
				mParent->requestRepaint();
			}
		//}

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

	void Widget::setFocused(bool focused) {
		mFocused = focused;
		Vector_each(WidgetListener*, wl, mWidgetListeners) {
			(*wl)->selectionChanged(this, mFocused);
		}
		requestRepaint();
	}
	
	bool Widget::isFocused() const {
		return mFocused;
	}

	void Widget::setEnabled(bool enabled) {
		mEnabled = enabled;

		Vector_each(Widget*,it,mChildren) {
			(*it)->setEnabled(mEnabled);
		}
		
		Vector_each(WidgetListener*, wl, mWidgetListeners) {
			(*wl)->enableStateChanged(this, mEnabled);
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

	bool Widget::keyPressed(int keyCode, int nativeCode) {
		return false;
	}

	bool Widget::keyReleased(int keyCode, int nativeCode) {
		return false;
	}

	bool Widget::pointerPressed(MAPoint2d p, int id) {
		MAUI_LOG("Widget::pointerPressed! %x", this);
		return false;
	}

	bool Widget::pointerMoved(MAPoint2d p, int id) {
		MAUI_LOG("Widget::pointerMoved! %x", this);
		return false;
	}

	bool Widget::pointerReleased(MAPoint2d p, int id) {
		MAUI_LOG("Widget::pointerReleased! %x", this);
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

	InputPolicy* Widget::getInputPolicy() {
		return mInputPolicy;
	}

	void Widget::setInputPolicy(InputPolicy* ip) {
		if(mInputPolicy) delete mInputPolicy;
		mInputPolicy = ip;
	}

	void Widget::setStyle(const Style* style) {
		mStyle = style;
		if(mStyle)
			restyle();
	}

	const Style* Widget::getStyle() {
		return mStyle;
	}


	void Widget::restyle() {
		//MAUI_LOG("Widget::restyle() called");
		if(!mStyle) {
			mStyle = Engine::getSingleton().getDefaultStyle("Widget");
		}

		if(!mStyle) maPanic(1, "No style set (not event a default style for Widget is available!");

		setPaddingLeft(mStyle->getSafe<IntegerProperty>("paddingLeft")->mValue);
		setPaddingRight(mStyle->getSafe<IntegerProperty>("paddingRight")->mValue);
		setPaddingTop(mStyle->getSafe<IntegerProperty>("paddingTop")->mValue);
		setPaddingBottom(mStyle->getSafe<IntegerProperty>("paddingBottom")->mValue);
		//MAUI_LOG("Widget::restyle, paddingLeft= %d, paddingTop = %d", mPaddingLeft, mPaddingTop);
	}

}
