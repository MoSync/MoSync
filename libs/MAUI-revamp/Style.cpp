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
* \file Style.cpp
* \brief Classes for managing styles.
* \author Niklas Nummelin
*/

#include "Style.h"

namespace MAUI {

const Property::Type ColorProperty::sType = Property::COLOR;
const Property::Type FontProperty::sType = Property::FONT;
const Property::Type DrawableProperty::sType = Property::DRAWABLE;
DrawableProperty DrawableProperty::null(NULL);
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
