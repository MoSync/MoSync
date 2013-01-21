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
				int replySize = marshaller->marshal(stream, buffer);

				// And last, the callback id
				int callbackId = MAUtil::stringToInteger(stream.getNext());

				maExtensionFunctionInvoke2(module->getExtensionFunction(fnId), marshaller->getChildCount(), (int) buffer);

				char* reply = (char*) malloc(replySize + 128);
				sprintf(reply, "mosync.bridge.reply(%d, {});", callbackId);
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

	for (char ch = def[pos]; !done; ch = def[pos]) {
		char chs[80];
		sprintf(chs, "Char: %c", ch);
		maWriteLog(chs, strlen(chs));
		switch (ch) {
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
		case '\0':
		case 'v':
			if (current) list.add(current);
			current = NULL;
			break;
		}
		done = ch == '\0';
		pos++;
	}

	if (list.size() == 1) {
		return list[0];
	} else {
		JSMarshaller* result = isArgList ? this : new JSMarshaller();
		result->mList = list;
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
	int offset = 0;
	for (int i = 0; i < mList.size(); i++) {
		JSMarshaller* marshaller = mList[i];
		offset += marshaller->marshal(stream, (char*) ((int) buffer + offset));
	}
	return offset;
}

int JSMarshaller::getChildCount() {
	return mList.size();
}

int JSArrayMarshaller::marshal(MessageStream& stream, char* buffer) {
	int arraySize = MAUtil::stringToInteger(stream.getNext());
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

int JSRefMarshaller::marshal(MessageStream& stream, char* buffer) {
	return (*mRefList)[mRefId]->marshal(stream, buffer);
}

int JSNumberMarshaller::marshal(MessageStream& stream, char* buffer) {
	const char* value = stream.getNext();
	maWriteLog(value, strlen(value));
	char var[80];
	sprintf(var, "Variant: %c", mVariant);
	maWriteLog(var, strlen(var));
	switch (mVariant) {
	case 'B':
		buffer[0] = (char) (MAUtil::stringToInteger(value) & 0xff);
		return sizeof(char);
	case 'I':
		((int*) buffer)[0] = MAUtil::stringToInteger(value);
		return sizeof(int);
	case 'L':
		((long long*) buffer)[0] = MAUtil::stringToLong(value);
		return sizeof(long long);
	case 'D':
		((double*) buffer)[0] = MAUtil::stringToDouble(value);
		return sizeof(double);
	case 'F':
		((float*) buffer)[0] = (float) MAUtil::stringToDouble(value);
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
