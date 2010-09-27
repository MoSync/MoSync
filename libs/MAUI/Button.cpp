#include "Button.h"
#include "Engine.h"
#include <mastdlib.h>

namespace MAUI {

Button::Button(int x, int y, int width, int height, Widget* parent, const String& caption) : Label(x, y, width, height, parent, caption), mPressed(false),
	mSkinPressed(NULL), mSkinReleased(NULL), mListeners(false) {
	this->setHorizontalAlignment(Label::HA_CENTER);
	this->setVerticalAlignment(Label::VA_CENTER);
}

bool Button::pointerPressed(MAPoint2d p, int id) {
	//MAUI_LOG("Button pressed! %x", this);
	mPressed = true;
	mStartX = p.x;
	mStartY = p.y;
	ListenerSet_fire(ButtonListener, mListeners, onButtonEvent(this, true));
	requestRepaint();
	return true;
}

bool Button::pointerMoved(MAPoint2d p, int id) {
	//MAUI_LOG("Button moved! %x", this);
	p.x-=mStartX;
	p.y-=mStartY;

	int length = (int)sqrt(p.x*p.x+p.y*p.y);
	if(length<15) return true;
	else return false;
}

bool Button::pointerReleased(MAPoint2d p, int id) {
    //MAUI_LOG("Button released! %x", this);
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

	if(mPressed) {
		if(mSkinPressed) {
			mSkinPressed->draw(0, 0, mBounds.width, mBounds.height);
		}
	} else {
		if(mSkinReleased) {
			mSkinReleased->draw(0, 0, mBounds.width, mBounds.height);
		}
	}

	Label::drawWidget();
}

void Button::restyle() {
	if(getStyle() == NULL) {
		setStyle(Engine::getSingleton().getDefaultStyle("Button"));
	}
	const ButtonStyle* style = (const ButtonStyle*)getStyle();
	mSkinPressed = style->getSafe<SkinProperty>("skinPressed");
	mSkinReleased = style->getSafe<SkinProperty>("skinReleased");

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
	mPressed = focused;
	requestRepaint();
}

void Button::addButtonListener(ButtonListener* l) {
	mListeners.add(l);
}

void Button::removeButtonListener(ButtonListener* l) {
	mListeners.remove(l);
}


ButtonStyle::ButtonStyle(
		SkinProperty* pressed,
		SkinProperty* unpressed,
		FontProperty* font) : LabelStyle(font, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL)
{
	this->mProperties["skinPressed"] = pressed;
	this->mProperties["skinReleased"] = unpressed;
}

} // namespace MAUI
