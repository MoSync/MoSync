#include "InputPolicy.h"
#include "Widget.h"

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

void InputPolicy::setOwner(Widget *widget) {
	owner = widget;
}

Widget* InputPolicy::getOwner() {
	return owner;
}

// returns true if focus should be changed.
bool InputPolicy::keyPressed(int keyCode, int nativeCode) {
	return false;
}
bool InputPolicy::keyReleased(int keyCode, int nativeCode) {
	return false;
}

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

bool DefaultInputPolicy::keyPressed(int keyCode, int nativeCode) {
	bool ret = owner->keyPressed(keyCode, nativeCode);
	if (!ret) {
		Direction dir = mapKeyCodeToDirection(keyCode);
		if (dir != NONE) {
			/*
			Widget* newFocus = owner->getFocusableInDirection(dir);
			if (newFocus) {
				Screen::getCurrentScreen()->setFocusedWidget(newFocus);
				newFocus->keyPressed(keyCode, nativeCode);
			}
			*/
		}
	}

	return false;
}

bool DefaultInputPolicy::keyReleased(int keyCode, int nativeCode) {
	return owner->keyReleased(keyCode, nativeCode);
}

bool DefaultInputPolicy::pointerPressed(MAPoint2d p, int id) {
	return owner->pointerPressed(p, id);
}

bool DefaultInputPolicy::pointerMoved(MAPoint2d p, int id) {
	return owner->pointerMoved(p, id);
}

bool DefaultInputPolicy::pointerReleased(MAPoint2d p, int id) {
	return owner->pointerReleased(p, id);
}

} // namespace MAUI
