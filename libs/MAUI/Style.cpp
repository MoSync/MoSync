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
		int numProperties,
		int paddingLeft,
		int paddingRight,
		int paddingTop,
		int paddingBottom,
		int marginLeft,
		int marginRight,
		int marginTop,
		int marginBottom,
		SkinProperty* backgroundSkinFocused,
		SkinProperty* backgroundSkinUnfocused
) {
	mProperties.resize(PROP_END+numProperties);
	mProperties[PADDING_LEFT] = new IntegerProperty(paddingLeft);
	mProperties[PADDING_RIGHT] = new IntegerProperty(paddingRight);
	mProperties[PADDING_TOP] = new IntegerProperty(paddingTop);
	mProperties[PADDING_BOTTOM] = new IntegerProperty(paddingBottom);
	mProperties[MARGIN_LEFT] = new IntegerProperty(marginLeft);
	mProperties[MARGIN_RIGHT] = new IntegerProperty(marginRight);
	mProperties[MARGIN_TOP] = new IntegerProperty(marginTop);
	mProperties[MARGIN_BOTTOM] = new IntegerProperty(marginBottom);
	mProperties[BACKGROUND_SKIN_FOCUSED] = backgroundSkinFocused;
	mProperties[BACKGROUND_SKIN_UNFOCUSED] = backgroundSkinUnfocused;
}


} // namespace MAUI
