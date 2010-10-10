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

	Widget::Widget(int x, int y, int width, int height)
		: mParent(NULL), mBounds(x,y,width,height), mRelX(0), mRelY(0),
		mDirty(false),
		mFocused(false),
		mEnabled(true),
		mWidgetListeners(false),
		mPaddingLeft(0),
		mPaddingTop(0),
		mPaddingBottom(0),
		mPaddingRight(0),
		mInputPolicy(NULL),
		mFocusedSkin(NULL),
		mUnfocusedSkin(NULL),
		mUserData(NULL),
		mStyle(NULL)
	{
		mInputPolicy = new DefaultInputPolicy(this);

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

			Gfx_translate(getTranslationX(), getTranslationY());

			if(res) {
				if(isDirty() || forceDraw) {
					drawWidget();

#if 1
					if(mFocused) {
						maSetColor(0x00ff00);
						Gfx_line(0, 0, mBounds.width-1, 0);
						Gfx_line(mPaddedBounds.width-1, 0, mPaddedBounds.width-1, mPaddedBounds.height-1);
						Gfx_line(0, mPaddedBounds.height-1, mPaddedBounds.width-1, mPaddedBounds.height-1);
						Gfx_line(0, 0, 0, mPaddedBounds.height-1);

					}
#endif
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
			//fireBoundsChanged();
			ListenerSet_fire(WidgetListener, mWidgetListeners, boundsChanged(this, this->mBounds));
		}
	}

	void Widget::drawBackground() {
		//MAUI_LOG("Widget::drawBackground() 1");
		if(!mStyle) return;

		//MAUI_LOG("Widget::drawBackground() 2");
		if(mFocused) {
			//mSkin->draw(mBounds.x, mBounds.y, mBounds.width, mBounds.height, WidgetSkin::SELECTED);
			//MAUI_LOG("Widget::drawBackground() 3a");
			if(mFocusedSkin) {
				//MAUI_LOG("Widget::drawBackground() 4a");
				mFocusedSkin->draw(0, 0, mBounds.width, mBounds.height);
			}
		}
		else {
			//MAUI_LOG("Widget::drawBackground() 3b");
			//mSkin->draw(mBounds.x, mBounds.y, mBounds.width, mBounds.height, WidgetSkin::UNSELECTED);
			if(mUnfocusedSkin) {
				//MAUI_LOG("Widget::drawBackground() 4b");
				mUnfocusedSkin->draw(0, 0, mBounds.width, mBounds.height);
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
			//fireBoundsChanged();
			ListenerSet_fire(WidgetListener, mWidgetListeners, boundsChanged(this, this->mBounds));
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
			//fireBoundsChanged();
			ListenerSet_fire(WidgetListener, mWidgetListeners, boundsChanged(this, this->mBounds));
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
		int xx = x - getTranslationX();
		int yy = y - getTranslationY();
		Vector_each(Widget *, it, mChildren) {
			Widget *ret = (*it)->widgetAt(xx, yy);
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
			x-=getTranslationX();
			y-=getTranslationY();
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
		//if(mDirty) return;

		setDirty();


		// TODO: Something like this this should be used, but isTransparent can't be called from the constructor.
		//if(isTransparent()) {
			/*
			if(mParent) {
				mParent->setDirty(true, this);
			}
			*/
		//}
	}

	bool Widget::isDirty() const {
		return mDirty;
	}

	void Widget::setDirty(bool d, Widget* caller) {
		if(mDirty == d) return;

		mDirty = d;

		if(d == true) {
			Engine::getSingleton().requestUIUpdate();

			Vector_each(Widget*, it, mChildren) {
				if(caller && caller == (*it)) continue;
				(*it)->setDirty(d);
			}

			if(mParent) {
				Widget* parent = mParent;
				Widget* prevParent = this;
				do {
					parent->setDirty(true, prevParent);
					prevParent = parent;
					parent = parent->getParent();
				} while(parent);
			}
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

		//fireBoundsChanged();
		ListenerSet_fire(WidgetListener, mWidgetListeners, boundsChanged(this, this->mBounds));
	}

	Vector<Widget*>& Widget::getChildren() {
		return mChildren;
	}

	const Vector<Widget*>& Widget::getChildren() const {
		return mChildren;
	}

	void Widget::addWidgetListener(WidgetListener* wl) {
		mWidgetListeners.add(wl);
	}

	void Widget::removeWidgetListener(WidgetListener* wl) {
		mWidgetListeners.remove(wl);
	}

	/*
	Vector<WidgetListener*>& Widget::getWidgetListeners()
	{
		return mWidgetListeners;
	}
	*/

	void Widget::setFocused(bool focused) {
		mFocused = focused;
		/*
		Vector_each(WidgetListener*, wl, mWidgetListeners) {
			(*wl)->focusChanged(this, mFocused);
		}*/
		ListenerSet_fire(WidgetListener, mWidgetListeners, focusChanged(this, mFocused));

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
		
		/*
		Vector_each(WidgetListener*, wl, mWidgetListeners) {
			(*wl)->enableStateChanged(this, mEnabled);
		}*/
		ListenerSet_fire(WidgetListener, mWidgetListeners, enableStateChanged(this, mEnabled));

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

	bool isToDirectionOf(Direction direction, Rect src, Rect dest) {
		switch (direction) {
		case LEFT:
			return src.x >= (dest.width+dest.x);
		case RIGHT:
			return (src.width+src.x) <= dest.x;
		case UP:
			return src.y >= (dest.height+dest.y);
		case DOWN:
			return (src.height+src.y) <= dest.y;
		default:
			return false;
		}
	}


	Widget* Widget::nearestWidget(Widget* w1, Widget* w2, Direction dir) {
		Rect rectThis = this->getBounds();

		if(w1==NULL) {
			if(w2) {
				Rect rectW2 = w2->getBounds();
				bool w2InDirection = isToDirectionOf(dir, rectThis, rectW2);
				return w2InDirection?w2:NULL;
			}
			return NULL;
		}
		if(w2==NULL) {
			if(w1) {
				Rect rectW1 = w1->getBounds();
				bool w1InDirection = isToDirectionOf(dir, rectThis, rectW1);
				return w1InDirection?w1:NULL;
			}
			return NULL;
		}

		Rect rectW1 = w1->getBounds();
		Rect rectW2 = w2->getBounds();

		bool w1InDirection = isToDirectionOf(dir, rectThis, rectW1);
		bool w2InDirection = isToDirectionOf(dir, rectThis, rectW2);
		if(!w1InDirection && !w2InDirection) return NULL;
		if(w1InDirection && !w2InDirection) return w1;
		if(!w1InDirection && w2InDirection) return w2;

		int distance1 = ((rectThis.x-rectW1.x)*(rectThis.x-rectW1.x)+(rectThis.y-rectW1.y)*(rectThis.y-rectW1.y));
		int distance2 = ((rectThis.x-rectW2.x)*(rectThis.x-rectW2.x)+(rectThis.y-rectW2.y)*(rectThis.y-rectW2.y));
		if(distance1<distance2) return w1;
		else return w2;

		/*
		switch(dir) {
			case LEFT:
			case RIGHT:
			{
				int distance1 = abs(rectThis.x - rectW1.x);
				int distance2 = abs(rectThis.x - rectW2.x);
				if(distance1<distance2) return w1;
				else return w2;
			}
			break;
			case UP:
			case DOWN:
			{
				int distance1 = abs(rectThis.y - rectW1.y);
				int distance2 = abs(rectThis.y - rectW2.y);
				if(distance1<distance2) return w1;
				else return w2;
			}
			break;
		}
		*/
	}

	Widget* Widget::getNearestFocusableInDirectionFrom(Widget* w, Direction dir, Widget* best) {

		for(int i = 0; i < mChildren.size(); i++) {
			if(mChildren[i] == w) continue;
			if(mChildren[i]->isFocusable()) {
				Widget* candidate = w->nearestWidget(mChildren[i], best, dir);
				if(candidate)
					best = candidate;
			} else {
				Widget* ret = mChildren[i]->getNearestFocusableInDirectionFrom(w, dir, best);
				if(ret && ret != best) {
					Widget* candidate = w->nearestWidget(ret, best, dir);
					if(candidate)
						best = candidate;
				}
			}
		}

		return best;
	}

	Widget* Widget::getFocusableInDirectionFrom(Widget* w, Direction dir) {
		if(!mParent) return NULL;

		if(mParent->getChildren().size() > 1) {
			Widget *candidate = mParent->getNearestFocusableInDirectionFrom(w, dir);
			if(candidate) {
				return candidate;
			}
		}

		return mParent->getFocusableInDirectionFrom(w, dir);
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

	int Widget::getTranslationX() const {
		return 0;
	}

	int Widget::getTranslationY() const {
		return 0;
	}

	void Widget::restyle() {
		//MAUI_LOG("Widget::restyle() called");
		if(!mStyle) {
			mStyle = Engine::getSingleton().getDefaultStyle("Widget");
		}

		if(!mStyle) maPanic(1, "No style set (not even a default style for Widget is available!");

		mFocusedSkin   = mStyle->get<SkinProperty>("backgroundSkinFocused");
		mUnfocusedSkin = mStyle->get<SkinProperty>("backgroundSkinUnfocused");

		setPaddingLeft(mStyle->getSafe<IntegerProperty>("paddingLeft")->mValue);
		setPaddingRight(mStyle->getSafe<IntegerProperty>("paddingRight")->mValue);
		setPaddingTop(mStyle->getSafe<IntegerProperty>("paddingTop")->mValue);
		setPaddingBottom(mStyle->getSafe<IntegerProperty>("paddingBottom")->mValue);
		//MAUI_LOG("skins: %x, %x", mFocusedSkin, mUnfocusedSkin);
	}

	void Widget::setUserData(void *userData) {
		mUserData = userData;
	}

	void* Widget::getUserData() {
		return mUserData;
	}

}
