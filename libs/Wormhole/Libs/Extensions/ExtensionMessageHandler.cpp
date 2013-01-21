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
#include <MAUtil/Vector.h>
#include <MAUtil/HashMap.h>
#include <MAUtil/String.h>

#include "../../MessageStream.h"
#include "ExtensionMessageHandler.h"

using namespace NativeUI;
using namespace std;

namespace Wormhole {
bool ExtensionMessageHandler::handleMessage(Wormhole::MessageStream& stream) {
	// TODO: Dynamic alloc?
	char buffer[8192];

	// TODO: Optimize this so we don't have to do it *every* invocation!!!
	const char* ext = stream.getNext();
	if (ext[0] == '*') {
		// Then it's a definition!
		int hash = MAUtil::stringToInteger(stream.getNext(), 16);
		int fnId = MAUtil::stringToInteger(stream.getNext());
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
		int fnId = MAUtil::stringToInteger(stream.getNext());
		if (module) {
			JSMarshaller* marshaller = module->getMarshaller(fnId);
			if (marshaller) {
				// Values.
				int size = marshaller->marshal(stream, buffer);

				// And last, the callback id
				int callbackId = MAUtil::stringToInteger(stream.getNext());

				maExtensionFunctionInvoke2(module->getExtensionFunction(fnId), marshaller->getChildCount(), (int) &buffer);

				MAUtil::String unmarshalled(3 * size); // <-- Some heuristics applied there
				marshaller->unmarshal(buffer, unmarshalled);

				char* reply = (char*) malloc(unmarshalled.size() + 128);

				sprintf(reply, "mosync.bridge.reply(%d, %s);", callbackId, unmarshalled.c_str());
				maWriteLog(reply, strlen(reply));
				mWebView->callJS(reply);
				free(reply);
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
	for (const char* def = stream.getNext(); strcmp("-", def); def = stream.getNext()) {
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
			case '!':
			case '?':
				bool out = ch == '?';
				current = new JSRefMarshaller(refId, &mRefList, out);
				refId = 0;
				break;
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

int JSMarshaller::marshal(MessageStream& stream, char* buffer) {
// TODO: Refactor arg list into separate class.
	int dataStart = mIsArgList ? 4 * mList.size() : 0;
	int offset = dataStart;
	for (int i = 0; i < mList.size(); i++) {
		JSMarshaller* marshaller = mList[i];
		// Set the pointer
		if (mIsArgList) {
			((int*)buffer)[i] = ((int) buffer) + offset;
		}
		// Then marshal stuff
		offset += marshaller->marshal(stream, (char*) ((int) buffer + offset));
	}
	return offset;
}

int JSMarshaller::unmarshal(char* buffer, MAUtil::String& jsExpr) {
	if (mIsArgList) {
		return unmarshalArgList(buffer, jsExpr);
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
			offset += marshaller->unmarshal((char*) ((int) buffer + offset), jsExpr);
		}
		jsExpr += "}";
		return offset;
	}
}

int JSMarshaller::unmarshalArgList(char* buffer, MAUtil::String& jsExpr) {
	// TODO: Again, refactor!
	// Note: the args list is the only type with both in/out types.
	if (!mHasOutValues && !mHasReturnValue) {
		jsExpr += "null";
		return 0;
	}

	jsExpr += "{";

	if (mHasOutValues) {
		bool first = true;
		jsExpr += "out: {";
		for (int i = 0; i < mList.size() - 1; i++) {
			JSMarshaller* marshaller = mList[i];
			if (marshaller->hasOutValues()) {
				char* ptr = (char*) ((int*)buffer)[i];
				if (!first) {
					jsExpr += ",";
					first = false;
				}
				// Then marshal stuff
				jsExpr += marshaller->mName;
				jsExpr += ':';
				marshaller->unmarshal(ptr, jsExpr);
			}
		}
		jsExpr += "}";
	}

	// Return value
	if (mHasReturnValue) {
		JSMarshaller* returnMarshaller = mList[mList.size() - 1];
		jsExpr += "result: ";
		returnMarshaller->unmarshal(buffer, jsExpr);
	}
	jsExpr += "}";

	// We never use this value.
	return 0;
}

int JSMarshaller::getChildCount() {
	return mList.size();
}

int JSArrayMarshaller::marshal(MessageStream& stream, char* buffer) {
	int arraySize = MAUtil::stringToInteger(stream.getNext());
	printf("ARRAY SIZE: %d\n", arraySize);
	int dataStart = 4 * arraySize;
	int offset = dataStart;
	for (int i = 0; i < arraySize; i++) {
		// Set the pointer
		((int*)buffer)[i] = ((int) buffer) + offset;
		// Then marshal stuff
		offset += mDelegate->marshal(stream, buffer + offset);
	}
	return offset;
}

bool JSArrayMarshaller::hasOutValues() {
	return mDelegate->hasOutValues();
}

int JSRefMarshaller::marshal(MessageStream& stream, char* buffer) {
	return (*mRefList)[mRefId]->marshal(stream, buffer);
}

int JSRefMarshaller::unmarshal(char* buffer, MAUtil::String& jsExpr) {
	return (*mRefList)[mRefId]->unmarshal(buffer, jsExpr);
}

int JSNumberMarshaller::marshal(MessageStream& stream, char* buffer) {
	const char* value = stream.getNext();
	printf("NUMBER?: %s\n", value);
	switch (mVariant) {
	case 'B':
		buffer[0] = value ? (char) (MAUtil::stringToInteger(value) & 0xff) : 0;
		return sizeof(char);
	case 'I':
		((int*) buffer)[0] = value ? MAUtil::stringToInteger(value) : 0;
		return sizeof(int);
	case 'L':
		((long long*) buffer)[0] = value ? MAUtil::stringToLong(value) : 0;
		return sizeof(long long);
	case 'D':
		((double*) buffer)[0] = value ? MAUtil::stringToDouble(value) : 0;
		return sizeof(double);
	case 'F':
		((float*) buffer)[0] = value ? (float) MAUtil::stringToDouble(value) : 0;
		return sizeof(float);
	}
	// case 'V':
	return 0;
}

int JSNumberMarshaller::unmarshal(char* buffer, MAUtil::String& jsExpr) {
	switch (mVariant) {
	case 'B':
		jsExpr += MAUtil::integerToString(buffer[0], 10);
		return sizeof(char);
	case 'I':
		jsExpr += MAUtil::integerToString(((int*)buffer)[0], 10);
		return sizeof(int);
	case 'L':
		jsExpr += MAUtil::longToString(((long long*)buffer)[0], 10);
		return sizeof(long long);
	case 'D':
		jsExpr += MAUtil::doubleToString(((double*)buffer)[0], 16);
		return sizeof(double);
	case 'F':
		jsExpr += MAUtil::doubleToString(((float*)buffer)[0], 8);
		return sizeof(float);
	}
	// case 'V':
	return 0;
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

}
