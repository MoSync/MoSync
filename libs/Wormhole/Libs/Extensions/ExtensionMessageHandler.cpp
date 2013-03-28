/*
 Copyright (C) 2013 MoSync AB

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License,
 version 2, as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 MA 02110-1301, USA.
 */

#include <maapi.h>
#include <mastdlib.h> // C string conversion functions
#include <mactype.h>
#include <mastring.h>
#include <mavsprintf.h>
#include <MAUtil/Vector.h>
#include <MAUtil/HashMap.h>
#include <MAUtil/String.h>

#include "../../Encoder.h"
#include "../../MessageStream.h"
#include "ExtensionMessageHandler.h"

using namespace NativeUI;
using namespace std;

#define ALIGNMENT 0x4
#define ALIGNMENT_MASK 0x3

namespace Wormhole {
bool ExtensionMessageHandler::handleMessage(Wormhole::MessageStream& stream) {
	char stackBuffer[256];

	// TODO: Optimize this so we don't have to do it *every* invocation!!!
	const char* ext = getNext(stream);
	if (ext[0] == '*') {
		// Then it's a definition!
		int hash = MAUtil::stringToInteger(getNext(stream), 16);
		int fnId = MAUtil::stringToInteger(getNext(stream));
		// Type definitions.
		JSMarshaller* marshaller = new JSMarshaller();
		marshaller->initialize(stream);

		const char* extName = ext + 1;
		JSExtensionModule* module = mModules[extName];
		if (!module) {
			module = new JSExtensionModule(extName, hash);
			mModules[extName] = module;
		}
		module->setMarshaller(fnId, marshaller);
	} else {
		// Otherwise it's a call!
		JSExtensionModule* module = mModules[ext];
		int fnId = MAUtil::stringToInteger(getNext(stream));
		if (module) {
			JSMarshaller* marshaller = module->getMarshaller(fnId);
			if (marshaller) {
				Allocator* allocator = new Allocator(stackBuffer);
				// Values.
				char* buffer = allocator->request(marshaller->getBufferSize(true));
				marshaller->marshal(stream, buffer, allocator);

				// And last, the callback id
				int callbackId = MAUtil::stringToInteger(getNext(stream));

				maExtensionFunctionInvoke2(module->getExtensionFunction(fnId), marshaller->getChildCount(), (int) buffer);
				MAUtil::String unmarshalled(3 * marshaller->getBufferSize(true)); // <-- Just some heuristics.
				marshaller->unmarshal(buffer, unmarshalled);

				char* reply = (char*) malloc(unmarshalled.size() + 128);
				sprintf(reply, "mosync.bridge.reply(%d, %s);", callbackId, unmarshalled.c_str());
				maWriteLog(reply, strlen(reply));
				mWebView->callJS(reply);
				free(reply);
				delete allocator;
			} else {
				maPanic(40075, "Unknown function id");
			}
		} else {
			maPanic(40075, "Unknown module");
		}
	}
	return true;
}

void JSMarshaller::initialize(MessageStream& stream) {
	// First one is the argument list; cannot be referred to.
	int id = -1;

	// Parse until we get a stop token (-)
	for (const char* def = getNext(stream); strcmp("-", def); def = getNext(stream)) {
		if (id < 0) {
			createMarshaller(def, true);
		} else {
			mRefList.add(createMarshaller(def, false));
		}
		id++;
	}
}

JSMarshaller* JSMarshaller::createMarshaller(const char* def, bool isArgList) {
	int pos = 0;
	int refId = 0;
	MAUtil::Vector<JSMarshaller*> list;
	JSMarshaller* current = NULL;
	bool done = false;
	bool inOutArg = false;
	bool hasReturnValue = true;
	bool hasOutValue = false;
	MAUtil::String currentOutArg;

	for (char ch = def[pos]; !done; ch = def[pos]) {
		if (inOutArg) {
			inOutArg = ch != '}';
			if (inOutArg) {
				currentOutArg += ch;
			} else {
				current->setName(currentOutArg);
				currentOutArg = "";
			}
		} else {
			switch (ch) {
			case '{':
				inOutArg = true;
				break;
			case 'B':
			case 'b':
			case 'I':
			case 'i':
			case 'L':
			case 'l':
			case 'F':
			case 'f':
			case 'D':
			case 'd':
				if (current) list.add(current);
				current = new JSNumberMarshaller(ch);
				break;
			case 'S':
			case 's':
				if (current) list.add(current);
				current = new JSStringMarshaller(ch == 's');
				break;
			case '!':
			case '?': {
				if (current) list.add(current);
				bool out = ch == '?';
				current = new JSRefMarshaller(refId, &mRefList, out);
				refId = 0;
				break;
			}
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				refId *= 10;
				refId += (ch - '0');
				break;
			case '*':
				current = new JSArrayMarshaller(current);
				break;
			case '.':
				// Structs in arg lists always have an extra
				// layer of indirection
				current = new JSIndirectMarshaller(current);
				break;
			case 'v':
				hasReturnValue = false;
				/* no break */
			case '\0':
				if (current) list.add(current);
				current = NULL;
				break;
			}
		}
		if (current) {
			hasOutValue |= current->hasOutValues();
		}
		done = ch == '\0';
		pos++;
	}

	if (list.size() == 1 && !isArgList) {
		return list[0];
	} else {
		JSMarshaller* result = isArgList ? this : new JSMarshaller();
		result->mIsArgList = isArgList;
		result->mList = list;
		result->mHasOutValues = hasOutValue;
		result->mHasReturnValue = hasReturnValue;
		return result;
	}

}

JSMarshaller::~JSMarshaller() {
	for (int i = 0; i < mList.size(); i++) {
		JSMarshaller* marshaller = mList[i];
		delete marshaller;
	}
	for (int i = 0; i < mRefList.size(); i++) {
		JSMarshaller* marshaller = mRefList[i];
		delete marshaller;
	}
}

JSArrayMarshaller::~JSArrayMarshaller() {
	if (mDelegate) {
		delete mDelegate;
		mDelegate = NULL;
	}
}

void JSMarshaller::marshal(MessageStream& stream, char* buffer, Allocator* allocator) {
// TODO: Refactor arg list into separate class.
	int dataStart = mIsArgList ? sizeof(int) * mList.size() : 0;
	int offset = dataStart;
	for (int i = 0; i < mList.size(); i++) {
		JSMarshaller* marshaller = mList[i];
		// Set the pointer
		if (mIsArgList) {
			((int*)buffer)[i] = ((int) buffer) + offset;
		}
		// Then marshal stuff
		if (!mIsArgList || !mHasReturnValue || i < mList.size() - 1) {
			char* ptr = (char*) buffer + offset;
			char* padded = pad(ptr);
			marshaller->marshal(stream, mIsArgList ? ptr : padded, allocator);
		}
		offset += marshaller->getBufferSize(true);
	}
}

int JSMarshaller::getBufferSize(bool aligned) {
	int result = 0;
	for (int i = 0; i < mList.size(); i++) {
		result += mList[i]->getBufferSize(aligned);
	}
	if (mIsArgList) {
		result += sizeof(int) * mList.size();
	}
	return result;
}

void JSMarshaller::unmarshal(char* buffer, MAUtil::String& jsExpr) {
	if (mIsArgList) {
		unmarshalArgList(buffer, jsExpr);
	} else {
		int offset = 0;
		jsExpr += "{";
		for (int i = 0; i < mList.size(); i++) {
			if (i > 0) {
				jsExpr += ",";
			}
			JSMarshaller* marshaller = mList[i];
			jsExpr += marshaller->mName;
			jsExpr += ':';
			char* ptr = (char*) buffer + offset;
			char* padded = pad(ptr);
			marshaller->unmarshal(padded, jsExpr);
			offset += marshaller->getBufferSize(true);
		}
		jsExpr += "}";
	}
}

void JSMarshaller::unmarshalArgList(char* buffer, MAUtil::String& jsExpr) {
	// TODO: Again, refactor!
	// Note: the args list is the only type with both in/out types.
	if (!mHasOutValues && !mHasReturnValue) {
		jsExpr += "null";
		return;
	}

	jsExpr += "{";

	if (mHasOutValues) {
		bool first = true;
		jsExpr += "out: {";
		for (int i = 0; i < mList.size() - (mHasReturnValue ? 1 : 0); i++) {
			JSMarshaller* marshaller = mList[i];
			if (marshaller->hasOutValues()) {
				char* ptr = unmarshalPtr(buffer + sizeof(char*) * i);
				if (!first) {
					jsExpr += ",";
				}
				// Then marshal stuff
				jsExpr += marshaller->mName;
				jsExpr += ':';
				marshaller->unmarshal(ptr, jsExpr);
				first = false;
			}
		}
		jsExpr += "}";
	}

	// Return value
	if (mHasReturnValue) {
		JSMarshaller* returnMarshaller = mList[mList.size() - 1];
		if (mHasOutValues) {
			jsExpr += ",";
		}
		jsExpr += "result: ";
		char* argPtr = buffer + sizeof(char*) * (mList.size() - 1);
		char* retPtr = unmarshalPtr(argPtr);
		returnMarshaller->unmarshal(retPtr, jsExpr);
	}
	jsExpr += "}";
}

int JSMarshaller::getChildCount() {
	return mList.size();
}

void JSArrayMarshaller::marshal(MessageStream& stream, char* buffer, Allocator* allocator) {
	// Note: we only support arrays as pointers (ie no fixed length arrays).
	int arraySize = MAUtil::stringToInteger(getNext(stream, "array-len")) & MAX_ARRAY_SIZE_MASK;
	// ...and then we create a pointer to the array, which just
	// happens to follow right after the pointer.
	char* array = NULL;
	if (arraySize > 0) {
		int elementSize = mDelegate->getBufferSize(true);
		char* arrayBuffer = allocator->request(elementSize * arraySize + sizeof(int));
		// The size is a 'hidden' variable that will be used later by the
		// unmarshalling code...
		marshalInt(arrayBuffer, arraySize);
		array = (char*)(arrayBuffer + sizeof(int));
		for (int i = 0; i < arraySize; i++) {
			mDelegate->marshal(stream, (char*) (array + i * elementSize), allocator);
		}
	}
	marshalPtr(buffer, array);
}

void JSArrayMarshaller::unmarshal(char* arrayPtr, MAUtil::String& jsExpr) {
	char* array = unmarshalPtr(arrayPtr);
	if (!array) {
		jsExpr += "null";
		return;
	}
	int arraySize = unmarshalInt(array - sizeof(int)) & MAX_ARRAY_SIZE_MASK;
	jsExpr += "[";
	for (int i = 0; i < arraySize; i++) {
		if (i > 0) {
			jsExpr += ",";
		}
		mDelegate->unmarshal(array + i * mDelegate->getBufferSize(true), jsExpr);
	}
	jsExpr += "]";
}

int JSArrayMarshaller::getBufferSize(bool aligned) {
	return sizeof(char*);
}

bool JSArrayMarshaller::hasOutValues() {
	return mDelegate->hasOutValues();
}

void JSIndirectMarshaller::marshal(MessageStream& stream, char* buffer, Allocator* allocator) {
	char* delegateBuffer = allocator->request(mDelegate->getBufferSize(true));
	mDelegate->marshal(stream, delegateBuffer, allocator);
	marshalPtr(buffer, delegateBuffer);
}

void JSIndirectMarshaller::unmarshal(char* buffer, MAUtil::String& jsExpr) {
	char* ptr = unmarshalPtr(buffer);
	mDelegate->unmarshal(ptr, jsExpr);
}

JSIndirectMarshaller::~JSIndirectMarshaller() {
	if (mDelegate) {
		delete mDelegate;
		mDelegate = NULL;
	}
}

int JSIndirectMarshaller::getBufferSize(bool aligned) {
	return sizeof(char*);
}

void JSRefMarshaller::marshal(MessageStream& stream, char* buffer, Allocator* allocator) {
	(*mRefList)[mRefId]->marshal(stream, buffer, allocator);
}

void JSRefMarshaller::unmarshal(char* buffer, MAUtil::String& jsExpr) {
	(*mRefList)[mRefId]->unmarshal(buffer, jsExpr);
}

int JSRefMarshaller::getBufferSize(bool aligned) {
	return (*mRefList)[mRefId]->getBufferSize(aligned);
}

void JSNumberMarshaller::marshal(MessageStream& stream, char* buffer, Allocator* allocator) {
	const char* value = getNext(stream, "number");
	switch (mVariant) {
	case 'B':
		buffer[0] = value ? (char) (MAUtil::stringToInteger(value) & 0xff) : 0;
		break;
	case 'I':
		marshalInt(buffer, value ? MAUtil::stringToInteger(value) : 0);
		break;
	case 'L':
		marshalLong(buffer, value ? MAUtil::stringToLong(value) : 0);
		break;
	case 'D':
		marshalDouble(buffer, value ? MAUtil::stringToDouble(value) : 0);
		break;
	case 'F':
		marshalFloat(buffer, value ? (float) MAUtil::stringToDouble(value) : 0);
		break;
	}
}

int JSNumberMarshaller::getBufferSize(bool aligned) {
	switch(mVariant) {
	case 'B':
		return aligned ? ALIGNMENT : sizeof(char);
	case 'I':
		return sizeof(int);
	case 'L':
		return sizeof(long long);
	case 'D':
		return sizeof(double);
	case 'F':
		return sizeof(float);
	}
	// case 'V':
	return 0;
}

void JSNumberMarshaller::unmarshal(char* buffer, MAUtil::String& jsExpr) {
	switch (mVariant) {
	case 'B':
		jsExpr += MAUtil::integerToString(buffer[0], 10);
		break;
	case 'I':
		jsExpr += MAUtil::integerToString(unmarshalInt(buffer), 10);
		break;
	case 'L':
		jsExpr += MAUtil::longToString(unmarshalLong(buffer), 10);
		break;
	case 'D':
		jsExpr += MAUtil::doubleToString(unmarshalDouble(buffer), 16);
		break;
	case 'F':
		jsExpr += MAUtil::doubleToString(unmarshalFloat(buffer), 8);
		break;
	}
}

void JSStringMarshaller::marshal(MessageStream& stream, char* buffer, Allocator* allocator) {
	// Bytes 0-3 are the length. Plus 1 byte to make sure we always zero-terminate
	int length = MAUtil::stringToInteger(getNext(stream, "string-len"));
	MAUtil::String msg = getNext(stream, "message");
	char* dataBuffer = NULL;
	if (length >= 0) {
		char* msgBuffer = allocator->request(length + sizeof(int) + sizeof(char));
		marshalInt(msgBuffer, length);
		dataBuffer = (char*)(msgBuffer + sizeof(int));
		strncpy(dataBuffer, msg.c_str(), length + 1);
		dataBuffer[length] = '\0';
	}

	marshalPtr(buffer, dataBuffer);
}

void JSStringMarshaller::unmarshal(char* buffer, MAUtil::String& jsExpr) {
	char* ptr = unmarshalPtr(buffer);
	if (ptr) {
		int length = unmarshalInt(ptr - sizeof(int)) & MAX_ARRAY_SIZE_MASK;
		ptr[length] = '\0'; // <-- We've allocated an extra byte for this purpose
		jsExpr += "decodeURIComponent(escape(atob(\"";
		jsExpr += Encoder::base64Encode(ptr, strlen(ptr));
		jsExpr += "\")))";
	} else {
		// A null value
		jsExpr += "null";
	}
}

int JSStringMarshaller::getBufferSize(bool aligned) {
	return sizeof(char*);
}

JSMarshaller* JSExtensionModule::getMarshaller(int fnId) {
	return mMarshallers[fnId];
}

JSMarshaller* JSExtensionModule::setMarshaller(int fnId, JSMarshaller* marshaller) {
	JSMarshaller* result = mMarshallers[fnId];
	mMarshallers[fnId] = marshaller;
	return result;
}

MAExtensionFunction JSExtensionModule::getExtensionFunction(int fnId) {
	// TODO: Too many lookups... maybe create another 'FunctionInvocation' class?
	MAExtensionModule moduleId = maExtensionModuleLoad(mExtName.c_str(), mHash);
	return maExtensionFunctionLoad(moduleId, fnId);
}

JSExtensionModule::~JSExtensionModule() {
	// TODO!!! Delete
}

char* Allocator::request(int size) {
	// Ok, we might want to just skip this alignment thing
	// and start allocing right away instead...
	if (mOffset + size >= 256) {
		// 256 is the 'magic' stack size;
		// after that we will allocate from the heap
		if (!mAllocations) {
			mAllocations = new MAUtil::Vector<char*>();
		}
		char* allocation = (char*) calloc(size, 1);
		mAllocations->add(allocation);
		//printf("Allocated %d bytes on heap at %d", size, (int) allocation);
		return allocation;
	} else {
		//printf("Allocated %d bytes at %d", size, (int) mStack + mOffset);
		char* result = (char*) ((int) mStack + mOffset);
		mOffset = (int) pad((char*) mOffset + size);
		return result;
	}
}

Allocator::~Allocator() {
	if (mAllocations) {
		for (int i = 0; i < mAllocations->size(); i++) {
			char* allocation = (*mAllocations)[i];
			free(allocation);
		}
	}
}

inline const char* getNext(MessageStream& stream, const char* w) {
	const char* result = stream.getNext(NULL);
	//printf("%s: %s\n", w, result);
	return result;
}

void marshalInt(char* buffer, int value) {
	// We trust our alignment :)
	((int*)buffer)[0] = value;
}

void marshalPtr(char* buffer, char* ptr) {
	((char**)buffer)[0] = ptr;
}

void marshalLong(char* buffer, long long value) {
	((long long*)buffer)[0] = value;
}

void marshalFloat(char* buffer, float value) {
	((float*)buffer)[0] = value;
}

void marshalDouble(char* buffer, double value) {
	((double*)buffer)[0] = value;
}

int unmarshalInt(char* buffer) {
	return ((int*)buffer)[0];
}

char* unmarshalPtr(char* buffer) {
	return ((char**)buffer)[0];
}

long long unmarshalLong(char* buffer) {
	return ((long long*)buffer)[0];
}

float unmarshalFloat(char* buffer) {
	return ((float*)buffer)[0];
}

double unmarshalDouble(char* buffer) {
	return ((double*)buffer)[0];
}

char* pad(char* ptr) {
	return ((int)ptr % ALIGNMENT) ?
			(char*) ((int) (ptr + ALIGNMENT) & ~ALIGNMENT_MASK) :
			ptr;
}

}
