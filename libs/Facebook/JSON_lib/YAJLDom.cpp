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
 * YAJLDom.cpp
 *
 *  Created on: Jan 3, 2011
 *  Author: Niklas Nummelin
 */

#include "YAJLDom.h"
#include <MAUtil/util.h>
#include <MAUtil/Stack.h>
#include "yajl/yajl_parse.h"
#include "yajl/yajl_gen.h"
#include <conprint.h>
#include <MAP/MemoryMgr.h>

using namespace MAPUtil;

namespace MAUtil {
namespace YAJLDom {

static NullValue sNullValue;

Value::Value(Type type) :
	mType(type) {
}

Value::~Value() {
}

Value::Type Value::getType() const {
	return mType;
}

bool Value::toBoolean() const {
	String ret = toString();
	if (ret == "true")
		return true;
	if (ret == "false")
		return false;
	else
		maPanic(1, "Not a boolean value!");
}

int Value::toInt() const {
	return stringToInteger(toString());
}

double Value::toDouble() const {
	return stringToDouble(toString());
}

Value* Value::getValueForKey(const MAUtil::String& key) {
	return &sNullValue;
}

Value* Value::getValueByIndex(int i) {
	return &sNullValue;
}

const Value* Value::getValueForKey(const MAUtil::String& key) const {
	return &sNullValue;
}

const Value* Value::getValueByIndex(int i) const {
	return &sNullValue;
}

int Value::getNumChildValues() const {
	return 0;
}

NullValue::NullValue() :
	Value(NUL) {
}

MAUtil::String NullValue::toString() const {
	return "";
}

BooleanValue::BooleanValue(bool value) :
	Value(BOOLEAN), mValue(value) {

}

MAUtil::String BooleanValue::toString() const {
	if (mValue == true)
		return "true";
	else
		return "false";
}

bool BooleanValue::toBoolean() const {
	return mValue;
}

void BooleanValue::setBoolean(bool value) {
	mValue = value;
}

NumberValue::NumberValue(double num) :
	Value(NUMBER), mValue(num) {
}

String NumberValue::toString() const {
	return doubleToString(mValue);
}

int NumberValue::toInt() const {
	return (int) mValue;
}

double NumberValue::toDouble() const {
	return mValue;
}

StringValue::StringValue(const char* str, size_t length) :
	Value(STRING), mValue(str, length) {
}

StringValue::StringValue(const String& str) :
	Value(STRING), mValue(str) {
}

String StringValue::toString() const {
	return mValue;
}

MapValue::MapValue() :
	Value(MAP) {
}

MapValue::~MapValue() {
	Map<String, Value*>::Iterator iter = mMap.begin();
	while (iter != mMap.end()) {
		deleteobject(iter->second);
		iter++;
	}
}

void MapValue::setValueForKey(const String& key, Value* value) {
	mMap[key] = value;
}

Value* MapValue::getValueForKey(const String& key) {
	Map<String, Value*>::ConstIterator iter = mMap.find(key);
	if (iter != mMap.end())
		return iter->second;
	else
		return &sNullValue;
}

const Value* MapValue::getValueForKey(const String& key) const {
	Map<String, Value*>::ConstIterator iter = mMap.find(key);
	if (iter != mMap.end())
		return iter->second;
	else
		return &sNullValue;
}

String MapValue::toString() const {
	String ret = "{";
	Map<String, Value*>::ConstIterator iter = mMap.begin();
	while (iter != mMap.end()) {
		ret += "\"" + iter->first + "\": ";

		Value *value = iter->second;
		bool isString = value->getType() == Value::STRING;
		if (isString)
			ret += "\"";
		if (value->getType() == Value::NUL)
			ret += "null";
		else
			ret += value->toString();
		if (isString)
			ret += "\"";

		iter++;

		if (iter != mMap.end())
			ret += ", ";
	}
	ret += "}";
	return ret;
}

ArrayValue::ArrayValue() :
	Value(ARRAY) {
}

ArrayValue::~ArrayValue() {
	for (int i = 0; i < mValues.size(); i++) {
		deleteobject(mValues[i]);
	}
}

void ArrayValue::addValue(Value* value) {
	mValues.add(value);
}

const Vector<Value*>& ArrayValue::getValues() const {
	return mValues;
}

int ArrayValue::getNumChildValues() const {
	return mValues.size();
}


String ArrayValue::toString() const {
	String ret = "[";
	for (int i = 0; i < mValues.size(); i++) {
		Value *value = mValues[i];
		bool isString = value->getType() == Value::STRING;
		if (isString)
			ret += "\"";
		ret += value->toString();
		if (isString)
			ret += "\"";

		if (i != mValues.size() - 1)
			ret += ", ";
	}
	ret += "]";
	return ret;
}

Value* ArrayValue::getValueByIndex(int i) {
	if (i < 0 || i >= mValues.size())
		return &sNullValue;
	return mValues[i];
}

const Value* ArrayValue::getValueByIndex(int i) const {
	if (i < 0 || i >= mValues.size())
		return &sNullValue;
	return mValues[i];
}

Value *sRoot = NULL;
Stack<Value*> sValueStack;
Stack<String> sKeyStack;

Value* validateValue(Value* value, Value::Type type) {
	if (value->getType() != type)
		maPanic(1, "Invalid value!");
	else
		return value;
}

void pushValue(Value *value) {

	Value* parent = sValueStack.peek();

	bool isContainer = (value->getType() == Value::MAP || value->getType()
			== Value::ARRAY);

	switch (parent->getType()) {
	case Value::MAP: {
		MapValue* map = (MapValue*) validateValue(parent, Value::MAP);
		map->setValueForKey(sKeyStack.peek(), value);
	}
		break;

	case Value::ARRAY: {
		ArrayValue* array = (ArrayValue*) validateValue(parent, Value::ARRAY);
		array->addValue(value);
	}
		break;

	default: {
		if (sRoot == NULL) {
			sRoot = value;
			break;
		}

		maPanic(1, "Invalid state!");

	}
		break;
	}

	if (isContainer)
		sValueStack.push(value);
}

void popValue() {
	sValueStack.pop();
}

static int parse_null(void * ctx) {
	yajl_gen g = (yajl_gen) ctx;
	yajl_gen_null(g);
	pushValue(newobject(NullValue, new NullValue()));
	return 1;
}

static int parse_boolean(void * ctx, int boolean) {
	yajl_gen g = (yajl_gen) ctx;
	yajl_gen_bool(g, boolean);
	pushValue(newobject(BooleanValue, new BooleanValue((bool) boolean)));
	return 1;
}

static int parse_number(void * ctx, const char * s, unsigned int l) {
	yajl_gen g = (yajl_gen) ctx;
	yajl_gen_number(g, s, l);
	pushValue(newobject(NumberValue, new NumberValue(stringToDouble(String(s, l)))));
	return 1;
}

static int parse_string(void * ctx, const unsigned char * stringVal,
		unsigned int stringLen) {
	yajl_gen g = (yajl_gen) ctx;
	yajl_gen_string(g, stringVal, stringLen);
	pushValue(newobject(StringValue, new StringValue(String((const char*) stringVal, stringLen))));
	return 1;
}

static int parse_map_key(void * ctx, const unsigned char * stringVal,
		unsigned int stringLen) {
	yajl_gen g = (yajl_gen) ctx;
	yajl_gen_string(g, stringVal, stringLen);
	sKeyStack.push(String((const char*) stringVal, stringLen));
	return 1;
}

static int parse_start_map(void * ctx) {
	yajl_gen g = (yajl_gen) ctx;
	yajl_gen_map_open(g);
	pushValue(newobject(MapValue, new MapValue()));
	return 1;
}

static int parse_end_map(void * ctx) {
	yajl_gen g = (yajl_gen) ctx;
	yajl_gen_map_close(g);
	popValue();
	return 1;
}

static int parse_start_array(void * ctx) {
	yajl_gen g = (yajl_gen) ctx;
	yajl_gen_array_open(g);
	pushValue(newobject(ArrayValue, new ArrayValue()));
	return 1;
}

static int parse_end_array(void * ctx) {
	yajl_gen g = (yajl_gen) ctx;
	yajl_gen_array_close(g);
	popValue();
	return 1;
}

static yajl_callbacks callbacks = { parse_null, parse_boolean, NULL, NULL,
		parse_number, parse_string, parse_start_map, parse_map_key,
		parse_end_map, parse_start_array, parse_end_array };

void parseError(yajl_handle hand, int verbose, const unsigned char* jsonText,
		size_t jsonTextLength) {
	unsigned char * str = yajl_get_error(hand, 1, jsonText, jsonTextLength);
	printf("%s\n", str);
	yajl_free_error(hand, str);
}

void gen_print(void *ctx, const char *str, unsigned int len) {
	printf("%.*s", len, str);
}

Value* parse(const unsigned char* jsonText, size_t jsonTextLength) {
	yajl_handle hand;
	yajl_gen_config conf = { 1, "  " };
	yajl_gen g;
	yajl_status stat;
	yajl_parser_config cfg = { 1, 1 };

	//conf.beautify = 0;
	//cfg.checkUTF8 = 1;

	g = yajl_gen_alloc(&conf, NULL);
	//g = yajl_gen_alloc2(gen_print, &conf, NULL, NULL);

	hand = yajl_alloc(&callbacks, &cfg, NULL, (void *) g);

	sValueStack.clear();
	sKeyStack.clear();
	sRoot = NULL;

	/* read file data, pass to parser */
	stat = yajl_parse(hand, jsonText, jsonTextLength);

	if (stat != yajl_status_ok && stat != yajl_status_insufficient_data) {
		parseError(hand, 1, jsonText, jsonTextLength);
		return NULL;
	}

	stat = yajl_parse_complete(hand);

	if (stat != yajl_status_ok && stat != yajl_status_insufficient_data) {
		parseError(hand, 1, jsonText, jsonTextLength);
		return NULL;
	}

	yajl_gen_free(g);
	yajl_free(hand);

	return sRoot;
}

} // namespace YAJLDom
} // namespace MAUtil
