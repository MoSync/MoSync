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
* \file Style.h
* \brief Classes for managing styles.
* \author Niklas Nummelin
*/

#ifndef _MAUI_STYLE_H_
#define _MAUI_STYLE_H_

#include <ma.h>
#include "Font.h"
#include "WidgetSkin.h"
#include <MAUtil/Map.h>
#include <MAUtil/String.h>

namespace MAUI {

/**
 * \brief class that descibes skin properties.
 */
class Property {
public:
	/**
	 * The property type.
	 */
	enum Type {
		COLOR,
		FONT,
		DRAWABLE,
		IMAGE,
		INTEGER
	};

	/**
	 * Constructor.
	 * \param type The property type.
	 */
	Property(Type type);

	/**
	 * Get the property type.
	 * \return The property type.
	 */
	Type getType() const { return mType; }

protected:
	/**
	 * The property type.
	 */
	const Type mType;
};

/**
 * \brief class that describes a color property.
 */
class ColorProperty : public Property {
public:
	/**
	 * Constructor.
	 * \param red Red color component.
	 * \param green Green color component.
	 * \param blue Blue color component.
	 * \param alpha Alpha color component.
	 */
	ColorProperty(
		unsigned char red, 
		unsigned char green, 
		unsigned char blue, 
		unsigned char alpha) : 
			Property(sType), 
			mR(red), 
			mG(green), 
			mB(blue), 
			mA(alpha) 
	{
	}

	/**
	 * Red color component.
	 */
	unsigned char mR;
	
	/**
	 * Green color component.
	 */
	unsigned char mG;
	
	/**
	 * Blue color component.
	 */
	unsigned char mB;
	
	/**
	 * Alpha color component.
	 */
	unsigned char mA;
	
	/**
	 * Property type.
	 */
	static const Type sType;
};


/**
 * \brief class that describes a font property.
 */
class FontProperty : public Property, public Font {
public:
	/**
	 * Constructor.
	 *
	 * \param font The font object.
	 */
	FontProperty(MAHandle font) : Property(sType), Font(font) {
	}
	
	/**
	 * Property type.
	 */
	static const Type sType;
};

/**
 * \brief class that describes a drawable property.
 */
class DrawableProperty : public Property {
public:
	/**
	 * Constructor.
	 *
	 * \param drawable TODO: Document.
	 */
	DrawableProperty(Drawable* drawable) : 
		Property(sType), 
		mDrawable(drawable) 
	{
	}

	/**
	 * Property value.
	 */
	Drawable *mDrawable;

	/**
	 * Property type.
	 */
	static const Type sType;
	
	/**
	 * TODO: Document.
	 */
	static DrawableProperty null;
};

/**
 * \brief class that describes a skin property.
 */
class SkinProperty : public DrawableProperty, public WidgetSkin {
public:
	/**
	 * Constructor.
	 *
	 * \param image TODO: Document.
	 * \param x1 TODO: Document.
	 * \param x2 TODO: Document.
	 * \param y1 TODO: Document.
	 * \param y2 TODO: Document.
	 * \param transparent TODO: Document.
	 */
	SkinProperty(
		MAHandle image, 
		int x1, 
		int x2, 
		int y1, 
		int y2, 
		bool transparent=true) :
			DrawableProperty(this), 
			WidgetSkin(image, x1, x2, y1, y2, transparent)
	{
	}
};

/**
 * \brief class that describes an image property.
 */
class ImageProperty : public Property {
public:
	/**
	 * Constructor.
	 *
	 * \param img The image that is the value of this property.
	 */
	ImageProperty(MAHandle img) : Property(sType), mHandle(img) {
	}

	/**
	 * Property value.
	 */
	MAHandle mHandle;
	
	/**
	 * Property type.
	 */
	static const Type sType;
};

/**
 * \brief class that describes a numeric integer property.
 */
class IntegerProperty : public Property {
public:
	/**
	 * Constructor.
	 *
	 * \param 
	 */
	IntegerProperty(int val) : Property(sType), mValue(val) {
	}

	/**
	 * Property value.
	 */
	int mValue;
	
	/**
	 * Property type.
	 */
	static const Type sType;
};


/**
 * \brief class that describes a style.
 */
class Style {
public:
	/**
	 * Constructor.
	 *
	 * \param paddingLeft Left padding (IntegerProperty).
	 * \param paddingRight Right padding (IntegerProperty).
	 * \param paddingTop Top padding (IntegerProperty).
	 * \param paddingBottom Bottom padding (IntegerProperty).
	 * \param backgroundSkinFocused TODO: Document.
	 * \param backgroundSkinUnfocused TODO: Document.
	 */
	Style(
		int paddingLeft = 0,
		int paddingRight = 0,
		int paddingTop = 0,
		int paddingBottom = 0,
		DrawableProperty* backgroundSkinFocused = NULL,
		DrawableProperty* backgroundSkinUnfocused = NULL);

	/**
	 * Destructor.
	 */
	~Style();

	/**
	 * TODO: Document.
	 */
	template<typename T>
	T* get(const MAUtil::String& id) const {
		MAUtil::Map<MAUtil::String, Property*>::ConstIterator i = mProperties.find(id);
		if((i!=mProperties.end()) && i->second && i->second->getType()==T::sType) {
			return (T*)i->second;
		}
		else {
			return NULL;
		}
	}

	/**
	 * TODO: Document.
	 */
	template<typename T>
	T* getSafe(const MAUtil::String& id) const {
		T* prop = get<T>(id);
		if(!prop) {
			maPanic(1, "Failed to retrieve style property.");
		}
		return prop;
	}

	/**
	 * TODO: Document.
	 */
	void set(const MAUtil::String& id, Property *prop) {
		mProperties[id] = prop;
	}

protected:
	/**
	 * TODO: Document.
	 */
	MAUtil::Map<MAUtil::String, Property*> mProperties;
};

} // namespace MAUI

#endif // _MAUI_STYLE_H_
