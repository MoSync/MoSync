/* Copyright (C) 2010 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
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
