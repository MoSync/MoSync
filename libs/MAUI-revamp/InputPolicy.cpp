/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/** 
* \file InputPolicy.cpp
* \brief Handling of input policies.
* TODO: Document what an input policy is.
* \author Niklas Nummelin
*/

#include "InputPolicy.h"
#include "Widget.h"
#include "Screen.h"

namespace MAUI {

Direction mapKeyCodeToDirection(int keyCode) {
	switch(keyCode) {
		case MAK_LEFT: return LEFT;
		case MAK_UP: return UP;
		case MAK_DOWN: return DOWN;
		case MAK_RIGHT: return RIGHT;
		default: return NONE;
	}
}

InputPolicy::InputPolicy(Widget* owner) : mOwner(owner) {
}

void InputPolicy::setOwner(Widget *widget) {
	mOwner = widget;
}

Widget* InputPolicy::getOwner() {
	return mOwner;
}

// TODO: Clarify this comment.
// returns true if focus should be changed.
bool InputPolicy::keyPressed(int keyCode, int nativeCode) {
	return false;
}

bool InputPolicy::keyReleased(int keyCode, int nativeCode) {
	return false;
}

// TODO: Clarify this comment.
// returns true if focus should be lost.
// what about: returns true if focus should be held after release.
bool InputPolicy::pointerPressed(MAPoint2d p, int id) {
	return false;
}

bool InputPolicy::pointerMoved(MAPoint2d p, int id) {
	return false;
}

bool InputPolicy::pointerReleased(MAPoint2d p, int id) {
	return false;
}

DefaultInputPolicy::DefaultInputPolicy(Widget* owner) : InputPolicy(owner) {
}

bool DefaultInputPolicy::keyPressed(int keyCode, int nativeCode) {
	if(!mOwner->isEnabled())
		return false;
	bool ret = mOwner->keyPressed(keyCode, nativeCode);
	if (!ret) {
		Direction dir = mapKeyCodeToDirection(keyCode);
		if (dir != NONE) {
			Widget* newFocus;
			newFocus = mOwner->getFocusableInDirectionFrom(mOwner, dir);
			if (newFocus) {
				MAUI_LOG("Got new focus!");
				mOwner->setFocused(false);
				Screen::getCurrentScreen()->setFocusedWidget(newFocus);			
				//newFocus->setFocused(true);
				//ret = newFocus->keyPressed(keyCode, nativeCode);
			}
		}
	}

	return ret;
}

bool DefaultInputPolicy::keyReleased(int keyCode, int nativeCode) {
	if(!mOwner->isEnabled())
		return false;
	return mOwner->keyReleased(keyCode, nativeCode);
}

bool DefaultInputPolicy::pointerPressed(MAPoint2d p, int id) {
	if(!mOwner->isEnabled())
		return false;
	return mOwner->pointerPressed(p, id);
}

bool DefaultInputPolicy::pointerMoved(MAPoint2d p, int id) {
	if(!mOwner->isEnabled())
		return false;
	return mOwner->pointerMoved(p, id);
}

bool DefaultInputPolicy::pointerReleased(MAPoint2d p, int id) {
	if(!mOwner->isEnabled())
		return false;
	return mOwner->pointerReleased(p, id);
}

} // namespace MAUI
