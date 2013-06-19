/* Copyright (C) 2011 Mobile Sorcery AB

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

/*
 * YAJLDom.h
 *
 *  Created on: Jan 3, 2011
 *  Author: Niklas Nummelin
 */

#ifndef _YAJL_DOM_H_
#define _YAJL_DOM_H_

#ifndef __WINDOWS_PHONE_8_LIB__
#include <MAUtil/Vector.h>
#include <MAUtil/Map.h>
#include <MAUtil/String.h>
#else
#include "../MAUtil/Vector.h"
#include "../MAUtil/Map.h"
#include "../MAUtil/String.h"
#endif

namespace MAUtil {
namespace YAJLDom {

class Value {
	public:
		enum Type {
			NUL,
			BOOLEAN,
			NUMBER,
			ARRAY,
			MAP,
			STRING
		};

		Value(Type type);
		virtual ~Value();

		Type getType() const;
		bool isNull() const;

		virtual MAUtil::String toString() const = 0;
		virtual bool toBoolean() const;
		virtual int toInt() const;
		virtual double toDouble() const;
		virtual Value* getValueForKey(const MAUtil::String& key);
		virtual Value* getValueByIndex(int i);

		virtual const Value* getValueForKey(const MAUtil::String& key) const;
		virtual  const Value* getValueByIndex(int i) const;

		virtual int getNumChildValues() const;

	private:
		Type mType;

	};

	class NullValue : public Value {
	public:
		NullValue();
		MAUtil::String toString() const;
	};

	class BooleanValue : public Value {
	public:
		BooleanValue(bool value);
		MAUtil::String toString() const;
		bool toBoolean() const;
		void setBoolean(bool value);

	private:
		bool mValue;
	};

	class NumberValue : public Value {
	public:
		NumberValue(double num);
		MAUtil::String toString() const;
		int toInt() const;
		double toDouble() const;

	private:
		double mValue;
	};

	class StringValue : public Value {
	public:
		StringValue(const char* str, size_t length);
		StringValue(const MAUtil::String& str);
		MAUtil::String toString() const;
	private:
		MAUtil::String mValue;
	};

	class MapValue : public Value {
	public:
		MapValue();
		~MapValue();

		void setValueForKey(const MAUtil::String& key, Value* value);
		Value* getValueForKey(const MAUtil::String& key);
		const Value* getValueForKey(const MAUtil::String& key) const;

		MAUtil::String toString() const;

	private:
		MAUtil::Map<MAUtil::String, Value*> mMap;
	};

	class ArrayValue : public Value {
	public:
		ArrayValue();
		~ArrayValue();

		void addValue(Value* value);

		Value* getValueByIndex(int i);
		const Value* getValueByIndex(int i) const;
		int getNumChildValues() const;

		const MAUtil::Vector<Value*>& getValues() const;

		MAUtil::String toString() const;
	private:
		MAUtil::Vector<Value*> mValues;
	};

	/**
	 * Parse Json string data and return the root node of
	 * the document tree.
	 * \param jsonText UTF8 or ASCII.
	 * \param jsonTextLength Length of Json text.
	 * \return The root node if successful, or NULL on error.
	 * The returned node must be deallocated with delete.
	 */
	Value* parse(const unsigned char* jsonText, size_t jsonTextLength);

	/**
	 * Use this function to safely delete a value (won't do anything if the value is NULL or equal to sNullValue).
	 * sNullValue might be returned if you do getValueByIndex or getValueForKey and the key or element doesn't exist.
	 */
	void deleteValue(Value* value);

} // namespace YAJLDom
} // namespace MAUtil


#endif // _YAJL_DOM_H_
