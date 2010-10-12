#include "Style.h"

namespace MAUI {
const Property::Type ColorProperty::sType = Property::COLOR;
const Property::Type FontProperty::sType = Property::FONT;
const Property::Type DrawableProperty::sType = Property::DRAWABLE;
const Property::Type ImageProperty::sType = Property::IMAGE;
const Property::Type IntegerProperty::sType = Property::INTEGER;

Property::Property(Type type) : mType(type) {
}

Style::Style(
		int paddingLeft,
		int paddingRight,
		int paddingTop,
		int paddingBottom,
		DrawableProperty* backgroundSkinFocused,
		DrawableProperty* backgroundSkinUnfocused
) {
	mProperties["paddingLeft"] = new IntegerProperty(paddingLeft);
	mProperties["paddingRight"] = new IntegerProperty(paddingRight);
	mProperties["paddingTop"] = new IntegerProperty(paddingTop);
	mProperties["paddingBottom"] = new IntegerProperty(paddingBottom);
	mProperties["backgroundSkinFocused"] = backgroundSkinFocused;
	mProperties["backgroundSkinUnfocused"] = backgroundSkinUnfocused;
}

Style::~Style() {
	Map<String, Property*>::Iterator iter = mProperties.begin();
	for(; iter != mProperties.end(); iter++) {
		if(iter->second != NULL) {
			delete iter->second;
		}
	}
}


} // namespace MAUI
