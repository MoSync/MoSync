#include "Style.h"

namespace MAUI {
Property::Type ColorProperty::sType = Property::COLOR;
Property::Type FontProperty::sType = Property::FONT;
Property::Type SkinProperty::sType = Property::SKIN;
Property::Type ImageProperty::sType = Property::IMAGE;
Property::Type IntegerProperty::sType = Property::INTEGER;

Property::Property(Type type) : mType(type) {
}

Style::Style(int numProperties) {
	mProperties.resize(PROP_END+numProperties);
	mProperties[PADDING_LEFT] = new IntegerProperty(0);
	mProperties[PADDING_RIGHT] = new IntegerProperty(0);
	mProperties[PADDING_TOP] = new IntegerProperty(0);
	mProperties[PADDING_BOTTOM] = new IntegerProperty(0);
}

} // namespace MAUI
