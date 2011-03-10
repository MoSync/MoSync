/* Copyright (C) 2011 MoSync AB
 
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

#include "Button.h"
#include "Engine.h"
#include <mastdlib.h>
#include <mavsprintf.h>

namespace MAUI {

Button::Button(int x, int y, int width, int height, const String& caption)
	: Label(x, y, width, height, caption), mPressed(false),
		mSkinFocusedPressed(NULL), mSkinFocusedReleased(NULL),
		mSkinUnfocusedReleased(NULL), mListeners(false)
{
	this->setHorizontalAlignment(Label::HA_CENTER);
	this->setVerticalAlignment(Label::VA_CENTER);
	this->setAutoSizeX(false);
	this->setAutoSizeY(false);
}

bool Button::keyPressed(int keyCode, int nativeCode) {
	if(keyCode == MAK_FIRE) {
		mPressed = true;
		ListenerSet_fire(ButtonListener, mListeners, onButtonEvent(this, true));
		requestRepaint();
		return true;
	}
	return false;
}

bool Button::keyReleased(int keyCode, int nativeCode) {
	if(keyCode == MAK_FIRE && mPressed) {
		mPressed = false;
		ListenerSet_fire(ButtonListener, mListeners, onButtonEvent(this, false));
		requestRepaint();
		return true;
	}
	return false;
}

bool Button::pointerPressed(MAPoint2d point, int id)
{
	//lprintfln("bpp %ix%i", p.x, p.y);
	mPressed = true;
	mStartX = point.x;
	mStartY = point.y;
	ListenerSet_fire(ButtonListener, mListeners, onButtonEvent(this, true));
	requestRepaint();

	return true;
}

bool Button::pointerMoved(MAPoint2d point, int id)
{
	//lprintfln("bpm %ix%i", p.x, p.y);
#if 0
	point.x-=mStartX;
	point.y-=mStartY;

	int length = (int)sqrt((double)(point.x*point.x+point.y*point.y));
	if(length<15) return true;
	else return false;
#else
	return mBounds.contains(point.x, point.y);
#endif
}

bool Button::pointerReleased(MAPoint2d point, int id)
{
	if(!mPressed) return false;
	mPressed = false;
	//fireTriggered();
	ListenerSet_fire(ButtonListener, mListeners, onButtonEvent(this, false));
	requestRepaint();
	return false;
}

void Button::drawWidget()
{
	if(mFocused)
	{
		if(mPressed)
		{
			if(mSkinFocusedPressed)
			{
				mSkinFocusedPressed->draw(0, 0, mBounds.width,
												mBounds.height);
			}
		}
		else
		{
			if(mSkinFocusedReleased)
			{
				mSkinFocusedReleased->draw(0, 0, mBounds.width,
												mBounds.height);
			}
		}
	}
	else
	{
		if(!mPressed)
		{
			if(mSkinUnfocusedReleased)
				mSkinUnfocusedReleased->draw(0, 0, mBounds.width, 
												mBounds.height);
		}
		else
		{
			maPanic(1, "Something is wrong, "
						"button can't be unfocused and pressed");
		}
	}

	Label::drawWidget();
}

void Button::restyle()
{
	if(getStyle() == NULL)
	{
		setStyle(Engine::getSingleton().getDefaultStyle("Button"));
	}
	const ButtonStyle* style = (const ButtonStyle*)getStyle();

	mSkinFocusedPressed = style->getSafe<DrawableProperty>
									("skinFocusedPressed")->mDrawable;
	
	mSkinFocusedReleased = style->getSafe<DrawableProperty>
									("skinFocusedReleased")->mDrawable;
	
	mSkinUnfocusedReleased = style->getSafe<DrawableProperty>
									("skinUnfocusedReleased")->mDrawable;
	Label::restyle();
}

bool Button::isTransparent() const
{
	return true;
}

void Button::setFocused(bool focused)
{
	Widget::setFocused(focused);
	if(mPressed==true && focused == false)
	{
		mPressed = false;
		// do not send onButtonEvent(this, false) event here
		// (it's obviously been cancelled before released).
	}
	requestRepaint();
}

void Button::addButtonListener(ButtonListener* listener)
{
	mListeners.add(listener);
}

void Button::removeButtonListener(ButtonListener* listener)
{
	mListeners.remove(listener);
}


ButtonStyle::ButtonStyle(
		DrawableProperty* focusedPressed,
		DrawableProperty* focusedReleased,
		DrawableProperty* unfocusedReleased,
		FontProperty* font) : LabelStyle(font, 0, 0, 0, 0, NULL, NULL)
{
	if(focusedPressed == NULL)
		focusedPressed = &MAUI::DrawableProperty::null;
	if(focusedReleased == NULL)
		focusedReleased = &MAUI::DrawableProperty::null;
	if(unfocusedReleased == NULL)
		unfocusedReleased = &MAUI::DrawableProperty::null;
	this->mProperties["skinFocusedPressed"] = focusedPressed;
	this->mProperties["skinFocusedReleased"] = focusedReleased;
	this->mProperties["skinUnfocusedReleased"] = unfocusedReleased;
}

} // namespace MAUI
