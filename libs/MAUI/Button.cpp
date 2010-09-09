#include "Button.h"
#include "Engine.h"

namespace MAUI {

Button::Button(int x, int y, int width, int height, Widget* parent, const String& caption) : Widget(x, y, width, height, parent), mPressed(false), mCaption(caption) {
}

bool Button::pointerPressed(MAPoint2d p, int id) {
	MAUI_LOG("Button pressed! %x", this);
	mPressed = true;
	requestRepaint();
}

bool Button::pointerReleased(MAPoint2d p, int id) {
	MAUI_LOG("Button released! %x", this);
	mPressed = false;
	requestRepaint();
}

void Button::drawWidget() {
	const ButtonStyle* style = (const ButtonStyle*)getStyle();
	if(style == NULL) {
		style = (const ButtonStyle*)Engine::getSingleton().getDefaultStyle("Button");
		setStyle(style);
	}

	if(mPressed)
		style->getSafe<SkinProperty>(ButtonStyle::SKIN_PRESSED)->draw(0, 0, mBounds.width, mBounds.height);
	else
		style->getSafe<SkinProperty>(ButtonStyle::SKIN_RELEASED)->draw(0, 0, mBounds.width, mBounds.height);

	int offs = mPressed?1:0;

	FontProperty* font = style->getSafe<FontProperty>(ButtonStyle::FONT);
	MAExtent dim = font->getStringDimensions(mCaption.c_str());
	font->drawString(mCaption.c_str(), (mBounds.width>>1)-(EXTENT_X(dim)>>1)+offs, (mBounds.height>>1)-(EXTENT_Y(dim)>>1)+offs);
}

void Button::setCaption(const String& caption) {
	mCaption = caption;
	requestRepaint();
}

bool Button::isTransparent() const {
	return true;
}

ButtonStyle::ButtonStyle(MAHandle pressed, MAHandle notpressed, MAHandle font, int startX, int endX, int startY, int endY, bool selectedTransparent, bool unselectedTransparent) : Style(3) {
	this->mProperties[SKIN_PRESSED] = new SkinProperty(pressed, startX, endX, startY, endY, selectedTransparent);
	this->mProperties[SKIN_RELEASED] = new SkinProperty(notpressed, startX, endX, startY, endY, unselectedTransparent);
	this->mProperties[FONT] = new FontProperty(font);
}


} // namespace MAUI
