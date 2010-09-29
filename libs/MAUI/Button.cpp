#include "Button.h"
#include "Engine.h"
#include <mastdlib.h>

namespace MAUI {

Button::Button(int x, int y, int width, int height, Widget* parent, const String& caption) : Label(x, y, width, height, parent, caption), mPressed(false),
	mSkinFocusedPressed(NULL), mSkinFocusedReleased(NULL), mSkinUnfocusedReleased(NULL), mListeners(false) {
	this->setHorizontalAlignment(Label::HA_CENTER);
	this->setVerticalAlignment(Label::VA_CENTER);
}

bool Button::pointerPressed(MAPoint2d p, int id) {
	mPressed = true;
	mStartX = p.x;
	mStartY = p.y;
	ListenerSet_fire(ButtonListener, mListeners, onButtonEvent(this, true));
	requestRepaint();

	return true;
}

bool Button::pointerMoved(MAPoint2d p, int id) {
	p.x-=mStartX;
	p.y-=mStartY;

	int length = (int)sqrt(p.x*p.x+p.y*p.y);
	if(length<15) return true;
	else return false;
}

bool Button::pointerReleased(MAPoint2d p, int id) {
	if(!mPressed) return false;
	mPressed = false;
	//fireTriggered();
	ListenerSet_fire(ButtonListener, mListeners, onButtonEvent(this, false));
	requestRepaint();
	return false;
}

void Button::drawWidget() {
	/*
	const ButtonStyle* style = (const ButtonStyle*)getStyle();
	if(style == NULL) {
		style = (const ButtonStyle*)Engine::getSingleton().getDefaultStyle("Button");
		setStyle(style);
	}
	*/

	if(mFocused) {
		if(mPressed) {
			if(mSkinFocusedPressed) {
				mSkinFocusedPressed->draw(0, 0, mBounds.width, mBounds.height);
			}
		} else {
			if(mSkinFocusedReleased) {
				mSkinFocusedReleased->draw(0, 0, mBounds.width, mBounds.height);
			}
		}
	} else {
		if(!mPressed) {
			mSkinUnfocusedReleased->draw(0, 0, mBounds.width, mBounds.height);
		} else {
			maPanic(1, "Something is wrong, button can't be unfocused and pressed");
		}
	}

	Label::drawWidget();
}

void Button::restyle() {
	if(getStyle() == NULL) {
		setStyle(Engine::getSingleton().getDefaultStyle("Button"));
	}
	const ButtonStyle* style = (const ButtonStyle*)getStyle();

	mSkinFocusedPressed = style->getSafe<SkinProperty>("skinFocusedPressed");
	mSkinFocusedReleased = style->getSafe<SkinProperty>("skinFocusedReleased");
	mSkinUnfocusedReleased = style->getSafe<SkinProperty>("skinUnfocusedReleased");

	Label::restyle();
}

void Button::setCaption(const String& caption) {
	mCaption = caption;
	requestRepaint();
}

bool Button::isTransparent() const {
	return true;
}

void Button::setFocused(bool focused) {
	Widget::setFocused(focused);
	if(mPressed==true && focused == false) {
		mPressed = false;
		// do not send onButtonEvent(this, false) event here (it's obviously been cancelled before released).
	}
	requestRepaint();
}

void Button::addButtonListener(ButtonListener* l) {
	mListeners.add(l);
}

void Button::removeButtonListener(ButtonListener* l) {
	mListeners.remove(l);
}


ButtonStyle::ButtonStyle(
		SkinProperty* focusedPressed,
		SkinProperty* focusedReleased,
		SkinProperty* unfocusedReleased,
		FontProperty* font) : LabelStyle(font, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL)
{
	this->mProperties["skinFocusedPressed"] = focusedPressed;
	this->mProperties["skinFocusedReleased"] = focusedReleased;
	this->mProperties["skinUnfocusedReleased"] = unfocusedReleased;
}

} // namespace MAUI
