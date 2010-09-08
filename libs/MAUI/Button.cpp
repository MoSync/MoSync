#include "Button.h"
#include "Engine.h"

namespace MAUI {

Button::Button(int x, int y, int width, int height, Widget* parent) : Widget(x, y, width, height, parent), mPressed(false) {
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
	const ButtonStyle* style = (const ButtonStyle*)Engine::getSingleton().getDefaultStyle("Button");
	style->getSafe<SkinProperty>(0)->draw(0, 0, mBounds.width, mBounds.height, mPressed?WidgetSkin::SELECTED:WidgetSkin::UNSELECTED);
}

ButtonStyle::ButtonStyle(MAHandle pressed, MAHandle notpressed) : Style(1) {
	this->mProperties[0] = new SkinProperty(pressed, notpressed, 12, 19, 12, 18, true, true);
}


} // namespace MAUI
