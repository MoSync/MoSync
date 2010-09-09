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
* \file Style
* \brief Classes for managing styles
* \author Patrick Broman and Niklas Nummelin
*/

#ifndef _MAUI_STYLE_H_
#define _MAUI_STYLE_H_

#include <ma.h>
#include "Font.h"
#include "WidgetSkin.h"
#include <MAUtil/Vector.h>

namespace MAUI {

class Property {
public:
	enum Type {
		COLOR,
		FONT,
		SKIN,
		IMAGE,
		INTEGER
	};

	Property(Type type);


	Type getType() const { return mType; }

	private:
		Type mType;

protected:
};

class ColorProperty : public Property {
public:
	ColorProperty(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : Property(sType), r(r), g(g), b(b), a(a) {
	}

	unsigned char r, g, b, a;
	static Type sType;
};

class FontProperty : public Property, public Font {
public:
	FontProperty(MAHandle font) : Property(sType), Font(font) {
	}
	static Type sType;
};

class SkinProperty : public Property, public WidgetSkin {
public:
	SkinProperty(MAHandle image, int x1, int x2, int y1, int y2, bool transparent=true) :
		Property(sType), WidgetSkin(image, x1, x2, y1, y2, transparent) {

	}
	static Type sType;
};

class ImageProperty : public Property {
public:
	ImageProperty(MAHandle img) : Property(sType), mHandle(img) {
	}

	MAHandle mHandle;
	static Type sType;
};

class IntegerProperty : public Property {
public:
	IntegerProperty(int val) : Property(sType), mValue(val) {
	}

	int mValue;
	static Type sType;
};


class Style {
public:
	enum {
		PADDING_LEFT=0,
		PADDING_RIGHT,
		PADDING_TOP,
		PADDING_BOTTOM,
		MARGIN_LEFT,
		MARGIN_RIGHT,
		MARGIN_TOP,
		MARGIN_BOTTOM,
		BACKGROUND_SKIN_FOCUSED,
		BACKGROUND_SKIN_UNFOCUSED,
		PROP_END
	};

	// The constructor will make room for numProperties + Style::PROP_END properties, make sure
	// property id:s for a derieved style class starts at Style::PROP_END
	Style(
			int numProperties,
			int paddingLeft = 0,
			int paddingRight = 0,
			int paddingTop = 0,
			int paddingBottom = 0,
			int marginLeft = 0,
			int marginRight = 0,
			int marginTop = 0,
			int marginBottom = 0,
			SkinProperty* backgroundSkinFocused = NULL,
			SkinProperty* backgroundSkinUnfocused = NULL
	);

	template<typename T>
	T* get(int id) const {
		if(id<0 || id>=mProperties.size()) return NULL;
		Property* prop = mProperties[id];
		if(prop && prop->getType()==T::sType) return (T*)prop;
		else return NULL;
	}

	template<typename T>
	T* getSafe(int id) const {
		T* prop = get<T>(id);
		if(!prop) maPanic(1, "Failed to retrieve style property.");
		return prop;
	}



protected:
	MAUtil::Vector<Property*> mProperties;
};

} // namespace MAUI

#endif // _MAUI_STYLE_H_
