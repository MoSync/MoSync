#include "Style.h"

namespace MAUI {
Property::Type ColorProperty::sType = Property::COLOR;
Property::Type FontProperty::sType = Property::FONT;
Property::Type SkinProperty::sType = Property::SKIN;
Property::Type ImageProperty::sType = Property::IMAGE;
Property::Type IntegerProperty::sType = Property::INTEGER;

Property::Property(Type type) : mType(type) {
}

Style::Style(
		int paddingLeft,
		int paddingRight,
		int paddingTop,
		int paddingBottom,
		SkinProperty* backgroundSkinFocused,
		SkinProperty* backgroundSkinUnfocused
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
